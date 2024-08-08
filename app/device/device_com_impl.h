/**
 * @file device_com_impl.h
 * @author hhool (hhool@outlook.com)
 * @brief device com implementation
 * @version 0.1
 * @date 2024-08-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_DEVICE_DEVICE_COM_IMPL_H_
#define APP_DEVICE_DEVICE_COM_IMPL_H_

#include <memory>
#include <string>
#include <vector>

#include "app/device/device_com.h"

namespace anx {
namespace device {

class ComPortDeviceImpl : public DeviceComInterface {
 public:
  ComPortDeviceImpl() = delete;
  ComPortDeviceImpl(std::string name);
  virtual ~ComPortDeviceImpl();

 public:
  void AddListener(DeviceComListener* listener);
  void RemoveListener(DeviceComListener* listener);

 public:
  /// impliment DeviceComInterface
  int32_t Open(const ComPortDevice& com_port) override;
  bool isOpened() override;
  void Close() override;
  int32_t Read(uint8_t* buffer, int32_t size) override;
  int32_t Write(const uint8_t* buffer, int32_t size) override;
  int32_t WriteRead(const uint8_t* write_buffer,
                    int32_t write_size,
                    uint8_t* read_buffer,
                    int32_t read_size) override;

 public:
  const std::string GetName() const;
  const ComPortDevice& GetComPortDevice() const;

 protected:
  std::string name_;
  ComPortDevice com_port_device_;
  DeviceComListener* listener_;
  void* native_serialport_;
  std::vector<DeviceComListener*> listeners_;
};

}  // namespace device
}  // namespace anx

#endif  // APP_DEVICE_DEVICE_COM_IMPL_H_
