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
#include "app/device/device_exp_load_static_settings.h"

#include "third_party/tinyxml2/source/tinyxml2.h"

#if defined(_WIN32)
#include <tchar.h>
#include <windows.h>
#endif

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

int32_t STLoadHelper::STLoadSetup() {
  int machineType = 4;
  int nDTCType = 2;
  int nCommport = 4;
  int nChannelNo = 2;
  int rate = 30;
  int sensorPosition = 0;
  int TestSpace = 0;
  int nDataBlockSize = 2;
  bool isAE = false;

  // 力传感器P值
  int lLoad_P = 20;
  int lLoad_I = 0;
  int lLoad_D = 0;

  // 位移传感器P值
  int lPosi_P = 20;
  int lPosi_I = 0;
  int lPosi_D = 0;

  // 引伸计P值
  int lExt_P = 30;
  int lExt_I = 0;
  int lExt_D = 0;
  LOG_F(LG_ERROR) << "STLoadSetup";
  BOOL bSuccess =
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.on_line(
          nChannelNo, 0, 0, 0, rate, machineType, nDTCType, sensorPosition,
          TestSpace, nDataBlockSize, isAE)
          ? true
          : false;
  if (!bSuccess) {
    LOG_F(LG_ERROR) << "on_line failed";
    return -1;
  }
  bSuccess =
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_pid(
          CH_LOAD, lLoad_P, lLoad_I, lLoad_D)
          ? true
          : false;
  if (!bSuccess) {
    LOG_F(LG_ERROR) << "carry_pid failed";
    return -2;
  }
  bSuccess =
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_pid(
          CH_POSI, lPosi_P, lPosi_I, lPosi_D)
          ? true
          : false;
  if (!bSuccess) {
    LOG_F(LG_ERROR) << "carry_pid failed";
    return -3;
  }
  bSuccess =
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_pid(
          CH_EXTN, lExt_P, lExt_I, lExt_D)
          ? true
          : false;
  if (!bSuccess) {
    LOG_F(LG_ERROR) << "carry_pid failed";
    return -4;
  }
  return 0;
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
