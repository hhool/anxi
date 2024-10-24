/**
 * @file cmd_parser.h
 * @author hhool (hhool@outlook.com)
 * @brief cmd parser for the application
 * @version 0.1
 * @date 2024-10-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_COMMON_CMD_PARSER_H_
#define APP_COMMON_CMD_PARSER_H_

#include <map>
#include <string>
#include <vector>

namespace anx {
namespace common {
/**
 * @note cmd parser for the application
 * @note Get key value from the cmd with the key name and default value
 * @note cmd format: -key1 value1 -key2 value2 -key3 value3
 * @note example: -key1 1 -key2 false -key3 1.0 -key4 1.0f -key5 1.0e+1 -key6
 * "value string"
 *
 */
class CmdParser {
 public:
  explicit CmdParser(const std::string& cmd);
  ~CmdParser() = default;

 public:
  std::string GetKeyValue(const std::string& key_name,
                          const std::string& default_value) const;
  bool GetKeyValue(const std::string& key_name,
                   const bool& default_value) const;
  int32_t GetKeyValue(const std::string& key_name,
                      const int32_t& default_value) const;
  double GetKeyValue(const std::string& key_name,
                     const double& default_value) const;
  float GetKeyValue(const std::string& key_name,
                    const float& default_value) const;
  uint32_t GetKeyValue(const std::string& key_name,
                       const uint32_t& default_value) const;
  uint64_t GetKeyValue(const std::string& key_name,
                       const uint64_t& default_value) const;
  int64_t GetKeyValue(const std::string& key_name,
                      const int64_t& default_value) const;

 protected:
  void ParseCmd(const std::string& cmd);

 private:
  std::string cmd_;
  std::map<std::string, std::string> key_values_;
};
}  // namespace common
}  // namespace anx

#endif  // APP_COMMON_CMD_PARSER_H_
