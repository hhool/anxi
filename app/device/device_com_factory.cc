/**
 * @file device_com_factory.cc
 * @author hhool (hhool@outlook.com)
 * @brief  device com factory, create device com with device type
 * enum and device com settings.
 * @version 0.1
 * @date 2024-08-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/device/device_com_factory.h"

#include "app/device/device_com_impl.h"
#include "app/device/device_com_settings.h"
#include "app/device/device_com_settings_helper.h"

#include "third_party/CSerialPort/source/include/CSerialPort/SerialPort.h"
#include "third_party/CSerialPort/source/include/CSerialPort/SerialPortInfo.h"

namespace anx {
namespace device {

DeviceComFactory* DeviceComFactory::instance_ = nullptr;
////////////////////////////////////////////////////////////////////////////////
DeviceComFactory* DeviceComFactory::Instance() {
  if (instance_ == nullptr) {
    instance_ = new DeviceComFactory();
  }
  return instance_;
}

void DeviceComFactory::ReleaseInstance() {
  if (instance_ != nullptr) {
    delete instance_;
    instance_ = nullptr;
  }
}

////////////////////////////////////////////////////////////////////////////////
// clz DeviceComFactory

DeviceComFactory::DeviceComFactory() {}

DeviceComFactory::~DeviceComFactory() {
  for (auto& it : device_com_map_) {
    it.second->Close();
  }
  device_com_map_.clear();
}

std::vector<ComPortDevice> DeviceComFactory::GetComPortDeviceList() {
  std::vector<ComPortDevice> com_port_device_list;

  std::vector<itas109::SerialPortInfo> availablePortsInfoVector =
      itas109::CSerialPortInfo::availablePortInfos();
  for (const itas109::SerialPortInfo& portInfo : availablePortsInfoVector) {
    ComAddressPort com_port;
    com_port.baud_rate = 9600;
    com_port.data_bits = 3;
    com_port.stop_bits = 0;
    com_port.parity = 0;
    com_port.flow_control = 0;
    com_port.timeout = 1000;

    ComPortDevice com_port_device(portInfo.portName, &com_port);
    com_port_device_list.push_back(com_port_device);
  }
  return com_port_device_list;
}

std::shared_ptr<DeviceComInterface>
DeviceComFactory::CreateOrGetDeviceComWithType(int32_t device_com_type,
                                               DeviceComListener* listener) {
  // find the device com pointer from the ,
  auto it = device_com_map_.find(device_com_type);
  if (it != device_com_map_.end()) {
    // add the listener to the device com
    if (listener != nullptr) {
      ComPortDeviceImpl* device_com_impl =
          reinterpret_cast<ComPortDeviceImpl*>(it->second.get());
      device_com_impl->AddListener(listener);
    }
    return it->second;
  }

  std::unique_ptr<ComSettings> com_settings =
      LoadDeviceComSettingsDefaultResourceWithType(device_com_type);
  if (com_settings == nullptr) {
    return nullptr;
  }
  // create the device com pointer
  std::shared_ptr<DeviceComInterface> device_com;
  if (device_com_type == kDeviceCom_Ultrasound) {
    device_com = std::make_shared<ComPortDeviceImpl>("ul");
  } else if (device_com_type == kDeviceCom_StaticLoad) {
    device_com = std::make_shared<ComPortDeviceImpl>("sl");
  } else if (device_com_type == kDeviceLan_StaticLoad) {
    device_com = std::make_shared<ComPortDeviceImpl>("sl2");
  } else {
    return nullptr;
  }

  // add the listener to the device com
  if (listener != nullptr) {
    ComPortDeviceImpl* device_com_impl =
        reinterpret_cast<ComPortDeviceImpl*>(device_com.get());
    device_com_impl->AddListener(listener);
  }
  // store the device com pointer to DeviceComManager
  device_com_map_[device_com_type] = device_com;
  return device_com;
}

int32_t DeviceComFactory::OpenDeviceComWithType(int32_t device_com_type) {
  std::unique_ptr<ComSettings> com_settings =
      LoadDeviceComSettingsDefaultResourceWithType(device_com_type);
  if (com_settings == nullptr) {
    return -1;
  }
  // find the device com pointer from the map
  auto it = device_com_map_.find(device_com_type);
  if (it != device_com_map_.end()) {
    // open the device com
    return it->second->Open(
        *(reinterpret_cast<ComPortDevice*>(com_settings.get())));
  }
  return -2;
}

void DeviceComFactory::CloseDeviceComWithType(int32_t device_com_type) {
  // find the device com pointer from the map
  auto it = device_com_map_.find(device_com_type);
  if (it != device_com_map_.end()) {
    // close the device com
    it->second->Close();
  }
}

}  // namespace device
}  // namespace anx
