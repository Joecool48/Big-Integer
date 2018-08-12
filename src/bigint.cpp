#include "../include/bigint.h"

bigint::bigint() {
    cells = new std::vector<cell_t>();
    cells->push_back(0);

}

byte_t bigint::get_digit(cell_t index) {
    if (index >= cells->size() * DIGITS_PER_CELL) { // If out of bounds return error
        return ERROR_OUT_OF_BOUNDS;
    }
    cell_t cellNum = index / DIGITS_PER_CELL;
    cell_t digitShift = (index % DIGITS_PER_CELL) * DIGIT_BIT_SIZE;
    cell_t bitmask = 0xf;
    bitmask = bitmask << digitShift;
    cell_t digit = (cells->at(cellNum) & bitmask) >> digitShift;
    return (byte_t) digit;
}

byte_t bigint::set_digit(cell_t index, cell_t value) {
    cell_t ret_val = STATUS_OK;
    if (value < 0 || value > 9) return ERROR_INVALID_DIGIT;
    else if (index >= (cells->size() * DIGITS_PER_CELL)) { // If exceeds bounds, create new cell and add digit to it
        ret_val = STATUS_NEW_CELL_CREATED;
        cells->insert(cells->begin(), value);
    }
    else {
        cell_t cellNum = index / DIGITS_PER_CELL;
        cell_t digitShift = (index % DIGITS_PER_CELL) * DIGIT_BIT_SIZE;
        cell_t bitmask = 0xf;
        bitmask = bitmask << digitShift;
        bitmask = ~bitmask; // Not it so that we can clear the current bit spot
        cell_t digit = cells->at(cellNum) & bitmask;
        bitmask = ~bitmask;
        digit = digit | (value << digitShift);
        cells->assign(cellNum, digit);
    }
    return ret_val;
}

void bigint::printDigits() {
    byte_t digit;
    cell_t index = 0;
    while ((digit = get_digit(index)) != ERROR_OUT_OF_BOUNDS) {
        std::cout << (int) digit;
    }
}
