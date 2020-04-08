#include "swap.h"
#include "revert_string.h"
#include <string.h>

void RevertString(char *str) {
  int length = strlen(str);
  char* begin = str;
  char* end = str;

  for (int c = 0; c < length - 1; c++)
    end++;

  for (int c = 0; c < length / 2; c++) {
    Swap(begin, end);
    begin++;
    end--;
  }
}
