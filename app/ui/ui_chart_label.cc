/**
 * @file ui_chart_label.cc
 * @author hhool (hhool@outlook.com)
 * @brief chart label class implementation
 * @version 0.1
 * @date 2024-08-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/ui_chart_label.h"

#include <sstream>
#include <string>

#include "app/common/module_utils.h"
#include "app/common/string_utils.h"

#include "app/ui/dmgraph.tlh"

namespace anx {
namespace ui {

////////////////////////////////////////////////////////////////////
// clz ChartLabelUI
ChartLabelUI::ChartLabelUI() : hwnd_(nullptr), native_chartctr_(nullptr) {}

ChartLabelUI::~ChartLabelUI() {}

void ChartLabelUI::SetInternVisible(bool visible) {
  __super::SetInternVisible(visible);
  ::ShowWindow(hwnd_, visible);
}

void ChartLabelUI::SetPos(RECT rc, bool need_invalidate) {
  __super::SetPos(rc, need_invalidate);
  ::SetWindowPos(hwnd_, NULL, rc.left, rc.top, rc.right - rc.left,
                 rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
}

bool ChartLabelUI::AttachGraph(HWND wnd) {
  if (!::IsWindow(wnd)) {
    return FALSE;
  }

  hwnd_ = wnd;
  return true;
}

HWND ChartLabelUI::DetachGraph() {
  HWND wnd = hwnd_;
  hwnd_ = NULL;
  return wnd;
}

}  // namespace ui
}  // namespace anx
