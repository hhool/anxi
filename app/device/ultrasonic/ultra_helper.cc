/**
 * @file ultra_helper.cc
 * @author hhool (hhool@outlook.com)
 * @brief  ultrasonic helper
 * @version 0.1
 * @date 2024-09-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/device/ultrasonic/ultra_helper.h"

#include <memory>
#include <string>

#include "third_party/tinyxml2/source/tinyxml2.h"

namespace anx {
namespace device {
namespace ultrasonic {

bool UltrasonicHelper::is_ultrasonic_simulation_ = false;

typedef struct Config_t {
  bool use_stload_simulation = false;
} ConfigStLoad;

/**
 * <?xml version="1.0" encoding="utf-8"?>
 * <ultrasound>
    <simulation>1</simulation>
 * </ultrasound>
 */

static bool LoadConfig(ConfigStLoad* config) {
  std::string module_dir = anx::common::GetModuleDir();
  std::string config_file = module_dir + "\\default\\config_ultrasound.xml";
  tinyxml2::XMLDocument doc;
  if (doc.LoadFile(config_file.c_str()) != tinyxml2::XML_SUCCESS) {
    LOG_F(LG_ERROR) << "Load config file failed: " << config_file;
    return false;
  }
  tinyxml2::XMLElement* root = doc.RootElement();
  if (root == nullptr) {
    LOG_F(LG_ERROR) << "Root element is null";
    return false;
  }
  tinyxml2::XMLElement* simulation = root->FirstChildElement("simulation");
  if (simulation == nullptr) {
    LOG_F(LG_ERROR) << "simulation element is null";
    return false;
  }
  config->use_stload_simulation = simulation->IntText() == 1;
  return true;
}

bool UltrasonicHelper::InitUltrasonic() {
  std::string module_dir = anx::common::GetModuleDir();
  ConfigStLoad config_stload;
  LoadConfig(&config_stload);
  if (config_stload.use_stload_simulation) {
    is_ultrasonic_simulation_ = true;
  }
  return true;
}

bool UltrasonicHelper::Is_Ultrasonic_Simulation() {
  return is_ultrasonic_simulation_;
}

}  // namespace ultrasonic
}  // namespace device
}  // namespace anx
