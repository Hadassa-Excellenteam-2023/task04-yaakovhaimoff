#pragma once

#include "macros.h"
#include "Coordinates.h"
#include "SortStructs.h"

class Data {

    using SquareResult = multimap<Coordinate, string, CompareByX>;

    Cities m_cities;
    map<Coordinate, string, CompareByX> m_xSortedMap;
    map<Coordinate, string, CompareByY> m_ySortedMap;

    SquareResult getSquare(const Coordinate &, double);

    Result getCitiesInCircle(SquareResult, const string&, const Coordinate &, double, const Func &, int&);

public:
    explicit Data(const string &);

    Result getListOfCitiesInRadius(const string &, double, const Func &, int&);

    void addCity(const string &, double, double);

    void removeCity(const string &);

    void printData(Result r, int) const;
};
