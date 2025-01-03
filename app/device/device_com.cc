/**
 * @file device_com.cc
 * @author hhool (hhool@outlook.com)
 * @brief  device com class implementation
 * @version 0.1
 * @date 2024-08-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/device/device_com.h"

#include <assert.h>
#include <algorithm>
#include <iostream>
#include <memory>

namespace anx {
namespace device {

////////////////////////////////////////////////////////
// @brief  ComAddressPort
ComAddressPort::ComAddressPort()
    : ComBase(1),
      baud_rate(9600),
      data_bits(3),
      stop_bits(0),
      parity(0),
      flow_control(0),
      timeout(1000) {}

ComAddressPort::~ComAddressPort() {}

std::string ComAddressPort::ValueBaudRateToString() const {
  switch (baud_rate) {
    case 9600:
      return "9600";
    case 19200:
      return "19200";
    case 38400:
      return "38400";
    case 57600:
      return "57600";
    case 115200:
      return "115200";
    default:
      return "9600";
  }
}

std::string ComAddressPort::ValueDataBitsToString() const {
  switch (data_bits) {
    case 0:
      return "5";
    case 1:
      return "6";
    case 2:
      return "7";
    case 3:
      return "8";
    default:
      return "8";
  }
}

std::string ComAddressPort::ValueStopBitsToString() const {
  switch (stop_bits) {
    case 0:
      return "1";
    case 1:
      return "2";
    case 2:
      return "1.5";
    default:
      return "1";
  }
}

std::string ComAddressPort::ValueParityToString() const {
  switch (parity) {
    case 0:
      return "none";
    case 1:
      return "odd";
    case 2:
      return "even";
    case 3:
      return "mark";
    case 4:
      return "space";
    default:
      return "none";
  }
}

std::string ComAddressPort::ValueFlowControlToString() const {
  switch (flow_control) {
    case 0:
      return "none";
    case 1:
      return "hardware";
    case 2:
      return "software";
    default:
      return "none";
  }
}

int32_t ComAddressPort::ValueBaudRateFromString(
    const std::string& baud_rate_str) {
  if (baud_rate_str == "9600") {
    return 9600;
  } else if (baud_rate_str == "19200") {
    return 19200;
  } else if (baud_rate_str == "38400") {
    return 38400;
  } else if (baud_rate_str == "57600") {
    return 57600;
  } else if (baud_rate_str == "115200") {
    return 115200;
  } else {
    return 9600;
  }
}

int32_t ComAddressPort::ValueDataBitsFromString(
    const std::string& data_bits_str) {
  if (data_bits_str == "5") {
    return 0;
  } else if (data_bits_str == "6") {
    return 1;
  } else if (data_bits_str == "7") {
    return 2;
  } else if (data_bits_str == "8") {
    return 3;
  } else {
    return 3;
  }
}

int32_t ComAddressPort::ValueStopBitsFromString(
    const std::string& stop_bits_str) {
  std::string str = stop_bits_str;
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  if (str == "1") {
    return 0;
  } else if (str == "2") {
    return 1;
  } else if (str == "1.5") {
    return 2;
  } else {
    return 0;
  }
}

int32_t ComAddressPort::ValueParityFromString(const std::string& parity_str) {
  std::string str = parity_str;
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  if (str == "none") {
    return 0;
  } else if (str == "odd") {
    return 1;
  } else if (str == "even") {
    return 2;
  } else if (str == "mark") {
    return 3;
  } else if (str == "space") {
    return 4;
  } else {
    return 0;
  }
}

int32_t ComAddressPort::ValueFlowControlFromString(
    const std::string& flow_control_str) {
  std::string str = flow_control_str;
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  if (str == "none") {
    return 0;
  } else if (str == "hardware") {
    return 1;
  } else if (str == "software") {
    return 2;
  } else {
    return 0;
  }
}

////////////////////////////////////////////////////////
// @brief  ComAddressLan
ComAddressLan::ComAddressLan() : ComBase(2), ip("192.168.1.10"), port(5000) {}

ComAddressLan::ComAddressLan(const std::string& ip, int32_t port)
    : ComBase(2), ip(ip), port(port) {}

////////////////////////////////////////////////////////
// @brief  ComPortDevice
ComPortDevice::ComPortDevice() : com_name_("COM1"), com_base_() {}

ComPortDevice::ComPortDevice(const std::string& com_name, ComBase* com_base)
    : com_name_(com_name), com_base_(com_base) {
  assert(com_base != nullptr);
  if (com_base->adrtype == 1) {
    com_type_ = 1;
    com_adr_port_ = *static_cast<ComAddressPort*>(com_base);
    com_base_ = &com_adr_port_;
  } else if (com_base->adrtype == 2) {
    com_type_ = 2;
    com_adr_lan_ = *static_cast<ComAddressLan*>(com_base);
    com_base_ = &com_adr_lan_;
  }
}

ComPortDevice::~ComPortDevice() {}

const std::string& ComPortDevice::GetComName() const {
  return com_name_;
}

ComBase* ComPortDevice::GetComPort() const {
  return com_base_;
}

}  // namespace device
}  // namespace anx
