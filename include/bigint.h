#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#ifndef BIGINT_H
#define BIGINT_H
typedef uint64_t cell_t;
typedef char byte_t;
class bigint {
public:
    std::vector<cell_t> *cells;
    enum STATUS {
        ERROR = -127,
        ERROR_OUT_OF_BOUNDS,
        ERROR_INVALID_DIGIT,
        STATUS_OK,
        STATUS_NEW_CELL_CREATED

    };
    enum SIGN {
        POS = 0,
        NEG
    };
    byte_t m_sign;
    const static int CELL_BIT_SIZE = 64;
    const static int DIGIT_BIT_SIZE = 4;
    const static int DIGITS_PER_CELL = 16;
    cell_t m_maxSetIndex;
    bigint();
    bigint(long value);
    bigint operator +(const bigint &other) const;
    bigint operator -(const bigint &other);
    bigint operator *(const bigint &other);
    bigint operator /(const bigint &other);
    bigint operator %(const bigint &other);
    bigint operator =(const bigint &other) const;
    byte_t get_digit(cell_t index) const;
    byte_t set_digit(cell_t index, cell_t value);
    void printDigits() const;

    byte_t from_string(std::string str);
};
inline cell_t max(cell_t a, cell_t b) {
    if (a > b) return a;
    return b;
}
inline cell_t min(cell_t a, cell_t b) {
    if (a > b) return b;
    else return a;
}
inline byte_t abs(byte_t a) {
    if (a < 0) a *= -1;
    return a;
}
#endif /*BIGINT_H*/
