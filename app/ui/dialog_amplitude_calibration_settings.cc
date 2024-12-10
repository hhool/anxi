/**
 * @file dialog_amplitude_calibration_settings.cc
 * @author hhool (hhool@outlook.com)
 * @brief amplitude calibration settings dialog class implementation
 * @version 0.1
 * @date 2024-08-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/dialog_amplitude_calibration_settings.h"

#include <map>
#include <string>

#include "app/common/logger.h"
#include "app/common/module_utils.h"
#include "app/common/string_utils.h"

#include "app/device/device_exp_amplitude_settings.h"
#include "app/ui/ui_constants.h"
#include "app/ui/ui_num_string_convert.hpp"

DUI_BEGIN_MESSAGE_MAP(anx::ui::DialogAmplitudeCalibrationSettings,
                      DuiLib::WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {
DialogAmplitudeCalibrationSettings::DialogAmplitudeCalibrationSettings() {}

DialogAmplitudeCalibrationSettings::~DialogAmplitudeCalibrationSettings() {}

void DialogAmplitudeCalibrationSettings::InitWindow() {
  __super::InitWindow();
  btn_close_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(kCloseButtonControlName));
  btn_ok_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("okbtn")));
  btn_cancel_ =
      static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("cancelbtn")));

  edit_amp_level_one_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_amp_level_one")));
  edit_amp_level_two_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_amp_level_two")));
  edit_amp_level_third_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_amp_level_third")));
  edit_amp_level_fourth_ = static_cast<CEditUI*>(
      m_PaintManager.FindControl(_T("tab_page_three_left_amp_level_fourth")));

  edit_amp_level_one_->OnNotify += MakeDelegate(
      this, &DialogAmplitudeCalibrationSettings::OnEditControlChanged);
  edit_amp_level_two_->OnNotify += MakeDelegate(
      this, &DialogAmplitudeCalibrationSettings::OnEditControlChanged);
  edit_amp_level_third_->OnNotify += MakeDelegate(
      this, &DialogAmplitudeCalibrationSettings::OnEditControlChanged);
  edit_amp_level_fourth_->OnNotify += MakeDelegate(
      this, &DialogAmplitudeCalibrationSettings::OnEditControlChanged);

  btn_reset_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(_T("btn_amp_params_reset")));
  btn_reset_->OnNotify +=
      MakeDelegate(this, &DialogAmplitudeCalibrationSettings::OnBtnResetClick);

  LoadSettingsFromResource();

  UpdateControlFromSettings();
}

void DialogAmplitudeCalibrationSettings::Notify(DuiLib::TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
    OnPrepare(msg);
  } else if (msg.sType == kClick) {
    OnClick(msg);
  }
}

void DialogAmplitudeCalibrationSettings::OnFinalMessage(HWND hWnd) {
  WindowImplBase::OnFinalMessage(hWnd);
  delete this;
}

LRESULT DialogAmplitudeCalibrationSettings::ResponseDefaultKeyEvent(
    WPARAM wParam) {
  if (wParam == VK_RETURN) {
    this->Close();
    return TRUE;
  } else if (wParam == VK_ESCAPE) {
    return TRUE;
  }
  return FALSE;
}

void DialogAmplitudeCalibrationSettings::OnClick(DuiLib::TNotifyUI& msg) {
  if (msg.pSender == btn_ok_) {
    SaveSettingsToResource();
    this->Close();
  } else if (msg.pSender == btn_close_) {
    this->Close();
  } else if (msg.pSender == btn_cancel_) {
    this->Close();
  }
}

void DialogAmplitudeCalibrationSettings::OnPrepare(
    const DuiLib::TNotifyUI& msg) {
  ::SetWindowLong(
      m_hWnd, GWL_STYLE,
      ::GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);
}

bool DialogAmplitudeCalibrationSettings::OnEditControlChanged(void* param) {
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
  if (pMsg->pSender == edit_amp_level_one_) {
    float amp_level_one = 0;
    if (get_value_from_control(edit_amp_level_one_, &amp_level_one)) {
      if (amp_level_one >= 1) {
        deas_->exp_power2amp_map_[20] = amp_level_one;
        save = true;
      }
    }
    set_value_to_edit(edit_amp_level_one_, deas_->exp_power2amp_map_[20], 2);
  } else if (pMsg->pSender == edit_amp_level_two_) {
    float amp_level_two = 0;
    if (get_value_from_control(edit_amp_level_two_, &amp_level_two)) {
      if (amp_level_two >= 1) {
        deas_->exp_power2amp_map_[40] = amp_level_two;
        save = true;
      }
    }
    set_value_to_edit(edit_amp_level_two_, deas_->exp_power2amp_map_[40], 2);
  } else if (pMsg->pSender == edit_amp_level_third_) {
    float amp_level_third = 0;
    if (get_value_from_control(edit_amp_level_third_, &amp_level_third)) {
      if (amp_level_third >= 1) {
        deas_->exp_power2amp_map_[60] = amp_level_third;
        save = true;
      }
    }
    set_value_to_edit(edit_amp_level_third_, deas_->exp_power2amp_map_[60], 2);
  } else if (pMsg->pSender == edit_amp_level_fourth_) {
    float amp_level_fourth = 0;
    if (get_value_from_control(edit_amp_level_fourth_, &amp_level_fourth)) {
      if (amp_level_fourth >= 1) {
        deas_->exp_power2amp_map_[80] = amp_level_fourth;
        save = true;
      }
    }
    set_value_to_edit(edit_amp_level_fourth_, deas_->exp_power2amp_map_[80], 2);
  } else {
    return false;
  }
  return true;
}

bool DialogAmplitudeCalibrationSettings::OnBtnResetClick(void* param) {
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr) {
    return false;
  }
  if (pMsg->sType != DUI_MSGTYPE_CLICK) {
    return false;
  }
  if (pMsg->pSender != btn_reset_) {
    return false;
  }
  int32_t ret = anx::device::ResetDeviceExpAmplitudeSettingsDefaultResource();
  if (ret != 0) {
    // TODO(hhool): notify msg box with error info;
    return true;
  }
  LoadSettingsFromResource();
  UpdateControlFromSettings();
  return true;
}

void DialogAmplitudeCalibrationSettings::LoadSettingsFromResource() {
  deas_ = anx::device::LoadDeviceExpAmplitudeSettingsDefaultResource();
  if (deas_ == nullptr) {
    deas_ = std::unique_ptr<anx::device::DeviceExpAmplitudeSettings>(
        new anx::device::DeviceExpAmplitudeSettings());
  }
}

void DialogAmplitudeCalibrationSettings::UpdateControlFromSettings() {
  if (deas_ == nullptr) {
    LOG_F(LG_ERROR) << "deas_ is nullptr";
    return;
  }
  std::map<int32_t, float> exp_power2amp_map = deas_->exp_power2amp_map_;
  set_value_to_edit(edit_amp_level_one_, exp_power2amp_map[20], 2);
  set_value_to_edit(edit_amp_level_two_, exp_power2amp_map[40], 2);
  set_value_to_edit(edit_amp_level_third_, exp_power2amp_map[60], 2);
  set_value_to_edit(edit_amp_level_fourth_, exp_power2amp_map[80], 2);
}

void DialogAmplitudeCalibrationSettings::SaveSettingsToResource() {
  if (deas_ == nullptr) {
    LOG_F(LG_INFO) << "deas_ is nullptr";
    return;
  }
  std::map<int32_t, float> exp_power2amp_map = deas_->exp_power2amp_map_;
  anx::device::SaveDeviceExpAmplitudeSettingsDefaultResource(exp_power2amp_map);
}

}  // namespace ui
}  // namespace anx
