#include <stdio.h>
#define _USE_CTIMEOUT // letting the file know we want to allocate additional memory for cancelable timeouts and to enable more functions
#include "timeout.h"

void editNumber(int* num) {
  *num = 1;
}

void say(int* num) {
  printf("number is %d\n", *num);
}

int main() {
  USE_CTIMEOUT(); // initialising the memory needed for cancelable timeouts
  int* number = (int*) malloc(sizeof(int));
  *number = 5;
 
  printf("number is %d\n", *number);
  unsigned int timeoutID = SetCancelTimeout(editNumber, 500, number); // automatically resizes (if needed) memory to keep the timeouts, use SetMultipleCancelTimeouts in addition to ResizeTimeoutStates if you want
  // the first call to set up a cancelable timeout does not allocate any more memory. When initialising the additional memory with USE_CTIMEOUT, it already allocates enough for 1 timeout.
  (void) TimeoutedCancelTimeout(timeoutID, 100); // casted to void to get rid of the returned timeoutID, if you want to instantly cancel a timeout, use CancelTimeout
  SetNoCancelTimeout(say, 600, number); // printing the number some time after
  // unsigned short int is used for timeoutIDs, because I don't expect anyone wanting to create more than 65535 threads at the same time. Anyway, if you would like to do so, just change a few types in the timeout.h file.
 
  //puts("press any key to exit");
  getc(stdin);
  free(number);
  return 0;
}
