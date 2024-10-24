/**
 * @file cmd_parser.cc
 * @author hhool (hhool@outlook.com)
 * @brief  cmd parser for the application
 * @version 0.1
 * @date 2024-10-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/common/cmd_parser.h"

#include <iostream>
#include <string>
#include <vector>

#include "app/common/string_utils.h"

namespace anx {
namespace common {

CmdParser::CmdParser(const std::string& cmd) : cmd_(cmd) {
  ParseCmd(cmd);
}
/**
 * @brief Get key value from the cmd with the key name and default value
 * @note cmd format: -key1 value1 -key2 value2 -key3 value3
 * @note example: -key1 1 -key2 false -key3 1.0 -key4 1.0f -key5 1.0e+1 -key6
 * "value string"
 * @param key_name  the cmd key value
 */

void CmdParser::ParseCmd(const std::string& cmd) {
  std::vector<std::string> cmd_parts = anx::common::Split(cmd, " ");
  for (size_t i = 0; i < cmd_parts.size(); i += 2) {
    if (i + 1 < cmd_parts.size()) {
      key_values_[cmd_parts[i]] = cmd_parts[i + 1];
    }
  }
}

std::string CmdParser::GetKeyValue(const std::string& key_name,
                                   const std::string& default_value) const {
  auto it = key_values_.find(key_name);
  if (it != key_values_.end()) {
    return it->second;
  }
  return default_value;
}

bool CmdParser::GetKeyValue(const std::string& key_name,
                            const bool& default_value) const {
  auto it = key_values_.find(key_name);
  if (it != key_values_.end()) {
    return it->second == "true" || it->second == "1" || it->second == "True" ||
           it->second == "TRUE" || it->second == "yes" || it->second == "Yes" ||
           it->second == "YES";
  }
  return default_value;
}

int32_t CmdParser::GetKeyValue(const std::string& key_name,
                               const int32_t& default_value) const {
  auto it = key_values_.find(key_name);
  if (it != key_values_.end()) {
    return std::stoi(it->second);
  }
  return default_value;
}

double CmdParser::GetKeyValue(const std::string& key_name,
                              const double& default_value) const {
  auto it = key_values_.find(key_name);
  if (it != key_values_.end()) {
    return std::stod(it->second);
  }
  return default_value;
}

float CmdParser::GetKeyValue(const std::string& key_name,
                             const float& default_value) const {
  auto it = key_values_.find(key_name);
  if (it != key_values_.end()) {
    return std::stof(it->second);
  }
  return default_value;
}

uint32_t CmdParser::GetKeyValue(const std::string& key_name,
                                const uint32_t& default_value) const {
  auto it = key_values_.find(key_name);
  if (it != key_values_.end()) {
    return std::stoul(it->second);
  }
  return default_value;
}

uint64_t CmdParser::GetKeyValue(const std::string& key_name,
                                const uint64_t& default_value) const {
  auto it = key_values_.find(key_name);
  if (it != key_values_.end()) {
    return std::stoull(it->second);
  }
  return default_value;
}

int64_t CmdParser::GetKeyValue(const std::string& key_name,
                               const int64_t& default_value) const {
  auto it = key_values_.find(key_name);
  if (it != key_values_.end()) {
    return std::stoll(it->second);
  }
  return default_value;
}

}  // namespace common
}  // namespace anx
