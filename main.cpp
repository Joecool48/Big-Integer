#include "include/bigint.h"

int main () {
    //bint1.from_string("-900");
    //bint2.from_string("500");
    //bigint bint;
    //bint = bint1 + bint2;
    long i = 1234567899999999999;
    bigint bint1(i);
    bigint bint2(i);
    bigint bint = bint1 + bint2;
    bint.printDigits();
}
