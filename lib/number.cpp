#include "number.h"


int Pow(int a, int b) {
    int t = 1;
    for(; b > 0; b--) {
        t *= a;
    }
    return t;
}

FixedPointNumber Abs(const FixedPointNumber& number) {
    if (number.IsNegative()) {
        return -number;
    } else {
        return number;
    }
}

uint32_t Round(uint64_t bytes, int rounding_type, int not_integer_part_size, bool is_result_negative) {
    if ((bytes & ((1 << not_integer_part_size) - 1)) == 0) {
        return bytes >> not_integer_part_size;
    }
    uint64_t not_integer_part = (bytes & ((1 << not_integer_part_size) - 1));
    uint64_t half = 1 << (not_integer_part_size - 1);
    switch (rounding_type) {
        case 1: // nearest even
            if (not_integer_part > half) {
                return (bytes >> not_integer_part_size) + 1;
            } else if (not_integer_part < half) {
                return (bytes >> not_integer_part_size);
            } else {
                bool is_ending_odd = (bytes >> not_integer_part_size) & 1;
                if (is_ending_odd) {
                    return (bytes >> not_integer_part_size) + 1;
                } else {
                    return (bytes >> not_integer_part_size);
                }
            }
        case 2: // toward positive infinity
            if (!is_result_negative) {
                return (bytes >> not_integer_part_size) + 1;
            } else {
                return bytes >> not_integer_part_size;
            }
        case 3: // toward negative infinity
            if (is_result_negative) {
                return (bytes >> not_integer_part_size) + 1;
            } else {
                return bytes >> not_integer_part_size;
            }
        default: // default and toward zero
            return bytes >> not_integer_part_size;
    }
}

FixedPointNumber FixedPointNumber::FromHex(int integer_part_size, int not_integer_part_size, const char* string) {
    FixedPointNumber number;
    number.integer_part_size = integer_part_size;
    number.not_integer_part_size = not_integer_part_size;

    while (*string != 0) {
        number.bytes <<= 4;
        int i = 0;
        while (kHexDigits[i] != tolower(*string)) {
            i++;
            if (i >= 16) {
                i = 0;
                break;
            }
        }
        number.bytes += i;
        string++;
    }
    number.Crop();
    return number;
}
uint32_t FixedPointNumber::GetBytes() const {
    return bytes;
}

int FixedPointNumber::GetIntegerPartSize() const {
    return integer_part_size;
}

int FixedPointNumber::GetNotIntegerPartSize() const {
    return not_integer_part_size;
}

FixedPointNumber& operator++(FixedPointNumber& first) {
    first.bytes++;
    first.Crop();
    return first;
}

FixedPointNumber operator-(const FixedPointNumber& first) {
    FixedPointNumber result = ~first;
    ++result;
    result.Crop();
    result.rounding_type = first.rounding_type;
    return result;
}

FixedPointNumber operator~(const FixedPointNumber& first) {
    FixedPointNumber result(~first.GetBytes(), first.GetIntegerPartSize(), first.GetNotIntegerPartSize());
    result.Crop();
    result.rounding_type = first.rounding_type;
    return result;
}

FixedPointNumber::FixedPointNumber(int a, int integer_part_size, int not_integer_part_size) {
    this->integer_part_size = integer_part_size;
    this->not_integer_part_size = not_integer_part_size;
    bytes = a;
}
FixedPointNumber::FixedPointNumber(uint32_t a, int integer_part_size, int not_integer_part_size) {
    this->integer_part_size = integer_part_size;
    this->not_integer_part_size = not_integer_part_size;
    bytes = a;
}

FixedPointNumber operator+(const FixedPointNumber& first, const FixedPointNumber& second) {
    FixedPointNumber result(0, first.GetIntegerPartSize(), first.GetNotIntegerPartSize());
    result.bytes = first.GetBytes() + second.GetBytes();
    result.Crop();
    result.rounding_type = first.rounding_type;
    return result;
}

