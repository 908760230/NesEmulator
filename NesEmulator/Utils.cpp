#include "Utils.h"

std::string Utils::toHex(uint32_t value, uint8_t d) {
    std::string result(d, '0');
    std::string flag("0123456789ABCDEF");

    for (int i = d - 1; i >= 0; i--) {
        result[i] = flag[value & 0xF];
        value >>= 4;
    }
    return result;
}