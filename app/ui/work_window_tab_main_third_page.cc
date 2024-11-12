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

#include <iomanip>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "app/common/logger.h"
#include "app/common/string_utils.h"
#include "app/db/database_helper.h"
#include "app/device/device_com_factory.h"
#include "app/device/device_com_settings.h"
#include "app/device/device_exp_load_static_settings.h"
#include "app/device/stload/stload_helper.h"
#include "app/esolution/solution_design.h"
#include "app/esolution/solution_design_default.h"
#include "app/ui/dialog_about.h"
#include "app/ui/dialog_com_port_settings.h"
#include "app/ui/ui_constants.h"
#include "app/ui/ui_num_string_convert.hpp"
#include "app/ui/work_window.h"
#include "app/ui/work_window_menu_design.h"
#include "app/ui/work_window_menu_store.h"
#include "app/ui/work_window_tab_main_page_base.h"

DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindowThirdPage, DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_TIMER, OnTimer)
DUI_ON_MSGTYPE(DUI_MSGTYPE_VALUECHANGED, OnValueChanged)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {

namespace {
const int32_t kTimerID = 0x1;
const int32_t kTimerElapse = 1000;
}  // namespace

class WorkWindowThirdPage::ListSendGetter
    : public DuiLib::IListVirtalCallbackUI {
 public:
  explicit ListSendGetter(DuiLib::CListUI* list_ui) : list_ui_(list_ui) {}
  ~ListSendGetter() {}

  CControlUI* CreateVirtualItem() override {
    CListHBoxElementUI* pHBox = new CListHBoxElementUI;
    //> 设置行高
    pHBox->SetFixedHeight(28);
    ///> 操作区域
    CLabelUI* pLabel = new CLabelUI;
    pLabel->SetAttributeList(_T("font=\"11\" algin=\"center\""));
    pLabel->SetFixedHeight(28);
    pLabel->SetFixedWidth(30);
    pHBox->Add(pLabel);

    return pHBox;
  }

  void DrawItem(CControlUI* pControl, int nRow) {
    if (pControl == nullptr) {
      return;
    }
    if (nRow < 0) {
      return;
    }

    std::string sql_str = "SELECT * FROM ";
    sql_str += anx::db::helper::kTableSendData;
    sql_str += " WHERE id=";
    sql_str += std::to_string(nRow + 1);
    std::vector<std::map<std::string, std::string>> result;
    anx::db::helper::QueryDataBase(anx::db::helper::kDefaultDatabasePathname,
                                   anx::db::helper::kTableSendData, sql_str,
                                   &result);
    if (result.size() == 0) {
      return;
    }

    CListHBoxElementUI* pHBox = static_cast<CListHBoxElementUI*>(
        pControl->GetInterface(DUI_CTR_LISTHBOXELEMENT));
    if (pHBox) {
      CDuiString strFormat =
          anx::common::String2WString(result[0]["content"].c_str()).c_str();
      pHBox->GetItemAt(0)->SetText(strFormat);
    }
  }

 private:
  DuiLib::CListUI* list_ui_;
};

class WorkWindowThirdPage::ListRecvGetter
    : public DuiLib::IListVirtalCallbackUI {
 public:
  explicit ListRecvGetter(DuiLib::CListUI* list_ui) : list_ui_(list_ui) {}
  ~ListRecvGetter() {}

  CControlUI* CreateVirtualItem() override {
    CListHBoxElementUI* pHBox = new CListHBoxElementUI;
    //> 设置行高
    pHBox->SetFixedHeight(28);
    ///> 操作区域
    CLabelUI* pLabel = new CLabelUI;
    pLabel->SetAttributeList(_T("font=\"11\" algin=\"center\""));
    pLabel->SetFixedHeight(28);
    pLabel->SetFixedWidth(30);
    pHBox->Add(pLabel);

    return pHBox;
  }

  void DrawItem(CControlUI* pControl, int nRow) {
    if (pControl == nullptr) {
      return;
    }
    if (nRow < 0) {
      return;
    }
    std::string sql_str = "SELECT * FROM ";
    sql_str += anx::db::helper::kTableSendNotify;
    sql_str += " WHERE id=";
    sql_str += std::to_string(nRow + 1);
    std::vector<std::map<std::string, std::string>> result;
    anx::db::helper::QueryDataBase(anx::db::helper::kDefaultDatabasePathname,
                                   anx::db::helper::kTableSendNotify, sql_str,
                                   &result);
    if (result.size() == 0) {
      return;
    }

    CListHBoxElementUI* pHBox = static_cast<CListHBoxElementUI*>(
        pControl->GetInterface(DUI_CTR_LISTHBOXELEMENT));
    if (pHBox) {
      CDuiString strFormat =
          anx::common::String2WString(result[0]["content"].c_str()).c_str();
      pHBox->GetItemAt(0)->SetText(strFormat);
    }
  }

 private:
  DuiLib::CListUI* list_ui_;
};

class WorkWindowThirdPage::ListRecvNotifyGetter
    : public DuiLib::IListVirtalCallbackUI {
 public:
  explicit ListRecvNotifyGetter(DuiLib::CListUI* list_ui) : list_ui_(list_ui) {}
  ~ListRecvNotifyGetter() {}

  CControlUI* CreateVirtualItem() override {
    CListHBoxElementUI* pHBox = new CListHBoxElementUI;
    //> 设置行高
    pHBox->SetFixedHeight(28);
    ///> 操作区域
    CLabelUI* pLabel = new CLabelUI;
    pLabel->SetAttributeList(_T("font=\"11\" algin=\"center\""));
    pLabel->SetFixedHeight(28);
    pLabel->SetFixedWidth(30);
    pHBox->Add(pLabel);

    return pHBox;
  }

  void DrawItem(CControlUI* pControl, int nRow) {
    if (pControl == nullptr) {
      return;
    }
    if (nRow < 0) {
      return;
    }
    std::string sql_str = "SELECT * FROM ";
    sql_str += anx::db::helper::kTableNotification;
    sql_str += " WHERE id=";
    sql_str += std::to_string(nRow + 1);
    std::vector<std::map<std::string, std::string>> result;
    anx::db::helper::QueryDataBase(anx::db::helper::kDefaultDatabasePathname,
                                   anx::db::helper::kTableNotification, sql_str,
                                   &result);
    if (result.size() == 0) {
      return;
    }

    CListHBoxElementUI* pHBox = static_cast<CListHBoxElementUI*>(
        pControl->GetInterface(DUI_CTR_LISTHBOXELEMENT));
    if (pHBox) {
      CDuiString strFormat =
          anx::common::String2WString(result[0]["content"].c_str()).c_str();
      pHBox->GetItemAt(0)->SetText(strFormat);
    }
  }

 private:
  DuiLib::CListUI* list_ui_;
};

WorkWindowThirdPage::WorkWindowThirdPage(
    WorkWindow* pWorkWindow,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : pWorkWindow_(pWorkWindow), paint_manager_ui_(paint_manager_ui) {
  paint_manager_ui_->AddNotifier(this);
}

WorkWindowThirdPage::~WorkWindowThirdPage() {
  list_recv_notify_getter_.reset();
  list_recv_getter_.reset();
  list_send_getter_.reset();
  paint_manager_ui_->RemoveNotifier(this);
}

void WorkWindowThirdPage::Notify(TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
  } else if (msg.sType == DUI_MSGTYPE_DRAWITEM) {
    if (msg.pSender == list_recv_notify_) {
      ListRecvNotifyGetter* lrng = reinterpret_cast<ListRecvNotifyGetter*>(
          list_recv_notify_getter_.get());
      lrng->DrawItem(reinterpret_cast<CControlUI*>(msg.wParam), msg.lParam);
    } else if (msg.pSender == list_send_) {
      ListSendGetter* lsg =
          reinterpret_cast<ListSendGetter*>(list_send_getter_.get());
      lsg->DrawItem(reinterpret_cast<CControlUI*>(msg.wParam), msg.lParam);
    } else if (msg.pSender == list_recv_) {
      ListRecvGetter* lrg =
          reinterpret_cast<ListRecvGetter*>(list_recv_getter_.get());
      lrg->DrawItem(reinterpret_cast<CControlUI*>(msg.wParam), msg.lParam);
    } else {
      // do nothing
    }
  }
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
    // TODO(hhool): do nothing
  }
}

