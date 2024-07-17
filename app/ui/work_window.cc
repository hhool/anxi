/**
 * @file work_window.cc
 * @author hhool (hhool@outlook.com)
 * @brief work window class definition
 * @version 0.1
 * @date 2024-07-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "app/ui/work_window.h"

namespace anx {
namespace ui {
WorkWindow::WorkWindow() {}

WorkWindow::~WorkWindow() {}

void WorkWindow::Show() {}

void WorkWindow::Hide() {}

void WorkWindow::Close() {}

void WorkWindow::SetTitle(const std::string& title) { title_ = title; }

void WorkWindow::SetSize(int width, int height) {
  width_ = width;
  height_ = height;
}

void WorkWindow::SetPosition(int x, int y) {
  x_ = x;
  y_ = y;
}
}  // namespace ui
}  // namespace anx

