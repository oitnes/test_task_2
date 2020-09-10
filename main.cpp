#include "test.h"

#include <iostream>
#include <string>


int main(int arvc, char **argv) {

    if (arvc != 2) {
        std::cout << "input arguments is not present";
        return EXIT_FAILURE;
    }

    std::chrono::seconds statistic_time;
    try {
        auto input_integer = std::stoi(argv[1]);
        if (input_integer <= 0) {
            std::cout << "invalid input argument";
            return EXIT_FAILURE;
        }

        statistic_time = std::chrono::seconds{input_integer};
    } catch (...) {
        std::cout << "could not parse input arguments";
        return EXIT_FAILURE;
    }

    std::cout << "start test for " << std::to_string(statistic_time.count()) << " seconds " << std::endl;
    test(statistic_time);

    return EXIT_SUCCESS;
}
