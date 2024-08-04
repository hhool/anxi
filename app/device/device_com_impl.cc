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

ComPortDeviceImpl::ComPortDeviceImpl(std::string name,
                                     const ComPortDevice& com_port_device)
    : name_(name), com_port_device_(com_port_device) {}

ComPortDeviceImpl::~ComPortDeviceImpl() {}

int32_t ComPortDeviceImpl::Open() {
  std::cout << "ComPortDeviceImpl::Open" << std::endl;
  if (com_port_device_.GetComPort().baud_rate == 0) {
    return -1;
  }

  std::unique_ptr<itas109::CSerialPort> native_serialport(
      new itas109::CSerialPort());
  if (native_serialport == nullptr) {
    return -2;
  }
  native_serialport->init(
      com_port_device_.GetComName().c_str(),
      com_port_device_.GetComPort().baud_rate,
	  toItas109Parity(com_port_device_.GetComPort().parity),
      toItas109DataBits(com_port_device_.GetComPort().data_bits),
      toItas109StopBits(com_port_device_.GetComPort().stop_bits),
      toItas109FlowControl(com_port_device_.GetComPort().flow_control),
      com_port_device_.GetComPort().timeout);

  if (!native_serialport->open()) {
    return -3;
  }
  native_serialport_ = native_serialport.release();
  return 0;
}

void ComPortDeviceImpl::Close() {
  std::cout << "ComPortDeviceImpl::Close" << std::endl;
  if (native_serialport_ == nullptr) {
    return;
  }

  itas109::CSerialPort* native_serialport =
      reinterpret_cast<itas109::CSerialPort*>(native_serialport_);
  native_serialport->close();
  delete native_serialport;
  native_serialport_ = nullptr;
}

int32_t ComPortDeviceImpl::Read(char* buffer, int32_t size) {
  std::cout << "ComPortDeviceImpl::Read" << std::endl;
  if (native_serialport_ == nullptr) {
    return -1;
  }

  itas109::CSerialPort* native_serialport =
      reinterpret_cast<itas109::CSerialPort*>(native_serialport_);
  int readed = native_serialport->readData(buffer, size);
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
