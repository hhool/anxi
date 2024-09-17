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

#include <iostream>
#include <map>
#include <utility>

#include "app/common/defines.h"
#include "app/common/logger.h"
#include "app/common/string_utils.h"
#include "app/common/time_utils.h"
#include "app/db/database_helper.h"
#include "app/device/device_com_factory.h"
#include "app/device/device_com_settings.h"
#include "app/device/device_exp_data_sample_settings.h"
#include "app/ui/ui_constants.h"
#include "app/ui/work_window.h"

////////////////////////////////////////////////////////////////////////////////
DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindowSecondPageData, DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {
namespace {

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
    sql_str += anx::db::helper::kTableExpData;
    sql_str += " WHERE id=";
    sql_str += std::to_string(nRow + 1);
    std::vector<std::map<std::string, std::string>> result;
    anx::db::helper::QueryDataBase(anx::db::helper::kDefaultDatabasePathname,
                                   anx::db::helper::kTableExpData, sql_str,
                                   &result);
    if (result.size() == 0) {
      return;
    }
    CListHBoxElementUI* pHBox = static_cast<CListHBoxElementUI*>(
        pControl->GetInterface(DUI_CTR_LISTCONTAINERELEMENT));
    if (pHBox) {
      CDuiString dui_string =
          anx::common::string2wstring(result[0]["id"]).c_str();
      DuiLib::CLabelUI* pText = static_cast<DuiLib::CLabelUI*>(
          pHBox->GetItemAt(0)->GetInterface(DUI_CTR_LABEL));
      pText->SetText(dui_string);

      dui_string = anx::common::string2wstring(result[0]["cycle"]).c_str();
      pText = static_cast<DuiLib::CLabelUI*>(
          pHBox->GetItemAt(1)->GetInterface(DUI_CTR_LABEL));
      pText->SetText(dui_string);

      double kHz = std::stod(result[0]["kHz"].c_str()) / kMultiFactor;
      dui_string = std::to_wstring(kHz).c_str();
      pText = static_cast<DuiLib::CLabelUI*>(
          pHBox->GetItemAt(2)->GetInterface(DUI_CTR_LABEL));
      pText->SetText(dui_string);

      double Mpa = std::stod(result[0]["MPa"].c_str()) / kMultiFactor;
      dui_string = std::to_wstring(Mpa).c_str();
      pText = static_cast<DuiLib::CLabelUI*>(
          pHBox->GetItemAt(3)->GetInterface(DUI_CTR_LABEL));
      pText->SetText(dui_string);

      double um = std::stod(result[0]["um"].c_str()) / kMultiFactor;
      dui_string = std::to_wstring(um).c_str();
      pText = static_cast<DuiLib::CLabelUI*>(
          pHBox->GetItemAt(4)->GetInterface(DUI_CTR_LABEL));
      pText->SetText(dui_string);

      LOG_F(LG_INFO) << "no:" << (nRow + 1) << " cycle:" << result[0]["cycle"]
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
  device_exp_data_settings_.reset(
      new anx::device::DeviceExpDataSampleSettings());
}

WorkWindowSecondPageData::~WorkWindowSecondPageData() {
  device_exp_data_settings_.reset();
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
  } else if (msg.sType == _T("selectchanged")) {
    // TODO(hhool): add selectchanged action
  }
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
  device_com_ul_ =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_Ultrasound, this);
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

  UpdateControlFromSettings();

  paint_manager_ui_->SetTimer(text_sample_interval_, kTimerIdRefreshControl,
                              1000);
  text_sample_interval_->OnNotify +=
      ::MakeDelegate(this, &WorkWindowSecondPageData::OnTimer);
}

void WorkWindowSecondPageData::Unbind() {
  // save the settings from the control
  SaveSettingsFromControl();

  paint_manager_ui_->KillTimer(text_sample_interval_, kTimerIdRefreshControl);

  if (device_com_ul_ != nullptr) {
    device_com_ul_->RemoveListener(this);
    device_com_ul_ = nullptr;
  }
}

