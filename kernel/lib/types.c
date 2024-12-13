#include "types.h"

int len(const char *string) {
    int length = 0;
    unsigned char c;
    while ((c = string[length]) != '\n') length++;
    return length;
}