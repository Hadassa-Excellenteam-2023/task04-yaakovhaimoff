#pragma once

#include "macros.h"
#include "Coordinates.h"

static bool sortByValue(const std::pair<std::string, double>& a, const std::pair<std::string, double>& b) {
    return a.second < b.second;
}

class Data {

    Cities m_cities;
    map<Coordinate, std::string, CompareByX> m_xSortedMap;
    map<Coordinate, std::string, CompareByY> m_ySortedMap;

    Cities getSquare(const Coordinate &, double);


    template <typename CoordinateType, typename MapType>
    void splitAndIntersect(MapType&, const MapType&,
                                 double, double) const;

    template <typename MapTypeA, typename MapTypeB>
    Cities getResult(const MapTypeA &mapA, const MapTypeB &mapB);


public:
    explicit Data(const std::string &);

    Result search(const std::string &, double, const Func &, int&);

    void addCity(const std::string &, double, double);

    void removeCity(const std::string &);

    void printData(Result r, int) const;
};
