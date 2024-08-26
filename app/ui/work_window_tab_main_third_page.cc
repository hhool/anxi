/**
 * @file work_window_tab_main_third_page.cc
 * @author hhool (hhool@outlook.com)
 * @brief work window tab main third page class definition
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/work_window_tab_main_third_page.h"

#include <string>
#include <utility>

#include "app/common/logger.h"
#include "app/common/string_utils.h"
#include "app/device/device_com_factory.h"
#include "app/device/device_com_settings.h"
#include "app/device/device_exp_load_static_settings.h"
#include "app/esolution/solution_design.h"
#include "app/esolution/solution_design_default.h"
#include "app/ui/dialog_about.h"
#include "app/ui/dialog_com_port_settings.h"
#include "app/ui/ui_constants.h"
#include "app/ui/work_window.h"
#include "app/ui/work_window_menu_design.h"
#include "app/ui/work_window_menu_store.h"

DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindowThirdPage, DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_TIMER, OnTimer)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {

std::string format_num(int64_t num) {
  std::string value;
  int64_t integer_part = num / 1000;
  int64_t decimal_part = num % 1000;
  // remove the 0 at the end of the decimal.
  while (decimal_part % 10 == 0) {
    decimal_part /= 10;
    if (decimal_part == 0) {
      break;
    }
  }
  // format integer part
  value += std::to_string(integer_part);
  if (decimal_part != 0) {
    value += ".";
    value += std::to_string(decimal_part);
  }
  return value;
}

namespace {
const int32_t kTimerID = 0x1;
const int32_t kTimerElapse = 1000;
}  // namespace

class WorkWindowThirdPage::ListSendGetter : public DuiLib::IListCallbackUI {
 public:
  explicit ListSendGetter(std::vector<std::string>* items) : items_(items) {}
  ~ListSendGetter() {}

  LPCTSTR GetItemText(DuiLib::CControlUI* pControl,
                      int iItem,
                      int iSubItem) override {
    if (iItem < 0 || iSubItem < 0) {
      pControl->SetUserData(_T(""));
      return pControl->GetUserData();
    }
    if (iItem < exp_data_table_start_row_no_ ||
        static_cast<size_t>(iItem) >=
            (exp_data_table_start_row_no_ + items_->size())) {
      // clear the data itmes and request the data from the database again
      items_->clear();
      exp_data_table_start_row_no_ = iItem;
      /*anx::common::RequestDataFromDatabase(iItem,
         exp_data_table_limit_row_no_, items_);*/
#if 1
      for (int i = 0; i < exp_data_table_limit_row_no_; i++) {
        items_->push_back("data" +
                          std::to_string(i + 1 + exp_data_table_start_row_no_));
      }
#endif
    }
    if (iItem >=
        static_cast<int32_t>(items_->size()) + exp_data_table_start_row_no_) {
      pControl->SetUserData(_T(""));
      return pControl->GetUserData();
    }

    int iItemIndex = iItem - exp_data_table_start_row_no_;
    pControl->SetUserData(
        anx::common::string2wstring(items_->at(iItemIndex).c_str()).c_str());
    return pControl->GetUserData();
  }

 private:
  std::vector<std::string>* items_;
  int32_t exp_data_table_start_row_no_ = 0;
  int32_t exp_data_table_limit_row_no_ = 100;
  int32_t exp_data_table_no_ = 0;
};

class WorkWindowThirdPage::ListRecvGetter : public DuiLib::IListCallbackUI {
 public:
  explicit ListRecvGetter(std::vector<std::string>* items) : items_(items) {}
  ~ListRecvGetter() {}

  LPCTSTR GetItemText(DuiLib::CControlUI* pControl,
                      int iItem,
                      int iSubItem) override {
    if (iItem < 0 || iSubItem < 0) {
      pControl->SetUserData(_T(""));
      return pControl->GetUserData();
    }
    if (iItem < exp_data_table_start_row_no_ ||
        static_cast<size_t>(iItem) >=
            (exp_data_table_start_row_no_ + items_->size())) {
      // clear the data itmes and request the data from the database again
      items_->clear();
      exp_data_table_start_row_no_ = iItem;
      /*anx::common::RequestDataFromDatabase(iItem,
         exp_data_table_limit_row_no_, items_);*/
#if 1
      for (int i = 0; i < exp_data_table_limit_row_no_; i++) {
        items_->push_back("data" +
                          std::to_string(i + 1 + exp_data_table_start_row_no_));
      }
#endif
    }
    if (iItem >=
        static_cast<int32_t>(items_->size()) + exp_data_table_start_row_no_) {
      pControl->SetUserData(_T(""));
      return pControl->GetUserData();
    }
    int iItemIndex = iItem - exp_data_table_start_row_no_;
    pControl->SetUserData(
        anx::common::string2wstring(items_->at(iItemIndex).c_str()).c_str());
    return pControl->GetUserData();
  }

