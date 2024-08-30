/**
 * @file ole_vartime_unittest.cc
 * @author hhool (hhool@outlook.com)
 * @brief test file for ole vartime test
 * @version 0.1
 * @date 2024-08-19
 * @copyright Copyright (c) 2024
 * @note
 * https://www.codeproject.com/articles/17576/systemtime-to-varianttime-with-milliseconds
 */

#include <atlbase.h>
#include <atlcom.h>
#include <atlcomcli.h>
#include <atlstr.h>
#include <atltime.h>
#include <comdef.h>
#include <comutil.h>

#include "gtest/gtest.h"

class OleVarTimeTest : public testing::Test {
 protected:
  void SetUp() override {}
  void TearDown() override {}
};

static BOOL WINAPI _TmFromOleDate(DATE dtSrc, struct tm* tmDest) {
  SYSTEMTIME st;
  ATLASSERT(tmDest);
  if (VariantTimeToSystemTime(dtSrc, &st) == FALSE) {
    ATLTRACE(_T("Failed to convert %g date to system time\n"), dtSrc);
    return FALSE;
  }
  ZeroMemory(tmDest, sizeof(tm));

  tmDest->tm_year = st.wYear - 1900;  // year is based on 1900
  tmDest->tm_mon = st.wMonth - 1;     // month of year is 0-based
  tmDest->tm_mday = st.wDay;
  tmDest->tm_wday = st.wDayOfWeek;

  tmDest->tm_hour = st.wHour;
  tmDest->tm_min = st.wMinute;
  tmDest->tm_sec = st.wSecond;

  return TRUE;
}

static void PrintTm(const struct tm* tm) {
  printf("%04d-%02d-%02d %02d:%02d:%02d\n", tm->tm_year + 1900, tm->tm_mon + 1,
         tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
}

static void PrintTmVariantTime(DATE dt) {
  struct tm tm;
  _TmFromOleDate(dt, &tm);
  PrintTm(&tm);
}

static void VariantTimeIncrement(DATE* dt, double increment) {
  printf("increment: %g\n", increment);
  *dt += increment;
  PrintTmVariantTime(*dt);
}

TEST_F(OleVarTimeTest, OleVarTimeTest) {
  // 1. Get current time
  SYSTEMTIME st;
  GetLocalTime(&st);
  double dt = 0.0;
  SystemTimeToVariantTime(&st, &dt);
  printf("st: %04d-%02d-%02d %02d:%02d:%02d->%g\n", st.wYear, st.wMonth,
         st.wDay, st.wHour, st.wMinute, st.wSecond, dt);
  // ignore year
  st.wYear = 1899;
  SystemTimeToVariantTime(&st, &dt);
  printf("st: %04d-%02d-%02d %02d:%02d:%02d->%g\n", st.wYear, st.wMonth,
         st.wDay, st.wHour, st.wMinute, st.wSecond, dt);
  // ignore month
  st.wMonth = 12;
  SystemTimeToVariantTime(&st, &dt);
  printf("st: %04d-%02d-%02d %02d:%02d:%02d->%g\n", st.wYear, st.wMonth,
         st.wDay, st.wHour, st.wMinute, st.wSecond, dt);

  // ignore day
  st.wDay = 30;
  SystemTimeToVariantTime(&st, &dt);
  printf("st: %04d-%02d-%02d %02d:%02d:%02d->%g\n", st.wYear, st.wMonth,
         st.wDay, st.wHour, st.wMinute, st.wSecond, dt);

  // ignore hour
  st.wHour = 0;
  SystemTimeToVariantTime(&st, &dt);
  printf("st: %04d-%02d-%02d %02d:%02d:%02d->%g\n", st.wYear, st.wMonth,
         st.wDay, st.wHour, st.wMinute, st.wSecond, dt);

  // 2. Convert to tm
  struct tm tm;
  _TmFromOleDate(dt, &tm);
  PrintTm(&tm);

  // 3. Increment 1 minute
  printf("Increment 1 minute\n");
  VariantTimeIncrement(&dt, 1.0 / (24.0 * 60.0));
  // 4. Increment 5 minutes
  printf("Increment 5 minutes\n");
  VariantTimeIncrement(&dt, 5.0 / (24.0 * 60.0));
  // 5. Increment 10 minutes
  printf("Increment 10 minutes\n");
  VariantTimeIncrement(&dt, 10.0 / (24.0 * 60.0));
  // 6. Increment 30 minutes
  printf("Increment 30 minutes\n");
  VariantTimeIncrement(&dt, 30.0 / (24.0 * 60.0));
  // 7. Increment 60 minutes
  printf("Increment 60 minutes\n");
  VariantTimeIncrement(&dt, 60.0 / (24.0 * 60.0));
  // 7. Increment 1 day
  printf("Increment 1 day\n");
  VariantTimeIncrement(&dt, 1.0);

  // 8. increment 2 seconds 4 seconds 6 seconds loop to 1 minute
  for (int i = 1; i <= 30; i++) {
    printf("Increment %g second\n", i * 2.0f);
    VariantTimeIncrement(&dt, (2.0f * i) / (24.0 * 60.0 * 60.0));
  }
}
