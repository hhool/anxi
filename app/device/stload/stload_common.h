/**
 * @file stload_common.h
 * @author hhool (hhool@outlook.com)
 * @brief st load common header file
 * @version 0.1
 * @date 2024-09-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_DEVICE_STLOAD_STLOAD_COMMON_H_
#define APP_DEVICE_STLOAD_STLOAD_COMMON_H_

#include <windows.h>
#include <string>
#include <vector>

#define CALL __stdcall

// 硬件通道号预定义CH_XXXX
#define CH_LOAD 0  // 负荷
#define CH_EXTN 1  // 引伸计
#define CH_EXT1 2  // 扩展1
#define CH_EXT2 3  // 扩展2
#define CH_EXT3 4  // 扩展3
#define CH_EXT4 5  // 扩展4
#define CH_EXT5 6  // 扩展5
#define CH_EXT6 7  // 扩展6
#define CH_POSI 8  // 位移

// 控制方式CTRL_XXXX
#define CTRL_LOAD 0  // 力控
#define CTRL_EXTN 1  // 变形控
#define CTRL_POSI 2  // 位移控

// 结束条件END_XXXX
#define END_LOAD 0  // 力相对量,AD码
#define END_EXTN 1  // 变形相对量,AD码
#define END_POSI 2  // 位移相对量,脉冲数
#define END_TIME 3  // 时间相对量,0.01s

// 运行方向DIR_XXXX
#define DIR_NO 0  // 方向由下位机自己定义
#define DIR_UP 1  // 正向

typedef BOOL(CALL* LoadHareWareParameters)(int nMachineType);
typedef char*(CALL* GetLoadSensors)();
typedef char*(CALL* GetExtensions)();
typedef char*(CALL* GetExtendSensors)();

typedef BOOL(CALL* OpenDevice)(long uUnit);  // NOLINT
typedef BOOL(CALL* CloseDevice)();
typedef BOOL(CALL* EndRead)();
typedef BOOL(CALL* OnLine)(long channelNo,  // NOLINT
                           int nLoadIndex,
                           int nExtensionIndex,
                           int nExtendIndex,
                           int rate,
                           int MachineType,
                           int DTCType,
                           BYTE sensorPosition,
                           BYTE TestSpace,
                           int dataBlockSize,
                           bool isAE);
typedef BOOL(CALL* BeforeGetSample)();
typedef void(CALL* AfterGetSample)();

typedef void(CALL* SetSectCorrA)(long channel,  // NOLINT
                                 double corr);  // NOLINT
typedef BOOL(CALL* SetResolve)(long channel,    // NOLINT
                               DWORD resolve,   // NOLINT
                               float norinal);  // NOLINT

typedef BOOL(CALL* SetDestWnd)(HWND dest_wnd);  // NOLINT

///////////////////////////////////////////////
/// @brief Get the load
/// @return the load
typedef double(CALL* GetLoad)();  // NOLINT
/// @brief Get the position
/// @return the position
typedef double(CALL* GetPosi)();  // NOLINT
/// @brief Get the extension
/// @return the extension
typedef double(CALL* GetExtn)();  // NOLINT
/// @brief Get the extension one
/// @return the extension one
typedef double(CALL* GetExt1)();  // NOLINT
/// @brief Get the test time
/// @return the test time
typedef double(CALL* GetTestTime)();  // NOLINT
/// @brief Get the test status
/// @return the test status
typedef DWORD(CALL* GetTestStatus)();

///////////////////////////////////////////////
/// @brief clear the load
/// @return BOOL
typedef BOOL(CALL* TareLoad)();
/// @brief clear the position
/// @return BOOL
typedef BOOL(CALL* TarePosi)();
/// @brief clear the extension
/// @return BOOL
typedef BOOL(CALL* TareExtn)();
/// @brief clear the extension one
/// @return BOOL
typedef BOOL(CALL* TareExt1)();
/// @brief clear the test time
/// @return BOOL
typedef BOOL(CALL* TareTime)();

///////////////////////////////////////////////
/// @brief Carry the PID
/// @param channel the channel
/// @param Kp the Kp
/// @param Ki the Ki
/// @param Kd the Kd
/// @return BOOL
typedef BOOL(CALL* CarryPID)(long channel,  // NOLINT
                             long Kp,       // NOLINT
                             long Ki,       // NOLINT
                             long Kd);      // NOLINT
/// @brief Set the test dir
/// @param dir the dir
typedef void(CALL* SetTestDir)(long dir);  // NOLINT
/// @brief Carry the 200
/// @param control the control
/// @param end the end
/// @param speed the speed
/// @param value the value
/// @param threshold the threshold
/// @param priority the priority
/// @param dir the dir
/// @return BOOL
typedef BOOL(CALL* Carry200)(long control,  // NOLINT
                             long end,      // NOLINT
                             float speed,
                             float value,
                             float threshold,
                             BOOL priority,
                             long dir,        // NOLINT
                             float keepvalue, /* 0 */
                             long keepdatum,  // NOLINT /* 1 */
                             long TestModle   // NOLINT /* 0* */
);                                            // NOLINT
///////////////////////////////////////////////
/// @brief Carry the 210
/// @param lOpen the lOpen
/// @return BOOL
typedef BOOL(CALL* Carry210)(long lOpen);  // NOLINT
/// @brief Stop the run
/// @return BOOL
typedef BOOL(CALL* StopRun)();
/// @brief OffLine the
/// @return BOOL
typedef BOOL(CALL* OffLine)();
/// @brief Close the device
/// @return BOOL
typedef BOOL(CALL* CloseDevice)();

namespace anx {
namespace device {
namespace stload {

struct stload_api {
  LoadHareWareParameters load_hardware_parameters;
  GetLoadSensors get_load_sensors;
  GetExtensions get_extensions;
  GetExtendSensors get_extend_sensors;
  OpenDevice open_device;
  CloseDevice close_device;
  OnLine on_line;
  OffLine off_line;
  EndRead end_read;
  BeforeGetSample before_get_sample;
  AfterGetSample after_get_sample;
  SetSectCorrA set_sect_corr_a;
  SetResolve set_resolve;
  SetDestWnd set_dest_wnd;
  GetLoad get_load;
  GetPosi get_posi;
  GetExtn get_extn;
  GetExt1 get_ext1;
  GetTestTime get_test_time;
  GetTestStatus get_test_status;
  TareLoad tare_load;
  TarePosi tare_posi;
  TareExtn tare_extn;
  TareExt1 tare_ext1;
  TareTime tare_time;
  CarryPID carry_pid;
  SetTestDir set_test_dir;
  Carry200 carry_200;
  Carry210 carry_210;
  StopRun stop_run;
};
}  // namespace stload
}  // namespace device
}  // namespace anx

#endif  // APP_DEVICE_STLOAD_STLOAD_COMMON_H_
