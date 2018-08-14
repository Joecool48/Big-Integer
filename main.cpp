#include "include/bigint.h"

int main () {
    //bint1.from_string("-900");
    //bint2.from_string("500");
    //bigint bint;
    //bint = bint1 + bint2;
    long i = 30;
    long i2 = -29;
    bigint bint1(i);
    bigint bint2(i2);
    bigint bint = bint1 + bint2;
    bint.printDigits();
}
