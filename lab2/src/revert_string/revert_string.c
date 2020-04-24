#include "revert_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void RevertString(char *str) {
    int shift = 3;
    int len = strlen(str)/2 + shift;
    //char *new_str = malloc(sizeof(char) * len);
    int i;
    char buff;
    for(i = strlen(str)/2 - shift; i < len + 1;i++, len--)
    {
        buff = str[i];
        str[i] = str[len];
        str[len] = buff;
        //printf("%c %d %d\n",new_str[i], i, len);
    }
}
