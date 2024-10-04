/**
 * @file time_utils.h
 * @author hhool (hhool@outlook.com)
 * @brief  time utils header file
 * @version 0.1
 * @date 2024-08-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_COMMON_TIME_UTILS_H_
#define APP_COMMON_TIME_UTILS_H_

#include <string>

namespace anx {
namespace common {

// get current time in microseconds
uint64_t GetCurrentTimeMicros();

// get current time in milliseconds
int64_t GetCurrentTimeMillis();

// get current time in seconds
int64_t GetCurrentTimeSeconds();

// sleep for milliseconds
void sleep_ms(int64_t ms);

/// @brief get current time with struct tm filled with current time info.
/// @param ltm output parameter, the current time struct tm filled with current
/// time info.
/// @return void
void GetLocalTime(struct tm* ltm);

// @brief get current date time string with format like "2024-08-04
// 12:00:00.000"
// @param dateTimeStr output parameter, the current date time string
// @return void
void GetCurrentDateTime(char dateTimeStr[256]);

/// @brief get current date time in double format with the date part ignored.
/// @return the current date time in double format.
double GetCurrrentDateTime();
}  // namespace common
}  // namespace anx

#endif  // APP_COMMON_TIME_UTILS_H_