void WorkWindowSecondPageData::RefreshSampleTimeControl() {
  std::string value = ("=");
  int32_t sample_time_interval = _ttol(edit_sample_interval_->GetText());
  if (device_exp_data_settings_->sampling_interval_ != sample_time_interval) {
    device_exp_data_settings_->sampling_interval_ = sample_time_interval;
    value += format_num(sample_time_interval * 100);
    value += "S";
    text_sample_interval_->SetText(anx::common::string2wstring(value).c_str());
  }
}

std::unique_ptr<anx::device::DeviceExpDataSampleSettings>
WorkWindowSecondPageData::UpdateExpClipTimeFromControl() {
  anx::device::DeviceExpDataSampleSettings des;
  DuiLib::CDuiString str = edit_sample_start_pos_->GetText();
  if (str.IsEmpty()) {
    return nullptr;
  }
  str = edit_sample_end_pos_->GetText();
  if (str.IsEmpty()) {
    return nullptr;
  }
  str = edit_sample_interval_->GetText();
  if (str.IsEmpty()) {
    return nullptr;
  }
  int64_t sample_start_pos = 0;
  int64_t sample_end_pos = 0;
  int64_t sample_time_interval = 0;
  sample_start_pos = _ttoll(edit_sample_start_pos_->GetText());
  sample_end_pos = _ttoll(edit_sample_end_pos_->GetText());
  sample_time_interval = _ttoll(edit_sample_interval_->GetText());
  if (sample_start_pos >= sample_end_pos) {
    return nullptr;
  }
  if (sample_time_interval <= 0) {
    return nullptr;
  }
  if (sample_time_interval > (sample_end_pos - sample_start_pos)) {
    return nullptr;
  }
  if (option_sample_mode_exp_->IsSelected()) {
    des.sample_mode_ = 0;
  } else {
    des.sample_mode_ = 1;
  }
  des.sampling_start_pos_ = static_cast<int32_t>(sample_start_pos);
  des.sampling_end_pos_ = static_cast<int32_t>(sample_end_pos);
  des.sampling_interval_ = static_cast<int32_t>(sample_time_interval);
  return std::unique_ptr<anx::device::DeviceExpDataSampleSettings>(
      new anx::device::DeviceExpDataSampleSettings(des));
}

void WorkWindowSecondPageData::UpdateControlFromSettings() {
  std::unique_ptr<anx::device::DeviceExpDataSampleSettings> settings =
      anx::device::LoadDeviceExpDataSampleSettingsDefaultResource();
  if (settings != nullptr) {
    if (settings->sample_mode_ == 0) {
      option_sample_mode_exp_->Selected(true);
    } else {
      option_sample_mode_linear_->Selected(true);
    }
    edit_sample_start_pos_->SetText(
        anx::common::string2wstring(
            std::to_string(settings->sampling_start_pos_))
            .c_str());
    edit_sample_end_pos_->SetText(
        anx::common::string2wstring(std::to_string(settings->sampling_end_pos_))
            .c_str());
    edit_sample_interval_->SetText(
        anx::common::string2wstring(
            std::to_string(settings->sampling_interval_))
            .c_str());

    device_exp_data_settings_.reset(settings.release());
  }
}

void WorkWindowSecondPageData::SaveSettingsFromControl() {
  std::unique_ptr<anx::device::DeviceExpDataSampleSettings> des =
      UpdateExpClipTimeFromControl();
  if (des != nullptr) {
    anx::device::SaveDeviceExpDataSampleSettingsDefaultResource(*des);
  }
}

void anx::ui::WorkWindowSecondPageData::UpdateUIWithExpStatus(int status) {
  if (status == 0) {
    // stop
    edit_sample_start_pos_->SetEnabled(true);
    edit_sample_end_pos_->SetEnabled(true);
    edit_sample_interval_->SetEnabled(true);

    option_sample_mode_exp_->SetEnabled(true);
    option_sample_mode_linear_->SetEnabled(true);
  } else if (status == 1) {
    // start
    edit_sample_start_pos_->SetEnabled(false);
    edit_sample_end_pos_->SetEnabled(false);
    edit_sample_interval_->SetEnabled(false);

    option_sample_mode_exp_->SetEnabled(false);
    option_sample_mode_linear_->SetEnabled(false);
  } else if (status == 2) {
    // pause
    edit_sample_start_pos_->SetEnabled(true);
    edit_sample_end_pos_->SetEnabled(true);
    edit_sample_interval_->SetEnabled(true);

    option_sample_mode_exp_->SetEnabled(true);
    option_sample_mode_linear_->SetEnabled(true);
  }
}

