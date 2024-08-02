/**
 * @file work_window_tab_main_second_page.cc
 * @author hhool (hhool@outlook.com)
 * @brief work window second page ui
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/work_window_tab_main_second_page.h"

#include <utility>

#include "app/common/defines.h"
#include "app/esolution/solution_design.h"
#include "app/esolution/solution_design_default.h"
#include "app/ui/dialog_amplitude_calibration_settings.h"
#include "app/ui/dialog_static_load_guaranteed_settings.h"
#include "app/ui/ui_constants.h"
#include "app/ui/work_window.h"

namespace anx {
namespace ui {
WorkWindowSecondPageBase::WorkWindowSecondPageBase(
    WorkWindow* pOwner,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : pOwner_(pOwner), paint_manager_ui_(paint_manager_ui) {}

WorkWindowSecondPageBase::~WorkWindowSecondPageBase() {}

void WorkWindowSecondPageBase::InitPage() {}

}  // namespace ui
}  // namespace anx
////////////////////////////////////////////////////////////////////////////////
DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindowSecondPage, DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {
WorkWindowSecondPage::WorkWindowSecondPage(
    WorkWindow* pOwner,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : WorkWindowSecondPageBase(pOwner, paint_manager_ui),
      pOwner_(pOwner),
      paint_manager_ui_(paint_manager_ui) {}

WorkWindowSecondPage::~WorkWindowSecondPage() {}

void WorkWindowSecondPage::OnClick(TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
  } else if (msg.sType == kClick) {
    if (msg.pSender == btn_tab_graph_) {
      btn_tablayout_->SelectItem(0);
    } else if (msg.pSender == btn_tab_data_) {
      btn_tablayout_->SelectItem(1);
    } else if (msg.pSender == this->btn_sa_clear_) {
    } else if (msg.pSender == this->btn_sa_setting_) {
      DialogStaticLoadGuaranteedSettings*
          dialog_static_load_guraranteed_settings =
              new DialogStaticLoadGuaranteedSettings();
      dialog_static_load_guraranteed_settings->Create(
          *pOwner_, _T("dialog_static_load_guraranteed_settings"),
          UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
      dialog_static_load_guraranteed_settings->CenterWindow();
      dialog_static_load_guraranteed_settings->ShowModal();
    } else if (msg.pSender == this->btn_sa_reset_) {
    } else if (msg.pSender == this->btn_exp_start_) {
    } else if (msg.pSender == this->btn_exp_stop_) {
    } else if (msg.pSender == this->btn_exp_pause_) {
    } else if (msg.pSender == this->btn_sa_up_) {
    } else if (msg.pSender == this->btn_sa_down_) {
    } else if (msg.pSender == this->btn_sa_stop_) {
    } else if (msg.pSender == this->btn_aa_setting_) {
		DialogAmplitudeCalibrationSettings*
			dialog_amplitude_calibration_settings =
			new DialogAmplitudeCalibrationSettings();
		dialog_amplitude_calibration_settings->Create(
			*pOwner_, _T("dialog_amplitude_calibration_settings"),
			UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
		dialog_amplitude_calibration_settings->CenterWindow();
		dialog_amplitude_calibration_settings->ShowModal();
    }
  } else if (msg.sType == _T("selectchanged")) {
  }
}

void anx::ui::WorkWindowSecondPage::InitPage() {
  btn_tablayout_ = static_cast<DuiLib::CTabLayoutUI*>(
      paint_manager_ui_->FindControl(_T("tab_graph_data")));
  btn_tab_graph_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("tab_graph")));
  btn_tab_data_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("tab_data")));

  /// @brief Static aircraft releated button
  btn_sa_clear_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_sa_clear")));
  btn_sa_setting_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_sa_setting")));
  btn_sa_reset_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_sa_reset")));

  /// @brief exp action releated button
  btn_exp_start_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_exp_start")));
  btn_exp_stop_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_exp_stop")));
  btn_exp_pause_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_exp_pause")));

  /// @brief Static aircraft action releated button
  btn_sa_up_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_sa_up")));
  btn_sa_down_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_sa_down")));
  btn_sa_stop_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_sa_stop")));

  /// @brief AA releated button
  btn_aa_setting_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_aa_setting")));
}

}  // namespace ui
}  // namespace anx
