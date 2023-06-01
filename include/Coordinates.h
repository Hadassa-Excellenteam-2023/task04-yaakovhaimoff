#pragma once

struct Coordinate {
    double x;
    double y;

    Coordinate(double x, double y) : x(x), y(y) {}
};

//struct CompareByX {
//    bool operator()(const Coordinate &lhs, const Coordinate &rhs) const { return lhs.x < rhs.x; }
//};
//
//struct CompareByY {
//    bool operator()(const Coordinate &lhs, const Coordinate &rhs) const { return lhs.y < rhs.y; }
//};
