/**
 * @file ui_chart_label.h
 * @author hhool (hhool@outlook.com)
 * @brief chart label class implementation
 * @version 0.1
 * @date 2024-08-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_UI_CHART_LABEL_H_
#define APP_UI_UI_CHART_LABEL_H_

#include <memory>
#include <string>

#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP

using namespace DuiLib;  // NOLINT

namespace anx {
namespace ui {
class ChartLabelUI : public CControlUI {
 public:
  ChartLabelUI();
  ~ChartLabelUI();

 public:
  void SetInternVisible(bool bVisible = true) override;
  void SetPos(RECT rc, bool bNeedInvalidate = true) override;

  bool AttachGraph(HWND wnd);
  HWND DetachGraph();

 protected:
  HWND hwnd_;
  void* native_chartctr_;
};
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_UI_CHART_LABEL_H