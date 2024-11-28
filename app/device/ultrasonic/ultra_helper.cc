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

#include "app/common/file_utils.h"
#include "app/common/logger.h"
#include "app/common/module_utils.h"

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

static std::string DefaultConfigXmlPath() {
#ifdef _WIN32
  return "default\\config_ultrasound.xml";
#else
  return "default/config_ultrasound.xml";
#endif
}

static bool LoadConfig(ConfigStLoad* config) {
  // get app data path
  std::string app_data_dir = anx::common::GetApplicationDataPath("anxi");
  std::string default_xml = DefaultConfigXmlPath();
  default_xml = app_data_dir + anx::common::kPathSeparator + default_xml;
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
