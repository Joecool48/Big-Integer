#include "include/bigint.h"

int main () {
    bigint bint1;
    bigint bint2;
    bint1.from_string("9");
    bint2.from_string("9");
    bigint bint;
    bint = bint1 + bint2;
    bint.printDigits();
}
