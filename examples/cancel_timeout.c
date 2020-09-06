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
  struct TimeoutInfo ti = SetTimeout(editNumber, 500, &number); // we need the return value to propertly free allocated data and stop the thread's execution
  CancelTimeout(ti); // should always succeed as long as the argument is correct
  (void) SetTimeout(say, 600, &number);
  // expected output: 5 5, which means success, the value didn't change, because the timeout got canceled
  getc(stdin);
  return 0;
}
