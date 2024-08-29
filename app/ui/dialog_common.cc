/**
 * @file dialog_common.cc
 * @author hhool (hhool@outlook.com)
 * @brief dialog about
 * @version 0.1
 * @date 2024-07-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/dialog_common.h"

#include <string>

#include "app/common/defines.h"
#include "app/common/string_utils.h"
#include "app/ui/ui_constants.h"

DUI_BEGIN_MESSAGE_MAP(anx::ui::DialogCommon, DuiLib::WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {
DialogCommon::DialogCommon(const std::string& title,
                           const std::string& content,
                           int32_t& result)
    : title_(title), content_(content), result_(result) {}

DialogCommon::~DialogCommon() {}

void DialogCommon::InitWindow() {
  __super::InitWindow();

  btn_close_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(kCloseButtonControlName));
  btn_ok_ = static_cast<DuiLib::CButtonUI*>(
      this->m_PaintManager.FindControl(_T("okbtn")));
  btn_cancel_ = static_cast<DuiLib::CButtonUI*>(
      this->m_PaintManager.FindControl(_T("cancelbtn")));

  DuiLib::CLabelUI* title = static_cast<DuiLib::CLabelUI*>(
      this->m_PaintManager.FindControl(kTitleControlName));
  DuiLib::CLabelUI* content = static_cast<DuiLib::CLabelUI*>(
      this->m_PaintManager.FindControl(_T("content")));
  if (title != nullptr) {
    DuiLib::CDuiString dui_title(
        anx::common::string2wstring(title_.c_str()).c_str());
    title->SetText(dui_title);
  }
  if (content != nullptr) {
    DuiLib::CDuiString dui_content(
        anx::common::string2wstring(content_.c_str()).c_str());
    content->SetText(dui_content);
  }
}

void DialogCommon::Notify(DuiLib::TNotifyUI& msg) {
  if (msg.sType == kClick) {
    if (msg.pSender == btn_close_) {
      this->Close();
      result_ = 0;
      return;
    } else if (msg.pSender == btn_ok_) {
      this->Close();
      result_ = 1;
      return;
    } else if (msg.pSender == btn_cancel_) {
      this->Close();
      result_ = 0;
      return;
    }
  }
}

void DialogCommon::OnFinalMessage(HWND hWnd) {
  WindowImplBase::OnFinalMessage(hWnd);
  delete this;
}

LRESULT DialogCommon::ResponseDefaultKeyEvent(WPARAM wParam, bool& bHandled) {
  if (wParam == VK_RETURN) {
    this->Close();
    return TRUE;
  } else if (wParam == VK_ESCAPE) {
    return TRUE;
  }
  return FALSE;
}

void DialogCommon::OnPrepare(const DuiLib::TNotifyUI& msg) {
  ::SetWindowLong(
      m_hWnd, GWL_STYLE,
      ::GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);
}
}  // namespace ui
}  // namespace anx
