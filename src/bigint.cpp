#include "../include/bigint.h"

bigint::bigint() {
    cells = new std::vector<cells_t>();
    cells.push_back(0);

}

byte_t bigint::shift_cells(int amount) {
    if (cells.size() == 0) {
        return -1;
    }
}

byte_t bigint::get_digit(cell_t index) {
    if (index >= cells.size() * DIGITS_PER_CELL) { // If out of bounds return error
        return ERROR_OUT_OF_BOUNDS;
    }
    cell_t cellNum = index / DIGITS_PER_CELL;
    cell_t digitShift = (index % DIGITS_PER_CELL) * DIGIT_BIT_SIZE;
    cell_t bitmask = 0xf;
    bitmask = bitmask << digitShift;
    cell_t digit = (cells[cellNum] & bitmask) >> digitShift;
    return (byte_t) digit;
}

byte_t bigint::set_digit(cell_t index, cell_t value) {
    cell_t ret_val = STATUS_OK;

    if (index >= cells.size() * DIGITS_PER_CELL) { // If exceeds bounds, create new cell and add digit to it
        ret_val = STATUS_NEW_CELL_CREATED;
        if (value >= 0 && value <= 9) {
            cells.push_front(value)
        }
        else {
            return ERROR_INVALID_DIGIT;
        }
    }
}
