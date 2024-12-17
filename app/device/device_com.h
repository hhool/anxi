/**
 * @file device_com.h
 * @author hhool (hhool@outlook.com)
 * @brief device com class, base struct and class for device com
 * @version 0.1
 * @date 2024-08-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_DEVICE_DEVICE_COM_H_
#define APP_DEVICE_DEVICE_COM_H_

#include <memory>
#include <string>

namespace anx {
namespace device {

////////////////////////////////////////////////////////////
// struct ComAddressPort
class ComBase {
 public:
  explicit ComBase(int32_t type = 1) : adrtype(type) {}
  virtual ~ComBase() {}

 public:
  /// @brief  address type 1: com, 2: lan
  int32_t adrtype;
};

class ComAddressPort : public ComBase {
 public:
  ComAddressPort();
  ~ComAddressPort() override;

 public:
  // @brief baud rate 9600, 19200, 38400, 57600, 115200
  // default 9600
  int32_t baud_rate;
  // @brief 0: 5, 1: 6, 2: 7, 3: 8
  // default 3:8
  int32_t data_bits;
  // @brief 1: 1, 2: 2, 3: 1.5
  int32_t stop_bits;
  // @brief 0: none, 1: odd, 2: even 3: mark, 4: space
  int32_t parity;
  // @brief 0: none, 1: hardware, 2: software
  int32_t flow_control;
  // @brief timeout ms
  int32_t timeout;

 public:
  std::string ValueBaudRateToString() const;
  std::string ValueDataBitsToString() const;
  std::string ValueStopBitsToString() const;
  std::string ValueParityToString() const;
  std::string ValueFlowControlToString() const;

 public:
  /// @brief  baud rate string value to int32_t
  /// @param baud_rate_str  9600, 19200, 38400, 57600, 115200
  /// @return  int32_t 9600, 19200, 38400, 57600, 115200
  static int32_t ValueBaudRateFromString(const std::string& baud_rate_str);
  /// @brief  data_bits string value to int32_t
  /// @param data_bits_str 5, 6, 7, 8
  /// @return  int32_t 0: 5, 1: 6, 2: 7, 3: 8
  static int32_t ValueDataBitsFromString(const std::string& data_bits_str);
  /// @brief stop_bits string value to int32_t
  /// @param stop_bits_str 1, 2, 1.5
  /// @return  int32_t 1: 1, 2: 2, 3: 1.5
  static int32_t ValueStopBitsFromString(const std::string& stop_bits_str);
  /// @brief parity string value to int32_t
  /// @param parity_str none, odd, even, mark, space
  /// @return  int32_t 0: none, 1: odd, 2: even, 3: mark, 4: space
  static int32_t ValueParityFromString(const std::string& parity_str);
  /// @brief flow_control string value to int32_t
  /// @param flow_control_str none, hardware, software
  /// @return  int32_t 0: none, 1: hardware, 2: software
  static int32_t ValueFlowControlFromString(
      const std::string& flow_control_str);
};

class ComAddressLan : public ComBase {
 public:
  ComAddressLan();
  ComAddressLan(const std::string& ip, int32_t port);
  ~ComAddressLan() override {}

 public:
  std::string ip;
  int32_t port;
};

class ComPortDevice {
 public:
  ComPortDevice();
  ComPortDevice(const std::string& com_name, ComBase* com_port);
  virtual ~ComPortDevice();

 public:
  const std::string& GetComName() const;
  ComBase* GetComPort() const;

 protected:
  std::string com_name_;
  ComBase* com_base_;
  int32_t com_type_;
  ComAddressPort com_adr_port_;
  ComAddressLan com_adr_lan_;
};

////////////////////////////////////////////////////////////
class DeviceNode {};

class DeviceComListener;
class DeviceComInterface {
 public:
  /// @brief  Add listener
  /// @param listener  listener
  virtual void AddListener(DeviceComListener* listener) = 0;
  /// @brief  Remove listener
  /// @param listener  listener
  virtual void RemoveListener(DeviceComListener* listener) = 0;

 public:
  virtual void AttachDeviceNode(DeviceNode* device) { device_ = device; }
  virtual void DetachDeviceNode(DeviceNode*) { device_ = nullptr; }
  virtual DeviceNode* Device() { return device_; }

 private:
  DeviceNode* device_;

 public:
  /// @brief  Open the device com
  /// @return int32_t 0 success, non-zero fail
  virtual int32_t Open(const ComPortDevice& com_port) = 0;
  /// @brief  Check the device com is opened
  /// @return bool true opened, false not opened
  virtual bool isOpened() = 0;
  /// @brief  Close the device com
  virtual void Close() = 0;
  /// @brief  Read data from the device com
  /// @param buffer  data buffer
  /// @param size  data buffer size
  /// @return int32_t  read data size
  virtual int32_t Read(uint8_t* buffer, int32_t size) = 0;
  /// @brief  Write data to the device com
  /// @param buffer  data buffer
  /// @param size  data buffer size
  /// @return int32_t  write data size
  virtual int32_t Write(const uint8_t* buffer, int32_t size) = 0;
  /// @brief  Write data to the device com and read data from the device com
  /// @param write_buffer  write data buffer
  /// @param write_size  write data buffer size
  /// @param read_buffer  read data buffer
  /// @param read_size  read data buffer size
  /// @return int32_t  read data size
  virtual int32_t WriteRead(const uint8_t* write_buffer,
                            int32_t write_size,
                            uint8_t* read_buffer,
                            int32_t read_size) = 0;
};

////////////////////////////////////////////////////////////
class DeviceComListener {
 public:
  DeviceComListener() = default;
  virtual ~DeviceComListener() = default;

  /// @brief  On data received
  /// @param data  data buffer
  /// @param size  data buffer size
  virtual void OnDataReceived(anx::device::DeviceComInterface* device,
                              const uint8_t* data,
                              int32_t size) = 0;
  /// @brief  On data outgoing
  /// @param data  data buffer
  /// @param size  data buffer size
  virtual void OnDataOutgoing(anx::device::DeviceComInterface* device,
                              const uint8_t* data,
                              int32_t size) = 0;
};
}  // namespace device
}  // namespace anx

#endif  // APP_DEVICE_DEVICE_COM_H_
	