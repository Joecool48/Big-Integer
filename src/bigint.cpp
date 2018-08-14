#include "../include/bigint.h"
#include <vector>
bigint::bigint() {
    cells = new std::vector<cell_t>();
    cells->push_back(0);
    m_sign = POS;
    m_maxSetIndex = 0;
}
bigint::bigint(long value) {
    cells = new std::vector<cell_t>();
    if (value == 0) cells->push_back(0);
    //cells->push_back(value % 10);
    byte_t index = 0;
    m_sign = POS;
    if (value < 0) {
        m_sign = NEG;
        value *= -1;
    }
    while (value > 0) {
        std::cout << (int) value << std::endl;
        set_digit(index, value % 10);
        value /= 10;
        index++;
    }
    if (index > 0) index -= 1;
    m_maxSetIndex = index;
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
    byte_t ret_val = STATUS_OK;
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
        bitmask = ~bitmask;
        digit = digit | (value << digitShift);
        *(cells->begin() + cellNum) = digit;
    }
    if (index > m_maxSetIndex) m_maxSetIndex = index;
    return ret_val;
}

void bigint::printDigits() const {
    byte_t digit;
    std::cout << "Max = " << m_maxSetIndex << std::endl;
    cell_t index = m_maxSetIndex;
    if (m_sign == NEG) {
        std::cout << "-";
    }
    while (index >= 0) {
        digit = get_digit(index);
        std::cout << (int) digit;
        if (index == 0) break;
        index--;
    }
    std::cout << std::endl;
}

byte_t bigint::from_string(std::string str) {
    if (str.empty()) return ERROR_INVALID_DIGIT;
    std::cout << str.size() << std::endl;
    m_sign = POS;
    cell_t end = 0;
    cell_t place = 0;
    if (str[0] == '-') {
        m_sign = NEG;
        end = 1;
    }
    else if (str[0] == '+') {
        end = 1;
    }
    for (cell_t i = str.size() - 1; i >= end; i--, place++) {
        if ((str[i] - '0') < 0 || (str[i] - '0') > 9) return ERROR_INVALID_DIGIT;
        else {
            set_digit(place, str[i] - '0');
        }
    }
    std::cout << std::endl;
    return STATUS_OK;
}

bigint bigint::operator+(bigint const &other) const {
    bigint bint;
    cell_t maxIndex = max(m_maxSetIndex, other.m_maxSetIndex);
    byte_t digita, digitb;
    byte_t carry = 0;
    cell_t i = 0;
    while (i <= maxIndex || carry != 0) {
        if ((digita = get_digit(i)) == ERROR_OUT_OF_BOUNDS) digita = 0;
        if ((digitb = other.get_digit(i)) == ERROR_OUT_OF_BOUNDS) digitb = 0;
        if (digita < 0 || digita > 9 || digitb < 0 || digitb > 9) {std::cout << "Break " << ((int) digita) << std::endl; break;}
        //std::cout << "result = " << (int)result << " index = " << (int)i << std::endl;
        if (m_sign == NEG) digita *= -1;
        if (other.m_sign == NEG) digitb *= -1;
        byte_t result = digita + digitb + carry;
        if (result < 0) {
            carry = -1;
            bint.set_digit(i, 10 - abs(result));
            bint.m_sign = NEG;
        }
        // else if (result < -9) {
        //     carry = -1;
        //     result += 10;
        //
        //     bint.set_digit(i, abs(result));
        //     bint.m_sign = NEG;
        // }
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
        i++;
    }
    bint.printDigits();
    return bint;
}
bigint bigint::operator-(bigint const &other) const {
    bigint bint = other;
    if (bint.m_sign == POS) bint.m_sign = NEG;
    else bint.m_sign = POS;
    return (bint + *this);
}
bigint bigint::operator*(bigint const &other) const {
    bigint bint(0);
    byte_t digita, digitb;
    cell_t itera = 0, iterb = 0;

    while (itera < m_maxSetIndex) {
        while (iterb < other.m_maxSetIndex) {
            
        }
    }
}
bigint bigint::operator=(bigint const &other) const {
    bigint bint;
    for (cell_t i = 0; i < other.cells->size(); i++) {
        bint.cells->push_back(other.cells->at(i));
    }
    bint.m_sign = other.m_sign;
    return bint;
}
