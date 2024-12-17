/**
 * @file dialog_app_settings_helper.h
 * @author hhool (hhool@outlook.com)
 * @brief  dialog app settings base class for app, help to manage app settings
 * @version 0.1
 * @date 2024-11-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_DIALOG_APP_SETTINGS_HELPER_H_
#define APP_UI_DIALOG_APP_SETTINGS_HELPER_H_

#include <map>
#include <string>
#include <vector>

namespace anx {
namespace settings {
class SettingSTLoad {
 public:
  SettingSTLoad() {}
  ~SettingSTLoad() {}
  class STLoadItem {
   public:
    /// @brief  STLoadItem constructor
    /// @param name  name of the stload item
    /// @param id  id of the stload item
    /// @param enabled  enable or disable the stload item
    /// @param sensor  sensor of the stload item default is "default", can be
    /// set "1KN"or "2KN" or "3KN" and so on
    STLoadItem(const std::string& name,
               const std::string& id,
               bool enabled,
               const std::string& sensor)
        : name_(name), id_(id), enable_(enabled), sensor_(sensor) {}
    ~STLoadItem() {}
    std::string name_;
    std::string id_;
    bool enable_ = false;
    std::string sensor_ = "default";
  };
  static int32_t LoadStloadList(
      std::vector<SettingSTLoad::STLoadItem>* stload_list);
  static int32_t SaveStloadList(
      const std::vector<SettingSTLoad::STLoadItem>& stload_list);
  static int32_t GetEnableStloadVersionFromList(
      const std::vector<SettingSTLoad::STLoadItem>& stload_list);
  static int32_t GetEnableStloadVersion();
  static std::string GetEnableStloadSensor();
  static int32_t ResetStload();

 public:
  std::string stload_dll_path_;
};

class SettingAppThird {
 public:
  SettingAppThird() {}
  ~SettingAppThird() {}

  /// @brief load third app from xml file app_settings_third_app.xml
  /// @return std::vector<std::string> third app info pos 0: name, pos 1: path
  static std::vector<std::string> LoadThirdApp();
  /// @brief save third app to xml file app_settings_third_app.xml
  /// @param name third app name and path info to save with file
  /// app_settings_third_app.xml
  static int32_t SaveThirdApp(const std::string& name,
                              const std::string& third_app_path);
  /// @brief reset third app to default value
  /// @return int32_t 0 success, other failed
  static int32_t ResetThirdApp();
  /// @brief  Get the third app path
  /// @return  std::string the third app path info, failed return empty string
  static std::string GetThirdAppPath();

  /// @brief  Get the third party name
  /// @return  std::string the third party name info, failed return empty string
  static std::string GetThirdAppName();

 public:
  std::vector<std::string> third_party_path_;
};

class SettingExpPathRule {
 public:
  SettingExpPathRule() {}
  ~SettingExpPathRule() {}

 public:
  /// @brief  Load the exp path rule from xml file config_exp_path_rule.xml
  /// @param exp_path_rule  the exp path rule to load
  /// @return  Rule index that enabled, 0 means starttime, 1 means endtime, 2
  /// current time
  static int32_t LoadExpPathRule(std::string* exp_path_rule);
  /// @brief  Save the exp path rule to xml file config_exp_path_rule.xml
  /// @param exp_path_rule  the exp path rule to save
  /// @return  int32_t 0 success, other failed
  static int32_t SaveExpPathRule(int32_t exp_path_rule);
  /// @brief  Reset the exp path rule to default value
  /// @return  int32_t 0 success, other failed
  static int32_t ResetExpPathRule();
  /// @brief  Get the exp path rule
  /// @return  int32_t the exp path rule info, 0 means starttime, 1menas
  /// endtime, 2 means current time point that is default value.
  static int32_t GetExpPathRule();
};

}  // namespace settings
}  // namespace anx

#endif  // APP_UI_DIALOG_APP_SETTINGS_HELPER_H_
