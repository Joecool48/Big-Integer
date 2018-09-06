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
    if (index >= (cells->size() * DIGITS_PER_CELL)) { // If exceeds bounds, create new cell and add digit to it
        ret_val = STATUS_NEW_CELL_CREATED;
        while (index >= (cells->size() * DIGITS_PER_CELL)) {
            cells->push_back(0);
        }
        std::cout << "New cell created" << std::endl;
    }

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

    if (index > m_maxSetIndex && get_digit(index) != 0) m_maxSetIndex = index;
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
    // Clear the array
    cells->clear();
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
    m_maxSetIndex = str.size();
    //std::cout << std::endl;
    return STATUS_OK;
}
// Under construction
byte_t bigint::shift_digits(long amount) {
    bigint bint;
    for (cell_t i = 0; i < m_maxSetIndex; i++) {
        if (i + amount >= 0) {
            bint.set_digit(i + amount, get_digit(i));
        }
    }
}
// Fails to work when a positive and negative add to a negative.
// Otherwise it works fine.
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
        if (result < 0 && result >= -9) {
            carry = -1;
            bint.set_digit(i, 10 - abs(result));
            bint.m_sign = NEG;
        }
        else if (result < -9) {
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
        i++;
    }
    return bint;
}
bigint bigint::operator+=(bigint const &other) const {
    bigint bint = *this + other;
    return bint;
}
bigint bigint::operator-(bigint const &other) const {
    bigint bint = other;
    if (bint.m_sign == POS) bint.m_sign = NEG;
    else bint.m_sign = POS;
    return (bint + *this);
}
bigint bigint::operator*(bigint const &other) const {
    byte_t digita, digitb;
    if ((m_sign == NEG && other.m_sign == NEG) || (m_sign == POS && other.m_sign == POS)) {
        bint.m_sign = POS;
    }
    else {
        bint.m_sign = NEG;
    }
    cell_t num1_index = 0, num2_index = 0;
    bigint result(0);
    while (num1_index < m_maxSetIndex) {
        num2_index = 0;
        while (num2_index < other.m_maxSetIndex) {
            
            num2_index++;
        }
        num1_index++;
    }
    // while (itera < m_maxSetIndex) {
    //     while (iterb < other.m_maxSetIndex) {
    //         digita = get_digit(itera);
    //         digitb = other.get_digit(iterb);
    //         bigint temp(0);

    //     }
    // }
    // Mult by repeated addition
}

bool operator==(bigint const &bint1, bigint const &bint2) {
    cell_t index = 0;
    byte_t digita, digitb;
    if (bint1.m_maxSetIndex != bint2.m_maxSetIndex) return false;
    if (bint1.m_sign != bint2.m_sign) return false;
    while (index < bint1.m_maxSetIndex) {
        digita = bint1.get_digit(index);
        digitb = bint2.get_digit(index);
        index++;
        if (digita != digitb) return false;
    }
    return true;
}
bool operator!=(bigint const &bint1, bigint const &bint2) {
    return !(bint1 == bint2);
}
bool operator>(bigint const &bint1, bigint const &bint2) {
    cell_t index = 0;
    byte_t digita, digitb;
    if (bint1.m_sign == bigint::POS && bint2.m_sign == bigint::NEG) return true;
    if (bint1.m_sign == bigint::NEG && bint2.m_sign == bigint::POS) return false;
    if (bint1.m_maxSetIndex > bint2.m_maxSetIndex) return true;
    else if (bint1.m_maxSetIndex < bint2.m_maxSetIndex) return false;
    else {
        while (index < bint1.m_maxSetIndex) {
            if (digita < digitb) return false;
            index++;
        }
    }
    return true;
}
bool operator<(bigint const &bint1, bigint const &bint2) {
    return bint2 > bint1;
}
bool operator<=(bigint const &bint1, bigint const &bint2) {
    return ((bint1 < bint2) || (bint1 == bint2));
}
bool operator>=(bigint const &bint1, bigint const &bint2) {
    return ((bint1 > bint2) || bint1 == bint2);
}
std::ostream& operator<<(std::ostream& out, bigint const &bint) {
    byte_t digit;
    cell_t index = bint.m_maxSetIndex;
    if (bint.m_sign == bigint::NEG) {
        out << "-";
    }
    while (index >= 0) {
        digit = bint.get_digit(index);
        out << (int) digit;
        if (index == 0) break;
        index--;
    }
    return out;
}
// Doesnt print the right amount of digits (prints leading zeros)
std::istream& operator>>(std::istream& in, bigint &bint) {
    std::string str;
    in >> str;
    bint.from_string(str);
    return in;
}
// = operator is broken currently
bigint bigint::operator=(bigint const &other) {
    if (this == &other) return *this;
    this->cells->clear();
    for (cell_t i = 0; i < other.cells->size(); i++) {
        this->cells->push_back(other.cells->at(i));
    }
    this->m_sign = other.m_sign;
    this->m_maxSetIndex = other.m_maxSetIndex;
    return *this;
}
