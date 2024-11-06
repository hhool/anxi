/**
 * @file stload_device.cc
 * @author hhool (hhool@outlook.com)
 * @brief  stload device class implementation
 * @version 0.1
 * @date 2024-09-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/device/stload_simulation/stload_device.h"

#include <memory>
#include <string>

#include "app/common/logger.h"
#include "app/common/module_utils.h"
#include "app/common/string_utils.h"
#include "app/common/thread.h"
#include "app/device/device_com_factory.h"
#include "app/device/device_com_settings.h"

#if defined(_WIN32)
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

#if defined(_WIN32)
HWND g_dest_wnd;
#else
pthread_t g_thread;
#endif

std::unique_ptr<anx::common::Mutex> g_mutex;
std::unique_ptr<anx::common::Thread> g_thread;
std::shared_ptr<anx::device::DeviceComInterface> g_device_com_sl_;

#if 0
static int32_t LOG_LEVEL = LG_INFO;
#else
static int32_t LOG_LEVEL = LG_SENSITIVE;
#endif

namespace {
class Worker : public anx::common::Runnable {
 public:
  Worker() {}
  ~Worker() override {}

  void run() override {
    while (true) {
      anx::common::AutoLock lock(&mutex_);
      printf("Worker\n");
#if defined(_WIN32)
      if (g_dest_wnd != nullptr) {
        // Send message to dest window
        PostMessage(g_dest_wnd, DLLMSG, 0, DLL_SAMPLE);
      }
      // Sleep 40ms
      Sleep(40);
#else
      // unimplemented
      // Sleep 40ms
      usleep(40 * 1000);
#endif
      // check if thread should be stopped
      if (is_interrupt()) {
        break;
      }
    }
  }

 private:
  anx::common::Mutex mutex_;
};
}  // namespace

std::unique_ptr<Worker> g_worker;

BOOL CALL SetDestWnd(HWND dest_wnd) {
  g_dest_wnd = dest_wnd;
  LOG_F(LOG_LEVEL) << "SetDestWnd: " << dest_wnd;
  return TRUE;
}

BOOL CALL LoadHareWareParameters(int nMachineType) {
  /// @note Get module directory
  std::string module_dir = anx::common::GetModuleDir();
  /// @note Get the dll path
  std::string dll_path = module_dir + "/stload_simulation.dll";
  LOG_F(LOG_LEVEL) << "module st load dll: " << dll_path;
  return TRUE;
}

char* CALL GetLoadSensors() {
  LOG_F(LOG_LEVEL) << "GetLoadSensors";
  return "GetLoadSensors";
}

char* CALL GetExtensions() {
  LOG_F(LOG_LEVEL) << "GetExtensions";
  return "GetExtensions";
}

char* CALL GetExtendSensors() {
  LOG_F(LOG_LEVEL) << "GetExtendSensors";
  return "GetExtendSensors";
}

void CALL SetTestDir(long dir) {  // NOLINT
  LOG_F(LOG_LEVEL) << "SetTestDir";
}

BOOL CALL CarryPID(long channel, long Kp, long Ki, long Kd) {  // NOLINT
  LOG_F(LOG_LEVEL) << "CarryPID";
  return TRUE;
}

BOOL CALL OpenDevice(long uUnit) {  // NOLINT
  LOG_F(LOG_LEVEL) << "OpenDevice";
  return TRUE;
}

BOOL CALL CloseDevice() {
  LOG_F(LOG_LEVEL) << "CloseDevice";
  return TRUE;
}

BOOL CALL StartRead() {
  LOG_F(LOG_LEVEL) << "StartRead";
  return TRUE;
}

BOOL CALL EndRead() {
  LOG_F(LOG_LEVEL) << "EndRead";
  return TRUE;
}

BOOL CALL OnLine(long channelNo,  // NOLINT
                 int nLoadIndex,
                 int nExtensionIndex,
                 int nExtendIndex,
                 int rate,
                 int machineType,
                 int DTCType,
                 BYTE sensorPosition,
                 BYTE CloseOfflineCheck,
                 int dataBlockSize,
                 BOOL isAE) {
  LOG_F(LOG_LEVEL) << "OnLine";
  if (g_thread.get() != nullptr) {
    return FALSE;
  }
  g_worker.reset(new Worker());
  g_thread.reset(new anx::common::Thread(g_worker.get()));
  g_thread->start();
  return TRUE;
}

BOOL CALL OffLine() {
  LOG_F(LOG_LEVEL) << "OffLine";
  if (g_thread.get() == nullptr) {
    return FALSE;
  }
  g_worker->interrupt();
  g_thread->join();
  g_thread.reset();
  return TRUE;
}

BOOL CALL Carry200(long control,  // NOLINT
                   long end,      // NOLINT
                   float speed,
                   float value,
                   float threshold,
                   BOOL priority,
                   long dir,        // NOLINT
                   float keepvalue, /* 0 */
                   long keepdatum,  // NOLINT /* 1 */
                   long TestModle   // NOLINT /* 0* */
) {                                 // NOLINT
  LOG_F(LOG_LEVEL) << "Carry200";
  return TRUE;
}

BOOL CALL Carry210(long lOpen) {  // NOLINT
  LOG_F(LOG_LEVEL) << "Carry210";
  return TRUE;
}

BOOL CALL StopRun() {
  LOG_F(LOG_LEVEL) << "StopRun";
  return TRUE;
}

BOOL CALL BeforeGetSample() {
  LOG_F(LOG_LEVEL) << "BeforeGetSample";
  return TRUE;
}

void CALL AfterGetSample() {
  LOG_F(LOG_LEVEL) << "AfterGetSample";
}

void CALL SetSectCorrA(long channel, double corr) {  // NOLINT
  LOG_F(LOG_LEVEL) << "SetSectCorrA";
}

BOOL CALL SetResolve(long channel,  // NOLINT
                     DWORD resolve,
                     float norinal) {  // NOLINT
  LOG_F(LOG_LEVEL) << "SetResolve";
  return TRUE;
}

double CALL GetLoad() {
  LOG_F(LOG_LEVEL) << "GetLoad";
  return 0.0;
}

double CALL GetPosi() {
  LOG_F(LOG_LEVEL) << "GetPosi";
  return 0.0;
}

double CALL GetExtn() {
  LOG_F(LOG_LEVEL) << "GetExtn";
  return 0.0;
}

double CALL GetExt1() {
  LOG_F(LOG_LEVEL) << "GetExt1";
  return 0.0;
}

double CALL GetTestTime() {
  LOG_F(LOG_LEVEL) << "GetTestTime";
  return 0.0;
}

DWORD CALL GetTestStatus() {
  LOG_F(LOG_LEVEL) << "GetTestStatus";
  return 0;
}

BOOL CALL TareLoad() {
  LOG_F(LOG_LEVEL) << "TareLoad";
  return TRUE;
}

BOOL CALL TarePosi() {
  LOG_F(LOG_LEVEL) << "TarePosi";
  return TRUE;
}

BOOL CALL TareExtn() {
  LOG_F(LOG_LEVEL) << "TareExtn";
  return TRUE;
}

BOOL CALL TareExt1() {
  LOG_F(LOG_LEVEL) << "TareExt1";
  return TRUE;
}

BOOL CALL TareTime() {
  LOG_F(LOG_LEVEL) << "TareTime";
  return TRUE;
}
