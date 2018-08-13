#include "include/bigint.h"

int main () {
    bigint bint1;
    bigint bint2;
    bint1.from_string("-900");
    bint2.from_string("500");
    bigint bint;
    bint = bint1 + bint2;
    bint.printDigits();
}
