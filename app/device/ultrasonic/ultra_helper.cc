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
  // get app data path
  std::string app_data_dir = anx::common::GetApplicationDataPath();
  std::string default_xml = "\\default\\config_ultrasound.xml";
#ifdef _WIN32
  default_xml = app_data_dir + "\\anxi\\" + default_xml;
#else
  default_xml = app_data_dir + "/anxi/" + default_xml;
#endif
  tinyxml2::XMLDocument doc;
  if (doc.LoadFile(default_xml.c_str()) != tinyxml2::XML_SUCCESS) {
    LOG_F(LG_ERROR) << "Load config file failed: " << default_xml;
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
  std::string module_dir = anx::common::GetApplicationDataPath();
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