void FixedPointNumber::Crop() {
    bytes &= static_cast<uint32_t>((static_cast<int64_t>(1) << (integer_part_size + not_integer_part_size)) - 1);
}
FixedPointNumber operator-(const FixedPointNumber& first, const FixedPointNumber& second) {
    return first + (-second);
}
FixedPointNumber operator*(const FixedPointNumber& first, const FixedPointNumber& second) {
    uint64_t result_bytes = Abs(first).GetBytes();
    result_bytes *= Abs(second).GetBytes();

    FixedPointNumber result = FixedPointNumber(
        Round(
            result_bytes,
            first.GetRoundingType(),
            first.GetNotIntegerPartSize(),
            first.IsNegative() ^ second.IsNegative()
            ),
        first.GetIntegerPartSize(),
        first.GetNotIntegerPartSize()
        );
    if (first.IsNegative() ^ second.IsNegative()) {
        result = -result;
    }
    result.Crop();
    result.rounding_type = first.rounding_type;
    return result;
}

FixedPointNumber operator/(const FixedPointNumber& first, const FixedPointNumber& second) {
    uint64_t first_bytes = Abs(first).GetBytes();
    first_bytes <<= first.GetNotIntegerPartSize();
    uint64_t result_bytes = first_bytes / Abs(second).GetBytes();

    FixedPointNumber result = FixedPointNumber(
        static_cast<uint32_t>(result_bytes),
        first.GetIntegerPartSize(),
        first.GetNotIntegerPartSize()
    );
    if (first.IsNegative() ^ second.IsNegative()) {
        result = -result;
    }
    result.Crop();
    result.rounding_type = first.rounding_type;
    return result;
}

int FixedPointNumber::GetRoundingType() const {
    return rounding_type;
}
bool FixedPointNumber::IsNegative() const {
    return bytes >> (integer_part_size + not_integer_part_size - 1);
}


bool operator==(const FixedPointNumber& first, const FixedPointNumber& second) {
    return first.bytes == second.bytes && first.not_integer_part_size == second.not_integer_part_size;
}


std::ostream& operator<<(std::ostream& stream, FixedPointNumber number) {
    uint64_t bytes = Abs(number).GetBytes();

    if (number.IsNegative()) {
        stream << '-';
    }
    bytes *= 1000;
    uint64_t remainder = bytes & ((static_cast<uint64_t>(1) << number.GetNotIntegerPartSize()) - 1);
    uint64_t half = static_cast<uint64_t>(1) << (number.GetNotIntegerPartSize() - 1);

    uint64_t bytes_to_out;

    switch (number.rounding_type) {
        case 1: // nearest even
            if (remainder > half) {
                bytes_to_out = (bytes >> number.GetNotIntegerPartSize()) + 1;
            } else if (remainder < half) {
                bytes_to_out = bytes >> number.GetNotIntegerPartSize();
            } else {
                bool is_ending_odd = (bytes >> number.GetNotIntegerPartSize()) & 1;
                if (is_ending_odd) {
                    bytes_to_out = (bytes >> number.GetNotIntegerPartSize()) + 1;
                } else {
                    bytes_to_out = bytes >> number.GetNotIntegerPartSize();
                }
            }
            break;
        case 2: // toward positive infinity
            if (!number.IsNegative()) {
                bytes_to_out = (bytes >> number.GetNotIntegerPartSize()) + 1;
            } else {
                bytes_to_out = bytes >> number.GetNotIntegerPartSize();
            }
            break;
        case 3: // toward negative infinity
            if (number.IsNegative()) {
                bytes_to_out = (bytes >> number.GetNotIntegerPartSize()) + 1;
            } else {
                bytes_to_out = bytes >> number.GetNotIntegerPartSize();
            }
            break;
        default: // default and toward zero
            bytes_to_out = bytes >> number.GetNotIntegerPartSize();
    }

    stream << bytes_to_out / 1000 << '.' << bytes_to_out % 1000;

    return stream;
}

