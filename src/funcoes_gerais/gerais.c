#include <stdio.h>
#include <ctype.h>
#include "gerais.h"

void minusculo(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}