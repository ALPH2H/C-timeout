#include <stdio.h>
#include "timeout.h"

void editNumber(int* num) {
  *num = 1;
}

void checkNumber(int* num) {
  printf("number is %d\n", *num);
}

int main() {
  int* number = (int*) malloc(sizeof(int));
  *number = 5;
  
  printf("number is %d\n", *number);
  SetNoCancelTimeout(editNumber, 500, number);
  printf("the number after setting first timeout is still %d\n", *number);
  SetNoCancelTimeout(checkNumber, 600, number);
  
  //puts("press any key to exit");
  getc(stdin);
  free(number);
  return 0;
}
