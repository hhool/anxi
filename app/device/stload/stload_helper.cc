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

#include "app/common/file_utils.h"
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
static std::string DefaultConfigXmlPath() {
  std::string default_config_xml_path = "default";
  default_config_xml_path += anx::common::kPathSeparator;
  default_config_xml_path += "config_stload.xml";
  return default_config_xml_path;
}

static bool LoadConfig(ConfigStLoad* config) {
  std::string app_data_dir = anx::common::GetApplicationDataPath("anxi");
  std::string config_file =
      app_data_dir + anx::common::kPathSeparator + DefaultConfigXmlPath();
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
int32_t anx::device::stload::STLoadHelper::version_ = 1;
int32_t anx::device::stload::STLoadHelper::sensor_ = 1;

bool STLoadHelper::InitStLoad(int32_t version, const char* sensor) {
  std::string module_dir = anx::common::GetModuleDir();
  std::string module_path = module_dir;
  if (version == 1) {
    // compare sensor with 10KN
    std::string file_settings_path =
        anx::common::GetModuleDir() + anx::common::kPathSeparator + "pilot";
    std::string dst_path =
        anx::common::GetModuleDir() + anx::common::kPathSeparator;
    std::string file_hard_param = "HardPara.mdb";
    std::string file_test_pilot = "TestPilot.ini";
    std::string file_wance_machine = "WanceMachine.mdb";
    std::string file_list[] = {file_hard_param, file_test_pilot,
                               file_wance_machine};
    if (!sensor || strcmp(sensor, "10KN") == 0) {
      file_settings_path += anx::common::kPathSeparator;
      file_settings_path += "ETM_";
      file_settings_path += sensor;
      sensor_ = 1;
    } else if (!sensor || strcmp(sensor, "20KN") == 0) {
      file_settings_path += anx::common::kPathSeparator;
      file_settings_path += "ETM_";
      file_settings_path += sensor;
      sensor_ = 2;
    } else {
      LOG_F(LG_ERROR) << "sensor not supported: " << sensor;
      return false;
    }
    for (const auto& file : file_list) {
      std::string src_file =
          file_settings_path + anx::common::kPathSeparator + file;
      std::string dst_file = dst_path + file;
      if (!anx::common::CCopyFile(src_file, dst_file)) {
        LOG_F(LG_ERROR) << "copy file failed: " << src_file << " to "
                        << dst_file;
        return false;
      }
    }
    module_path = dst_path;
    module_path += "\\CTRL.dll";
  } else {
    module_path += "\\CTRL2.dll";
  }
  ConfigStLoad config_stload;
  LoadConfig(&config_stload);
  if (config_stload.use_stload_simulation) {
    module_path = module_dir + "\\app_stload_simulator.dll";
    is_stload_simulation_ = true;
  }
  if (!STLoadLoader::Load(module_path)) {
    return false;
  }
  version_ = version;
  if (version == 1) {
    STLoadLoader::st_api_.load_hardware_parameters(4);
  }
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
  if (version_ == 2) {
    rate = 150;
    machineType = 0;
    nDTCType = 0;
    nDataBlockSize = 5;
  }
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
  if (version_ == 1) {
    // 力传感器P值
    // sensoer 20KN
    int lLoad_P = 80;
    int lLoad_I = 0;
    int lLoad_D = 0;
    if (sensor_ == 1) {
      lLoad_P = 50;
    }
    // 位移传感器P值
    int lPosi_P = 20;
    int lPosi_I = 0;
    int lPosi_D = 0;

    // 引伸计P值
    int lExt_P = 30;
    int lExt_I = 0;
    int lExt_D = 0;
    bSuccess =
        anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_pid(
            CH_LOAD, lLoad_P, lLoad_I, lLoad_D)
            ? true
            : false;
    if (!bSuccess) {
      LOG_F(LG_ERROR) << "carry_pid failed";
      return -2;
    }
    LOG_F(LG_INFO) << "carry_pid success: " << "lLoad_P:" << lLoad_P << " "
                   << "lLoad_I:" << lLoad_I << " " << "lLoad_D:" << lLoad_D;
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
  } else {
    int lLoad_P = 75;
    int lLoad_I = 0;
    int lLoad_D = 0;

    // 位移传感器P值
    int lPosi_P = 100;
    int lPosi_I = 0;
    int lPosi_D = 0;

    // 引伸计P值
    int lExt_P = 80;
    int lExt_I = 0;
    int lExt_D = 0;
    bSuccess =
        anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_pid(
            CH_LOAD_V2, lLoad_P, lLoad_I, lLoad_D)
            ? true
            : false;
    if (!bSuccess) {
      LOG_F(LG_ERROR) << "carry_pid failed";
      return -2;
    }
    bSuccess =
        anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_pid(
            CH_POSI_V2, lPosi_P, lPosi_I, lPosi_D)
            ? true
            : false;
    if (!bSuccess) {
      LOG_F(LG_ERROR) << "carry_pid failed";
      return -3;
    }
    bSuccess =
        anx::device::stload::STLoadHelper::st_load_loader_.st_api_.carry_pid(
            CH_EXTN_V2, lExt_P, lExt_I, lExt_D)
            ? true
            : false;
    if (!bSuccess) {
      LOG_F(LG_ERROR) << "carry_pid failed";
      return -4;
    }
  }
  anx::device::stload::STLoadHelper::st_load_loader_.st_api_.set_test_dir(0);
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
