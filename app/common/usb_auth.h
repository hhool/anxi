/**
 * @file usb_auth.h
 * @author hhool (hhool@outlook.com)
 * @brief  usb auth
 * @version 0.1
 * @date 2025-01-06
 */

#ifndef APP_COMMON_USB_AUTH_H_
#define APP_COMMON_USB_AUTH_H_

#include <string>

namespace anx {
namespace common {
/**
 * @brief check usb auth
 * @return 0 if success, -1 if failed
 */
int32_t CheckUsbAuth();
}  // namespace common
}  // namespace anx

#endif  // APP_COMMON_USB_AUTH_H_
