/**
 * @file device_com_settings.h
 * @author hhool (hhool@outlook.com)
 * @brief com settings struct
 * @version 0.1
 * @date 2024-08-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_DEVICE_DEVICE_COM_SETTINGS_H_
#define APP_DEVICE_DEVICE_COM_SETTINGS_H_

#include <string>

namespace anx {
namespace device {

////////////////////////////////////////////////////////////
// constants
extern const int32_t kDeviceCom_Ultrasound;
extern const int32_t kDeviceCom_StaticLoad;
extern const int32_t kDeviceCom_Air_compressor;

////////////////////////////////////////////////////////////
// struct ComPort
class ComPort {
 public:
  ComPort();
  virtual ~ComPort() {}

 public:
  // @brief baud rate 9600, 19200, 38400, 57600, 115200
  // default 9600
  int baud_rate;
  // @brief 0: 5, 1: 6, 2: 7, 3: 8
  // default 3:8
  int data_bits;
  // @brief 0:none, 1: 1, 2: 2, 3: 1.5
  int stop_bits;
  // @brief 0: none, 1: odd, 2: even 3: mark, 4: space
  int parity;
  int flow_control;
  int timeout;
};

class ComSettings {
 public:
  ComSettings();
  ComSettings(std::string name,
              int32_t device_com_type,
              const ComPort& com_port);
  virtual ~ComSettings();

  /// @brief  ToXml function
  /// @param close_tag
  /// @return  std::string
  virtual std::string ToXml(bool close_tag = true) const;

 public:
  std::string name_;
  int32_t device_com_type_;
  ComPort com_port_;

 public:
  static int32_t FromXml(const std::string& xml, ComSettings* design);
};

}  // namespace device
}  // namespace anx

#endif  // APP_DEVICE_DEVICE_COM_SETTINGS_H_
