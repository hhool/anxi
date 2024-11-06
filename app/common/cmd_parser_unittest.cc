/**
 * @file cmd_parser_unittest.cc
 * @author hhool (hhool@outlook.com)
 * @brief cmd parser unit test
 * @version 0.1
 * @date 2024-11-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/common/cmd_parser.h"

#include <gtest/gtest.h>

namespace anx {
namespace common {
namespace {
TEST(CmdParserTest, ParseCmd) {
  std::string cmd = "-key1 value1 -key2 value2 -key3 value3";
  CmdParser cmd_parser(cmd);
}

TEST(CmdParserTest, GetKeyValue) {
  std::string cmd = "-key1 value1 -key2 value2 -key3 value3";
  CmdParser cmd_parser(cmd);
}
}  // namespace
}  // namespace common
}  // namespace anx
