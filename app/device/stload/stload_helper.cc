/**
 * @file stload_helper.cc
 * @author hhool (hhool@outlook.com)
 * @brief st load helper
 * @version 0.1
 * @date 2024-09-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/device/stload/stload_helper.h"

#include <memory>
#include <string>

#include "app/common/logger.h"
#include "app/common/module_utils.h"
#include "app/common/string_utils.h"

#include "third_party/tinyxml2/source/tinyxml2.h"

namespace anx {
namespace device {
namespace stload {
typedef struct Config_t {
  bool use_stload_simulation = false;
} ConfigStLoad;

/**
 * <?xml version="1.0" encoding="utf-8"?>
 * <stload>
     <simulation>1</simulation>
 * </stload>
 */
static bool LoadConfig(ConfigStLoad* config) {
  std::string module_dir = anx::common::GetModuleDir();
  std::string config_file = module_dir + "\\default\\config_stload.xml";
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

STLoadLoader anx::device::stload::STLoadHelper::st_load_loader_;
bool anx::device::stload::STLoadHelper::is_stload_simulation_ = false;

bool STLoadHelper::InitStLoad() {
  std::string module_dir = anx::common::GetModuleDir();
  std::string module_path = module_dir + "\\CTRL.dll";
  ConfigStLoad config_stload;
  LoadConfig(&config_stload);
  if (config_stload.use_stload_simulation) {
    module_path = module_dir + "\\app_stload_simulator.dll";
    is_stload_simulation_ = true;
  }
  if (!STLoadLoader::Load(module_path)) {
    return false;
  }
  STLoadLoader::st_api_.load_hardware_parameters(4);
  return true;
}

void STLoadHelper::UnInitStLoad() {
  STLoadLoader::Unload();
}

bool STLoadHelper::Is_Stload_Simulation() {
  return is_stload_simulation_;
}

}  // namespace stload
}  // namespace device
}  // namespace anx
