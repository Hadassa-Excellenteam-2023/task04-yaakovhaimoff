#include "Data.h"

/**
 * Constructor
 * @param filename - the name of the file to read from
 * @throws std::runtime_error if the file cannot be opened
 * @throws std::exception if the file is empty
 * @throws std::exception if the file is not in the correct format
 * @throws std::exception if the file contains invalid data
 */
Data::Data(const string &filename) {
    std::ifstream inputFile(filename);
    string line, city, hyphen;
    double x, y;

    try {
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
    catch (const std::exception &e) {
        throw; // Re-throw the exception
    }
}

/**
 * Run the program
 */
void Data::run() {
    std::string city;
    double radius;
    int searchWay;

    while (true) {
        try {
            city = readCityName();

            while (readValue<double>(radius, "Please enter the wanted radius:",
                                     "Invalid type of radius!") < 0);

            while (!(0 <= readValue<int>(searchWay, "Please enter the wanted norm (0 – L2, 1 – Linf, 2 – L1):",
                                         "Invalid type of choice!") && searchWay <= 2));

            Func normFunc = Norm::searchNormFunction(searchWay);

            if (normFunc) {
                int counter = 0;
                auto result = getListOfCitiesInRadius(city, radius, normFunc, counter);
                printData(result, counter);
            } else {
                std::cout << "Invalid choice!" << std::endl;
            }
        } catch (const std::exception &e) {
            throw; // Re-throw the exception
        }
    }
}

/**
 * readCityName - read the name of the city from the user
 * @return the name of the city
 * @throws std::runtime_error if the user enters an invalid type of city name
 */
string Data::readCityName() {
    string city;
    while (true) {
        //  Yoncalla, OR
        std::cout << "Please enter selected city name (with line break after it):" << std::endl;
        std::getline(std::cin, city);

        if (std::cin.fail()) {
            throw std::runtime_error("Invalid type of city name!");
        }
        if (city == "0") {
            std::cout << "Bye bye!" << std::endl;
            exit(0); // Exit the program immediately
        }
        if (!nameExists(city)) {
            cout << "city: " << city << "\n";
            std::cout << "Invalid city name!" << std::endl;
            continue;
        } else {
            break;
        }
    }
    return city;
}

/**
 * nameExists - check if the given city exists in the map
 * @param cityName - the name of the city to check
 * @return true if the city exists in the map, false otherwise
 */
bool Data::nameExists(const string &cityName) const {

    auto cityIt = m_cities.find(cityName);
    return !(cityIt == m_cities.end()); //  return if the given city exists in the map
}

void Data::addCity(const string &city, double latitude, double longitude) {
    m_cities[city] = std::make_pair(latitude, longitude);
}

void Data::removeCity(const string &city) {
    m_cities.erase(city);
}

/**
 * getListOfCitiesInRadius - get the list of cities in the given radius
 * @param cityName - the name of the city to check
 * @param radius - the radius to check
 * @param normFunc - the norm function to use
 * @param counter - the number of cities that are north to the given city
 * @return the list of cities in the given radius
 */
Result Data::getListOfCitiesInRadius(const string &cityName,
                                     double radius,
                                     const Func &normFunc,
                                     int &counter) {
    Result result;

    auto cityIt = m_cities.find(cityName);

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

/**
 * getSquare - get the square of the given radius
 * @param cityCoords - the coordinates of the given city
 * @param radius - the radius to check
 * @return the cities in square of the given radius
 */
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

/**
 * getCitiesInCircle - get the cities in the given circle
 * @param squareResult - the cities in square of the given radius
 * @param cityName - the name of the city to check
 * @param cityCoords - the coordinates of the given city
 * @param radius - the radius to check
 * @param normFunc - the norm function to use
 * @param counter - the number of cities that are north to the given city
 * @return the cities in the given circle
 */
Result Data::getCitiesInCircle(SquareResult squareResult,
                               const string &cityName,
                               const Coordinate &cityCoords,
                               double radius,
                               const Func &normFunc,
                               int &counter) {
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
                           // Check if the current city is to the north of the given city
                           if (cityCoords.x < currentCoords.first) {
                               counter++;
                           }
                           return std::make_pair(currentCity, distance);
                       }
                       return std::make_pair(currentCity, 0.0);
                   });
    return result;
}

/**
 * printData - print the data
 * @param r - the result to print
 * @param northCities - the number of cities that are north to the given city
 */
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