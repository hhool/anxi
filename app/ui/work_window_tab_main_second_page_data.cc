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

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 2) {
  int nn = n;
  std::ostringstream out;
  out << std::fixed << std::setprecision(nn) << a_value;
  return out.str();
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
          anx::common::string2wstring(result[0]["id"]).c_str();
      DuiLib::CLabelUI* pText = static_cast<DuiLib::CLabelUI*>(
          pHBox->GetItemAt(0)->GetInterface(DUI_CTR_LABEL));
      pText->SetText(dui_string);

      dui_string = anx::common::string2wstring(result[0]["cycle"]).c_str();
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
  } else if (msg.pSender == edit_sample_start_pos_ ||
             msg.pSender == edit_sample_end_pos_ ||
             msg.pSender == edit_sample_interval_) {
    if (msg.sType == DUI_MSGTYPE_TEXTCHANGED) {
      RefreshSampleTimeControl();
      SaveSettingsFromControl();
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
      ENMsgStruct* enmsg = reinterpret_cast<ENMsgStruct*>(msg.wParam);
      if (enmsg == nullptr) {
        return;
      }
      if (enmsg->type_ == enmsg_type_exp_stress_amp) {
        anx::esolution::SolutionDesign* design =
            reinterpret_cast<anx::esolution::SolutionDesign*>(enmsg->ptr_);
        if (design == nullptr) {
          return;
        }
        double exp_amplitude =
            reinterpret_cast<anx::esolution::ExpDesignResult0*>(
                design->result_.get())
                ->f_eamplitude_;
        double exp_statc_load_mpa = 0.0f;
        if (design->result_->solution_type_ ==
            anx::esolution::kSolutionName_Stresses_Adjustable) {
          exp_statc_load_mpa =
              reinterpret_cast<
                  anx::esolution::ExpDesignResultStressesAdjustable*>(
                  design->result_.get())
                  ->f_static_load_MPa_;
        } else if (design->result_->solution_type_ ==
                   anx::esolution::kSolutionName_Th3point_Bending) {
          exp_statc_load_mpa =
              reinterpret_cast<anx::esolution::ExpDesignResultTh3pointBending*>(
                  design->result_.get())
                  ->f_static_load_MPa_;
        }
        LOG_F(LG_INFO) << "exp_amplitude:" << exp_amplitude
                       << " exp_statc_load_mpa:" << exp_statc_load_mpa;
        // TODO(hhool):
        if (exp_amplitude > 0.0f) {
        }
        if (exp_statc_load_mpa > 0.0f) {
        }
      }
    } else {
      // do nothing
    }
  }
}

bool WorkWindowSecondPageData::OnOptDataSampleChange(void* param) {
  if (param == nullptr) {
    return false;
  }
  DuiLib::COptionUI* pOpt = reinterpret_cast<DuiLib::COptionUI*>(param);
  if (pOpt == nullptr) {
    return false;
  }
  if (pOpt == option_sample_mode_exp_) {
    device_exp_data_settings_->sample_mode_ = 0;
  } else if (pOpt == option_sample_mode_linear_) {
    device_exp_data_settings_->sample_mode_ = 1;
  }
  SaveSettingsFromControl();
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

  UpdateControlFromSettings();
  RefreshSampleTimeControl(true);

  paint_manager_ui_->SetTimer(text_sample_interval_, kTimerIdRefreshControl,
                              1000);
  text_sample_interval_->OnNotify +=
      ::MakeDelegate(this, &WorkWindowSecondPageData::OnTimer);
  option_sample_mode_exp_->OnNotify +=
      ::MakeDelegate(this, &WorkWindowSecondPageData::OnOptDataSampleChange);
  option_sample_mode_linear_->OnNotify +=
      ::MakeDelegate(this, &WorkWindowSecondPageData::OnOptDataSampleChange);
}

void WorkWindowSecondPageData::Unbind() {
  // save the settings from the control
  SaveSettingsFromControl();

  paint_manager_ui_->KillTimer(text_sample_interval_, kTimerIdRefreshControl);

  if (ultra_device_ != nullptr) {
    ultra_device_->GetPortDevice()->RemoveListener(this);
    ultra_device_ = nullptr;
  }
}

void WorkWindowSecondPageData::RefreshSampleTimeControl(bool force) {
  std::string value = ("=");
  int32_t sample_time_interval = _ttol(edit_sample_interval_->GetText());
  if (force ||
      device_exp_data_settings_->sampling_interval_ != sample_time_interval) {
    if (sample_time_interval == 0) {
      return;
    }
    device_exp_data_settings_->sampling_interval_ = sample_time_interval;
    value += to_string_with_precision((sample_time_interval * 100) / 1000.0f, 1);
    value += "S";
    text_sample_interval_->SetText(anx::common::string2wstring(value).c_str());
    exp_data_info_->exp_sample_interval_ms_ = sample_time_interval * 100;
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
  if (sample_start_pos > sample_end_pos && sample_end_pos > 0) {
    return nullptr;
  }
  if (sample_time_interval <= 0) {
    return nullptr;
  }
  if (sample_time_interval > (sample_end_pos - sample_start_pos) &&
      sample_end_pos > 0) {
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

void WorkWindowSecondPageData::UpdateUIWithExpStatus(int status) {
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
  if (exp_data_info_->exp_time_interval_num_ <= exp_time_interval_num_) {
    return;
  }
  if (!ultra_device_->IsUltraStarted()) {
    LOG_F(LG_INFO) << "is not run";
    return;
  }
  LOG_F(LG_INFO) << "exp_data_table_no_:" << exp_data_info_->exp_data_table_no_;
  exp_time_interval_num_ = exp_data_info_->exp_time_interval_num_;
  list_data_->SetVirtualItemCount(exp_data_info_->exp_data_table_no_);
}

void WorkWindowSecondPageData::OnExpStart() {
  is_exp_state_ = 1;
  exp_time_interval_num_ = 0;
  exp_start_date_time_ = time(nullptr);

  UpdateUIWithExpStatus(1);
  list_data_->RemoveAll();
  list_data_->SetVirtualItemCount(0);
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

void WorkWindowSecondPageData::ClearExpData() {
  exp_time_interval_num_ = 0;
  list_data_->RemoveAll();
  list_data_->SetVirtualItemCount(0);
  if (is_exp_state_ == 0) {
    UpdateUIWithExpStatus(0);
  } else if (is_exp_state_ == 1) {
    exp_start_date_time_ = time(nullptr);
    UpdateUIWithExpStatus(1);
  }
}

void WorkWindowSecondPageData::ExportToCSV(int64_t start_time) {
  std::string sql_str = "SELECT * FROM ";
  sql_str += anx::db::helper::kTableExpDataList;
  sql_str += " ORDER BY date ASC";
  sql_str += ";";
  std::vector<std::map<std::string, std::string>> result;
  anx::db::helper::QueryDataBase(anx::db::helper::kDefaultDatabasePathname,
                                 anx::db::helper::kTableExpDataList, sql_str,
                                 &result);
  if (result.size() == 0) {
    return;
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
  // TODO(hhool): auto save the data to the file
  // file path is execuatable path/recored/xxx.csv
  // file format is csv
  anx::expdata::SaveExperimentDataToCsvWithDefaultPath(exp_datas, start_time);
}

}  // namespace ui
}  // namespace anx
