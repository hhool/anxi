/**
 * @file work_window_tab_main_second_page_data.cc
 * @author hhool (hhool@outlook.com)
 * @brief work window second page data ui
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/work_window_tab_main_second_page_data.h"

#include <iomanip>
#include <iostream>
#include <map>
#include <utility>

#include "app/common/logger.h"
#include "app/common/string_utils.h"
#include "app/common/time_utils.h"
#include "app/db/database_helper.h"
#include "app/device/device_com_factory.h"
#include "app/device/device_com_settings.h"
#include "app/device/device_exp_data_sample_settings.h"
#include "app/esolution/solution_design.h"
#include "app/esolution/solution_design_default.h"
#include "app/ui/ui_constants.h"
#include "app/ui/ui_num_string_convert.hpp"
#include "app/ui/work_window.h"
#include "app/ui/work_window_tab_main_page_base.h"

////////////////////////////////////////////////////////////////////////////////
DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindowSecondPageData, DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_VALUECHANGED, OnValueChanged)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {
namespace {

////////////////////////////////////////////////////////////////////////////////
/// @brief timer id for refresh control
const uint32_t kTimerIdRefreshControl = 1;
}  // namespace

class WorkWindowSecondPageData::ListVirtalDataView
    : public DuiLib::IListVirtalCallbackUI {
 public:
  explicit ListVirtalDataView(DuiLib::CListUI* list_ui) : list_ui_(list_ui) {}
  ~ListVirtalDataView() {}

  CControlUI* CreateVirtualItem() override {
    CListHBoxElementUI* pHBox = new CListHBoxElementUI;
    //> 设置行高
    pHBox->SetFixedHeight(28);
    ///> No
    DuiLib::CLabelUI* pText = new DuiLib::CLabelUI();
    pText->SetAttributeList(_T("font=\"11\" algin=\"center\""));
    pText->SetFixedHeight(28);
    pText->SetFixedWidth(40);
    pText->SetName(_T("No"));
    pHBox->Add(pText);
    ///> cycle count
    pText = new DuiLib::CLabelUI();
    pText->SetAttributeList(_T("font=\"11\" algin=\"center\""));
    pText->SetFixedHeight(28);
    pText->SetFixedWidth(40);
    pText->SetName(_T("cycle"));
    pHBox->Add(pText);
    ///> frequency value
    pText = new DuiLib::CLabelUI();
    pText->SetAttributeList(_T("font=\"11\" algin=\"center\""));
    pText->SetFixedHeight(28);
    pText->SetFixedWidth(40);
    pText->SetName(_T("frequency"));
    pHBox->Add(pText);
    ///> displacement value
    pText = new DuiLib::CLabelUI();
    pText->SetAttributeList(_T("font=\"11\" algin=\"center\""));
    pText->SetFixedHeight(28);
    pText->SetFixedWidth(40);
    pText->SetName(_T("displacement"));
    pHBox->Add(pText);
    ///> strength value
    pText = new DuiLib::CLabelUI();
    pText->SetAttributeList(_T("font=\"11\" algin=\"center\""));
    pText->SetFixedHeight(28);
    pText->SetFixedWidth(40);
    pText->SetName(_T("strength"));
    pHBox->Add(pText);

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
    sql_str += anx::db::helper::kTableExpDataList;
    sql_str += " WHERE id=";
    sql_str += std::to_string(nRow + 1);
    std::vector<std::map<std::string, std::string>> result;
    anx::db::helper::QueryDataBase(anx::db::helper::kDefaultDatabasePathname,
                                   anx::db::helper::kTableExpDataList, sql_str,
                                   &result);
    if (result.size() == 0) {
      return;
    }
    CListHBoxElementUI* pHBox = static_cast<CListHBoxElementUI*>(
        pControl->GetInterface(DUI_CTR_LISTCONTAINERELEMENT));
    if (pHBox) {
      CDuiString dui_string =
          anx::common::String2WString(result[0]["id"]).c_str();
      DuiLib::CLabelUI* pText = static_cast<DuiLib::CLabelUI*>(
          pHBox->GetItemAt(0)->GetInterface(DUI_CTR_LABEL));
      pText->SetText(dui_string);

      dui_string = anx::common::String2WString(result[0]["cycle"]).c_str();
      pText = static_cast<DuiLib::CLabelUI*>(
          pHBox->GetItemAt(1)->GetInterface(DUI_CTR_LABEL));
      pText->SetText(dui_string);

      double kHz = std::stod(result[0]["kHz"].c_str()) / 1000.f;
      std::string str_kHz = to_string_with_precision(kHz, 3);
      dui_string = anx::common::UTF8ToUnicode(str_kHz).c_str();
      pText = static_cast<DuiLib::CLabelUI*>(
          pHBox->GetItemAt(2)->GetInterface(DUI_CTR_LABEL));
      pText->SetText(dui_string);

      double Mpa = std::stod(result[0]["MPa"].c_str());
      std::string str_Mpa = to_string_with_precision(Mpa, 6);
      dui_string = anx::common::UTF8ToUnicode(str_Mpa).c_str();
      pText = static_cast<DuiLib::CLabelUI*>(
          pHBox->GetItemAt(3)->GetInterface(DUI_CTR_LABEL));
      pText->SetText(dui_string);

      double um = std::stod(result[0]["um"].c_str());
      std::string str_um = to_string_with_precision(um, 2);
      dui_string = anx::common::UTF8ToUnicode(str_um).c_str();
      pText = static_cast<DuiLib::CLabelUI*>(
          pHBox->GetItemAt(4)->GetInterface(DUI_CTR_LABEL));
      pText->SetText(dui_string);

      LOG_F(LG_SENSITIVE) << "no:" << (nRow + 1)
                          << " cycle:" << result[0]["cycle"]
                          << " kHz:" << result[0]["kHz"] << " MPa:" << Mpa
                          << " um:" << um;
    }
  }

 private:
  DuiLib::CListUI* list_ui_;
};

WorkWindowSecondPageData::WorkWindowSecondPageData(
    WorkWindow* pWorkWindow,
    DuiLib::CPaintManagerUI* paint_manager_ui,
    ExpDataInfo* exp_data)
    : pWorkWindow_(pWorkWindow),
      paint_manager_ui_(paint_manager_ui),
      exp_data_info_(exp_data) {
  paint_manager_ui_->AddNotifier(this);
  dedss_.reset(new anx::device::DeviceExpDataSampleSettings());
}

WorkWindowSecondPageData::~WorkWindowSecondPageData() {
  dedss_.reset();
  paint_manager_ui_->RemoveNotifier(this);
}

void WorkWindowSecondPageData::Notify(TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
  } else if (msg.sType == DUI_MSGTYPE_DRAWITEM) {
    if (msg.pSender == list_data_) {
      ListVirtalDataView* lsg =
          reinterpret_cast<ListVirtalDataView*>(list_data_view_.get());
      lsg->DrawItem(reinterpret_cast<CControlUI*>(msg.wParam), msg.lParam);
    } else {
      // do nothing
    }
  } else if (msg.pSender == edit_sample_start_pos_ ||
             msg.pSender == edit_sample_end_pos_ ||
             msg.pSender == edit_sample_interval_) {
    if (msg.sType == DUI_MSGTYPE_TEXTCHANGED) {
      RefreshSampleTimeControl();
      SaveSettingsToResource();
    }
  } else if (msg.sType == kTimer) {
    if (msg.pSender == text_sample_interval_) {
      RefreshSampleTimeControl();
      SaveSettingsToResource();
    }
  } else {
    // do nothing
  }
}

void WorkWindowSecondPageData::OnClick(TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
  } else if (msg.sType == kClick) {
    if (msg.pSender->GetName() == _T("btn_exp_start")) {
      OnExpStart();
    } else if (msg.pSender->GetName() == _T("btn_exp_stop")) {
      OnExpStop();
    } else if (msg.pSender->GetName() == _T("btn_exp_pause")) {
      OnExpPause();
    } else if (msg.pSender->GetName() == _T("btn_exp_resume")) {
      OnExpResume();
    }
  } else {
    // TODO(hhool): do nothing
  }
}

void WorkWindowSecondPageData::OnValueChanged(TNotifyUI& msg) {
  if (msg.sType == DUI_MSGTYPE_VALUECHANGED) {
    if (msg.pSender->GetName() == _T("work_args_area")) {
      // TODO(hhool): do nothing
    } else {
      // TODO(hhool): do nothing
    }
  }
}

bool WorkWindowSecondPageData::OnOptDataSampleChange(void* param) {
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr) {
    return false;
  }
  if (pMsg->pSender == option_sample_mode_exp_) {
    dedss_->sample_mode_ =
        anx::device::DeviceExpDataSample::kSampleModeExponent;
    edit_sample_interval_->SetEnabled(false);
    edit_sample_end_pos_->SetEnabled(false);
  } else if (pMsg->pSender == option_sample_mode_linear_) {
    dedss_->sample_mode_ = anx::device::DeviceExpDataSample::kSampleModeLinear;
    edit_sample_interval_->SetEnabled(true);
    edit_sample_end_pos_->SetEnabled(true);
  } else {
    return false;
  }
  SaveSettingsToResource();
  return true;
}

bool WorkWindowSecondPageData::OnEditDataSampleChange(void* param) {
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr) {
    return false;
  }
  if (pMsg->sType != DUI_MSGTYPE_KILLFOCUS &&
      pMsg->sType != DUI_MSGTYPE_RETURN) {
    return false;
  }
  LOG_F(LG_INFO) << pMsg->pSender->GetName() << " " << pMsg->sType << " "
                 << pMsg->wParam << " " << pMsg->lParam;
  bool save = false;
  if (pMsg->pSender == edit_sample_start_pos_) {
    int64_t value = 0;
    if (get_value_from_control(edit_sample_start_pos_,
                               static_cast<int64_t*>(&value))) {
      if (value != dedss_->sampling_start_pos_) {
        dedss_->sampling_start_pos_ = value;
        save = true;
      }
    }
    set_value_to_edit(edit_sample_start_pos_, dedss_->sampling_start_pos_);
  } else if (pMsg->pSender == edit_sample_end_pos_) {
    int64_t value = 0;
    if (get_value_from_control(edit_sample_end_pos_, &value)) {
      if (value != dedss_->sampling_end_pos_) {
        dedss_->sampling_end_pos_ = value;
        save = true;
      }
    }
    set_value_to_edit(edit_sample_end_pos_, dedss_->sampling_end_pos_);
  } else if (pMsg->pSender == edit_sample_interval_) {
    int64_t value = 0;
    if (get_value_from_control(edit_sample_interval_, &value)) {
      if (value != dedss_->sampling_interval_) {
        dedss_->sampling_interval_ = static_cast<int32_t>(value);
        save = true;
      }
    }
    set_value_to_edit(edit_sample_interval_, dedss_->sampling_interval_);
  } else {
    return false;
  }
  if (save) {
    SaveSettingsToResource();
  }
  return true;
}

bool WorkWindowSecondPageData::OnBtnDataSampleReset(void* param) {
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr) {
    return false;
  }
  if (pMsg->sType != DUI_MSGTYPE_CLICK) {
    return false;
  }
  if (pMsg->pSender != btn_sample_reset_) {
    return false;
  }
  int32_t ret = anx::device::ResetDeviceExpDataSampleSettingsDefaultResource();
  if (ret != 0) {
    LOG_F(LG_ERROR) << "ResetDeviceExpDataSampleSettingsDefaultResource failed";
    return true;
  }
  LoadSettingsFromResource();
  UpdateControlFromSettings();
  return true;
}

bool WorkWindowSecondPageData::OnTimer(void* param) {
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr) {
    return false;
  }
  if (pMsg->pSender != text_sample_interval_) {
    return false;
  }
  if (pMsg->wParam == kTimerIdRefreshControl) {
    RefreshSampleTimeControl();
    return true;
  } else {
    // TODO(hhool): do nothing;
    return false;
  }
  return true;
}

void WorkWindowSecondPageData::Bind() {
  /// @brief device com interface initialization
  std::shared_ptr<anx::device::DeviceComInterface> device_com_ul =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_Ultrasound, this);
  ultra_device_ =
      reinterpret_cast<anx::device::UltraDevice*>(device_com_ul->Device());
  /// @brief sample mode option button exp
  option_sample_mode_exp_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("opt_sampling_exponent")));
  /// @brief sample mode option button linear
  option_sample_mode_linear_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("opt_sampling_linear")));
  /// @brief sample start pos edit
  edit_sample_start_pos_ = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(_T("edit_sampling_start_pos")));
  /// @brief sample end pos edit
  edit_sample_end_pos_ = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(_T("edit_sampling_end_pos")));
  /// @brief sample time duration edit
  edit_sample_interval_ = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(_T("edit_sampling_interval")));
  text_sample_interval_ = static_cast<DuiLib::CTextUI*>(
      paint_manager_ui_->FindControl(_T("text_sampling_interval")));

  list_data_ = static_cast<DuiLib::CListUI*>(
      paint_manager_ui_->FindControl(_T("tab_graph_data_list_data")));

  list_data_view_.reset(new ListVirtalDataView(list_data_));
  list_data_->SetVirtual(true);
  list_data_->SetVirtualItemFormat(
      static_cast<DuiLib::IListVirtalCallbackUI*>(list_data_view_.get()));
  list_data_->SetVirtualItemCount(0);

  btn_sample_reset_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("btn_sample_params_reset")));
  btn_sample_reset_->OnNotify +=
      ::MakeDelegate(this, &WorkWindowSecondPageData::OnBtnDataSampleReset);

  LoadSettingsFromResource();

  UpdateControlFromSettings();

  RefreshSampleTimeControl(true);

  paint_manager_ui_->SetTimer(text_sample_interval_, kTimerIdRefreshControl,
                              1000);
  text_sample_interval_->OnNotify +=
      ::MakeDelegate(this, &WorkWindowSecondPageData::OnEditDataSampleChange);
  edit_sample_start_pos_->OnNotify +=
      ::MakeDelegate(this, &WorkWindowSecondPageData::OnEditDataSampleChange);
  edit_sample_end_pos_->OnNotify +=
      ::MakeDelegate(this, &WorkWindowSecondPageData::OnEditDataSampleChange);
  option_sample_mode_exp_->OnNotify +=
      ::MakeDelegate(this, &WorkWindowSecondPageData::OnOptDataSampleChange);
  option_sample_mode_linear_->OnNotify +=
      ::MakeDelegate(this, &WorkWindowSecondPageData::OnOptDataSampleChange);
}

void WorkWindowSecondPageData::Unbind() {
  // save the settings from the control
  SaveSettingsToResource();

  paint_manager_ui_->KillTimer(text_sample_interval_, kTimerIdRefreshControl);

  if (ultra_device_ != nullptr) {
    ultra_device_->GetPortDevice()->RemoveListener(this);
    ultra_device_ = nullptr;
  }
}

void WorkWindowSecondPageData::LoadSettingsFromResource() {
  dedss_ = anx::device::LoadDeviceExpDataSampleSettingsDefaultResource();
  if (dedss_ == nullptr) {
    dedss_.reset(new anx::device::DeviceExpDataSampleSettings());
  }
}

void WorkWindowSecondPageData::UpdateControlFromSettings() {
  if (dedss_.get() == nullptr) {
    LOG_F(LG_WARN) << "dedss_ is nullptr";
    return;
  }
  if (dedss_->sample_mode_ ==
      anx::device::DeviceExpDataSample::kSampleModeExponent) {
    option_sample_mode_exp_->Selected(true);
    edit_sample_interval_->SetEnabled(false);
    edit_sample_end_pos_->SetEnabled(false);
  } else {
    option_sample_mode_linear_->Selected(true);
    edit_sample_interval_->SetEnabled(true);
    edit_sample_end_pos_->SetEnabled(true);
  }
  set_value_to_edit(edit_sample_start_pos_, dedss_->sampling_start_pos_);
  set_value_to_edit(edit_sample_end_pos_, dedss_->sampling_end_pos_);
  set_value_to_edit(edit_sample_interval_, dedss_->sampling_interval_);
}

void WorkWindowSecondPageData::SaveSettingsToResource() {
  if (dedss_.get() == nullptr) {
    LOG_F(LG_INFO) << "dedss_ is nullptr";
    return;
  }
  anx::device::SaveDeviceExpDataSampleSettingsDefaultResource(*dedss_);
}

void WorkWindowSecondPageData::UpdateUIWithExpStatus(int status) {
  if (status == 0) {
    // stop
    edit_sample_start_pos_->SetEnabled(true);
    if (option_sample_mode_linear_->IsSelected()) {
      edit_sample_interval_->SetEnabled(true);
      edit_sample_end_pos_->SetEnabled(true);
    } else {
      edit_sample_interval_->SetEnabled(false);
      edit_sample_end_pos_->SetEnabled(false);
    }

    option_sample_mode_exp_->SetEnabled(true);
    option_sample_mode_linear_->SetEnabled(true);
    btn_sample_reset_->SetEnabled(true);
  } else if (status == 1) {
    // start
    edit_sample_start_pos_->SetEnabled(false);
    edit_sample_end_pos_->SetEnabled(false);
    edit_sample_interval_->SetEnabled(false);

    option_sample_mode_exp_->SetEnabled(false);
    option_sample_mode_linear_->SetEnabled(false);
    btn_sample_reset_->SetEnabled(false);
  } else if (status == 2) {
    // pause
    edit_sample_start_pos_->SetEnabled(false);
    if (option_sample_mode_linear_->IsSelected()) {
      edit_sample_interval_->SetEnabled(true);
      edit_sample_end_pos_->SetEnabled(true);
    } else {
      edit_sample_interval_->SetEnabled(false);
      edit_sample_end_pos_->SetEnabled(false);
    }

    option_sample_mode_exp_->SetEnabled(false);
    option_sample_mode_linear_->SetEnabled(false);
    btn_sample_reset_->SetEnabled(false);
  }
}

void WorkWindowSecondPageData::RefreshSampleTimeControl(bool force) {
  std::string value = ("=");
  if (!edit_sample_interval_->GetText().IsEmpty()) {
    int32_t sample_time_interval = 0;
    if (get_value_from_control(edit_sample_interval_, &sample_time_interval)) {
      if (dedss_->sampling_interval_ != sample_time_interval || force) {
        if (sample_time_interval > 0) {
          dedss_->sampling_interval_ = sample_time_interval;
          double f_value = static_cast<double>(sample_time_interval);
          f_value /= 10.0f;
          value += to_string_with_precision(f_value, 1);
          value += "S";
          text_sample_interval_->SetText(
              anx::common::String2WString(value).c_str());
          exp_data_info_->exp_sample_interval_ms_ = sample_time_interval * 100;
        } else {
          // TODO(hhool): tips the user the value is invalid
        }
      }
    }
  }
}

void WorkWindowSecondPageData::OnDataReceived(
    anx::device::DeviceComInterface* device,
    const uint8_t* data,
    int32_t size) {
  /// @note process intermittent exp clipping enabled
  if (!ultra_device_->IsUltraStarted()) {
    LOG_F(LG_SENSITIVE) << "is not run";
    return;
  }
  list_data_->SetVirtualItemCount(exp_data_info_->exp_data_table_no_);
}

void WorkWindowSecondPageData::OnExpStart() {
  is_exp_state_ = kExpStateStart;
  exp_time_interval_num_ = 0;
  exp_start_date_time_ = time(nullptr);

  UpdateUIWithExpStatus(1);
  list_data_->RemoveAll();
  list_data_->SetVirtualItemCount(0);
}

void WorkWindowSecondPageData::OnExpStop() {
  is_exp_state_ = kExpStateStop;
  UpdateUIWithExpStatus(0);
  ExportExpResult();
  // reset exp params;
  exp_time_interval_num_ = 0;
  exp_start_date_time_ = 0;
}

void WorkWindowSecondPageData::OnExpPause() {
  UpdateUIWithExpStatus(2);
  is_exp_state_ = kExpStatePause;
}

void WorkWindowSecondPageData::OnExpResume() {
  UpdateUIWithExpStatus(1);
  is_exp_state_ = kExpStateStart;
  exp_time_interval_num_ = 0;
}

void WorkWindowSecondPageData::ClearExpData() {
  exp_time_interval_num_ = 0;
  list_data_->RemoveAll();
  list_data_->SetVirtualItemCount(0);
  if (is_exp_state_ == kExpStateStop) {
    UpdateUIWithExpStatus(0);
  } else if (is_exp_state_ == kExpStateStart) {
    exp_start_date_time_ = time(nullptr);
    UpdateUIWithExpStatus(1);
  }
}

int32_t WorkWindowSecondPageData::ExportExpResult() {
  std::string sql_str = "SELECT * FROM ";
  sql_str += anx::db::helper::kTableExpDataList;
  sql_str += " ORDER BY date ASC";
  sql_str += ";";
  std::vector<std::map<std::string, std::string>> result;
  anx::db::helper::QueryDataBase(anx::db::helper::kDefaultDatabasePathname,
                                 anx::db::helper::kTableExpDataList, sql_str,
                                 &result);
  if (result.size() == 0) {
    LOG_F(LG_ERROR) << "exp data is empty";
    return -1;
  }

  std::vector<anx::expdata::ExperimentData> exp_datas;
  for (auto& item : result) {
    anx::expdata::ExperimentData data;
    data.id_ = std::stoi(item["id"]);
    data.cycle_count_ = std::stoi(item["cycle"]);
    data.KHz_ = std::stod(item["kHz"]);
    data.MPa_ = std::stod(item["MPa"]);
    data.um_ = std::stod(item["um"]);
    exp_datas.push_back(data);
  }

  if (pWorkWindow_->exp_report_.get() == nullptr) {
    LOG_F(LG_ERROR) << "exp report is nullptr";
    return -2;
  }
  anx::expdata::ExperimentReport report = *(pWorkWindow_->exp_report_.get());
  std::string file_pathname_xml;
  int32_t ret = anx::expdata::SaveExperimentReportToXmlWithDefaultPath(
      report, &file_pathname_xml);
  if (ret != 0) {
    LOG_F(LG_ERROR) << "save exp report to xml failed";
    return -3;
  }
  std::string file_pathname_csv;
  ret = anx::expdata::SaveExperimentDataToCsvWithDefaultPath(
      report, exp_datas, &file_pathname_csv);
  if (ret != 0) {
    LOG_F(LG_ERROR) << "save exp data to csv failed";
    return -4;
  }
  std::string file_pathname_docx;
  ret = anx::expdata::SaveReportToDocxWithDefaultPath(report, file_pathname_csv,
                                                      &file_pathname_docx);
  if (ret != 0) {
    LOG_F(LG_ERROR) << "save report to docx failed";
    return -5;
  }
  return 0;
}

}  // namespace ui
}  // namespace anx
