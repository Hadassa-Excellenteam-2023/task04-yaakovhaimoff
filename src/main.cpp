#include "macros.h"
#include "Data.h"
#include "Norms.h"

int main() {
//    data.txt
//    Yoncalla, OR

    string filename, city;
    double radius;
    int searchWay;

    cout << "type file name:" << std::endl;
    cin >> filename;
    auto data = Data(filename);

    while (true) {
        cout << "Please enter selected city name (with line break after it):" << std::endl;
        cin.ignore(); // Ignore any previous newline character
        getline(cin, city);

        if (city == "0") {
            break; // Exit the loop if city name is '0'
        }

        cout << "Please enter the wanted radius:" << std::endl;
        cin >> radius;

        cout << "Please enter the wanted norm (0 – L2, Euclidean distance, 1 – Linf,"
                " Chebyshev distance, 2 – L1, Manhattan distance):"
             << std::endl;
        cin >> searchWay;
        Func normFunc = Norm::searchNormFunction(searchWay);

        if (normFunc) {
            int counter = 0;
            auto result = data.getListOfCitiesInRadius(city, radius, normFunc, counter);
            data.printData(result, counter);
        } else {
            std::cout << "Invalid choice!" << std::endl;
        }
    }

    return 0;
}