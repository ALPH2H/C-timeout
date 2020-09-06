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
  (void) SetTimeout(say, 600, &number); // time is measured in milliseconds, even though nanosleep() is used
  // expected output: 5 1, which means success in changing the number after some time
  getc(stdin);
  return 0;
}
