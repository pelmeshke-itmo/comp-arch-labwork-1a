#include <lib/number.h>
#include <gtest/gtest.h>


double ToDouble(const FixedPointNumber& num){
    FixedPointNumber number = Abs(num);
    
    int not_integer_part_size = num.GetNotIntegerPartSize();
    double integer_part = number.GetBytes() >> not_integer_part_size;
    uint32_t not_integer_part = number.GetBytes() & ((1 << not_integer_part_size) - 1);
    double fractional_part = 0;
    for (int i = 1; i < not_integer_part_size + 1; i++) {
        int bit = (not_integer_part >> (not_integer_part_size - i)) & 1;
        if (!bit) {
            continue;
        }
        fractional_part += 1.0 / (1 << i);
    }

    double result = integer_part + fractional_part;
    if (num.IsNegative()) {
        result *= -1;
    }
    return result;
}

TEST(CreationTest, Test1) {
    FixedPointNumber number = FixedPointNumber::FromHex(8, 8, "0x16D4");

    ASSERT_EQ(number.GetBytes(), 5844);
    ASSERT_EQ(ToDouble(number), 22.828125);
}

TEST(CreationTest, Test2) {
    FixedPointNumber number = FixedPointNumber::FromHex(16, 12, "0x17360");

    ASSERT_EQ(number.GetBytes(), 95072);
    ASSERT_EQ(ToDouble(number), 23.2109375);
}

TEST(NegationTest, Test1) {
    FixedPointNumber number = FixedPointNumber::FromHex(16, 8, "0x1736");

    ASSERT_EQ(number.GetBytes(), 5942);

    number = -number;
    ASSERT_EQ(ToDouble(number), -23.2109375);
}

TEST(NegationTest, Test2) {
    FixedPointNumber number = FixedPointNumber::FromHex(8, 8, "0x0");

    ASSERT_EQ(number.GetBytes(), 0);

    ASSERT_EQ(ToDouble(number), 0);
    number = -number;
    ASSERT_EQ(ToDouble(number), 0);
}

TEST(AdditionTest, Test1) {
    FixedPointNumber first = FixedPointNumber::FromHex(8, 8, "0xdc9f");
    FixedPointNumber second = FixedPointNumber::FromHex(8, 8, "0xd736");

    FixedPointNumber result = first + second;
    ASSERT_EQ(ToDouble(result), -76.16796875);
}

TEST(AdditionTest, Test2) {
    FixedPointNumber first = FixedPointNumber::FromHex(8, 8, "0xdc9f");
    FixedPointNumber second = FixedPointNumber::FromHex(8, 8, "0");

    FixedPointNumber result = first + second;
    ASSERT_EQ(ToDouble(result), -35.37890625);
}

TEST(AdditionTest, Test3) {
    FixedPointNumber first = FixedPointNumber::FromHex(8, 8, "0xdc9f");
    FixedPointNumber second = FixedPointNumber::FromHex(8, 8, "0xdc9f");

    FixedPointNumber result = first - second;
    ASSERT_EQ(ToDouble(result), 0.0);
}

TEST(AdditionTest, Test4) {
    FixedPointNumber first = FixedPointNumber::FromHex(8, 8, "0xdc9f");
    FixedPointNumber second = -FixedPointNumber::FromHex(8, 8, "0xdc9f");

    FixedPointNumber result = first + second;
    ASSERT_EQ(ToDouble(result), 0.0);
}

TEST(AdditionTest, Test5) {
    FixedPointNumber first = FixedPointNumber::FromHex(8, 8, "0xdd9f");
    FixedPointNumber second = -FixedPointNumber::FromHex(8, 8, "0xdc9f");

    FixedPointNumber result = first + second;
    ASSERT_EQ(ToDouble(result), 1.0);
}

TEST(MultiplicationTest, Test1) {
    FixedPointNumber first = FixedPointNumber::FromHex(8, 8, "0x0200");
    FixedPointNumber second = FixedPointNumber::FromHex(8, 8, "0x0300");

    FixedPointNumber result = first * second;
    ASSERT_EQ(ToDouble(result), 6.0);
}

TEST(MultiplicationTest, Test2) {
    FixedPointNumber first = FixedPointNumber::FromHex(8, 8, "0x0280");
    FixedPointNumber second = FixedPointNumber::FromHex(8, 8, "0xfc00");
    ASSERT_EQ(ToDouble(second), -4.0);

    FixedPointNumber result = first * second;
    ASSERT_EQ(ToDouble(result), -10.0);
}

TEST(MultiplicationTest, Test3) {
    FixedPointNumber first = FixedPointNumber::FromHex(8, 8, "0x0280");
    FixedPointNumber second = FixedPointNumber::FromHex(8, 8, "0xfc00");
    ASSERT_EQ(ToDouble(second), -4.0);

    FixedPointNumber result = first * second;
    ASSERT_EQ(ToDouble(result), -10.0);
}

TEST(MultiplicationTest, Test4) {
    FixedPointNumber first = FixedPointNumber::FromHex(8, 8, "0x10");
    FixedPointNumber second = FixedPointNumber::FromHex(8, 8, "0");

    FixedPointNumber result = first * second;
    ASSERT_EQ(ToDouble(result), 0.0);
}

TEST(MultiplicationTest, Test5) {
    FixedPointNumber first = FixedPointNumber::FromHex(12, 12, "0x5a76");
    FixedPointNumber second = FixedPointNumber::FromHex(12, 12, "0x2d57a");

    FixedPointNumber result = first * second;
    ASSERT_EQ(ToDouble(result), 256.3564453125);
}