 private:
  std::vector<std::string>* items_;
  int32_t exp_data_table_start_row_no_ = 0;
  int32_t exp_data_table_limit_row_no_ = 100;
  int32_t exp_data_table_no_ = 0;
};

class WorkWindowThirdPage::ListRecvNotifyGetter
    : public DuiLib::IListCallbackUI {
 public:
  explicit ListRecvNotifyGetter(std::vector<std::string>* items)
      : items_(items) {}
  ~ListRecvNotifyGetter() {}

  LPCTSTR GetItemText(DuiLib::CControlUI* pControl,
                      int iItem,
                      int iSubItem) override {
    if (iItem < 0 || iSubItem < 0) {
      pControl->SetUserData(_T(""));
      return pControl->GetUserData();
    }
    if (iItem < exp_data_table_start_row_no_ ||
        static_cast<size_t>(iItem) >=
            (exp_data_table_start_row_no_ + items_->size())) {
      // clear the data itmes and request the data from the database again
      items_->clear();
      exp_data_table_start_row_no_ = iItem;
      /*anx::common::RequestDataFromDatabase(iItem,
         exp_data_table_limit_row_no_, items_);*/
#if 1
      for (int i = 0; i < exp_data_table_limit_row_no_; i++) {
        items_->push_back("data" +
                          std::to_string(i + 1 + exp_data_table_start_row_no_));
      }
#endif
    }
    if (iItem >=
        static_cast<int32_t>(items_->size()) + exp_data_table_start_row_no_) {
      pControl->SetUserData(_T(""));
      return pControl->GetUserData();
    }

    int iItemIndex = iItem - exp_data_table_start_row_no_;
    pControl->SetUserData(
        anx::common::string2wstring(items_->at(iItemIndex).c_str()).c_str());
    return pControl->GetUserData();
  }

 private:
  std::vector<std::string>* items_;
  int32_t exp_data_table_start_row_no_ = 0;
  int32_t exp_data_table_limit_row_no_ = 100;
  int32_t exp_data_table_no_ = 0;
};

WorkWindowThirdPage::WorkWindowThirdPage(
    WorkWindow* pWorkWindow,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : pWorkWindow_(pWorkWindow), paint_manager_ui_(paint_manager_ui) {
  send_data_items_.clear();
  recv_data_items_.clear();
  recv_notify_data_items_.clear();
  list_send_getter_.reset(new ListSendGetter(&send_data_items_));
  list_recv_getter_.reset(new ListRecvGetter(&recv_data_items_));
  list_recv_notify_getter_.reset(
      new ListRecvNotifyGetter(&recv_notify_data_items_));
}

WorkWindowThirdPage::~WorkWindowThirdPage() {
  list_recv_notify_getter_.reset();
  list_recv_getter_.reset();
  list_send_getter_.reset();
}

void WorkWindowThirdPage::OnClick(TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
  } else if (msg.sType == kClick) {
    if (msg.pSender == check_box_display_send_) {
      // TODO(hhool):
    } else if (msg.pSender == check_box_display_stop_recv_notify_) {
      // TODO(hhool):
    } else {
      // TODO(hhool): do nothing
    }
  } else {
    // TODO(hhool): do nothing
  }
}

void WorkWindowThirdPage::OnTimer(TNotifyUI& msg) {
  if (msg.wParam == kTimerID) {
    UpdateControlFromSettings();
  } else {
  }
}

void WorkWindowThirdPage::Bind() {
  // initialize the device com interface
  device_com_ul_ =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_Ultrasound, this);
  device_com_sl_ =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_StaticLoad, this);
  // bind timer
  DuiLib::CHorizontalLayoutUI* layout =
      static_cast<DuiLib::CHorizontalLayoutUI*>(
          paint_manager_ui_->FindControl(_T("tab_page_three")));
  paint_manager_ui_->SetTimer(layout, kTimerID, kTimerElapse);

  // bind control
  opt_direct_up_ = static_cast<COptionUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_left_move_up")));
  opt_direct_down_ = static_cast<COptionUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_left_move_down")));

  opt_action_pull_ = static_cast<COptionUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_left_pull")));
  opt_action_push_ = static_cast<COptionUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_left_push")));

  edit_speed_ = static_cast<CEditUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_left_speed")));

  edit_retention_ = static_cast<CEditUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_left_retention")));

  label_displacement_ = static_cast<CLabelUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_label_displacement")));

  label_strength_ = static_cast<CLabelUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_label_strength")));

  check_box_display_send_ = static_cast<CCheckBoxUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_right_refresh_data")));

  check_box_display_stop_recv_notify_ =
      static_cast<CCheckBoxUI*>(paint_manager_ui_->FindControl(
          _T("tab_page_three_right_stop_recv_output")));

  list_send_ = static_cast<CListUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_list_send")));
  list_recv_ = static_cast<CListUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_list_recv")));

  list_recv_notify_ = static_cast<CListUI*>(
      paint_manager_ui_->FindControl(_T("tab_page_three_list_recv_notify")));

  list_send_->SetTextCallback(list_send_getter_.get());
  list_recv_->SetTextCallback(list_recv_getter_.get());
  list_recv_notify_->SetTextCallback(list_recv_notify_getter_.get());

  UpdateControlFromSettings();
}

