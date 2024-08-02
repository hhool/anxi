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
  UIVirtualWndBase() {}
  virtual ~UIVirtualWndBase() {}

 public:
  virtual void Bind() = 0;
  virtual void Unbind() = 0;
};

}  // namespace ui
}  // namespace anx

#endif  // APP_UI_UI_VIRTUAL_WND_BASE_H_
