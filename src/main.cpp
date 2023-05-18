#include "macros.h"
#include "Data.h"
#include "Norms.h"

int main() {
//    data.txt
//    Yoncalla, OR
//    100 1
    string filename, city;
    double radius;
    int searchWay;

    cout << "type file name:" << std::endl;
    cin >> filename;
    auto data = Data(filename);

    cout << "Type city name:" << std::endl;
    cout << "Type radius:" << std::endl;
    cout << "Type search way:" << std::endl
         << "0 - L2 norm (Euclidean distance)" << std::endl
         << "1 - Infinity norm" << std::endl
         << "2 - L1 norm (Manhattan distance)" << std::endl;

    cin.ignore(); // Ignore any previous newline character
    getline(cin, city);
    cin >> radius;
    cin >> searchWay;


    Func normFunc = Norm::searchNormFunction(searchWay);

    if (normFunc) {
        auto result = data.search(city, radius, normFunc);
        data.printData(result);
    } else {
        std::cout << "Invalid choice!" << std::endl;
    }

    return 0;
}