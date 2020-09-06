#include <stdio.h>
#include "timeout.c"

void editNumber(int* num) {
  *num = 1;
}

void say(int* num) {
  printf("number is %i\n", *num);
}

int main() {
  int number = 5;
  printf("number is %i\n", number);
  (void) SetTimeout(editNumber, 500, &number); // we don't need to care about the return value if we will not cancel the timeout
  (void) SetTimeout(say, 600, &number);
  
  getc(stdin);
  return 0;
}
