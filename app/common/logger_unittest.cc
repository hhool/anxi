/**
 * @file logger_unittest.cc
 * @author hhool (hhool@outlook.com)
 * @brief  logger unittest file
 * @version 0.1
 * @date 2024-08-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <gtest/gtest.h>

#include "app/common/logger.h"

#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace anx {
namespace common {
class LoggerSinkMock : public LoggerSink {
 public:
  void Log(int level, std::string& log) override {
#ifdef _WIN32
    OutputDebugStringA((log + "\n").c_str());
#endif
  }
};

class LoggerTest : public ::testing::Test {
 protected:
  void SetUp() override { Logger::clear_sinks(); }
  void TearDown() override { Logger::clear_sinks(); };
};

TEST_F(LoggerTest, TestLogger) {
  LoggerSinkMock sink;
  Logger::add_sink(std::make_shared<LoggerSinkMock>(sink));
  Logger::set_log_level(LS_ERROR);
  int64_t i = 10;
  float f = 10.0f;
  double d = 10.00001f;
  std::string str = "abc";
  LOG_F(LS_INFO) << "info:" << "i:" << i << " " << "f:" << f << " "
                 << "d:" << d;
  LOG_F(LS_WARN) << "warn" << "i:" << i << " " << "f:" << f << " " << "d:" << d;
  LOG_F(LS_ERROR) << "error" << "i:" << i << " " << "f:" << f << " "
                  << "d:" << d;
  LOG_F(LS_FATAL) << "fatal" << "i:" << i << " " << "f:" << f << " "
                  << "d:" << d;
}

}  // namespace common
}  // namespace anx
