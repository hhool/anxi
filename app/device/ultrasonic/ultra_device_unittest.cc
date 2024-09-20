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
  }

  void TearDown() override { delete ultra_device_; }

  UltraDevice* ultra_device_;
  std::shared_ptr<anx::device::DeviceComInterface> device_com_interface_;
  DeviceComEvent device_com_event_;
};

TEST_F(UltraDeviceTest, Open) {
  ComPort com_port;
  com_port.baud_rate = 9600;
  com_port.data_bits = 8;
  com_port.stop_bits = 1;
  com_port.parity = 0;
  com_port.flow_control = 0;
  com_port.timeout = 1000;
  anx::device::ComSettings com_settings(anx::device::kDeviceCom_Ultrasound,
                                        ("COM3"), com_port);
  EXPECT_EQ(0, ultra_device_->Open(com_settings));
}

TEST_F(UltraDeviceTest, Close) {
  ultra_device_->Close();
}

}  // namespace device
}  // namespace anx
