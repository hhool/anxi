/**
 * @file thread.cc
 * @author hhool (hhool@outlook.com)
 * @brief thread class implementation
 * @version 0.1
 * @date 2024-09-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/common/thread.h"

namespace anx {
namespace common {

///////////////////////////////////////////////////////////////////////////////
// clz Mutex
Mutex::Mutex() {
#if defined(_WIN32)
  InitializeCriticalSection(&mutex_);
#else
  pthread_mutex_init(&mutex_, nullptr);
#endif
}

Mutex::~Mutex() {
#if defined(_WIN32)
  DeleteCriticalSection(&mutex_);
#else
  pthread_mutex_destroy(&mutex_);
#endif
}

void Mutex::lock() {
#if defined(_WIN32)
  EnterCriticalSection(&mutex_);
#else
  pthread_mutex_lock(&mutex_);
#endif
}

void Mutex::unlock() {
#if defined(_WIN32)
  LeaveCriticalSection(&mutex_);
#else
  pthread_mutex_unlock(&mutex_);
#endif
}
///////////////////////////////////////////////////////////////////////////////
// clz AutoLock
AutoLock::AutoLock(Mutex* mutex) : mutex_(mutex) {
  mutex_->lock();
}

AutoLock::~AutoLock() {
  mutex_->unlock();
}

///////////////////////////////////////////////////////////////////////////////
// clz Condition
#define USE_MONOTONIC_CLOCK 1

Condition::Condition() {
#if defined(_WIN32)
  InitializeConditionVariable(&cond_);
#else
#if USE_MONOTONIC_CLOCK
  pthread_condattr_t attr;
  pthread_condattr_init(&attr);
  pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
  pthread_cond_init(&cond_, &attr);
  pthread_condattr_destroy(&attr);
#else
  pthread_cond_init(&cond_, nullptr);
#endif
#endif
}

Condition::~Condition() {
#if defined(_WIN32)
  // do nothing
#else
  pthread_cond_destroy(&cond_);
#endif
}

bool Condition::wait(Mutex* mutex, unsigned int timeout) {
#if defined(_WIN32)
  if (timeout <= 0) {
    return SleepConditionVariableCS(&cond_, &mutex->mutex_, INFINITE) ? true
                                                                      : false;
  } else {
    return SleepConditionVariableCS(&cond_, &mutex->mutex_, timeout) ? true
                                                                     : false;
  }
#else
  if (timeout <= 0) {
    pthread_cond_wait(&cond_, &mutex->mutex_);
  } else {
    struct timespec ts;
#if USE_MONOTONIC_CLOCK
    clock_gettime(CLOCK_MONOTONIC, &ts);
#else
    clock_gettime(CLOCK_REALTIME, &ts);
#endif
    ts.tv_sec += timeout / 1000;
    ts.tv_nsec += (timeout % 1000) * 1000000;
    int ret = pthread_cond_timedwait(&cond_, &mutex->mutex_, &ts);
    if (ret == ETIMEDOUT) {
      return false;
    }
    return true;
  }
#endif
}

void Condition::signal() {
#if defined(_WIN32)
  WakeConditionVariable(&cond_);
#else
  pthread_cond_signal(&cond_);
#endif
}

void Condition::broadcast() {
#if defined(_WIN32)
  WakeAllConditionVariable(&cond_);
#else
  pthread_cond_broadcast(&cond_);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// clz Runnable
Runnable::~Runnable() {}

///////////////////////////////////////////////////////////////////////////////
// clz Thread
Thread::Thread() {
#if defined(_WIN32)
  thread_ = nullptr;
#else
  thread_ = 0;
#endif
}

Thread::Thread(Runnable* runnable) : runnable_(runnable) {
#if defined(_WIN32)
  thread_ = nullptr;
#else
  thread_ = 0;
#endif
}

Thread::~Thread() {}

void Thread::start() {
#if defined(_WIN32)
  thread_ = (HANDLE)_beginthreadex(nullptr, 0, thread_func, this, 0, nullptr);
#else
  pthread_create(&thread_, nullptr, thread_func, this);
#endif
}

void Thread::join() {
#if defined(_WIN32)
  WaitForSingleObject(thread_, INFINITE);
#else
  pthread_join(thread_, nullptr);
#endif
}

void Thread::detach() {
#if defined(_WIN32)
  CloseHandle(thread_);
#else
  pthread_detach(thread_);
#endif

  thread_ = nullptr;
}

#if defined(_WIN32)
unsigned int __stdcall Thread::thread_func(void* arg) {
#else
void* Thread::thread_func(void* arg) {
#endif
  Thread* thread = static_cast<Thread*>(arg);
  thread->runnable_->run();
  return 0;
}
}  // namespace common
}  // namespace anx
