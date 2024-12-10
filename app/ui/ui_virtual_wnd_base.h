/**
 * @file ui_virtual_wnd_base.h
 * @author hhool (hhool@outlook.com)
 * @brief  virtual window base class for duilib virtual window
 * to implement the virtual window, you should inherit this class
 * for virtual window initialization, deinitialization and message handling
 * @version 0.1
 * @date 2024-08-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_UI_VIRTUAL_WND_BASE_H_
#define APP_UI_UI_VIRTUAL_WND_BASE_H_

#include <memory>
#include <string>

namespace anx {
namespace ui {
class UIVirtualWndBase {
 public:
  /// @brief Bind the virtual window, only bind the virtual window to the
  /// paint manager, only run once after the virtual window is created.
  virtual void Bind() = 0;
  /// @brief Unbind the virtual window, only unbind the virtual window to the
  /// paint manager, only run once. before the virtual window is destroyed.
  virtual void Unbind() = 0;
};

class UIExpStateBase {
 public:
  virtual void UpdateExpError(int32_t code, const std::string& msg) = 0;
};
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_UI_VIRTUAL_WND_BASE_H_
