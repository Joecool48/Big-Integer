#include "include/bigint.h"

int main () {
    //bint1.from_string("-900");
    //bint2.from_string("500");
    //bigint bint;
    //bint = bint1 + bint2;
    long i = 4654321;
    long i2 = 5654321;
    bigint bint1(i);
    bigint bint2(i2);
    if (i != i2) std::cout << "True" << std::endl;
    else std::cout << "False" << std::endl;
    //bint1 = bint2;
    std::cout << bint1 << std::endl;
    std::cin >> bint1;
    std::cout << bint1 << std::endl;
}
