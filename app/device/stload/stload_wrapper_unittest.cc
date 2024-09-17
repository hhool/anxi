/**
 * @file stload_wrapper_unittest.cc
 * @author hhool (hhool@outlook.com)
 * @brief st load api wrapper unittest
 * @version 0.1
 * @date 2024-09-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/device/stload/stload_wrapper.h"

#include <gtest/gtest.h>

#include "app/common/logger.h"
#include "app/common/module_utils.h"
#include "app/common/string_utils.h"

namespace anx {
namespace device {
namespace stload {

class STLoadLoaderTest : public ::testing::Test {
 protected:
  void SetUp() override {
    std::string module_dir = anx::common::GetModuleDir();
    module_path_ = module_dir + "\\CTRL.dll";
    ASSERT_TRUE(st_load_loader_.Load(module_path_));
  }
  void TearDown() override { st_load_loader_.Unload(); }

 public:
  bool Connect() {
    bool is_open = false;
    if (st_load_loader_.st_api_.open_device(nCommport)) {
      LOG_F(LG_INFO) << "open device success";
      is_open = true;
    }
    return is_open;
  }

  bool SetTestDir(int nTestDir) {
    st_load_loader_.st_api_.set_test_dir(nTestDir);
    return true;
  }

  bool OnLine() {
    return st_load_loader_.st_api_.on_line(nChannelNo, 0, 0, 0, rate, 4,
                                           nDTCType, sensorPosition, TestSpace,
                                           nDataBlockSize, isAE)
               ? true
               : false;
  }

  bool CarryPID(int type, int lPosi_P, int lPosi_I, int lPosi_D) {
    return st_load_loader_.st_api_.carry_pid(type, lPosi_P, lPosi_I, lPosi_D)
               ? true
               : false;
  }

  bool Carry200(int tCtrlMode,
                int tEndMode,
                float fTestSpeed,
                float tEndValue,
                int dir_no) {
    return st_load_loader_.st_api_.carry_200(tCtrlMode, tEndMode, fTestSpeed,
                                             tEndValue, 1, true, dir_no, 0,1,0)
               ? true
               : false;
  }

  bool StopRun() { return st_load_loader_.st_api_.stop_run() ? true : false; }

  bool Disconnect() {
    return st_load_loader_.st_api_.close_device() ? true : false;
  }

 public:
  std::string module_path_;
  STLoadLoader st_load_loader_;
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
};

TEST_F(STLoadLoaderTest, main) {
  try {
    st_load_loader_.st_api_.load_hardware_parameters(machineType);
  } catch (...) {
	  LOG_F(LG_ERROR);
  }
  /// connect device
  ASSERT_TRUE(Connect());
  ASSERT_TRUE(SetTestDir(1));
  ASSERT_TRUE(OnLine());
  ASSERT_TRUE(CarryPID(CH_LOAD, lLoad_P, lLoad_I, lLoad_D));
  ASSERT_TRUE(CarryPID(CH_POSI, lPosi_P, lPosi_I, lPosi_D));
  ASSERT_TRUE(CarryPID(CH_EXTN, lExt_P, lExt_I, lExt_D));
  // Run
  ASSERT_TRUE(Carry200(2, 2, 2.0f / 60.0f, 5, DIR_NO));
  // sleep 10 ms to wait for the test to finish
  Sleep(10);
  // Stop
  ASSERT_TRUE(StopRun());
  /// disconnect device
  ASSERT_TRUE(Disconnect());
}
// TEST_F(STLoadLoaderTest, OpenDevice) {  // NOLINT
//   ASSERT_NE(st_load_loader_.st_api_.open_device, nullptr);
//   ASSERT_TRUE(st_load_loader_.st_api_.open_device(1));
//   ASSERT_TRUE(st_load_loader_.st_api_.close_device());
//   ASSERT_TRUE(st_load_loader_.st_api_.open_device(2));
//   ASSERT_TRUE(st_load_loader_.st_api_.close_device());
//   ASSERT_TRUE(st_load_loader_.st_api_.open_device(3));
//   ASSERT_TRUE(st_load_loader_.st_api_.close_device());
//   ASSERT_TRUE(st_load_loader_.st_api_.open_device(4));
//   ASSERT_TRUE(st_load_loader_.st_api_.close_device());
// }

// TEST_F(STLoadLoaderTest, OpenDeviceTestDir) {
//   ASSERT_NE(st_load_loader_.st_api_.open_device, nullptr);
//   bool is_open = false;
//   for (int i = 1; i < 10; i++) {
//     if (st_load_loader_.st_api_.open_device(i)) {
//       LOG_F(LG_INFO) << "i:" << i;
//       is_open = true;
//       break;
//     }
//   }
//   if (!is_open) {
//     ASSERT_TRUE(false);
//   } else {
//     st_load_loader_.st_api_.set_test_dir(1);
//     ASSERT_TRUE(st_load_loader_.st_api_.close_device());
//   }
// }

// TEST_F(STLoadLoaderTest, OpenDeviceTestDirOnline) {
//   ASSERT_NE(st_load_loader_.st_api_.open_device, nullptr);
//   bool is_open = false;
//   for (int i = 1; i < 10; i++) {
//     if (st_load_loader_.st_api_.open_device(i)) {
//       LOG_F(LG_INFO) << "i:" << i;
//       is_open = true;
//       break;
//     }
//   }
//   if (!is_open) {
//     ASSERT_TRUE(false);
//   } else {
//     st_load_loader_.st_api_.set_test_dir(1);
//     // 通道量
//     // 1-力通道，2-力，引伸计通道，3-力，引伸计，扩展1通道
//     int nChannelNo = 2;
//     // loadSensitivity
//     // 力传感器灵敏度
//     int rate = 30;
//     int TestSpace = 20;
//     int DCType = 2;
//     uint8_t sensorPosition = 0;
//     /*
//     OnLine(long channelNo = 2, int nLoadIndex=0, int nExtensionIndex=0, int
//     nExtendIndex=0,int rate = 30, int MachineType =4,int
//     DTCType=2, BYTE sensorPosition = SENSOR_MIDBEAM_UP,  BYTE
//     CloseOfflineCheck = 0, int TestSpace=20; int dataBlockSize = 2, bool isAE
//     = 0);*/
// ASSERT_TRUE(
//     st_load_loader_.st_api_
//         .on_line(2, 0, 0, 0, rate, 4, DCType, sensorPosition, TestSpace, 2,
//         0));
// ASSERT_TRUE(st_load_loader_.st_api_.close_device());
// }
// }

