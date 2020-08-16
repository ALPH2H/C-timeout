#ifndef ftyIGIpM8CCTc_LjEitjVE2_O_d_raHw
#define ftyIGIpM8CCTc_LjEitjVE2_O_d_raHw

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#ifdef _WIN32

#include <windows.h>

struct NCTimeout {
  const void (*function)(void* data);
  void* data;
  unsigned int milliseconds;
};

DWORD WINAPI NoCancelTimeout(struct NCTimeout* data) {
  Sleep(data->milliseconds);
  data->function(data->data);
  free(data);
  return 0;
}

inline void SetNoCancelTimeout(const void (*function)(void* data), const unsigned int milliseconds, void* data) {
  struct NCTimeout* t = (struct NCTimeout*) malloc(sizeof(struct NCTimeout));
  t->function = function;
  t->milliseconds = milliseconds;
  t->data = data;
  HANDLE thread = CreateThread(NULL, 0, NoCancelTimeout, t, 0, NULL);
  if(thread == NULL) {
    exit(1);
  }
}

#ifdef _USE_CTIMEOUT

static unsigned char* TimeoutStates;
static unsigned char TimeoutStatesAvail;
static unsigned char* TimeoutStatesAddr;
static unsigned short int TimeoutStatesSize;
static unsigned short int* TimeoutStatesFree;
static unsigned short int* TimeoutStatesFreeAddr;
static unsigned short int TimeoutStatesFreeLength;

void USE_CTIMEOUT() {
  TimeoutStatesSize = 1;
  TimeoutStatesAvail = 0;
  TimeoutStatesFreeLength = 1;
  TimeoutStates = (unsigned char*) calloc(1, sizeof(char));
  TimeoutStatesAddr = TimeoutStates;
  TimeoutStatesFree = (unsigned short int*) calloc(1, sizeof(short int));
  TimeoutStatesFreeAddr = TimeoutStatesFree;
}

struct Timeout {
  const void (*function)(void* data);
  void* data;
  unsigned int milliseconds;
  unsigned short int timeoutID;
  const unsigned char* timeoutstatesavail;
  unsigned char** timeoutstatesaddr;
  unsigned short int* timeoutstatesfreelength;
  unsigned short int** timeoutstatesfreeaddr;
};

inline void ResizeTimeoutStates(const unsigned short int newSize) {
  if(newSize > TimeoutStatesSize) {
    TimeoutStatesAvail = 1;
    TimeoutStates = (unsigned char*) realloc(TimeoutStates, newSize * sizeof(char));
    if(TimeoutStates == NULL) {
      exit(1);
    }
    TimeoutStatesAddr = TimeoutStates;
    TimeoutStatesFree = (unsigned short int*) realloc(TimeoutStatesFree, newSize * sizeof(char));
    if(TimeoutStatesFree == NULL) {
      exit(1);
    }
    TimeoutStatesFreeAddr = TimeoutStatesFree;
    for(register unsigned short int i = TimeoutStatesSize; i < newSize; ++i) {
      TimeoutStates[i] = 0;
      TimeoutStatesFree[TimeoutStatesFreeLength++] = i;
    }
    TimeoutStatesAvail = 0;
    TimeoutStatesSize = newSize;
  }
}

inline void CancelTimeout(const unsigned short int timeoutID) {
  TimeoutStates[timeoutID] = 1;
}

DWORD WINAPI CTimeout(struct Timeout* data) {
  Sleep(data->milliseconds);
  while(*(data->timeoutstatesavail) == 1) {
    Sleep(0);
  }
  if((*(data->timeoutstatesaddr))[sizeof(short int) * data->timeoutID] == 0) {
    data->function(data->data);
  }
  (*(data->timeoutstatesaddr))[sizeof(short int) * data->timeoutID] = 0;
  (*(data->timeoutstatesfreeaddr))[(*(data->timeoutstatesfreelength))++] = data->timeoutID;
  free(data);
  return 0;
}

inline unsigned short int SetCancelTimeout(const void (*function)(void* data), const unsigned int milliseconds, void* data) {
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
  HANDLE thread = CreateThread(NULL, 0, CTimeout, t, 0, NULL);
  if(thread == NULL) {
    exit(1);
  }
  return t->timeoutID;
}

struct SCTimeout {
  unsigned short int timeoutID;
  unsigned char** timeoutstatesaddr;
};
void TimeoutCancelTimeout(const struct SCTimeout* data) {
  (*(data->timeoutstatesaddr))[data->timeoutID] = 1;
}
inline unsigned short int TimeoutedCancelTimeout(const unsigned short int timeoutID, const unsigned int milliseconds) {
  struct SCTimeout* t = (struct SCTimeout*) malloc(sizeof(struct SCTimeout));
  t->timeoutID = timeoutID;
  t->timeoutstatesaddr = &TimeoutStatesAddr;
  return SetCancelTimeout(TimeoutCancelTimeout, milliseconds, t);
}

inline unsigned short int SetMultipleCancelTimeouts(const void (*function)(void* data), const unsigned int milliseconds, void* data) {
  struct Timeout* t = (struct Timeout*) malloc(sizeof(struct Timeout));
  t->function = function;
  t->milliseconds = milliseconds;
  t->data = data;
  t->timeoutID = TimeoutStatesFree[--TimeoutStatesFreeLength];
  t->timeoutstatesavail = &TimeoutStatesAvail;
  t->timeoutstatesaddr = &TimeoutStatesAddr;
  t->timeoutstatesfreeaddr = &TimeoutStatesFreeAddr;
  t->timeoutstatesfreelength = &TimeoutStatesFreeLength;
  HANDLE thread = CreateThread(NULL, 0, CTimeout, t, 0, NULL);
  if(thread == NULL) {
    exit(1);
  }
  return t->timeoutID;
}

#endif // _USE_CTIMEOUT

#endif // _WIN32

#ifdef __cplusplus
}
#endif

#endif // ftyIGIpM8CCTc_LjEitjVE2_O_d_raHw
