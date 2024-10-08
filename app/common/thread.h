/**
 * @file thread.h
 * @author hhool (hhool@outlook.com)
 * @brief thread class implementation
 * @version 0.1
 * @date 2024-09-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_COMMON_THREAD_H_
#define APP_COMMON_THREAD_H_

#if defined(_WIN32)
#include <process.h>  // _beginthreadex
#include <windows.h>
#else
#include <errno.h>  // ETIMEDOUT
#include <pthread.h>
#include <time.h>  // clock_gettime
#endif

namespace anx {
namespace common {
class Condition;
class Mutex {
 public:
  Mutex();
  virtual ~Mutex();

  friend class Condition;
  virtual void lock();
  virtual void unlock();

 private:
#if defined(_WIN32)
  CRITICAL_SECTION mutex_;
#else
  pthread_mutex_t mutex_;
#endif
};

class Condition {
 public:
  Condition();
  virtual ~Condition();

  virtual bool wait(Mutex* mutex, unsigned int timeout = 0);
  virtual void signal();
  virtual void broadcast();

 private:
#if defined(_WIN32)
  CONDITION_VARIABLE cond_;
#else
  pthread_cond_t cond_;
#endif
};

class AutoLock {
 public:
  explicit AutoLock(Mutex* mutex);
  ~AutoLock();

 private:
  Mutex* mutex_;
};

class Runnable {
 public:
  virtual ~Runnable();
  virtual void run() = 0;
  virtual void interrupt() { stop_ = true; }
  virtual bool is_interrupt() { return stop_; }

 protected:
  bool stop_ = false;
};

#if defined(_WIN32)
typedef HANDLE ThreadIt;
#else
typedef pthread_t ThreadIt;
#endif

class Thread {
 public:
  Thread();
  Thread(const Thread&) = delete;
  Thread& operator=(const Thread&) = delete;

  explicit Thread(Runnable* runnable);
  virtual ~Thread();

  virtual void start();
  virtual void join();
  virtual void detach();

  bool is_current_thread();

  ThreadIt current_thread();

 private:
#if defined(_WIN32)
  static unsigned int __stdcall thread_func(void* arg);
#else
  static void* thread_func(void* arg);
#endif

 private:
  Runnable* runnable_;
#if defined(_WIN32)
  HANDLE thread_;
#else
  pthread_t thread_;
#endif
};
}  // namespace common
}  // namespace anx

#endif  // APP_COMMON_THREAD_H_
