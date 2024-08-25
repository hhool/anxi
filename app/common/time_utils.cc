/**
 * @file time_utils.h
 * @author hhool (hhool@outlook.com)
 * @brief  time utils source file, implement time utils functions
 * use windows api on windows, use linux api on linux
 * @version 0.1
 * @date 2024-08-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/common/time_utils.h"

#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace anx {
namespace common {

int64_t GetCurrentTimeMicros() {
#if defined(_WIN32)
  LARGE_INTEGER freq;
  LARGE_INTEGER counter;
  QueryPerformanceFrequency(&freq);
  QueryPerformanceCounter(&counter);
  return counter.QuadPart * 1000000 / freq.QuadPart;
#else
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
#endif
}

int64_t GetCurrentTimeMillis() {
  // use GetCurrentTimeNanos() to get current time in nanoseconds
  return GetCurrentTimeMicros() / 1000;
}

int64_t GetCurrentTimeSeconds() {
  // use GetCurrentTimeNanos() to get current time in nanoseconds
  return GetCurrentTimeMicros() / 1000000;
}

void sleep_ms(int64_t ms) {
#ifdef _WIN32
  Sleep((DWORD)ms);
#else
  usleep(ms * 1000);
#endif
}

void GetLocalTime(struct tm* ltm) {
#if defined(_WIN32)
  FILETIME fileTime;
  GetSystemTimeAsFileTime(&fileTime);

  ULARGE_INTEGER uli;
  uli.LowPart = fileTime.dwLowDateTime;
  uli.HighPart = fileTime.dwHighDateTime;

  time_t time = (uli.QuadPart - 116444736000000000) / 10000000;
  localtime_s(ltm, &time);
#else
  struct timeval now;
  gettimeofday(&now, NULL);
  *ltm = *localtime(&now.tv_sec);
#endif
}

void GetCurrentDateTime(char dateTimeStr[256]) {
  struct tm ltm;
  int msec = 0;
#if defined(_WIN32)
  FILETIME fileTime;
  GetSystemTimeAsFileTime(&fileTime);

  ULARGE_INTEGER uli;
  uli.LowPart = fileTime.dwLowDateTime;
  uli.HighPart = fileTime.dwHighDateTime;

  time_t time = (uli.QuadPart - 116444736000000000) / 10000000;
  localtime_s(&ltm, &time);

  msec = (uli.QuadPart % 10000000) / 10000;
#else
  struct timeval now;
  gettimeofday(&now, NULL);
  ltm = *localtime(&now.tv_sec);
  msec = now.tv_usec / 1000;
#endif
  sprintf(dateTimeStr, "%04d-%02d-%02d %02d:%02d:%02d.%03d", 1900 + ltm.tm_year,
          1 + ltm.tm_mon, ltm.tm_mday, ltm.tm_hour, ltm.tm_min, ltm.tm_sec,
          msec);
}

double GetCurrrentDateTime() {
  SYSTEMTIME st;
  GetLocalTime(&st);
  /// @note use SystemTimeToVariantTime to convert the time to double
  /// @note only the time part is used, the date part is ignored.
  DATE dt;
  st.wYear = 1899;
  st.wMonth = 12;
  st.wDay = 30;
  st.wHour = 0;
  st.wMilliseconds = 0;
  st.wDayOfWeek = 0;

  SystemTimeToVariantTime(&st, &dt);
  return dt;
}

}  // namespace common
}  // namespace anx
