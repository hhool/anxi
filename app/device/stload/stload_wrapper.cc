/**
 * @file stload_wrapper.cc
 * @author hhool (hhool@outlook.com)
 * @brief  st load api wrapper, load the st load api dll and call the api.
 * @version 0.1
 * @date 2024-09-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/device/stload/stload_wrapper.h"

#include <wchar.h>
#include <windows.h>
#include <string>
#include <vector>

#include "app/common/logger.h"
#include "app/common/module_utils.h"
#include "app/common/string_utils.h"

namespace anx {
namespace device {
namespace stload {

void* STLoadLoader::handle_ = nullptr;
stload_api STLoadLoader::st_api_;

STLoadLoader::STLoadLoader() {}

STLoadLoader::~STLoadLoader() {}

bool STLoadLoader::Load(const std::string& dll_path) {
  if (dll_path.empty()) {
    return false;
  }
  HMODULE h_module_ = nullptr;
  if (h_module_ != nullptr) {
    return false;
  }
  LOG_F(LG_INFO) << "Load st load dll: " << dll_path;
  h_module_ =
      LoadLibraryW(anx::common::string2wstring(dll_path.c_str()).c_str());
  if (h_module_ == nullptr) {
    return false;
  }

  st_api_.load_hardware_parameters = reinterpret_cast<LoadHareWareParameters>(
      GetProcAddress(h_module_, "LoadHareWareParameters"));
  st_api_.get_load_sensors = reinterpret_cast<GetLoadSensors>(
      GetProcAddress(h_module_, "GetLoadSensors"));
  st_api_.get_extensions = reinterpret_cast<GetExtensions>(
      GetProcAddress(h_module_, "GetExtensions"));
  st_api_.get_extend_sensors = reinterpret_cast<GetExtendSensors>(
      GetProcAddress(h_module_, "GetExtendSensors"));
  st_api_.open_device =
      reinterpret_cast<OpenDevice>(GetProcAddress(h_module_, "OpenDevice"));
  st_api_.close_device =
      reinterpret_cast<CloseDevice>(GetProcAddress(h_module_, "CloseDevice"));

  st_api_.on_line =
      reinterpret_cast<OnLine>(GetProcAddress(h_module_, "OnLine"));
  st_api_.off_line =
      reinterpret_cast<OffLine>(GetProcAddress(h_module_, "OffLine"));

  st_api_.end_read =
      reinterpret_cast<EndRead>(GetProcAddress(h_module_, "EndRead"));
  st_api_.before_get_sample = reinterpret_cast<BeforeGetSample>(
      GetProcAddress(h_module_, "BeforeGetSample"));
  st_api_.after_get_sample = reinterpret_cast<AfterGetSample>(
      GetProcAddress(h_module_, "AfterGetSample"));
  st_api_.set_sect_corr_a =
      reinterpret_cast<SetSectCorrA>(GetProcAddress(h_module_, "SetSectCorrA"));
  st_api_.set_resolve =
      reinterpret_cast<SetResolve>(GetProcAddress(h_module_, "SetResolve"));
  st_api_.set_dest_wnd =
      reinterpret_cast<SetDestWnd>(GetProcAddress(h_module_, "SetDestWnd"));

  st_api_.get_load =
      reinterpret_cast<GetLoad>(GetProcAddress(h_module_, "GetLoad"));
  st_api_.get_posi =
      reinterpret_cast<GetPosi>(GetProcAddress(h_module_, "GetPosi"));
  st_api_.get_extn =
      reinterpret_cast<GetExtn>(GetProcAddress(h_module_, "GetExtn"));
  st_api_.get_ext1 =
      reinterpret_cast<GetExt1>(GetProcAddress(h_module_, "GetExt1"));
  st_api_.get_test_time =
      reinterpret_cast<GetTestTime>(GetProcAddress(h_module_, "GetTestTime"));
  st_api_.get_test_status = reinterpret_cast<GetTestStatus>(
      GetProcAddress(h_module_, "GetTestStatus"));
  st_api_.tare_load =
      reinterpret_cast<TareLoad>(GetProcAddress(h_module_, "TareLoad"));
  st_api_.tare_posi =
      reinterpret_cast<TarePosi>(GetProcAddress(h_module_, "TarePosi"));
  st_api_.tare_extn =
      reinterpret_cast<TareExtn>(GetProcAddress(h_module_, "TareExtn"));
  st_api_.tare_ext1 =
      reinterpret_cast<TareExt1>(GetProcAddress(h_module_, "TareExt1"));
  st_api_.tare_time =
      reinterpret_cast<TareTime>(GetProcAddress(h_module_, "TareTime"));
  st_api_.carry_pid =
      reinterpret_cast<CarryPID>(GetProcAddress(h_module_, "CarryPID"));
  st_api_.set_test_dir =
      reinterpret_cast<SetTestDir>(GetProcAddress(h_module_, "SetTestDir"));
  st_api_.carry_200 =
      reinterpret_cast<Carry200>(GetProcAddress(h_module_, "Carry200"));
  st_api_.carry_210 =
      reinterpret_cast<Carry210>(GetProcAddress(h_module_, "Carry210"));
  st_api_.stop_run =
      reinterpret_cast<StopRun>(GetProcAddress(h_module_, "StopRun"));
  if (st_api_.open_device == nullptr ||
      st_api_.load_hardware_parameters == nullptr ||
      st_api_.on_line == nullptr || st_api_.get_posi == nullptr ||
      st_api_.get_extn == nullptr || st_api_.get_ext1 == nullptr ||
      st_api_.get_test_time == nullptr || st_api_.get_test_status == nullptr ||
      st_api_.tare_load == nullptr || st_api_.tare_posi == nullptr ||
      st_api_.tare_extn == nullptr || st_api_.tare_ext1 == nullptr ||
      st_api_.tare_time == nullptr || st_api_.carry_pid == nullptr ||
      st_api_.set_test_dir == nullptr || st_api_.carry_200 == nullptr ||
      st_api_.carry_210 == nullptr || st_api_.stop_run == nullptr ||
      st_api_.off_line == nullptr || st_api_.close_device == nullptr ||
      st_api_.end_read == nullptr || st_api_.before_get_sample == nullptr ||
      st_api_.after_get_sample == nullptr ||
      st_api_.set_sect_corr_a == nullptr || st_api_.set_resolve == nullptr ||
      st_api_.set_dest_wnd == nullptr || st_api_.get_load == nullptr) {
    FreeLibrary(h_module_);
    return false;
  }

  handle_ = h_module_;
  ForceModuleAsCurrentWorkingDirectory();
  return true;
}

void STLoadLoader::Unload() {
  if (handle_ != nullptr) {
    HMODULE h_module_ = static_cast<HMODULE>(handle_);
    FreeLibrary(h_module_);
    h_module_ = nullptr;
    handle_ = nullptr;
  }
}

void STLoadLoader::ForceModuleAsCurrentWorkingDirectory() {
  std::string module_dir = anx::common::GetAppPath();
  SetCurrentDirectory(anx::common::string2wstring(module_dir.c_str()).c_str());
}

}  // namespace stload
}  // namespace device
}  // namespace anx
