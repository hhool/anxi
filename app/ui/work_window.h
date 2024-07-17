/**
 * @file work_window.h
 * @author hhool (hhool@outlook.com)
 * @brief work window class definition
 * @version 0.1
 * @date 2024-07-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef ANXI_APP_UI_WORK_WINDOW_H_
#define ANXI_APP_UI_WORK_WINDOW_H_

#include <memory>
#include <string>

#include "third_party\duilib\source\DuiLib\UIlib.h"

namespace anx {
namespace ui {
class WorkWindow : public DuiLib::CWindowWnd {
 public:
  WorkWindow();
  ~WorkWindow();

  void Show();
  void Hide();
  void Close();

  void SetTitle(const std::string& title);
  void SetSize(int width, int height);
  void SetPosition(int x, int y);

 private:
  std::string title_;
  int width_;
  int height_;
  int x_;
  int y_;
};
}  // namespace ui
}  // namespace anx

#endif  // ANXI_APP_UI_WORK_WINDOW_H_
