#pragma once

#include <iostream>
#include <cinttypes>

const int kDecimalFractionalPartSize = 4;
const int kMaxDecimalFractionalPartSize = 9;
const int kBitsInByte = 8;
static const char* kHexDigits = "0123456789abcdef";

class FixedPointNumber;

int Pow(int a, int b);
FixedPointNumber Abs(const FixedPointNumber& number);
uint32_t Round(uint64_t bytes, int rounding_type, int not_integer_part_size, bool is_result_negative = false);

class FixedPointNumber {
public:
    int rounding_type = 0;

    FixedPointNumber() = default;
    FixedPointNumber(int a, int integer_part_size, int not_integer_part_size);
    FixedPointNumber(uint32_t a, int integer_part_size, int not_integer_part_size);

    static FixedPointNumber FromHex(int integer_part_size, int not_integer_part_size, const char* string);

    uint32_t GetBytes() const;
    int GetIntegerPartSize() const;
    int GetNotIntegerPartSize() const;
    int GetRoundingType() const;
    bool IsNegative() const;
    void Crop();

    friend FixedPointNumber& operator++(FixedPointNumber& first);
    friend FixedPointNumber operator+(const FixedPointNumber& first, const FixedPointNumber& second);
    friend FixedPointNumber operator-(const FixedPointNumber& first, const FixedPointNumber& second);
    friend FixedPointNumber operator*(const FixedPointNumber& first, const FixedPointNumber& second);
    friend FixedPointNumber operator/(const FixedPointNumber& first, const FixedPointNumber& second);
    friend bool operator==(const FixedPointNumber& first, const FixedPointNumber& second);
private:
    int integer_part_size = 1;
    int not_integer_part_size = 0;
    uint32_t bytes = 0;
};

std::ostream& operator<<(std::ostream& stream, FixedPointNumber number);

FixedPointNumber& operator++(FixedPointNumber& first);
FixedPointNumber operator-(const FixedPointNumber& first);
FixedPointNumber operator~(const FixedPointNumber& first);
FixedPointNumber operator+(const FixedPointNumber& first, const FixedPointNumber& second);
FixedPointNumber operator-(const FixedPointNumber& first, const FixedPointNumber& second);
FixedPointNumber operator*(const FixedPointNumber& first, const FixedPointNumber& second);
FixedPointNumber operator/(const FixedPointNumber& first, const FixedPointNumber& second);
bool operator==(const FixedPointNumber& first, const FixedPointNumber& second);



