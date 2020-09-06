#ifdef __unix__

#ifndef ftyIGIpM8CCTc_LjEitjVE2_O_d_raHw
#define ftyIGIpM8CCTc_LjEitjVE2_O_d_raHw

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "timeout.h"

void* Timeout(struct TimeoutS* data) {
  struct timespec t = { .tv_sec = (time_t) data->milliseconds / 1000, .tv_nsec = (data->milliseconds - (data->milliseconds / 1000) * 1000) * 1000 };
  (void) nanosleep(&t, NULL);
  pthread_testcancel();
  data->function(data->data);
  free(data);
  return NULL;
}

struct TimeoutInfo SetTimeout(const void (*function)(void* data), const uint32_t milliseconds, void* data) {
  struct TimeoutS* t = (struct TimeoutS*) malloc(sizeof(struct TimeoutS));
  t->function = function;
  t->milliseconds = milliseconds;
  t->data = data;
  pthread_t threadID;
  if(pthread_create(&threadID, NULL, Timeout, t) != 0) {
    exit(1);
  }
  return (struct TimeoutInfo){ .threadID = threadID, .threadData = t };
}

void CancelTimeout(struct TimeoutInfo threadInfo) {
  free(threadInfo.threadData);
  (void) pthread_cancel(threadInfo.threadID);
}

#ifdef __cplusplus
}
#endif

#endif // ftyIGIpM8CCTc_LjEitjVE2_O_d_raHw

#endif // __unix__
