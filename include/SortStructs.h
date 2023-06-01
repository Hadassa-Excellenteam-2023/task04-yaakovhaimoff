#pragma once

#include "macros.h"
#include "Coordinates.h"

static bool SortByValue(const pair<string, double> &a, const pair<string, double> &b) {
    return a.second < b.second;
}


static bool SortSquare(const pair<Coordinate, string> &a, const pair<Coordinate, string> &b) {
    return a.first.x < b.first.x;
}


struct CompareByX {
    bool operator()(const Coordinate &lhs, const Coordinate &rhs) const { return lhs.x < rhs.x; }
};

struct CompareByY {
    bool operator()(const Coordinate &lhs, const Coordinate &rhs) const { return lhs.y < rhs.y; }
};