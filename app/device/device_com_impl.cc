/**
 * @file device_com_impl.cc
 * @author hhool (hhool@outlook.com)
 * @brief device com implementation
 * @version 0.1
 * @date 2024-08-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/device/device_com_impl.h"

#include <iostream>

#include "third_party/CSerialPort/source/include/CSerialPort/SerialPort.h"
#include "third_party/CSerialPort/source/include/CSerialPort/SerialPortInfo.h"

namespace anx {
namespace device {

namespace {
itas109::Parity toItas109Parity(int parity) {
  switch (parity) {
    case 0:
      return itas109::Parity::ParityNone;
    case 1:
      return itas109::Parity::ParityOdd;
    case 2:
      return itas109::Parity::ParityEven;
    case 3:
      return itas109::Parity::ParityMark;
    case 4:
      return itas109::Parity::ParitySpace;
    default:
      return itas109::Parity::ParityNone;
  }
}

itas109::DataBits toItas109DataBits(int data_bits) {
  switch (data_bits) {
    case 0:
      return itas109::DataBits::DataBits5;
    case 1:
      return itas109::DataBits::DataBits6;
    case 2:
      return itas109::DataBits::DataBits7;
    case 3:
      return itas109::DataBits::DataBits8;
    default:
      return itas109::DataBits::DataBits8;
  }
}

itas109::StopBits toItas109StopBits(int stop_bits) {
  switch (stop_bits) {
    case 0:
    case 1:
      return itas109::StopBits::StopOne;
    case 2:
      return itas109::StopBits::StopTwo;
    case 3:
      return itas109::StopBits::StopOneAndHalf;
    default:
      return itas109::StopBits::StopOne;
  }
}

itas109::FlowControl toItas109FlowControl(int flow_control) {
  switch (flow_control) {
    case 0:
      return itas109::FlowControl::FlowNone;
    case 1:
      return itas109::FlowControl::FlowHardware;
    case 2:
      return itas109::FlowControl::FlowSoftware;
    default:
      return itas109::FlowControl::FlowNone;
  }
}
}  // namespace

ComPortDeviceImpl::ComPortDeviceImpl(std::string name) : name_(name) {
  std::unique_ptr<itas109::CSerialPort> native_serialport(
      new itas109::CSerialPort());
  native_serialport_ = native_serialport.release();
}

ComPortDeviceImpl::~ComPortDeviceImpl() {
  itas109::CSerialPort* native_serialport =
      reinterpret_cast<itas109::CSerialPort*>(native_serialport_);
  delete native_serialport;
  native_serialport_ = nullptr;
}

void ComPortDeviceImpl::AddListener(DeviceComListener* listener) {
  std::cout << "ComPortDeviceImpl::AddListener" << std::endl;
  // find the listener from the list
  for (auto& it : listeners_) {
    if (it == listener) {
      return;
    }
  }
  listeners_.push_back(listener);
}

void ComPortDeviceImpl::RemoveListener(DeviceComListener* listener) {
  std::cout << "ComPortDeviceImpl::RemoveListener" << std::endl;
  // find the listener from the list
  for (auto it = listeners_.begin(); it != listeners_.end(); ++it) {
    if (*it == listener) {
      listeners_.erase(it);
      return;
    }
  }
}

int32_t ComPortDeviceImpl::Open(const ComPortDevice& com_port) {
  std::cout << "ComPortDeviceImpl::Open" << std::endl;
  if (com_port.GetComPort().baud_rate == 0) {
    return -1;
  }

  if (native_serialport_ == nullptr) {
    return -2;
  }

  itas109::CSerialPort* native_serialport =
      reinterpret_cast<itas109::CSerialPort*>(native_serialport_);

  native_serialport->init(
      com_port.GetComName().c_str(), com_port.GetComPort().baud_rate,
      toItas109Parity(com_port.GetComPort().parity),
      toItas109DataBits(com_port.GetComPort().data_bits),
      toItas109StopBits(com_port.GetComPort().stop_bits),
      toItas109FlowControl(com_port.GetComPort().flow_control));

  native_serialport->setReadIntervalTimeout(com_port.GetComPort().timeout);

  if (!native_serialport->open()) {
    return -3;
  }
  return 0;
}

bool ComPortDeviceImpl::isOpened() {
  std::cout << "ComPortDeviceImpl::IsOpen" << std::endl;
  if (native_serialport_ == nullptr) {
    return false;
  }

  itas109::CSerialPort* native_serialport =
      reinterpret_cast<itas109::CSerialPort*>(native_serialport_);
  return native_serialport->isOpen();
}

void ComPortDeviceImpl::Close() {
  std::cout << "ComPortDeviceImpl::Close" << std::endl;
  if (native_serialport_ == nullptr) {
    return;
  }

  itas109::CSerialPort* native_serialport =
      reinterpret_cast<itas109::CSerialPort*>(native_serialport_);
  native_serialport->close();
}

int32_t ComPortDeviceImpl::Read(uint8_t* buffer, int32_t size) {
  std::cout << "ComPortDeviceImpl::Read" << std::endl;
  if (native_serialport_ == nullptr) {
    return -1;
  }

  itas109::CSerialPort* native_serialport =
      reinterpret_cast<itas109::CSerialPort*>(native_serialport_);
  int readed = native_serialport->readData(buffer, size);
  if (readed > 0) {
    for (auto& it : listeners_) {
      it->OnDataReceived(this, buffer, readed);
    }
  }
  return readed;
}

int32_t ComPortDeviceImpl::Write(const uint8_t* buffer, int32_t size) {
  std::cout << "ComPortDeviceImpl::Write" << std::endl;
  if (native_serialport_ == nullptr) {
    return -1;
  }

  itas109::CSerialPort* native_serialport =
      reinterpret_cast<itas109::CSerialPort*>(native_serialport_);
  int written =
      native_serialport->writeData(reinterpret_cast<const void*>(buffer), size);
  if (written > 0) {
    for (auto& it : listeners_) {
      it->OnDataOutgoing(this, buffer, written);
    }
  }
  return written;
}

int32_t ComPortDeviceImpl::WriteRead(const uint8_t* write_buffer,
                                     int32_t write_size,
                                     uint8_t* read_buffer,
                                     int32_t read_size) {
  std::cout << "ComPortDeviceImpl::WriteRead" << std::endl;
  return 0;
}

const std::string ComPortDeviceImpl::GetName() const {
  return name_;
}

const ComPortDevice& ComPortDeviceImpl::GetComPortDevice() const {
  return com_port_device_;
}

}  // namespace device
}  // namespace anx
