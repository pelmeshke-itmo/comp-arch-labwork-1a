#include <iostream>
#include "lib/number.h"

int main(int argc, const char** argv) {
    if (argc != 4 && argc != 6) {
        std::cerr << "Wrong number of arguments" << std::endl;
        return 1;
    }

    int integer_part_size = 0;
    int not_integer_part_size = 0;

    const char* format = argv[1];
    while (*format != '.') {
        integer_part_size *= 10;
        int num = *format - '0';
        if (num < 0 || num > 9) {
            std::cerr << "Error while parsing arguments" << std::endl;
            return 1;
        }
        integer_part_size += num;
        format++;
    }
    format++;
    while (*format != 0) {
        not_integer_part_size *= 10;
        int num = *format - '0';
        if (num < 0 || num > 9) {
            std::cerr << "Error while parsing arguments" << std::endl;
            return 1;
        }
        not_integer_part_size += num;
        format++;
    }

    if (integer_part_size < 1 || integer_part_size + not_integer_part_size > 32) {
        std::cerr << "Format or a number should consist of less or equal 32 bits in which at least 1 bit should be used by integer part" << std::endl;
        return 1;
    }

    int rounding_type = argv[2][0] - '0';
    if (rounding_type < 0 || rounding_type > 3 || argv[2][1] != 0) {
        std::cerr << "Rounding type should be a integer between 0 and 3" << std::endl;
        return 1;
    }

    FixedPointNumber first_number = FixedPointNumber::FromHex(integer_part_size, not_integer_part_size, argv[3]);
    if (argc == 4) {
        std::cout << first_number;
    } else {
        char operation = argv[4][0];
        FixedPointNumber second_number = FixedPointNumber::FromHex(integer_part_size, not_integer_part_size, argv[5]);

        FixedPointNumber result;
        switch (operation) {
            case '+':
                result = first_number + second_number;
                break;
            case '-':
                result = first_number - second_number;
                break;
            case '*':
                result = first_number * second_number;
                break;
            case '/':
                if (second_number.GetBytes() == 0) {
                    std::cerr << "Second number you entered is zero, but division by zero is impossible" << std::endl;
                    return 0;
                }
                result = first_number / second_number;
                break;
            default:
                std::cerr << "You typed the wrong operation that I can not do :(" << std::endl;
                return 0;
        }
        result.rounding_type = rounding_type;
        std::cout << result << std::endl;
    }


    return 0;
}
