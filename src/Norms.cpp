#include "Norms.h"

Norm::Norm() {
    m_normFunctions[0] = euclideanDistance;
    m_normFunctions[1] = infinityNorm;
    m_normFunctions[2] = manhattanDistance;
}

Norm &Norm::getInstance() {
    static auto instance = Norm();
    return instance;
}

double Norm::euclideanDistance(std::pair<double, double> q1, std::pair<double, double> q2) {
    return std::sqrt(std::pow((q1.first - q2.first), 2) + std::pow((q1.second - q2.second), 2));
}

double Norm::infinityNorm(std::pair<double, double> q1, std::pair<double, double> q2) {
    return std::max(std::abs(q1.first - q2.first), std::abs(q1.second - q2.second));
}

double Norm::manhattanDistance(std::pair<double, double> q1, std::pair<double, double> q2) {
    return std::abs(q1.first - q2.first) + std::abs(q1.second - q2.second);
}

Func Norm::searchNormFunction(int key) {
    auto &instance = getInstance();
    auto normFuncIt = instance.m_normFunctions.find(key);
    if (normFuncIt != instance.m_normFunctions.end()) {
        return normFuncIt->second;
    } else {
        return nullptr;
    }
}
