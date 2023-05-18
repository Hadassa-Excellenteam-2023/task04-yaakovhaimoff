#include "Data.h"

Data::Data(const std::string &filename) {
    std::ifstream inputFile(filename);
    std::string line, city, hyphen;
    double x, y;

    while (std::getline(inputFile, line)) {

        if (line.empty()) continue;  // Skip empty lines

        if (city.empty()) {
            city = line;
        } else {
            std::istringstream iss(line);
            iss >> x >> hyphen >> y;
            m_cities[city] = std::make_pair(x, y);
            m_xSortedMap.insert(std::make_pair(Coordinate(x, y), city));
            m_ySortedMap.insert(std::make_pair(Coordinate(x, y), city));
            city.clear();  // Reset the city for the next pair of coordinates
        }
    }
}

void Data::addCity(const std::string &city, double latitude, double longitude) {
    m_cities[city] = std::make_pair(latitude, longitude);
}

void Data::removeCity(const std::string &city) {
    m_cities.erase(city);
}

cities Data::search(const std::string &cityName, double radius, const Func &normFunc) {
    cities result;

    // Check if the given city exists in the map
    auto cityIt = m_cities.find(cityName);
    if (cityIt == m_cities.end()) {
        // City not found
        return result;
    }

    // Get the coordinates of the given city
    auto cityCoords = Coordinate(cityIt->second.first, cityIt->second.second);
    auto squareResult = getSquare(cityCoords, radius);

    cout << "searching for " << cityName << " with radius " << radius << endl;
    cout << "city coords: " << cityIt->second.first << " " << cityIt->second.second << endl;
    cout << "m_cities size " << m_cities.size() << endl;
    cout << "square result size: " << squareResult.size() << endl;

//     Iterate over the cities and filter based on proximity
    for (const auto &city: squareResult) {
        const std::string &currentCity = city.first;
        const std::pair<double, double> &currentCoords = city.second;

        // Skip the given city itself
        if (currentCity == cityName) {
            continue;
        }
        // Calculate the distance between the given city and the current city
        double distance = normFunc({cityCoords.x, cityCoords.y}, currentCoords);

        // Check if the current city falls within the specified radius
        if (distance <= radius) {
            result[currentCity] = currentCoords;
        }
    }
    cout << "result size: " << result.size() << endl;

    return result;
}

cities Data::getSquare(const Coordinate &cityCoords, double radius) {
    cities result;

    multimap<Coordinate, std::string, CompareByX> xSortedMap;
    multimap<Coordinate, std::string, CompareByY> ySortedMap;

    splitAndIntersect<Coordinate, multimap<Coordinate, std::string, CompareByX>>
            (xSortedMap, m_xSortedMap, cityCoords.x, radius);
    splitAndIntersect<Coordinate, multimap<Coordinate, std::string, CompareByY>>
            (ySortedMap, m_ySortedMap, cityCoords.y, radius);

    // send the smaller map first to getResult to reduce the number of iterations
    const auto &shorterMap = (xSortedMap.size() <= ySortedMap.size()) ?
            result = getResult(xSortedMap, ySortedMap) : result = getResult(ySortedMap, xSortedMap);

    return result;
}

template<typename MapTypeA, typename MapTypeB>
cities Data::getResult(const MapTypeA &mapA, const MapTypeB &mapB) {
    cities result;
    for (const auto &pair: mapA) {
        const std::string &city = pair.second;
        const auto &coords = pair.first;
        if (mapB.find(coords) != mapB.end()) {
            result[city] = m_cities[city];
        }
    }
    return result;
}


template<typename CoordinateType, typename MapType>
void Data::splitAndIntersect(MapType &result, const MapType &sortedMap,
                             double cityCoords, double radius) const {
    double lowerBound = cityCoords - radius;
    double upperBound = cityCoords + radius;
    auto lowerBoundIt = sortedMap.lower_bound(CoordinateType(lowerBound, lowerBound));
    auto upperBoundIt = sortedMap.upper_bound(CoordinateType(upperBound, upperBound));

    for (auto it = lowerBoundIt; it != upperBoundIt; ++it) {
        result.insert(*it);
    }
}