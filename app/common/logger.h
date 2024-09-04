/**
 * @file logger.h
 * @author hhool (hhool@outlook.com)
 * @brief logger for win and linux macro. output log to console and file.
 * @note the log file will be saved in the same directory as the executable
 * file. can be used in multi-thread environment. use the macro LOG_INFO,
 * LOG_WARN, LOG_ERROR, LOG_FATAL to log. for example. LOG_F(LS_ERROR) << "error
 * message"; LOG_F(LS_WARN) << "warn message"; LOG_F(LS_INFO) << "info message"
 * << 1;
 *
 * @version 0.1
 * @date 2024-08-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_COMMON_LOGGER_H_
#define APP_COMMON_LOGGER_H_

#include "app/common/time_utils.h"

#include <fstream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

namespace anx {
namespace common {

enum LogLevel {
  LS_INFO = 0,
  LS_WARN = 1,
  LS_ERROR = 2,
  LS_FATAL = 3,
};

class LoggerSink {
 public:
  LoggerSink() = default;
  virtual ~LoggerSink() = default;

  virtual void Log(int level, std::string& log) = 0;
};

/// @brief logger class for logging
/// @note with the logger class, you can log the message to console and file
/// @note by default, the log level is set to LS_INFO, you can change it by
/// set_log_level function. can set sink interface to log to console or file.
/// @note specify the log level with the macro LOG_INFO, LOG_WARN, LOG_ERROR,
/// LOG_FATAL, log messge with thread id, time, file name, line number.

class Logger {
 public:
  static void set_log_level(int level);
  static void clear_sinks();
  static void add_sink(std::shared_ptr<LoggerSink> sink);
  static void remove_sink(std::shared_ptr<LoggerSink> sink);
  static void Log(int level, std::string& log);

 private:
  static int log_level_;
  static std::vector<std::shared_ptr<LoggerSink>> sinks_;
  static std::mutex mutex_;
};

class LoggerStream {
 public:
  LoggerStream(const char* file, int line, const char* func, int level);
  ~LoggerStream();

  template <typename T>
  LoggerStream& operator<<(const T& t) {
    stream_ << t;
    return *this;
  }

 private:
  const char* file_;
  int line_;
  const char* func_;
  int level_;
  std::stringstream stream_;
};

inline std::string GetCurrentDateTime() {
  char dateTimeStr[256];
  anx::common::GetCurrentDateTime(dateTimeStr);
  return std::string(dateTimeStr);
}

class ConsoleLoggerSink : public LoggerSink {
 public:
  ConsoleLoggerSink() = default;
  virtual ~ConsoleLoggerSink() = default;

  void Log(int level, std::string& log) override;
};

class FileLoggerSink : public LoggerSink {
 public:
  explicit FileLoggerSink(std::string file_path);
  explicit FileLoggerSink(const FileLoggerSink& other) = delete;
  virtual ~FileLoggerSink();

  void Log(int level, std::string& log) override;

 private:
  std::fstream file_;
};

}  // namespace common
}  // namespace anx

#define LG_INFO anx::common::LS_INFO
#define LG_WARN anx::common::LS_WARN
#define LG_ERROR anx::common::LS_ERROR
#define LG_FATAL anx::common::LS_FATAL

/// @brief log macro for logging with file name, line number, function name
///        and thread id info. use the macro like LOG_F(LS_INFO) << "info
///        message";
/// @param level the log level
/// @return the logger object
#define LOG_F(level) \
  anx::common::LoggerStream(__FILE__, __LINE__, __FUNCTION__, level)

#define LOG_F_INFO_TAG(TAG)                                            \
  anx::common::LoggerStream(__FILE__, __LINE__, __FUNCTION__, LG_INFO) \
      << TAG << ": "

#endif  // APP_COMMON_LOGGER_H_
