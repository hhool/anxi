/**
 * @file dialog_static_load_guaranteed_settings.cc
 * @author hhool (hhool@outlook.com)
 * @brief staic load guaranteed settings dialog class implementation
 * @version 0.1
 * @date 2024-08-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/dialog_static_load_guaranteed_settings.h"

#include <string>

#include "app/common/logger.h"
#include "app/common/string_utils.h"
#include "app/device/device_exp_load_static_settings.h"
#include "app/device/stload/stload_common.h"
#include "app/ui/ui_constants.h"
#include "app/ui/ui_num_string_convert.hpp"

DUI_BEGIN_MESSAGE_MAP(anx::ui::DialogStaticLoadGuaranteedSettings,
                      DuiLib::WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {
DialogStaticLoadGuaranteedSettings::DialogStaticLoadGuaranteedSettings(
    int32_t* result)
    : DialogCommon("", "", result) {}

DialogStaticLoadGuaranteedSettings::~DialogStaticLoadGuaranteedSettings() {}

void DialogStaticLoadGuaranteedSettings::InitWindow() {
  btn_close_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(kCloseButtonControlName));
  btn_ok_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("okbtn")));
  btn_cancel_ =
      static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("cancelbtn")));
  opt_displacement_ = static_cast<COptionUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_opt_displacement")));
  opt_retention_ = static_cast<COptionUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_opt_retention")));
  opt_up_ = static_cast<COptionUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_opt_move_up")));
  opt_down_ = static_cast<COptionUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_opt_move_down")));
  edit_speed_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_speed")));
  edit_retention_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_retention")));
  edit_displacement_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_displacement")));
  edit_keep_load_duration_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_keep_load_duration")));

  /// bind event
  opt_displacement_->OnNotify += MakeDelegate(
      this, &DialogStaticLoadGuaranteedSettings::OnCtrlTypeChanged);
  opt_retention_->OnNotify += MakeDelegate(
      this, &DialogStaticLoadGuaranteedSettings::OnCtrlTypeChanged);
  opt_up_->OnNotify += MakeDelegate(
      this, &DialogStaticLoadGuaranteedSettings::OnCtrlMoveChanged);
  opt_down_->OnNotify += MakeDelegate(
      this, &DialogStaticLoadGuaranteedSettings::OnCtrlMoveChanged);
  edit_speed_->OnNotify += MakeDelegate(
      this, &DialogStaticLoadGuaranteedSettings::OnEditControlChanged);
  edit_retention_->OnNotify += MakeDelegate(
      this, &DialogStaticLoadGuaranteedSettings::OnEditControlChanged);
  edit_displacement_->OnNotify += MakeDelegate(
      this, &DialogStaticLoadGuaranteedSettings::OnEditControlChanged);
  edit_keep_load_duration_->OnNotify += MakeDelegate(
      this, &DialogStaticLoadGuaranteedSettings::OnEditControlChanged);

  /// load settings from resource or create a new one.
  LoadSettingsFromResource();

  UpdateControlFromSettings();
}

void DialogStaticLoadGuaranteedSettings::Notify(DuiLib::TNotifyUI& msg) {
  __super::Notify(msg);
  if (msg.sType == kWindowInit) {
    OnPrepare(msg);
  } else if (msg.sType == kClick) {
    OnClick(msg);
  }
}

void DialogStaticLoadGuaranteedSettings::OnFinalMessage(HWND hWnd) {
  WindowImplBase::OnFinalMessage(hWnd);
  delete this;
}

LRESULT DialogStaticLoadGuaranteedSettings::ResponseDefaultKeyEvent(
    WPARAM wParam) {
  if (wParam == VK_RETURN) {
    this->Close();
    return TRUE;
  } else if (wParam == VK_ESCAPE) {
    return TRUE;
  }
  return FALSE;
}

void DialogStaticLoadGuaranteedSettings::OnClick(DuiLib::TNotifyUI& msg) {
  if (msg.pSender == btn_ok_) {
    SaveSettingsToResource();
    this->Close();
  } else if (msg.pSender == btn_close_) {
    this->Close();
  } else if (msg.pSender == btn_cancel_) {
    this->Close();
  }
}

void DialogStaticLoadGuaranteedSettings::OnPrepare(
    const DuiLib::TNotifyUI& msg) {
  ::SetWindowLong(
      m_hWnd, GWL_STYLE,
      ::GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);
}

bool DialogStaticLoadGuaranteedSettings::OnEditControlChanged(void* param) {
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr) {
    return false;
  }
  LOG_F(LG_INFO) << pMsg->pSender->GetName() << " " << pMsg->sType << " "
                 << pMsg->wParam << " " << pMsg->lParam;
  if (pMsg->sType != DUI_MSGTYPE_KILLFOCUS &&
      pMsg->sType != DUI_MSGTYPE_RETURN) {
    return false;
  }
  bool save = false;
  if (pMsg->pSender == edit_speed_) {
    int64_t speed = 0;
    if (get_value_from_control(edit_speed_, &speed)) {
      if (speed != lss_->speed_) {
        lss_->speed_ = static_cast<int32_t>(speed);
        save = true;
      }
    }
    set_value_to_control(edit_speed_, lss_->speed_);
  } else if (pMsg->pSender == edit_retention_) {
    int64_t retention = 0;
    if (get_value_from_control(edit_retention_, &retention)) {
      if (retention != lss_->retention_) {
        lss_->retention_ = static_cast<int32_t>(retention);
        save = true;
      }
    }
    set_value_to_control(edit_retention_, lss_->retention_);
  } else if (pMsg->pSender == edit_displacement_) {
    int64_t displacement = 0;
    if (get_value_from_control(edit_displacement_, &displacement)) {
      if (displacement != lss_->displacement_) {
        lss_->displacement_ = static_cast<int32_t>(displacement);
        save = true;
      }
    }
    set_value_to_control(edit_displacement_, lss_->displacement_);
  } else if (pMsg->pSender == edit_keep_load_duration_) {
    int64_t keep_load_duration = 0;
    if (get_value_from_control(edit_keep_load_duration_, &keep_load_duration)) {
      if (keep_load_duration != lss_->keep_load_duration_) {
        lss_->keep_load_duration_ = static_cast<int32_t>(keep_load_duration);
        save = true;
      }
    }
    set_value_to_control(edit_keep_load_duration_, lss_->keep_load_duration_);
  }

  SaveSettingsToResource();
  return true;
}

bool DialogStaticLoadGuaranteedSettings::OnCtrlTypeChanged(void* param) {
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr) {
    return false;
  }
  if (pMsg->pSender == opt_displacement_) {
    edit_displacement_->SetEnabled(true);
    edit_retention_->SetEnabled(false);
    edit_speed_->SetEnabled(true);
    edit_keep_load_duration_->SetEnabled(false);
    if (lss_.get() != nullptr) {
      lss_->ctrl_type_ = CTRL_POSI;
    }
  } else if (pMsg->pSender == opt_retention_) {
    edit_displacement_->SetEnabled(false);
    edit_retention_->SetEnabled(true);
    edit_speed_->SetEnabled(false);
    edit_keep_load_duration_->SetEnabled(true);
    if (lss_.get() != nullptr) {
      lss_->ctrl_type_ = CTRL_LOAD;
    }
  }
  return true;
}

bool DialogStaticLoadGuaranteedSettings::OnCtrlMoveChanged(void* param) {
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr) {
    return false;
  }
  if (pMsg->pSender == opt_up_) {
    opt_down_->Selected(false);
    opt_up_->Selected(true);
    if (lss_.get() != nullptr) {
      lss_->direct_ = 1;
    }
  } else if (pMsg->pSender == opt_down_) {
    opt_down_->Selected(true);
    opt_up_->Selected(false);
    if (lss_.get() != nullptr) {
      lss_->direct_ = 2;
    }
  }
  return true;
}

void DialogStaticLoadGuaranteedSettings::LoadSettingsFromResource() {
  lss_ = std::unique_ptr<anx::device::DeviceLoadStaticSettings>(
      anx::device::LoadDeviceLoadStaticSettingsDefaultResource());
  if (lss_.get() == nullptr) {
    lss_ = std::unique_ptr<anx::device::DeviceLoadStaticSettings>(
        new anx::device::DeviceLoadStaticSettings());
  }
}

void DialogStaticLoadGuaranteedSettings::UpdateControlFromSettings() {
  if (lss_.get() == nullptr) {
    LOG_F(LG_ERROR) << "lss_ is nullptr";
    return;
  }
  if (lss_->ctrl_type_ == CTRL_LOAD) {
    opt_displacement_->Selected(false);
    opt_retention_->Selected(true);
    edit_speed_->SetEnabled(false);
    edit_displacement_->SetEnabled(false);
  } else {
    opt_displacement_->Selected(true);
    opt_retention_->Selected(false);
    edit_speed_->SetEnabled(true);
    edit_retention_->SetEnabled(false);
  }
  if (lss_->direct_ == 0) {
    opt_up_->Selected(false);
    opt_down_->Selected(false);
  } else if (lss_->direct_ == 1) {
    opt_up_->Selected(true);
    opt_down_->Selected(false);
  } else if (lss_->direct_ == 2) {
    opt_up_->Selected(false);
    opt_down_->Selected(true);
  }
  edit_speed_->SetText(
      anx::common::String2WString(std::to_string(lss_->speed_).c_str())
          .c_str());
  edit_retention_->SetText(
      anx::common::String2WString(std::to_string(lss_->retention_).c_str())
          .c_str());
  edit_displacement_->SetText(
      anx::common::String2WString(std::to_string(lss_->displacement_).c_str())
          .c_str());
  edit_keep_load_duration_->SetText(
      anx::common::String2WString(
          std::to_string(lss_->keep_load_duration_).c_str())
          .c_str());
}

void DialogStaticLoadGuaranteedSettings::SaveSettingsToResource() {
  if (lss_.get() == nullptr) {
    LOG_F(LG_ERROR) << "lss_ is nullptr";
    return;
  }
  anx::device::SaveDeviceLoadStaticSettingsDefaultResource(*lss_);
}

}  // namespace ui
}  // namespace anx
