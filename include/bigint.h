#include <stdio.h>
#include <stdint>
#include <stdlib.h>
#include <iostream>
#include <vector>


class bigint {
    typedef uint64_t cell_t;
    typedef char byte_t;
    std::vector<cell_t> *cells;
    enum STATUS {
        ERROR = -127,
        ERROR_OUT_OF_BOUNDS,
        ERROR_INVALID_DIGIT,
        STATUS_OK,
        STATUS_NEW_CELL_CREATED

    }
    const static int CELL_BIT_SIZE = 64;
    const static int DIGIT_BIT_SIZE = 4;
    const static int DIGITS_PER_CELL = 16;

    bigint();
    bigint(long value);
    bigint operator +(bigint &other);
    bigint operator -(bigint &other);
    bigint operator *(bigint &other);
    bigint operator /(bigint &other);
    bigint operator %(bigint &other);

    byte_t shift_cells(int amount);

    byte_t get_digit(cell_t index);
    byte_t set_digit(cell_t index);
};