TEST(RoundingTest, Test1) {
    FixedPointNumber first = FixedPointNumber::FromHex(12, 12, "0x5a76");

    first.rounding_type = 1;
    FixedPointNumber second = FixedPointNumber::FromHex(12, 12, "0x2d57a");

    FixedPointNumber result = first * second;
    ASSERT_EQ(ToDouble(result), 256.356689453125);
}

TEST(RoundingTest, Test2) {
    FixedPointNumber first = FixedPointNumber::FromHex(12, 12, "0x5a76");

    first.rounding_type = 2;
    FixedPointNumber second = FixedPointNumber::FromHex(12, 12, "0x2d57a");

    FixedPointNumber result = first * second;
    ASSERT_EQ(ToDouble(result), 256.356689453125);
}

TEST(RoundingTest, Test3) {
    FixedPointNumber first = FixedPointNumber::FromHex(12, 12, "0x5a76");

    first.rounding_type = 3;
    FixedPointNumber second = FixedPointNumber::FromHex(12, 12, "0x2d57a");

    FixedPointNumber result = first * second;
    ASSERT_EQ(ToDouble(result), 256.3564453125);
}

TEST(RoundingTest, Test4) {
    FixedPointNumber first = FixedPointNumber::FromHex(12, 12, "0x5a60");

    first.rounding_type = 1;
    FixedPointNumber second = FixedPointNumber::FromHex(12, 12, "0x2d540");

    // product will be XXXX.YYYYYYYYYYYY1
    // so it will be rounded to nearest even
    FixedPointNumber result = first * second;
    ASSERT_EQ(ToDouble(result), 256.033203125);
}

TEST(RoundingTest, Test5) {
    FixedPointNumber first = FixedPointNumber::FromHex(12, 12, "0x5a60");

    first.rounding_type = 2;
    FixedPointNumber second = FixedPointNumber::FromHex(12, 12, "0x2d540");

    FixedPointNumber result = first * second;
    ASSERT_EQ(ToDouble(result), 256.033203125);
}

TEST(RoundingTest, Test6) {
    FixedPointNumber first = FixedPointNumber::FromHex(12, 12, "0x5a60");

    first.rounding_type = 3;
    FixedPointNumber second = FixedPointNumber::FromHex(12, 12, "0x2d540");

    FixedPointNumber result = first * second;
    ASSERT_EQ(ToDouble(result), 256.032958984375);
}

TEST(RoundingTest, Test7) {
    FixedPointNumber first = FixedPointNumber::FromHex(12, 12, "0x5a60");

    first.rounding_type = 0;
    FixedPointNumber second = FixedPointNumber::FromHex(12, 12, "0x2d540");

    FixedPointNumber result = first * second;
    ASSERT_EQ(ToDouble(result), 256.032958984375);
}

TEST(RoundingNegativeTest, Test1) {
    FixedPointNumber first = FixedPointNumber::FromHex(12, 12, "0xffa59f");

    first.rounding_type = 0;
    FixedPointNumber second = FixedPointNumber::FromHex(12, 12, "0x2d540");

    FixedPointNumber result = first * second;
    ASSERT_EQ(ToDouble(result), -256.0439453125);
}

TEST(RoundingNegativeTest, Test2) {
    FixedPointNumber first = FixedPointNumber::FromHex(12, 12, "0xffa59f");

    first.rounding_type = 1;
    FixedPointNumber second = FixedPointNumber::FromHex(12, 12, "0x2d540");

    FixedPointNumber result = first * second;
    ASSERT_EQ(ToDouble(result), -256.044189453125);
}

TEST(RoundingNegativeTest, Test3) {
    FixedPointNumber first = FixedPointNumber::FromHex(12, 12, "0xffa59f");

    first.rounding_type = 2;
    FixedPointNumber second = FixedPointNumber::FromHex(12, 12, "0x2d540");

    FixedPointNumber result = first * second;
    ASSERT_EQ(ToDouble(result), -256.0439453125);
}

TEST(RoundingNegativeTest, Test4) {
    FixedPointNumber first = FixedPointNumber::FromHex(12, 12, "0xffa59f");

    first.rounding_type = 3;
    FixedPointNumber second = FixedPointNumber::FromHex(12, 12, "0x2d540");

    FixedPointNumber result = first * second;
    ASSERT_EQ(ToDouble(result), -256.044189453125);
}

TEST(DivisionTest, Test1) {
    FixedPointNumber first = FixedPointNumber::FromHex(12, 8, "0x0c00");
    FixedPointNumber second = FixedPointNumber::FromHex(12, 8, "0x0800");

    FixedPointNumber result = first / second;
    ASSERT_EQ(ToDouble(result), 1.5);
}

TEST(DivisionTest, Test2) {
    FixedPointNumber first = -FixedPointNumber::FromHex(12, 8, "0x0c00");
    FixedPointNumber second = FixedPointNumber::FromHex(12, 8, "0x0800");

    FixedPointNumber result = first / second;
    ASSERT_EQ(ToDouble(result), -1.5);
}

TEST(DivisionTest, Test3) {
    FixedPointNumber first = FixedPointNumber::FromHex(12, 12, "0x5a60");
    FixedPointNumber second = FixedPointNumber::FromHex(12, 12, "0x2d540");

    FixedPointNumber result = first / second;
    ASSERT_EQ(ToDouble(result), 0.12451171875);
}

TEST(DivisionTest, Test4) {
    FixedPointNumber first = FixedPointNumber::FromHex(12, 12, "0x5a60");
    FixedPointNumber second = FixedPointNumber::FromHex(12, 12, "0x2d540");

    FixedPointNumber result = -first / second;
    ASSERT_EQ(ToDouble(result), -0.12451171875);
}