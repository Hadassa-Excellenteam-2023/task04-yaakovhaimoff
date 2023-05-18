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

std::ostream &operator<<(std::ostream &os, const Data &data) {
    for (const auto &city: data.m_cities) {
        os << "City: " << city.first << ", x: " << city.second.first
           << ", y: " << city.second.second << std::endl;
    }
    return os;
}

cities Data::search(const std::string& cityName, double radius, const  Func& normFunc) {
    cities result;

    // Check if the given city exists in the map
    auto cityIt = m_cities.find(cityName);
    if (cityIt == m_cities.end()) {
        // City not found
        return result;
    }

    // Get the coordinates of the given city
    std::pair<double, double> cityCoords = cityIt->second;

    cout << "cityName: " << cityName << endl;
    cout << "cityCoords: " << cityCoords.first << " " << cityCoords.second << endl;

    // Iterate over the cities and filter based on proximity
    for (const auto& city : m_cities) {
        const std::string& currentCity = city.first;
        const std::pair<double, double>& currentCoords = city.second;

        // Skip the given city itself
        if (currentCity == cityName) {
            continue;
        }

        // Calculate the distance between the given city and the current city
        double distance = normFunc(cityCoords, currentCoords);

        // Check if the current city falls within the specified radius
        if (distance <= radius) {
            result[currentCity] = currentCoords;
        }
    }

    return result;
}