// TEST_F(STLoadLoaderTest, OpenDeviceTestDirOnlineCanrryPID) {
//   ASSERT_NE(st_load_loader_.st_api_.open_device, nullptr);
//   bool is_open = false;
//   for (int i = 1; i < 10; i++) {
//     if (st_load_loader_.st_api_.open_device(i)) {
//       LOG_F(LG_INFO) << "i:" << i;
//       is_open = true;
//       break;
//     }
//   }
//   if (!is_open) {
//     ASSERT_TRUE(false);
//   } else {
//     st_load_loader_.st_api_.set_test_dir(1);
//     // 通道量
//     // 1-力通道，2-力，引伸计通道，3-力，引伸计，扩展1通道
//     int nChannelNo = 2;
//     // loadSensitivity
//     // 力传感器灵敏度
//     int rate = 30;
//     int TestSpace = 20;
//     int DCType = 2;
//     uint8_t sensorPosition = 0;
//     /*
//     OnLine(long channelNo = 2, int nLoadIndex=0, int nExtensionIndex=0, int
//     nExtendIndex=0,int rate = 30, int MachineType =4,int
//     DTCType=2, BYTE sensorPosition = SENSOR_MIDBEAM_UP,  BYTE
//     CloseOfflineCheck = 0, int TestSpace=20; int dataBlockSize = 2, bool isAE
//     = 0);*/ ASSERT_TRUE(st_load_loader_.st_api_.on_line(
//         2, 0, 0, 0, rate, 4, DCType, sensorPosition, TestSpace, 2, 0));
//     int CH_POSI = 8;   // 位移
//     int lPosi_P = 20;  // 位移P值
//     int lPosi_I = 0;   // 位移感器I值
//     int lPosi_D = 0;   // 位移感器D值
//     ASSERT_TRUE(
//         st_load_loader_.st_api_.carry_pid(CH_POSI, lPosi_P, lPosi_I,
//         lPosi_D));
//     ASSERT_TRUE(st_load_loader_.st_api_.close_device());
//   }
// }

// TEST_F(STLoadLoaderTest, OpenDeviceTestDirOnlineCanrryPID_RUN) {
//   ASSERT_NE(st_load_loader_.st_api_.open_device, nullptr);
//   bool is_open = false;
//   for (int i = 1; i < 10; i++) {
//     if (st_load_loader_.st_api_.open_device(i)) {
//       LOG_F(LG_INFO) << "i:" << i;
//       is_open = true;
//       break;
//     }
//   }
//   if (!is_open) {
//     ASSERT_TRUE(false);
//   } else {
//     st_load_loader_.st_api_.set_test_dir(1);
//     // 通道量
//     // 1-力通道，2-力，引伸计通道，3-力，引伸计，扩展1通道
//     int nChannelNo = 2;
//     // loadSensitivity
//     // 力传感器灵敏度
//     int rate = 30;
//     int TestSpace = 20;
//     int DCType = 2;
//     uint8_t sensorPosition = 0;
//     /*
//     OnLine(long channelNo = 2, int nLoadIndex=0, int nExtensionIndex=0, int
//     nExtendIndex=0,int rate = 30, int MachineType =4,int
//     DTCType=2, BYTE sensorPosition = SENSOR_MIDBEAM_UP,  BYTE
//     CloseOfflineCheck = 0, int TestSpace=20; int dataBlockSize = 2, bool isAE
//     = 0);*/ ASSERT_TRUE(st_load_loader_.st_api_.on_line(
//         2, 0, 0, 0, rate, 4, DCType, sensorPosition, TestSpace, 2, 0));
//     int CH_POSI = 8;   // 位移
//     int lPosi_P = 20;  // 位移P值
//     int lPosi_I = 0;   // 位移感器I值
//     int lPosi_D = 0;   // 位移感器D值
//     ASSERT_TRUE(
//         st_load_loader_.st_api_.carry_pid(CH_POSI, lPosi_P, lPosi_I,
//         lPosi_D));
//     int tCtrlMode = 2;
//     int tEndMode = 2;
//     float fTestSpeed = 2.0f / 60.0f;
//     int tEndValue = 5;
//     int DIR_NO = 0;
//     /*ASSERT_TRUE(st_load_loader_.st_api_.carry_200(
//         tCtrlMode, tEndMode, fTestSpeed, tEndValue, 1, true, DIR_NO));*/
//     ASSERT_TRUE(st_load_loader_.st_api_.close_device());
//   }
// }
}  // namespace stload
}  // namespace device
}  // namespace anx
