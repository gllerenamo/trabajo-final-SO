#include "types.h"

int len(const uint8_t *string) {
    int length = 0;
    uint8_t ch;
    while ((ch = string[length]) != '\n') length++;
    return length;
}