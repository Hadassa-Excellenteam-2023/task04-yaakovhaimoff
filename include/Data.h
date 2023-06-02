#pragma once

#include "macros.h"
#include "Coordinates.h"
#include "SortStructs.h"
#include "Norms.h"

class Data {

    using SquareResult = multimap<Coordinate, string, CompareByX>;

    Cities m_cities;
    map<Coordinate, string, CompareByX> m_xSortedMap;
    map<Coordinate, string, CompareByY> m_ySortedMap;

    SquareResult getSquare(const Coordinate &, double);

    Result getCitiesInCircle(SquareResult, const string &, const Coordinate &, double, const Func &, int &);

    bool nameExists(const string &) const;

    Result getListOfCitiesInRadius(const string &, double, const Func &, int &);

    void printData(Result r, int) const;

    string readCityName();

    template<typename T>
    T readValue(T &value, const std::string &prompt, const std::string &errorMessage) {
        std::string input;
        std::cout << prompt << std::endl;
        std::getline(std::cin, input);
        std::istringstream iss(input);
        if (!(iss >> value) || !iss.eof()) {
            throw std::runtime_error(errorMessage);
        }
        return value;
    }

public:

    explicit Data(const string &);

    void run();

    void addCity(const string &, double, double);

    void removeCity(const string &);
};