void WorkWindowThirdPage::OnValueChanged(TNotifyUI& msg) {
  if (msg.sType == DUI_MSGTYPE_VALUECHANGED) {
    if (msg.pSender->GetName() == _T("work_args_area")) {
      ENMsgStruct* enmsg = reinterpret_cast<ENMsgStruct*>(msg.wParam);
      if (enmsg == nullptr) {
        return;
      }
      if (enmsg->type_ == enmsg_type_stload_value_cur) {
        anx::device::stload::STResult* st_result =
            reinterpret_cast<anx::device::stload::STResult*>(enmsg->ptr_);
        // update the label_displacement_ with random value
        double pos = st_result->pos_;
        // format the number keep 1 decimal
        std::string num_pos_str = to_string_with_precision(pos, 1);
        label_displacement_->SetText(
            anx::common::String2WString(num_pos_str).c_str());
        // update the label_strength_ with random value
        double load = st_result->load_;
        std::string num_load_str = to_string_with_precision(load, 1);
        label_strength_->SetText(
            anx::common::String2WString(num_load_str).c_str());
      } else if (enmsg->type_ == enmsg_type_exp_stress_amp) {
        // TODO(hhool):
      }
    } else {
      // do nothing
    }
  }
}

void WorkWindowThirdPage::Bind() {
  // initialize the device com interface
  std::shared_ptr<anx::device::DeviceComInterface> device_com_ul =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_Ultrasound, this);
  ultra_device_ =
      reinterpret_cast<anx::device::UltraDevice*>(device_com_ul->Device());
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

  list_send_->SetVirtual(true);
  list_send_->SetVirtualItemCount(0);
  list_send_getter_.reset(new ListSendGetter(list_send_));
  ListSendGetter* lsg = static_cast<ListSendGetter*>(list_send_getter_.get());
  list_send_->SetVirtualItemFormat(
      static_cast<DuiLib::IListVirtalCallbackUI*>(lsg));

  list_recv_->SetVirtual(true);
  list_recv_->SetVirtualItemCount(0);
  list_recv_getter_.reset(new ListRecvGetter(list_recv_));
  ListRecvGetter* lrg = static_cast<ListRecvGetter*>(list_recv_getter_.get());
  list_recv_->SetVirtualItemFormat(
      static_cast<DuiLib::IListVirtalCallbackUI*>(lrg));

  list_recv_notify_->SetVirtual(true);
  list_recv_notify_->SetVirtualItemCount(0);
  list_recv_notify_getter_.reset(new ListRecvNotifyGetter(list_recv_notify_));
  ListRecvNotifyGetter* lrng =
      static_cast<ListRecvNotifyGetter*>(list_recv_notify_getter_.get());
  list_recv_notify_->SetVirtualItemFormat(
      static_cast<DuiLib::IListVirtalCallbackUI*>(lrng));

  UpdateControlFromSettings();
}

