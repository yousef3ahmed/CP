#include <iostream>

#include "stress test/generator/include/random_generator_lib.h"

int main()
{

    std::cout << random_flag() << "\n";
    std::cout << random_flag() << "\n";
    std::cout << random_flag() << "\n";

    return 0;
}