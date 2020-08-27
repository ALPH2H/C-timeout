#ifdef __unix__

#ifndef ftyIGIpM8CCTc_LjEitjVE2_O_d_raHw
#define ftyIGIpM8CCTc_LjEitjVE2_O_d_raHw

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <pthread.h>

struct NCTimeout {
  const void (*function)(void* data);
  void* data;
  long milliseconds;
};

void* NoCancelTimeout(struct NCTimeout* data) {
  struct timespec t = { .tv_sec = (time_t) data->milliseconds / 1000, .tv_nsec = (data->milliseconds - (data->milliseconds / 1000) * 1000) * 1000 };
  (void) nanosleep(&t, NULL);
  data->function(data->data);
  free(data);
  return NULL;
}

void SetNoCancelTimeout(const void (*function)(void* data), const long milliseconds, void* data) {
  struct NCTimeout* t = (struct NCTimeout*) malloc(sizeof(struct NCTimeout));
  t->function = function;
  t->milliseconds = milliseconds;
  t->data = data;
  pthread_t threadID;
  int thread = pthread_create(&threadID, NULL, NoCancelTimeout, t);
  if(thread != 0) {
    exit(1);
  }
}

#ifdef _USE_CTIMEOUT

static unsigned char* TimeoutStates;
static unsigned char TimeoutStatesAvail;
static unsigned char* TimeoutStatesAddr;
static unsigned int TimeoutStatesSize;
static unsigned int* TimeoutStatesFree;
static unsigned int* TimeoutStatesFreeAddr;
static unsigned int TimeoutStatesFreeLength;

void USE_CTIMEOUT() {
  TimeoutStatesSize = 1;
  TimeoutStatesAvail = 0;
  TimeoutStatesFreeLength = 1;
  TimeoutStates = (unsigned char*) calloc(1, sizeof(char));
  TimeoutStatesAddr = TimeoutStates;
  TimeoutStatesFree = (unsigned int*) calloc(1, sizeof(int));
  TimeoutStatesFreeAddr = TimeoutStatesFree;
}

struct Timeout {
  const void (*function)(void* data);
  void* data;
  long milliseconds;
  unsigned int timeoutID;
  unsigned int* timeoutstatesfreelength;
  unsigned int** timeoutstatesfreeaddr;
  const unsigned char* timeoutstatesavail;
  unsigned char** timeoutstatesaddr;
};

void ResizeTimeoutStates(const unsigned int newSize) {
  TimeoutStatesAvail = 1;
  TimeoutStates = (unsigned char*) realloc(TimeoutStates, newSize * sizeof(char));
  if(TimeoutStates == NULL) {
    exit(1);
  }
  TimeoutStatesAddr = TimeoutStates;
  TimeoutStatesFree = (unsigned int*) realloc(TimeoutStatesFree, newSize * sizeof(char));
  if(TimeoutStatesFree == NULL) {
    exit(1);
  }
  TimeoutStatesFreeAddr = TimeoutStatesFree;
  for(register unsigned int i = TimeoutStatesSize; i < newSize; ++i) {
    TimeoutStates[i] = 0;
    TimeoutStatesFree[TimeoutStatesFreeLength++] = i;
  }
  TimeoutStatesAvail = 0;
  TimeoutStatesSize = newSize;
}

void CancelTimeout(const unsigned int timeoutID) {
  TimeoutStates[timeoutID] = 1;
}

void* CTimeout(struct Timeout* data) {
  struct timespec t = { .tv_sec = (time_t) data->milliseconds / 1000, .tv_nsec = (data->milliseconds - (data->milliseconds / 1000) * 1000) * 1000 };
  (void) nanosleep(&t, NULL);
  while(*(data->timeoutstatesavail) == 1) {
    t.tv_sec = 0;
    t.tv_nsec = 0;
    (void) nanosleep(&t, NULL);
  }
  if((*(data->timeoutstatesaddr))[sizeof(int) * data->timeoutID] == 0) {
    data->function(data->data);
  }
  (*(data->timeoutstatesaddr))[sizeof(int) * data->timeoutID] = 0;
  (*(data->timeoutstatesfreeaddr))[(*(data->timeoutstatesfreelength))++] = data->timeoutID;
  free(data);
  return NULL;
}

unsigned int SetCancelTimeout(const void (*function)(void* data), const long milliseconds, void* data) {
  if(TimeoutStatesFreeLength == 0) {
    ResizeTimeoutStates(TimeoutStatesSize + 1);
  }
  struct Timeout* t = (struct Timeout*) malloc(sizeof(struct Timeout));
  t->function = function;
  t->milliseconds = milliseconds;
  t->data = data;
  t->timeoutID = TimeoutStatesFree[--TimeoutStatesFreeLength];
  t->timeoutstatesavail = &TimeoutStatesAvail;
  t->timeoutstatesaddr = &TimeoutStatesAddr;
  t->timeoutstatesfreeaddr = &TimeoutStatesFreeAddr;
  t->timeoutstatesfreelength = &TimeoutStatesFreeLength;
  pthread_t threadID;
  int thread = pthread_create(&threadID, NULL, NoCancelTimeout, t);
  if(thread != 0) {
    exit(1);
  }
  return t->timeoutID;
}

struct SCTimeout {
  unsigned int timeoutID;
  unsigned char** timeoutstatesaddr;
};
void TimeoutCancelTimeout(const struct SCTimeout* data) {
  (*(data->timeoutstatesaddr))[data->timeoutID] = 1;
}
unsigned int TimeoutedCancelTimeout(const unsigned int timeoutID, const long milliseconds) {
  struct SCTimeout* t = (struct SCTimeout*) malloc(sizeof(struct SCTimeout));
  t->timeoutID = timeoutID;
  t->timeoutstatesaddr = &TimeoutStatesAddr;
  return SetCancelTimeout(TimeoutCancelTimeout, milliseconds, t);
}

unsigned int SetMultipleCancelTimeouts(const void (*function)(void* data), const long milliseconds, void* data) {
  struct Timeout* t = (struct Timeout*) malloc(sizeof(struct Timeout));
  t->function = function;
  t->milliseconds = milliseconds;
  t->data = data;
  t->timeoutID = TimeoutStatesFree[--TimeoutStatesFreeLength];
  t->timeoutstatesavail = &TimeoutStatesAvail;
  t->timeoutstatesaddr = &TimeoutStatesAddr;
  t->timeoutstatesfreeaddr = &TimeoutStatesFreeAddr;
  t->timeoutstatesfreelength = &TimeoutStatesFreeLength;
  pthread_t threadID;
  int thread = pthread_create(&threadID, NULL, NoCancelTimeout, t);
  if(thread != 0) {
    exit(1);
  }
  return t->timeoutID;
}

#endif // _USE_CTIMEOUT

#ifdef __cplusplus
}
#endif

#endif // ftyIGIpM8CCTc_LjEitjVE2_O_d_raHw

#endif // __unix__