void WorkWindowThirdPage::Unbind() {
  DuiLib::CHorizontalLayoutUI* layout =
      static_cast<DuiLib::CHorizontalLayoutUI*>(
          paint_manager_ui_->FindControl(_T("tab_page_three")));
  // unbind timer
  paint_manager_ui_->KillTimer(layout, kTimerID);

  // release the device com interface
  if (device_com_ul_ != nullptr) {
    device_com_ul_->RemoveListener(this);
    device_com_ul_.reset();
  }
  if (device_com_sl_ != nullptr) {
    device_com_sl_->RemoveListener(this);
    device_com_sl_.reset();
  }
}

void WorkWindowThirdPage::UpdateControlFromSettings() {
  // update control from settings
  std::unique_ptr<anx::device::DeviceLoadStaticSettings> lss =
      anx::device::LoadDeviceLoadStaticSettingsDefaultResource();
  if (lss == nullptr) {
    return;
  }
  opt_direct_up_->Selected(lss->direct_ == 1);
  opt_direct_down_->Selected(lss->direct_ == 2);
  opt_action_pull_->Selected(lss->action_ == 1);
  opt_action_push_->Selected(lss->action_ == 2);
  edit_speed_->SetText(
      anx::common::string2wstring(std::to_string(lss->speed_).c_str()).c_str());
  edit_retention_->SetText(
      anx::common::string2wstring(std::to_string(lss->retention_).c_str())
          .c_str());
}

void WorkWindowThirdPage::OnDataReceived(
    anx::device::DeviceComInterface* device,
    const uint8_t* data,
    int32_t size) {
  // TODO(hhool): review the implementation
  if (device == device_com_ul_.get()) {
  } else if (device == device_com_sl_.get()) {
  }
  if (!check_box_display_stop_recv_notify_->IsSelected()) {
    std::string hex_str;
    hex_str = anx::common::ByteArrayToHexString(data, size);
    DuiLib::CListTextElementUI* item = new DuiLib::CListTextElementUI();
    recv_notify_table_no_++;
    // recv_notify_data_items_.push_back(hex_str);
    list_recv_notify_->Add(item);
  }
  if (check_box_display_send_->IsSelected()) {
    std::string hex_str;
    hex_str = anx::common::ByteArrayToHexString(data, size);
    DuiLib::CListTextElementUI* item = new DuiLib::CListTextElementUI();
    //recv_data_items_.push_back(hex_str);
	recv_table_no_++;
    list_recv_->Add(item);
  }
  // update the label_displacement_ with random value
  float displacement = 0.0f;
  displacement = static_cast<float>(rand() % 100);
  std::string num_format = format_num(static_cast<int64_t>(displacement * 100));
  label_displacement_->SetText(
      anx::common::string2wstring(num_format.c_str()).c_str());
  // update the label_strength_ with random value
  float strength = 0.0f;
  strength = static_cast<float>(rand() % 100);
  num_format = format_num(static_cast<int64_t>(strength * 100));
  label_strength_->SetText(
      anx::common::string2wstring(num_format.c_str()).c_str());
}

void WorkWindowThirdPage::OnDataOutgoing(
    anx::device::DeviceComInterface* device,
    const uint8_t* data,
    int32_t size) {
  // TODO(hhool): review the implementation
  if (device == device_com_ul_.get()) {
  } else if (device == device_com_sl_.get()) {
  }
  if (check_box_display_send_->IsSelected()) {
    std::string hex_str;
    hex_str = anx::common::ByteArrayToHexString(data, size);
    DuiLib::CListTextElementUI* item = new DuiLib::CListTextElementUI();
   // send_data_items_.push_back(hex_str);
	send_table_no_++;
    list_send_->Add(item);
  }
}

}  // namespace ui
}  // namespace anx
