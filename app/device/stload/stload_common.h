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
#define DIR_DOWN -1  // 负向


#define KP_DEST 0  // 保持指定值
#define KP_CURR 1  // 保持当前值

// 属于DSP内部消息,
#define DSPMSG WM_USER + 4000  // 发送给主窗口的消息
// lParam的值如下DSP_XXXX
#define DSP_KBSTOP 0x1     // 停止
#define DSP_RUN 0x2        // 运行
#define DSP_QKUP 0x4       // 快上
#define DSP_QKDWN 0x8      // 快下
#define DSP_SLWUP 0x10     // 慢上
#define DSP_SLWDWN 0x20    // 慢下
#define DSP_CMDEND 0x40    // 命令完成状态
#define DSP_BD 0x80        // 标定
#define DSP_LDCTRL 0x100   // 力控运行
#define DSP_EXCTRL 0x200   // 变形控运行
#define DSP_PSCTRL 0x400   // 位移控运行
#define DSP_TOPLMT 0x800   // 上限位
#define DSP_LOWLMT 0x1000  // 下限位
#define DSP_KBRUN 0x2000   // 小键盘运行命令
#define DSP_KBPRT 0x4000   // 小键盘试样保护命令
#define DSP_ADC 0x8000     // 被控量ADC转换标志,内部标志

#define DSP_BDREV 0x10000      // 小键盘状态
#define DSP_BATERR 0x20000     // 电池报警
#define DSP_SWITCH 0x40000     // 命令状态交替
#define DSP_PCSTS 0x80000      // PC状态
#define DSP_XXXX 0x100000      // 给定/自动位移脉冲,内部标志
#define DSP_OVERLD 0x200000    // 力超载
#define DSP_DIR 0x400000       // 向上/向下
#define DSP_OVERDIR 0x800000   // 超载时的方向
#define DSP_TEST 0x1000000     // 是否是试验状态
#define DSP_HIT 0x2000000      // 防碰撞运行
#define DSP_NGTV 0x4000000     // 负方向标志
#define DSP_PSTV 0x8000000     // 正方向标志
#define DSP_AUTOSW 0x10000000  // 自动切换
#define DSP_PSXOR 0x20000000   // 位移和上下的异或值
#define DSP_LDXOR 0x40000000   // 力和上下的异或值
#define DSP_EXXOR 0x80000000   // 引伸计与上下的异或值
#define DSP_MASK 0xFFFFFFFF    // 已经定义的状态位

#define CODE_MASK10 0xFFFFFFC0   // 10万码
#define CODE_MASK20 0xFFFFFFE0   // 20万码
#define CODE_MASK50 0xFFFFFFF0   // 50万码
#define CODE_MASK100 0xFFFFFFF8  // 100万码

#define CODE_MASK0 0xFFFFFFFF  // 不滤
#define CODE_MASK1 0xFFFFFFFE  // 滤一位
#define CODE_MASK2 0xFFFFFFFC  // 滤二位
#define CODE_MASK3 0xFFFFFFF8  // 滤三位
#define CODE_MASK4 0xFFFFFFF0  // 滤四位
#define CODE_MASK5 0xFFFFFFE0  // 滤五位
#define CODE_MASK6 0xFFFFFFC0  // 滤六位

// 属于DLL自己发送的消息,告诉主程序某些信息
#define DLLMSG WM_USER + 4001
// lParam的值如下DLL_XXXX
#define DLL_SAMPLE 0x00000001  // 一次采样完成
#define DLL_FEEDBK 0x00000002  // 通讯反馈
#define DLL_198OK 0x00000004   // 198号命令发送成功
#define DLL_199OK 0x00000008   // 199号命令发送成功
#define DLL_200OK 0x00000010   // 200号命令发送成功
#define DLL_201OK 0x00000020   // 201号命令发送成功
#define DLL_202OK 0x00000040   // 202号命令发送成功
#define DLL_203OK 0x00000080   // 203号命令发送成功
#define DLL_SETOK 0x00010000   // 初始发送命令完成

// 有关单位
#define UNT_FORCE_N 0x0001    // 牛
#define UNT_FORCE_KN 0x0002   // 千牛
#define UNT_FORCE_CN 0x0004   // 厘牛
#define UNT_FORCE_KGF 0x0008  // 公斤力
#define UNT_FORCE_LBF 0x0010  // 磅力
#define UNT_FORCE_G 0x0020    // 克

#define UNT_LENTH_MM 0x0101  // 毫米
#define UNT_LENTH_IN 0x0102  // 英寸

// 机型
#define MACHINE_HUT_SP 0x00  // 液压单空间
#define MACHINE_HUT_DP 0x01  // 液压双空间
#define MACHINE_HHCT 0x02    // 液压压力机
#define MACHINE_XHCT 0x03    // 油电混动压力机
#define MACHINE_ETM 0x04     // 电拉万能拉力机
#define MACHINE_EHCT 0x05    // 电拉压力
#define MACHINE_ETMF 0x06    // 压折
#define MACHINE_ETT 0x07     // 扭转

// 传感器位置
#define SENSOR_MIDBEAM_DOWN 0x00  // 中横梁下置
#define SENSOR_MIDBEAM_UP 0x02    // 中横梁上置
#define SENSOR_UPBEAM 0x04        // 上横梁
#define SENSOR_BOTTOMBEAM 0x06    // 下横梁

// 试验空间
#define SPACE_DOWN 0x00  // 下空间
#define SPACE_UP 0x01    // 上空间

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
