#include "macros.h"
#include "Data.h"

int main() {
    try {
        auto data = Data(fileName);
        data.run();
    }
    catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}