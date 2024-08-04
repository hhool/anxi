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

#include <memory>
#include <string>

#include "app/device/device_com.h"

namespace anx {
namespace device {
class ComPortDevice;
}
}  // namespace anx
namespace anx {
namespace device {

////////////////////////////////////////////////////////////
// constants
extern const int32_t kDeviceCom_Ultrasound;
extern const int32_t kDeviceCom_StaticLoad;
extern const int32_t kDeviceCom_Air_compressor;

class ComSettings : public ComPortDevice {
 public:
  ComSettings(int32_t device_com_type,
              const std::string& com_name,
              const ComPort& com_port);
  virtual ~ComSettings();

 public:
  int32_t GetDeviceComType() const;

  /// @brief  ToXml function
  /// @param close_tag
  /// @return  std::string
  virtual std::string ToXml(bool close_tag = true) const;

 protected:
  /// @brief device com type @see kDeviceCom_Ultrasound, kDeviceCom_StaticLoad,
  int32_t device_com_type_;

 public:
  static std::unique_ptr<ComSettings> FromXml(const std::string& xml);
};

}  // namespace device
}  // namespace anx

#endif  // APP_DEVICE_DEVICE_COM_SETTINGS_H_
