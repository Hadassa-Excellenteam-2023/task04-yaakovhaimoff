#pragma once

#include "macros.h"

class Norm {
    SearchFunc m_normFunctions;
    Norm();

public:
    Norm(const Norm &) = delete; // Delete copy constructor
    Norm &operator=(const Norm &) = delete; // Delete copy assignment operator


    static Norm &getInstance();

    // Euclidean distance (L2 norm)
    static double euclideanDistance(std::pair<double, double> q1, std::pair<double, double> q2);

    // Infinity norm (L-infinity norm)
    static double infinityNorm(std::pair<double, double> q1, std::pair<double, double> q2);

    // Manhattan distance (L1 norm)
    static double manhattanDistance(std::pair<double, double> q1, std::pair<double, double> q2);

    // Search for a norm function based on the key
    static Func searchNormFunction(int key);
};