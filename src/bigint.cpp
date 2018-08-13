#include "../include/bigint.h"
#include <vector>
bigint::bigint() {
    cells = new std::vector<cell_t>();
    cells->push_back(0);
    std::cout << cells->size() << std::endl;
    m_sign = POS;
    m_maxSetIndex = 0;
}

byte_t bigint::get_digit(cell_t index) const {
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
    std::cout << "Size = " << cells->size() << std::endl;
    cell_t ret_val = STATUS_OK;
    if (value < 0 || value > 9) return ERROR_INVALID_DIGIT;
    else if (index >= (cells->size() * DIGITS_PER_CELL)) { // If exceeds bounds, create new cell and add digit to it

        cells->push_back(value);
        std::cout << "New cell created" << std::endl;
    }
    else {
        std::cout << "Adding digit " << value << " at index " << index << std::endl;
        cell_t cellNum = index / DIGITS_PER_CELL;
        cell_t digitShift = (index % DIGITS_PER_CELL) * DIGIT_BIT_SIZE;
        cell_t bitmask = 0xf;
        bitmask = bitmask << digitShift;
        bitmask = ~bitmask; // Not it so that we can clear the current bit spot
        cell_t digit = cells->at(cellNum) & bitmask;
        std::cout << "Size = " << cells->size() << std::endl;
        bitmask = ~bitmask;
        digit = digit | (value << digitShift);
        *(cells->begin() + cellNum) = digit;
        std::cout << "Size = " << cells->size() << std::endl;
    }
    if (index > m_maxSetIndex) m_maxSetIndex = index;
    return ret_val;
}

void bigint::printDigits() const {
    byte_t digit;
    cell_t index = 0;
    if (m_sign == NEG) {
        std::cout << "-";
    }
    while (index <= m_maxSetIndex) {
        digit = get_digit(index);
        std::cout << (int) digit;
        index++;
    }
    std::cout << std::endl;
}

byte_t bigint::from_string(std::string str) {
    cell_t istr = 0;
    m_sign = POS;
    if (str[0] == '-') {
        m_sign = NEG;
        istr = 1;
    }
    else if (str[0] == '+') {
        istr = 1;
    }
    for (int i = 0; i < str.size() - 1; i++, istr++) {
        if ((str[istr] - '0') < 0 || (str[istr] - '0') > 9) std::cout << "from_string: invalid digit" << std::endl;
        else {
            set_digit(i, str[istr] - '0');
        }
    }
    return STATUS_OK;
}
bigint bigint::operator+(const bigint &other) const {
    bigint bint;
    cell_t maxIndex = max(m_maxSetIndex, other.m_maxSetIndex);
    byte_t digita, digitb;
    byte_t carry = 0;
    for (cell_t i = 0; i < maxIndex; i++) {
        if ((digita = get_digit(i)) == ERROR_OUT_OF_BOUNDS) digita = 0;
        if ((digitb = other.get_digit(i)) == ERROR_OUT_OF_BOUNDS) digitb = 0;
        if (m_sign == NEG) digita *= -1;
        if (other.m_sign == NEG) digitb *= -1;
        byte_t result = digita + digitb + carry;
        if (result < -9) {
            carry = -1;
            result += 10;

            bint.set_digit(i, abs(result));
            bint.m_sign = NEG;
        }
        else if (result > 9) {
            carry = 1;
            result -= 10;

            bint.set_digit(i, result);
            bint.m_sign = POS;
        }
        else {
            carry = 0;
            bint.set_digit(i, result);
            bint.m_sign = POS;
        }
    }
    return bint;
}
bigint bigint::operator=(const bigint &other) const {
    bigint bint;
    for (cell_t i = 0; i < other.cells->size(); i++) {
        bint.cells->push_back(other.cells->at(i));
    }
    bint.m_sign = other.m_sign;
    return bint;
}
