#include <pthread.h>
#include <stdint.h>

struct TimeoutS {
  const void (*function)(void*);
  void* data;
  uint32_t milliseconds;
};

struct TimeoutInfo {
  pthread_t threadID;
  struct TimeoutS* threadData;
};

void* Timeout(struct TimeoutS*);

struct TimeoutInfo SetTimeout(const void (*)(void*), const uint32_t, void*);

void CancelTimeout(struct TimeoutInfo);
