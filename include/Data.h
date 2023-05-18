#pragma once

#include "macros.h"
#include "Coordinates.h"


class Data {

    cities m_cities;
    multimap<Coordinate, std::string, CompareByX> m_xSortedMap;
    multimap<Coordinate, std::string, CompareByY> m_ySortedMap;

    cities getSquare(const Coordinate &, double);


    template <typename CoordinateType, typename MapType>
    void splitAndIntersect(MapType&, const MapType&,
                                 double, double) const;

    template <typename MapTypeA, typename MapTypeB>
    cities getResult(const MapTypeA &mapA, const MapTypeB &mapB);


public:
    explicit Data(const std::string &);

    cities search(const std::string &, double, const Func &);

    void addCity(const std::string &, double, double);

    void removeCity(const std::string &);

    void printData(cities c) const {
        for (const auto &city: c) {
            std::cout << "City: " << city.first << ", Latitude: " << city.second.first
                      << ", Longitude: " << city.second.second << std::endl;
        }
    }
};