void WorkWindowSecondPageData::OnDataReceived(
    anx::device::DeviceComInterface* device,
    const uint8_t* data,
    int32_t size) {
  // TODO(hhool): process data
  std::string hex_str;
  if (device == device_com_ul_.get()) {
    // process the data from ultrasound device
    // 1. parse the data
    // 2. update the data to the graph
    // 3. update the data to the data table
    // output the data as hex to the std::string
    hex_str = anx::common::ByteArrayToHexString(data, size);
    std::cout << hex_str << std::endl;
  }
  if (exp_data_info_->exp_time_interval_num_ <= exp_time_interval_num_) {
    return;
  }
  LOG_F(LG_INFO) << "exp_data_table_no_:" << exp_data_info_->exp_data_table_no_;
  exp_time_interval_num_ = exp_data_info_->exp_time_interval_num_;
  list_data_->SetVirtualItemCount(exp_data_info_->exp_data_table_no_);
}

void WorkWindowSecondPageData::OnDataOutgoing(
    anx::device::DeviceComInterface* device,
    const uint8_t* data,
    int32_t size) {
  // TODO(hhool):
  if (device == device_com_ul_.get()) {
    // process the data from ultrasound device
    // 1. parse the data
    // 2. update the data to the graph
    // 3. update the data to the data table
  }
}

void WorkWindowSecondPageData::OnExpStart() {
  is_exp_state_ = 1;
  exp_time_interval_num_ = 0;
  exp_start_date_time_ = time(nullptr);

  UpdateUIWithExpStatus(1);
  list_data_->RemoveAll();
}

void WorkWindowSecondPageData::OnExpStop() {
  is_exp_state_ = 0;
  UpdateUIWithExpStatus(0);
  ExportToCSV(exp_start_date_time_);
  // reset exp params;
  exp_time_interval_num_ = 0;
  exp_start_date_time_ = 0;
}

void WorkWindowSecondPageData::OnExpPause() {
  UpdateUIWithExpStatus(2);
  is_exp_state_ = 2;
}

void WorkWindowSecondPageData::OnExpResume() {
  UpdateUIWithExpStatus(1);
  is_exp_state_ = 1;
}
/// TODO(hhool): add the function to clear the data
/// save the data to the file?
void WorkWindowSecondPageData::ClearExpData() {
  list_data_->RemoveAll();
}

void anx::ui::WorkWindowSecondPageData::ExportToCSV(int64_t start_time) {
  std::string sql_str = "SELECT * FROM ";
  sql_str += anx::db::helper::kTableExpData;
  sql_str += " ORDER BY date ASC";
  sql_str += ";";
  std::vector<std::map<std::string, std::string>> result;
  anx::db::helper::QueryDataBase(anx::db::helper::kDefaultDatabasePathname,
                                 anx::db::helper::kTableExpData, sql_str,
                                 &result);
  if (result.size() == 0) {
    return;
  }

  std::vector<anx::expdata::ExperimentData> exp_datas;
  for (auto& item : result) {
    anx::expdata::ExperimentData data;
    data.id_ = std::stoi(item["id"]);
    data.cycle_count_ = std::stoi(item["cycle"]);
    data.KHz_ = std::stod(item["kHz"]) / kMultiFactor;
    data.MPa_ = std::stod(item["MPa"]) / kMultiFactor;
    data.um_ = std::stod(item["um"]) / kMultiFactor;
    exp_datas.push_back(data);
  }
  // TODO(hhool): auto save the data to the file
  // file path is execuatable path/recored/xxx.csv
  // file format is csv
  anx::expdata::SaveExperimentDataToCsvWithDefaultPath(exp_datas, start_time);
}

}  // namespace ui
}  // namespace anx
