#pragma once

#include "macros.h"

class Data {

    cities m_cities;

public:
    explicit Data(const std::string &);

    cities search(const std::string &, double, const Func&) ;

    void addCity(const std::string &, double, double);

    void removeCity(const std::string &);

    friend std::ostream &operator<<(std::ostream &, const Data &);

    void printData(cities c) const {
        for (const auto& city : c) {
            std::cout << "City: " << city.first << ", Latitude: " << city.second.first
                      << ", Longitude: " << city.second.second << std::endl;
        }
    }
};
