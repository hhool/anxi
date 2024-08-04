/**
 * @file device_com_factory.h
 * @author hhool (hhool@outlook.com)
 * @brief  device com factory, create device com with device type
 * enum and device com settings.
 * @version 0.1
 * @date 2024-08-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_DEVICE_DEVICE_COM_FACTORY_H_
#define APP_DEVICE_DEVICE_COM_FACTORY_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "app/device/device_com.h"

namespace anx {
namespace device {

class DeviceComFactory {
 public:
  DeviceComFactory();
  virtual ~DeviceComFactory();

  DeviceComFactory(const DeviceComFactory&) = delete;
  DeviceComFactory& operator=(const DeviceComFactory&) = delete;
  DeviceComFactory(DeviceComFactory&&) = delete;
  DeviceComFactory& operator=(DeviceComFactory&&) = delete;

  /// @brief get the instance of the DeviceComFactory
  /// @return the instance of the DeviceComFactory
  static DeviceComFactory* Instance();

  /// @brief release the instance of the DeviceComFactory
  static void ReleaseInstance();

 public:
  /// @brief get ComPortDevice info list
  /// @return ComPortDevice info list
  static std::vector<ComPortDevice> GetComPortDeviceList();

 public:
  /// @brief Create the device com with the device type and settings
  /// @param device_com_type the device com type
  /// @return the device com pointer
  std::shared_ptr<DeviceComInterface> CreateOrGetDeviceComWithType(
      int32_t device_com_type);

  /// @brief Close the device com with the device type
  /// @param device_com_type the device com type
  void CloseDeviceComWithType(int32_t device_com_type);

 private:
  /// @brief the instance of the DeviceComFactory
  static DeviceComFactory* instance_;
  /// @brief device com interface shared pointer map list,
  /// key is the device com type, value is the device com interface shared
  /// pointer
  std::map<int32_t, std::shared_ptr<DeviceComInterface>> device_com_map_;
};

}  // namespace device
}  // namespace anx

#endif  // APP_DEVICE_DEVICE_COM_FACTORY_H_
