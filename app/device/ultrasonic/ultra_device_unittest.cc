/**
 * @file ultra_device_unittest.cc
 * @author hhool (hhool@outlook.com)
 * @brief  ultrasonic device class unit test
 * @version 0.1
 * @date 2024-09-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/device/ultrasonic/ultra_device.h"

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

#include "app/common/crc16.h"
#include "app/common/time_utils.h"
#include "app/device/device_com.h"
#include "app/device/device_com_factory.h"

namespace anx {
namespace device {

class DeviceComEvent : public DeviceComListener {
 public:
  void OnDataReceived(anx::device::DeviceComInterface* device,
                      const uint8_t* data,
                      int32_t size) override {}
  /// @brief  On data outgoing
  /// @param data  data buffer
  /// @param size  data buffer size
  void OnDataOutgoing(anx::device::DeviceComInterface* device,
                      const uint8_t* data,
                      int32_t size) override {}
};

class UltraDeviceTest : public ::testing::Test {
 protected:
  void SetUp() override {
    device_com_interface_ =
        anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
            anx::device::kDeviceCom_Ultrasound, &device_com_event_);
    ultra_device_ = new UltraDevice(device_com_interface_.get());
    com_port_name_ = "COM6";
    MakeComPort();
  }

  void TearDown() override {
    delete ultra_device_;
    anx::device::DeviceComFactory::ReleaseInstance();
  }

  void MakeComPort() {
    com_port_.baud_rate = 115200;
    com_port_.data_bits = 8;
    com_port_.stop_bits = 1;
    com_port_.parity = 0;
    com_port_.flow_control = 0;
    com_port_.timeout = 1000;
  }

  UltraDevice* ultra_device_;
  std::shared_ptr<anx::device::DeviceComInterface> device_com_interface_;
  DeviceComEvent device_com_event_;
  ComAddressPort com_port_;
  std::string com_port_name_;
  static const int kSleepTimeMs = 1000;
};

TEST_F(UltraDeviceTest, OpenClose) {
  anx::device::ComSettings com_settings(anx::device::kDeviceCom_Ultrasound,
                                        com_port_name_.c_str(), &com_port_);
  EXPECT_EQ(0, ultra_device_->Open(com_settings));
  anx::common::sleep_ms(kSleepTimeMs);
  ultra_device_->Close();
}

TEST_F(UltraDeviceTest, StartStopUltraLoop) {
  anx::device::ComSettings com_settings(anx::device::kDeviceCom_Ultrasound,
                                        com_port_name_.c_str(), &com_port_);
  EXPECT_EQ(0, ultra_device_->Open(com_settings));
  EXPECT_EQ(0, ultra_device_->StartUltra());
  anx::common::sleep_ms(kSleepTimeMs);
  EXPECT_EQ(0, ultra_device_->StopUltra());
  ultra_device_->Close();
}

TEST_F(UltraDeviceTest, GetFaultCode) {
  anx::device::ComSettings com_settings(anx::device::kDeviceCom_Ultrasound,
                                        com_port_name_.c_str(), &com_port_);
  EXPECT_EQ(0, ultra_device_->Open(com_settings));
  EXPECT_EQ(0, ultra_device_->GetFaultCode());
  anx::common::sleep_ms(kSleepTimeMs);
  ultra_device_->StopUltra();

  EXPECT_EQ(0, ultra_device_->GetFaultCode());
  ultra_device_->Close();
}

TEST_F(UltraDeviceTest, GetCurrentFreq) {
  anx::device::ComSettings com_settings(anx::device::kDeviceCom_Ultrasound,
                                        com_port_name_.c_str(), &com_port_);
  EXPECT_EQ(0, ultra_device_->Open(com_settings));
  EXPECT_LT(0, ultra_device_->GetCurrentFreq());
  anx::common::sleep_ms(kSleepTimeMs);
  ultra_device_->StopUltra();

  EXPECT_LT(0, ultra_device_->GetCurrentFreq());
  ultra_device_->Close();
}

TEST_F(UltraDeviceTest, GetCurrentPower) {
  anx::device::ComSettings com_settings(anx::device::kDeviceCom_Ultrasound,
                                        com_port_name_.c_str(), &com_port_);
  EXPECT_EQ(0, ultra_device_->Open(com_settings));
  EXPECT_LT(0, ultra_device_->GetCurrentPower());
  anx::common::sleep_ms(kSleepTimeMs);
  ultra_device_->StopUltra();

  EXPECT_LT(0, ultra_device_->GetCurrentPower());
  ultra_device_->Close();
}

TEST_F(UltraDeviceTest, SetAndGetAmplitude) {
  anx::device::ComSettings com_settings(anx::device::kDeviceCom_Ultrasound,
                                        com_port_name_.c_str(), &com_port_);
  EXPECT_EQ(0, ultra_device_->Open(com_settings));
  EXPECT_EQ(0, ultra_device_->SetAmplitude(100));
  EXPECT_EQ(100, ultra_device_->GetAmplitude());
  EXPECT_EQ(0, ultra_device_->SetAmplitude(20));
  EXPECT_EQ(20, ultra_device_->GetAmplitude());
  anx::common::sleep_ms(kSleepTimeMs);
  ultra_device_->StopUltra();

  EXPECT_EQ(0, ultra_device_->SetAmplitude(100));
  EXPECT_EQ(100, ultra_device_->GetAmplitude());
  EXPECT_EQ(0, ultra_device_->SetAmplitude(20));
  EXPECT_EQ(20, ultra_device_->GetAmplitude());
  ultra_device_->Close();
}

TEST_F(UltraDeviceTest, SetGetWedingTime) {
  anx::device::ComSettings com_settings(anx::device::kDeviceCom_Ultrasound,
                                        com_port_name_.c_str(), &com_port_);
  EXPECT_EQ(0, ultra_device_->Open(com_settings));
  EXPECT_EQ(0, ultra_device_->SetWedingTime(90));
  EXPECT_EQ(90, ultra_device_->GetWedingTime());
  EXPECT_EQ(0, ultra_device_->SetWedingTime(0));
  EXPECT_EQ(0, ultra_device_->GetWedingTime());
  EXPECT_EQ(0, ultra_device_->StartUltra());
  anx::common::sleep_ms(kSleepTimeMs);
  ultra_device_->StopUltra();

  EXPECT_EQ(0, ultra_device_->SetWedingTime(80));
  EXPECT_EQ(80, ultra_device_->GetWedingTime());

  ultra_device_->Close();
}

TEST_F(UltraDeviceTest, GetMaxFreq) {
  anx::device::ComSettings com_settings(anx::device::kDeviceCom_Ultrasound,
                                        com_port_name_.c_str(), &com_port_);
  EXPECT_EQ(0, ultra_device_->Open(com_settings));
  EXPECT_LT(0, ultra_device_->GetMaxFreq());
  anx::common::sleep_ms(kSleepTimeMs);
  ultra_device_->StopUltra();

  EXPECT_LT(0, ultra_device_->GetMaxFreq());
  ultra_device_->Close();
}

TEST_F(UltraDeviceTest, GetMinFreq) {
  anx::device::ComSettings com_settings(anx::device::kDeviceCom_Ultrasound,
                                        com_port_name_.c_str(), &com_port_);
  EXPECT_EQ(0, ultra_device_->Open(com_settings));
  EXPECT_LT(0, ultra_device_->GetMinFreq());
  anx::common::sleep_ms(kSleepTimeMs);
  ultra_device_->StopUltra();

  EXPECT_LT(0, ultra_device_->GetMinFreq());
  ultra_device_->Close();
}

TEST_F(UltraDeviceTest, GetMaxPower) {
  anx::device::ComSettings com_settings(anx::device::kDeviceCom_Ultrasound,
                                        com_port_name_.c_str(), &com_port_);
  EXPECT_EQ(0, ultra_device_->Open(com_settings));
  EXPECT_EQ(0, ultra_device_->StartUltra());
  EXPECT_LT(0, ultra_device_->GetMaxPower());
  anx::common::sleep_ms(kSleepTimeMs);
  ultra_device_->StopUltra();

  EXPECT_LT(0, ultra_device_->GetMaxPower());
  ultra_device_->Close();
}

TEST_F(UltraDeviceTest, GetFreqAtMachineOn) {
  anx::device::ComSettings com_settings(anx::device::kDeviceCom_Ultrasound,
                                        com_port_name_.c_str(), &com_port_);
  EXPECT_EQ(0, ultra_device_->Open(com_settings));
  EXPECT_LT(0, ultra_device_->GetFreqAtMachineOn());
  anx::common::sleep_ms(kSleepTimeMs);
  ultra_device_->StopUltra();

  EXPECT_LT(0, ultra_device_->GetFreqAtMachineOn());
  ultra_device_->Close();
}

TEST_F(UltraDeviceTest, GetSoftTimeAtMachineOn) {
  anx::device::ComSettings com_settings(anx::device::kDeviceCom_Ultrasound,
                                        com_port_name_.c_str(), &com_port_);
  EXPECT_EQ(0, ultra_device_->Open(com_settings));
  EXPECT_LT(0, ultra_device_->GetSoftTimeAtMachineOn());
  anx::common::sleep_ms(kSleepTimeMs);
  ultra_device_->StopUltra();

  EXPECT_LT(0, ultra_device_->GetSoftTimeAtMachineOn());
  ultra_device_->Close();
}

}  // namespace device
}  // namespace anx
