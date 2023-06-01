#include "Data.h"

Data::Data(const string &filename) {
    std::ifstream inputFile(filename);
    string line, city, hyphen;
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

void Data::addCity(const string &city, double latitude, double longitude) {
    m_cities[city] = std::make_pair(latitude, longitude);
}

void Data::removeCity(const string &city) {
    m_cities.erase(city);
}

Result Data::getListOfCitiesInRadius(const string &cityName, double radius, const Func &normFunc, int &counter) {
    Result result;

    //  Check if the given city exists in the map
    auto cityIt = m_cities.find(cityName);
    if (cityIt == m_cities.end()) {
        // City not found
        return result;
    }

    //  Get the coordinates of the given city
    auto cityCoords = Coordinate(cityIt->second.first, cityIt->second.second);
    auto squareResult = getSquare(cityCoords, radius);

    // get the cities in the circle
    result = getCitiesInCircle(squareResult, cityName, cityCoords, radius, normFunc, counter);
    // remove the given city from the result
    std::erase(result, std::make_pair(cityName, 0.0));
    // sort the result by the distance from the given city
    std::sort(result.begin(), result.end(), SortByValue);

    return result;
}

Data::SquareResult Data::getSquare(const Coordinate &cityCoords, double radius) {
    auto itBeginX = m_xSortedMap.lower_bound(Coordinate(cityCoords.x - radius, cityCoords.y));
    auto itEndX = m_xSortedMap.upper_bound(Coordinate(cityCoords.x + radius, cityCoords.y));
    auto itBeginY = m_ySortedMap.lower_bound(Coordinate(cityCoords.x, cityCoords.y - radius));
    auto itEndY = m_ySortedMap.upper_bound(Coordinate(cityCoords.x, cityCoords.y + radius));

    std::multimap<Coordinate, string, CompareByX> subMmapY(itBeginY, itEndY);
    std::multimap<Coordinate, string, CompareByX> square;
    std::set_intersection(itBeginX, itEndX, subMmapY.begin(), subMmapY.end(), std::inserter(square, square.begin()),
                          SortSquare);
    return square;
}

Result Data::getCitiesInCircle(SquareResult squareResult, const string &cityName, const Coordinate &cityCoords, double radius, const Func &normFunc, int&counter) {
    Result result;
    // Calculate the distance between the given city and the current city in the circle
    std::transform(squareResult.begin(), squareResult.end(), std::back_inserter(result),
                   [&cityCoords, &normFunc, &radius, &counter](const auto &city) {
                       const string &currentCity = city.second;
                       const std::pair<double, double> &currentCoords = {city.first.x, city.first.y};

                       //  Calculate the distance between the given city and the current city
                       double distance = normFunc({cityCoords.x, cityCoords.y}, currentCoords);

                       // Check if the current city falls within the specified radius
                       if (distance <= radius) {
                           if (cityCoords.y < currentCoords.second) {
                               counter++;
                           }
                           return std::make_pair(currentCity, distance);
                       }
                       return std::make_pair(currentCity, 0.0);
                   });
    return result;
}

void Data::printData(Result r, int northCities) const {
    cout << endl << "Search result:" << endl;
    cout << r.size() << " city/cities found in the given radius." << endl;
    cout << northCities << " cities are to the north of the selected city." << endl;
    cout << "City list:" << endl;
    for (const auto &city: r) {
        cout << city.first << endl;
    }
    cout << endl;
}