void WorkWindowThirdPage::Unbind() {
  DuiLib::CHorizontalLayoutUI* layout =
      static_cast<DuiLib::CHorizontalLayoutUI*>(
          paint_manager_ui_->FindControl(_T("tab_page_three")));
  // unbind timer
  paint_manager_ui_->KillTimer(layout, kTimerID);

  // release the device com interface
  if (ultra_device_ != nullptr) {
    ultra_device_->GetPortDevice()->RemoveListener(this);
    ultra_device_ = nullptr;
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
  edit_speed_->SetText(
      anx::common::String2WString(std::to_string(lss->speed_).c_str()).c_str());
  edit_retention_->SetText(
      anx::common::String2WString(std::to_string(lss->retention_).c_str())
          .c_str());
}

void WorkWindowThirdPage::OnDataReceived(
    anx::device::DeviceComInterface* device,
    const uint8_t* data,
    int32_t size) {
  // TODO(hhool): review the implementation
  if (!check_box_display_stop_recv_notify_->IsSelected()) {
    std::string hex_str;
    hex_str = anx::common::ByteArrayToHexString(data, size);
    recv_notify_table_no_++;
    // insert the data into the database
    std::string sql_str = "INSERT INTO ";
    sql_str += anx::db::helper::kTableNotification;
    sql_str += " (content, date) VALUES (";
    sql_str += "\'";
    sql_str += hex_str;
    sql_str += "\'";
    sql_str += ", ";
    sql_str += std::to_string(anx::common::GetCurrrentSystimeAsVarTime());
    sql_str += ")";
    anx::db::helper::InsertDataTable(anx::db::helper::kDefaultDatabasePathname,
                                     anx::db::helper::kTableNotification,
                                     sql_str);

    list_recv_notify_->SetVirtualItemCount(recv_notify_table_no_);
  }
  if (check_box_display_send_->IsSelected()) {
    std::string hex_str;
    hex_str = anx::common::ByteArrayToHexString(data, size);
    recv_table_no_++;
    // insert the data into the database
    // INSERT INTO send_data (content, date) VALUES ('0x123456', 123456)
    std::string sql_str = "INSERT INTO ";
    sql_str += anx::db::helper::kTableSendNotify;
    sql_str += " (content, date) VALUES (";
    sql_str += "\'";
    sql_str += hex_str;
    sql_str += "\'";
    sql_str += ", ";
    sql_str += std::to_string(anx::common::GetCurrrentSystimeAsVarTime());
    sql_str += ")";
    anx::db::helper::InsertDataTable(anx::db::helper::kDefaultDatabasePathname,
                                     anx::db::helper::kTableSendNotify,
                                     sql_str);

    list_recv_->SetVirtualItemCount(recv_table_no_);
  }
}

void WorkWindowThirdPage::OnDataOutgoing(
    anx::device::DeviceComInterface* device,
    const uint8_t* data,
    int32_t size) {
  if (check_box_display_send_->IsSelected()) {
    std::string hex_str;
    hex_str = anx::common::ByteArrayToHexString(data, size);
    send_table_no_++;

    // insert the data into the database
    std::string sql_str = "INSERT INTO ";
    sql_str += anx::db::helper::kTableSendData;
    sql_str += " (content, date) VALUES (";
    sql_str += "\'";
    sql_str += hex_str;
    sql_str += "\'";
    sql_str += ", ";
    sql_str += std::to_string(anx::common::GetCurrrentSystimeAsVarTime());
    sql_str += ")";
    anx::db::helper::InsertDataTable(anx::db::helper::kDefaultDatabasePathname,
                                     anx::db::helper::kTableSendData, sql_str);
    list_send_->SetVirtualItemCount(send_table_no_);
  }
}

}  // namespace ui
}  // namespace anx
