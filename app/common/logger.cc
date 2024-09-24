/**
 * @file logger.cc
 * @author hhool (hhool@outlook.com)
 * @brief logger file for the logger
 * @version 0.1
 * @date 2024-08-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/common/logger.h"

#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace anx {
namespace common {

//////////////////////////////////////////////////////////////////////////
// Logger
std::vector<std::shared_ptr<LoggerSink>> Logger::sinks_;
int Logger::log_level_ = LS_INFO;
std::mutex Logger::mutex_;

void Logger::set_log_level(int level) {
  log_level_ = level;
}

void Logger::clear_sinks() {
  std::lock_guard<std::mutex> lock(mutex_);
  sinks_.clear();
}

void Logger::add_sink(std::shared_ptr<LoggerSink> sink) {
  std::lock_guard<std::mutex> lock(mutex_);
  sinks_.push_back(sink);
}

void Logger::remove_sink(std::shared_ptr<LoggerSink> sink) {
  std::lock_guard<std::mutex> lock(mutex_);
  for (auto it = sinks_.begin(); it != sinks_.end(); ++it) {
    if (*it == sink) {
      sinks_.erase(it);
      break;
    }
  }
}

void Logger::Log(int level, std::string& log) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (level < log_level_) {
    return;
  }
#if defined(_WIN32) || defined(_WIN64)
  OutputDebugStringA(log.c_str());
  OutputDebugStringA("\n");
#endif
  for (auto& sink : sinks_) {
    sink->Log(level, log);
  }
}

//////////////////////////////////////////////////////////////////////////
// LoggerStream

namespace {
/// @brief get the current time in milliseconds
/// @return  the current time in milliseconds %Y-%m-%d %H:%M:%S.%3d
std::string FormatTimeMillis(int64_t millis) {
  time_t t = millis / 1000;
  int32_t ms = millis % 1000;
  struct tm ltm;
  GetLocalTime(&ltm);
  char buf[32];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ltm);
  snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), ".%03d", ms);
  return std::string(buf);
}
}  // namespace

LoggerStream::LoggerStream(const char* file,
                           int line,
                           const char* func,
                           int level)
    : file_(file), line_(line), func_(func), level_(level) {
  /// get file name
  std::string file_name(file);
  size_t pos = file_name.find_last_of("\\/");
  if (pos != std::string::npos) {
    file_name = file_name.substr(pos + 1);
  }
  stream_ << "[" << std::this_thread::get_id() << ":"
          << std::this_thread::get_id() << ":"
          << FormatTimeMillis(GetCurrentTimeMillis()) << ":" << file_name << ":"
          << line << ":" << func << "] ";
}

LoggerStream::~LoggerStream() {
  Logger::Log(level_, stream_.str());
}

//////////////////////////////////////////////////////////////////////////
// ConsoleLoggerSink

void ConsoleLoggerSink::Log(int level, std::string& log) {
  std::string level_str;
  switch (level) {
    case LS_INFO:
      level_str = "INFO";
      break;
    case LS_WARN:
      level_str = "WARN";
      break;
    case LS_ERROR:
      level_str = "ERROR";
      break;
    case LS_FATAL:
      level_str = "FATAL";
      break;
    default:
      level_str = "UNKNOWN";
      break;
  }
  std::cout << level_str << ": " << log << std::endl;
#ifdef _WIN32
  OutputDebugStringA((level_str + ": " + log + "\n").c_str());
#endif
}

//////////////////////////////////////////////////////////////////////////
// FileLoggerSink

FileLoggerSink::FileLoggerSink(std::string file_path) {
  file_.open(file_path, std::ios::out | std::ios::binary);
}

FileLoggerSink::~FileLoggerSink() {
  file_.close();
}

void FileLoggerSink::Log(int level, std::string& log) {
  std::string level_str;
  switch (level) {
    case LS_INFO:
      level_str = "INFO";
      break;
    case LS_WARN:
      level_str = "WARN";
      break;
    case LS_ERROR:
      level_str = "ERROR";
      break;
    case LS_FATAL:
      level_str = "FATAL";
      break;
    default:
      level_str = "UNKNOWN";
      break;
  }
  std::string out = level_str;
  out.append(":");
  out.append(log);
  out.append("\n");
  file_.write(out.data(), out.size());
  file_.flush();
}

}  // namespace common
}  // namespace anx
