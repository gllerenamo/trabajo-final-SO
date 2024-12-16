#include "types.h"

int len(const uint8_t *string) {
    int length = 0;
    uint8_t ch;
    while ((ch = string[length]) != '\n') length++;
    return length;
}

int strcmp(string *str1, string *str2) {
    if (!str1 || !str2 || !str1->text || !str2->text) return 0;
    if (str1->length != str2->length) return 0;

    for (int i = 0; i < str1->length; i++) {
        if (str1->text[i] != str2->text[i]) return 0;
    }
    return 1;
}
