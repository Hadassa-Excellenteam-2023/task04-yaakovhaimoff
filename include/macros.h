#pragma once

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <functional>
#include <set>

using std::map;
using std::pair;
using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::ifstream;
using std::multimap;
using std::abs;
using std::set;

using cities = std::map<std::string, std::pair<double, double>>;
using Func = std::function<double(pair<double, double>, pair<double, double>)>;
using SearchFunc = std::map<int, Func>;