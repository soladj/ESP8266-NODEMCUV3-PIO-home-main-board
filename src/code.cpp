#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 256

#define DIGITS "0123456789ABCDEF"

void get_code(char* str_var, unsigned rand_init, unsigned length)
{
    srand(rand_init);
    char* str = str_var;
    char tmp_char;

    for (unsigned i = 0; i < length; i++) {
        if (i == 0) {
            do {
                tmp_char = *(DIGITS + (rand() % 16));
            } while (tmp_char == '0');
        }
        *str = tmp_char;
        tmp_char = *(DIGITS + (rand() % 16));
        str++;
    }
    *str = '\0';
    }
