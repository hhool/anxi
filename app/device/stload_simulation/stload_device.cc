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
  printf("SetDestWnd\n");
  g_dest_wnd = dest_wnd;
  return TRUE;
}

BOOL CALL LoadHareWareParameters(int nMachineType) {
  printf("LoadHareWareParameters\n");
  return TRUE;
}

char* CALL GetLoadSensors() {
  printf("GetLoadSensors\n");
  return "GetLoadSensors";
}

char* CALL GetExtensions() {
  printf("GetExtensions\n");
  return "GetExtensions";
}

char* CALL GetExtendSensors() {
  printf("GetExtendSensors\n");
  return "GetExtendSensors";
}

void CALL SetTestDir(long dir) {  // NOLINT
  printf("SetTestDir\n");
}

BOOL CALL CarryPID(long channel, long Kp, long Ki, long Kd) {  // NOLINT
  printf("CarryPID\n");
  return TRUE;
}

BOOL CALL OpenDevice(long uUnit) {  // NOLINT
  printf("OpenDevice\n");
  return TRUE;
}

BOOL CALL CloseDevice() {
  printf("CloseDevice\n");
  return TRUE;
}

BOOL CALL StartRead() {
  printf("StartRead\n");
  return TRUE;
}

BOOL CALL EndRead() {
  printf("EndRead\n");
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
  printf("OnLine\n");
  if (g_thread.get() != nullptr) {
    return FALSE;
  }
  g_worker.reset(new Worker());
  g_thread.reset(new anx::common::Thread(g_worker.get()));
  g_thread->start();
  return TRUE;
}

BOOL CALL OffLine() {
  printf("OffLine\n");
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
  printf("Carry200\n");
  return TRUE;
}

BOOL CALL Carry210(long lOpen) {  // NOLINT
  printf("Carry210\n");
  return TRUE;
}

BOOL CALL StopRun() {
  printf("StopRun\n");
  return TRUE;
}

BOOL CALL BeforeGetSample() {
  printf("BeforeGetSample\n");
  return TRUE;
}

void CALL AfterGetSample() {
  printf("AfterGetSample\n");
}

void CALL SetSectCorrA(long channel, double corr) {  // NOLINT
  printf("SetSectCorrA\n");
}

BOOL CALL SetResolve(long channel,  // NOLINT
                     DWORD resolve,
                     float norinal) {  // NOLINT
  printf("SetResolve\n");
  return TRUE;
}

double CALL GetLoad() {
  printf("GetLoad\n");
  return 0.0;
}

double CALL GetPosi() {
  printf("GetPosi\n");
  return 0.0;
}

double CALL GetExtn() {
  printf("GetExtn\n");
  return 0.0;
}

double CALL GetExt1() {
  printf("GetExt1\n");
  return 0.0;
}

double CALL GetTestTime() {
  printf("GetTestTime\n");
  return 0.0;
}

DWORD CALL GetTestStatus() {
  printf("GetTestStatus\n");
  return 0;
}

BOOL CALL TareLoad() {
  printf("TareLoad\n");
  return TRUE;
}

BOOL CALL TarePosi() {
  printf("TarePosi\n");
  return TRUE;
}

BOOL CALL TareExtn() {
  printf("TareExtn\n");
  return TRUE;
}

BOOL CALL TareExt1() {
  printf("TareExt1\n");
  return TRUE;
}

BOOL CALL TareTime() {
  printf("TareTime\n");
  return TRUE;
}