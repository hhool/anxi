/**
 * @file work_window_tab_main_second_page_graph.cc
 * @author hhool (hhool@outlook.com)
 * @brief work window second page graph ui
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "app/ui/work_window_tab_main_second_page_graph.h"

#include <iostream>
#include <map>
#include <utility>
#include <vector>

#include "app/common/defines.h"
#include "app/common/logger.h"
#include "app/common/string_utils.h"
#include "app/common/time_utils.h"
#include "app/db/database_helper.h"
#include "app/device/device_com_factory.h"
#include "app/device/device_com_settings.h"
#include "app/device/device_exp_graph_settings.h"
#include "app/device/device_exp_ultrasound_settings.h"
#include "app/esolution/solution_design.h"
#include "app/esolution/solution_design_default.h"
#include "app/ui/dialog_amplitude_calibration_settings.h"
#include "app/ui/dialog_static_load_guaranteed_settings.h"
#include "app/ui/dmgraph.tlh"
#include "app/ui/ui_constants.h"
#include "app/ui/work_window.h"

////////////////////////////////////////////////////////////////////////////////
DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindowSecondPageGraph, DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {

const int32_t kGraphCtrlSampleTotalMinutesOne = 1;
const int32_t kGraphCtrlSampleTotalMinutesFive = 5;
const int32_t kGraphCtrlSampleTotalMinutesTen = 10;
const int32_t kGraphCtrlSampleTotalMinutesThirty = 30;
const int32_t kGraphCtrlSampleTotalMinutesSixty = 60;
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

const double kYAxisAmpInitialValue = 0.05f;

const double kYAxisStressInitialValue = 0.03f;

const int32_t kTimeGraphButtonId = 2;
}  // namespace

namespace {
std::vector<std::map<std::string, std::string>> QueryExpDataItemByStartTime(
    double start_time,
    int32_t item_count) {
  std::string sql_str = " SELECT* FROM ";
  sql_str += anx::db::helper::kTableExpData;
  sql_str += " WHERE ";
  sql_str += " date > ";
  sql_str += std::to_string(start_time);
  sql_str += " ORDER BY date ASC";
  sql_str += " LIMIT ";
  sql_str += std::to_string(item_count);
  sql_str += ";";
  std::vector<std::map<std::string, std::string>> result;
  anx::db::helper::QueryDataBase(anx::db::helper::kDefaultDatabasePathname,
                                 anx::db::helper::kTableExpData, sql_str,
                                 &result);
  return result;
}

std::vector<std::map<std::string, std::string>> QueryExpDataItemById(
    int32_t id,
    int32_t item_count) {
  std::string sql_str = " SELECT* FROM ";
  sql_str += anx::db::helper::kTableExpData;
  sql_str += " WHERE ";
  sql_str += " id >= ";
  sql_str += std::to_string(id);
  sql_str += " ORDER BY date ASC";
  sql_str += " LIMIT ";
  sql_str += std::to_string(item_count);
  sql_str += ";";
  std::vector<std::map<std::string, std::string>> result;
  anx::db::helper::QueryDataBase(anx::db::helper::kDefaultDatabasePathname,
                                 anx::db::helper::kTableExpData, sql_str,
                                 &result);
  return result;
}

void VartimeToTimeinfo(double vartime, struct tm* timeinfo) {
  time_t t = static_cast<time_t>(vartime);
  SYSTEMTIME st;
  VariantTimeToSystemTime(vartime, &st);
  timeinfo->tm_year = st.wYear - 1900;
  timeinfo->tm_mon = st.wMonth - 1;
  timeinfo->tm_mday = st.wDay;
  timeinfo->tm_hour = st.wHour;
  timeinfo->tm_min = st.wMinute;
  timeinfo->tm_sec = st.wSecond;
  timeinfo->tm_isdst = -1;
}
}  // namespace

class WorkWindowSecondPageGraph::GraphCtrlEvent
    : public GraphCtrlEventInterface {
 public:
  explicit GraphCtrlEvent(WorkWindowSecondPageGraph* pWorkWindowSecondPageGraph)
      : pWorkWindowSecondPageGraph_(pWorkWindowSecondPageGraph) {}
  virtual ~GraphCtrlEvent() = default;

 public:
  void OnGraphCtrlEvent(WorkWindowSecondWorkWindowSecondPageGraphCtrl* pOwner,
                        int32_t action) override {
    if (pOwner == nullptr) {
      return;
    }

    /// @note amp and stress graph control has the same sample count for one
    /// page graph data. so we only need to get the sample count for one page
    /// graph data from the stress graph control.
    int32_t graph_sample_count_one_page =
        pWorkWindowSecondPageGraph_->page_graph_amplitude_ctrl_
            ->GetGraphSampleCountOfXDurationAxis();
    int32_t data_sample_count_for_one_graph_sample =
        pWorkWindowSecondPageGraph_->page_graph_amplitude_ctrl_
            ->GetDataSampleCountForOneGraphSample();
    int32_t total_data_sample_count_one_page =
        graph_sample_count_one_page * data_sample_count_for_one_graph_sample;

    pWorkWindowSecondPageGraph_->exp_data_info_
        ->exp_data_view_current_start_no_ =
        (pWorkWindowSecondPageGraph_->exp_data_info_->exp_data_table_no_ /
         total_data_sample_count_one_page) *
            total_data_sample_count_one_page +
        1;
    pWorkWindowSecondPageGraph_->btn_next_page_->SetEnabled(false);
    pWorkWindowSecondPageGraph_->btn_pre_page_->SetEnabled(true);
    LOG_F(LG_INFO) << "OnGraphCtrlEvent: " << action << " "
                   << pWorkWindowSecondPageGraph_->exp_data_info_->ToString();
    // get the current x min value. update the graph title with the current x
    // (x==vartime) min value.
    double x_min = pOwner->GetXMinOfAxis();
    double x_duration = pOwner->GetXDurationOfAxis();
    pWorkWindowSecondPageGraph_->RefreshExpGraphTitleControl(x_min);
  }

 private:
  WorkWindowSecondPageGraph* pWorkWindowSecondPageGraph_;
};

////////////////////////////////////////////////////////////////////////////////
WorkWindowSecondPageGraph::WorkWindowSecondPageGraph(
    WorkWindow* pWorkWindow,
    DuiLib::CPaintManagerUI* paint_manager_ui,
    ExpDataInfo* exp_data_info)
    : pWorkWindow_(pWorkWindow),
      graph_ctrl_event_(new GraphCtrlEvent(this)),
      paint_manager_ui_(paint_manager_ui),
      exp_data_info_(exp_data_info) {
  paint_manager_ui_->AddNotifier(this);
}

WorkWindowSecondPageGraph::~WorkWindowSecondPageGraph() {
  paint_manager_ui_->KillTimer(btn_pre_page_, kTimeGraphButtonId);
  device_com_sl_.reset();
  device_com_ul_.reset();
}

void WorkWindowSecondPageGraph::Notify(DuiLib::TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
  } else if (msg.sType == kClick) {
    if (msg.pSender->GetName() == _T("graph_settings_pre_page")) {
      OnPagePre(&msg);
    } else if (msg.pSender->GetName() == _T("graph_settings_next_page")) {
      OnPageNext(&msg);
    }
  }
}

void WorkWindowSecondPageGraph::OnClick(TNotifyUI& msg) {
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

bool WorkWindowSecondPageGraph::OnOptGraphTimeModeChange(void* param) {
  // TODO(hhool):
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr) {
    return false;
  }
  return true;
}

bool WorkWindowSecondPageGraph::OnChkGraphAlwaysShowNewChange(void* param) {
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr) {
    return false;
  }

  if (pMsg->pSender == chk_graph_always_show_new_) {
    if (chk_graph_always_show_new_->IsSelected()) {
      /// @note show the last page data
      int32_t graphctrl_sample_total_minutes = graphctrl_sample_total_minutes_;
      int32_t data_sample_count_for_one_graph_sample =
          page_graph_amplitude_ctrl_->GetCurrentDataSampleCountOfOneGraphPlot();
      int32_t data_sample_count =
          minutes_to_data_sample_count(graphctrl_sample_total_minutes);
      int32_t id = exp_data_info_->exp_data_view_current_start_no_;
      while (id + data_sample_count < exp_data_info_->exp_data_table_no_) {
        id += data_sample_count;
      }
      std::vector<std::map<std::string, std::string>> result =
          QueryExpDataItemById(
              id, data_sample_count + data_sample_count_for_one_graph_sample);
      if (result.size() <= 0) {
        return true;
      }
      id = std::stoi(result[0]["id"]);
      assert(exp_data_info_->exp_data_view_current_start_no_ % 10 == 1);
      if (exp_data_info_->exp_data_view_current_start_no_ % 10 != 1) {
        return false;
      }
      exp_data_info_->exp_data_view_current_start_no_ = id;

      /// @note update the graph control with the data from the database.
      this->UpdateGraphCtrl("amp", result);
      this->UpdateGraphCtrl("stress", result);

      /// @note update vartime and update the graph title
      double vartime = std::stod(result[0]["date"]) / kMultiFactor;
      RefreshExpGraphTitleControl(vartime);

      /// always show new
      LOG_F(LG_INFO) << "chk_graph_always_show_new_ selected";
      exp_data_info_->mode_ = view_mode_real;
      /// update the graph to last page data
      if (page_graph_amplitude_ctrl_.get() != nullptr) {
        page_graph_amplitude_ctrl_->SetAutoRefresh(true);
      }
      if (page_graph_stress_ctrl_.get() != nullptr) {
        page_graph_stress_ctrl_->SetAutoRefresh(true);
      }
    } else {
      // not always show new
      LOG_F(LG_INFO) << "chk_graph_always_show_new_ not selected";
      exp_data_info_->mode_ = view_mode_history;
      if (page_graph_amplitude_ctrl_.get() != nullptr) {
        page_graph_amplitude_ctrl_->SetAutoRefresh(false);
      }
      if (page_graph_stress_ctrl_.get() != nullptr) {
        page_graph_stress_ctrl_->SetAutoRefresh(false);
      }
    }
  }
  return true;
}

bool WorkWindowSecondPageGraph::OnOptGraphTimeRangeChange(void* param) {
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr) {
    return false;
  }
  int32_t graphctrl_sample_total_minutes = -1;
  if (pMsg->pSender == opt_graph_time_range_1_minitues_) {
    LOG_F(LG_INFO) << "opt_graph_time_range_1_minitues_";
    graphctrl_sample_total_minutes = kGraphCtrlSampleTotalMinutesOne;
  } else if (pMsg->pSender == opt_graph_time_range_5_minitues_) {
    LOG_F(LG_INFO) << "opt_graph_time_range_5_minitues_";
    graphctrl_sample_total_minutes = kGraphCtrlSampleTotalMinutesFive;
  } else if (pMsg->pSender == opt_graph_time_range_10_minitues_) {
    LOG_F(LG_INFO) << "opt_graph_time_range_10_minitues_";
    graphctrl_sample_total_minutes = kGraphCtrlSampleTotalMinutesTen;
  } else if (pMsg->pSender == opt_graph_time_range_30_minitues_) {
    LOG_F(LG_INFO) << "opt_graph_time_range_30_minitues_";
    graphctrl_sample_total_minutes = kGraphCtrlSampleTotalMinutesThirty;
  } else if (pMsg->pSender == opt_graph_time_range_60_minitues_) {
    LOG_F(LG_INFO) << "opt_graph_time_range_30_minitues_";
    graphctrl_sample_total_minutes = kGraphCtrlSampleTotalMinutesSixty;
  } else {
    return false;
  }
  if (graphctrl_sample_total_minutes == graphctrl_sample_total_minutes_) {
    return true;
  }
  graphctrl_sample_total_minutes_ = graphctrl_sample_total_minutes;
  assert(page_graph_amplitude_ctrl_ != nullptr);
  // TODO(hhool): reset the graph control
  // 1. get the current x min value. the current x min value is the start time.
  double x_min = page_graph_amplitude_ctrl_->GetXMinOfAxis();
  double x_duration = minutes_to_vartime(graphctrl_sample_total_minutes);
  // 2. read the data from the database. from the current x min value to the
  //    current x min value + new x duration value and plus the data sample
  //    count for one graph sample.
  int32_t data_sample_count_for_one_graph_sample =
      page_graph_amplitude_ctrl_->GetCurrentDataSampleCountOfOneGraphPlot();
  int32_t data_sample_count =
      minutes_to_data_sample_count(graphctrl_sample_total_minutes) +
      data_sample_count_for_one_graph_sample;

  int32_t id = exp_data_info_->exp_data_view_current_start_no_;
  std::vector<std::map<std::string, std::string>> result =
      QueryExpDataItemById(id, data_sample_count);

  if (result.size() <= 0) {
    return true;
  }
  exp_data_info_->exp_data_view_current_start_no_ = std::stoi(result[0]["id"]);
  assert(exp_data_info_->exp_data_view_current_start_no_ % 10 == 1);
  if (exp_data_info_->exp_data_view_current_start_no_ % 10 != 1) {
    return true;
  }
  /// @note update the graph control with the data from the database.
  /// @note update auto refresh check box to false
  /// @note update the mode to 1 history view for the graph control
  bool is_last_page = false;
  if ((exp_data_info_->exp_data_view_current_start_no_ + data_sample_count) >
      exp_data_info_->exp_data_table_no_) {
    is_last_page = true;
  }
  bool is_first_page = false;
  if (exp_data_info_->exp_data_view_current_start_no_ == 1) {
    is_first_page = true;
  }

  /// @note update the graph control with the data from the database.
  this->UpdateGraphCtrl("amp", result);
  this->UpdateGraphCtrl("stress", result);

  /// @note update vartime and update the graph title
  double vartime = std::stod(result[0]["date"]) / kMultiFactor;
  RefreshExpGraphTitleControl(vartime);

  /// @note update auto refresh check box to false
  /// @note update the mode to 1 history view for the graph control
  this->RefreshPreNextAlwaysShowNewControl(is_first_page, is_last_page);
  return true;
}

bool WorkWindowSecondPageGraph::OnPagePre(void* param) {
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr || pMsg->pSender != btn_pre_page_) {
    return false;
  }

  /// @note must have the exp_data_table_no_ > 0
  assert(exp_data_info_->exp_data_table_no_ > 0);
  if (exp_data_info_->exp_data_table_no_ <= 0) {
    return true;
  }

  /// @note amp and stress graph control has the same sample count for one
  /// page graph data. so we only need to get the sample count for one page
  /// graph data from the stress graph control.
  int32_t graph_sample_count_one_page =
      page_graph_amplitude_ctrl_->GetGraphSampleCountOfXDurationAxis();
  int32_t data_sample_count_for_one_graph_sample =
      page_graph_amplitude_ctrl_->GetDataSampleCountForOneGraphSample();
  int32_t total_data_sample_count_one_page =
      graph_sample_count_one_page * data_sample_count_for_one_graph_sample;

  /// @note update the current no to the pre page no and update the graph data
  /// from the database and update the graph control. and update the pre page
  /// button status.
  assert(exp_data_info_->exp_data_view_current_start_no_ % 10 == 1);
  int32_t id = exp_data_info_->exp_data_view_current_start_no_;
  bool is_first_page = false;
  id -= graph_sample_count_one_page * data_sample_count_for_one_graph_sample;
  if (id <= 1) {
    id = 1;
    is_first_page = true;
  }
  bool is_last_page = false;
  if (id + total_data_sample_count_one_page >=
      exp_data_info_->exp_data_table_no_) {
    is_last_page = true;
  }
  int32_t no = id;
  LOG_F(LG_INFO) << "no: " << no
                 << " exp_data_info: " << exp_data_info_->ToString();
  std::vector<std::map<std::string, std::string>> result = QueryExpDataItemById(
      id, graph_sample_count_one_page * data_sample_count_for_one_graph_sample);
  if (result.size() <= 0) {
    return true;
  }
  exp_data_info_->exp_data_view_current_start_no_ = std::stoi(result[0]["id"]);
  assert(exp_data_info_->exp_data_view_current_start_no_ % 10 == 1);
  if (exp_data_info_->exp_data_view_current_start_no_ % 10 != 1) {
    return false;
  }
  this->UpdateGraphCtrl("amp", result);
  this->UpdateGraphCtrl("stress", result);

  /// @note update vartime and update the graph title
  double vartime = std::stod(result[0]["date"]) / kMultiFactor;
  RefreshExpGraphTitleControl(vartime);

  /// @note update the graph control with the data from the database.
  /// @note update auto refresh check box to false
  /// @note update the mode to 1 history view for the graph control
  this->RefreshPreNextAlwaysShowNewControl(is_first_page, is_last_page);
  return true;
}

bool WorkWindowSecondPageGraph::OnPageNext(void* param) {
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr || pMsg->pSender != btn_next_page_) {
    return false;
  }
  /// @note must have the exp_data_table_no_ > 0
  assert(exp_data_info_->exp_data_table_no_ > 0);
  if (exp_data_info_->exp_data_table_no_ <= 0) {
    return true;
  }
  /// @note amp and stress graph control has the same sample count for one
  /// page graph data. so we only need to get the sample count for one page
  /// graph data from the stress graph control.
  int32_t graph_sample_count_one_page =
      page_graph_amplitude_ctrl_->GetGraphSampleCountOfXDurationAxis();
  int32_t data_sample_count_for_one_graph_sample =
      page_graph_amplitude_ctrl_->GetDataSampleCountForOneGraphSample();
  int32_t total_data_sample_count_one_page =
      graph_sample_count_one_page * data_sample_count_for_one_graph_sample;
  int32_t graph_sample_count = graph_sample_count_one_page;

  bool is_last_page = false;
  int32_t id = exp_data_info_->exp_data_view_current_start_no_;
  int32_t id_next_page = id + total_data_sample_count_one_page;
  assert(id % 10 == 1);
  if ((id_next_page + total_data_sample_count_one_page) >=
      static_cast<int32_t>(exp_data_info_->exp_data_table_no_)) {
    /// @note next page has 1 more page data than the current page data.
    is_last_page = true;
  }

  id += total_data_sample_count_one_page;
  int32_t no = id;
  LOG_F(LG_INFO) << "no: " << no << " exp_data_view_current_start_no_: "
                 << exp_data_info_->exp_data_view_current_start_no_
                 << " total: " << exp_data_info_->exp_data_table_no_;

  /// @note query the data from the exp_data of the database and update the
  /// graph control.
  std::vector<std::map<std::string, std::string>> result = QueryExpDataItemById(
      id, graph_sample_count * data_sample_count_for_one_graph_sample);
  if (result.size() == 0) {
    return true;
  }
  /// @note update the current no to the next page no and update the graph data
  /// from the database and update the graph control. and update the next page
  /// button status.
  exp_data_info_->exp_data_view_current_start_no_ = std::stoi(result[0]["id"]);
  assert(exp_data_info_->exp_data_view_current_start_no_ % 10 == 1);
  if (exp_data_info_->exp_data_view_current_start_no_ % 10 != 1) {
    return false;
  }

  /////////////////////////////////////////////////////////////////////////////
  /// @note clear current graph data and update the graph data from the
  /// exp_data of the database. and update the graph control.
  this->UpdateGraphCtrl("amp", result);
  this->UpdateGraphCtrl("stress", result);

  /// @note update vartime and update the graph title
  double vartime = std::stod(result[0]["date"]) / kMultiFactor;
  RefreshExpGraphTitleControl(vartime);

  /// @note update the graph control with the data from the database.
  /// @note update auto refresh check box to false
  /// @note update the mode to 1 history view for the graph control
  this->RefreshPreNextAlwaysShowNewControl(false, is_last_page);
  return true;
}

bool WorkWindowSecondPageGraph::OnTimer(void* param) {
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr) {
    return false;
  }
  if (pMsg->pSender != btn_graph_amplitude_title_ &&
      pMsg->pSender != btn_pre_page_) {
    return false;
  }
  if (pMsg->wParam == kTimeGraphButtonId) {
    // update the graph button
    RefreshPreNextControl();
    return true;
  } else {
    return false;
  }
  return true;
}

void WorkWindowSecondPageGraph::Bind() {
  /// @brief device com interface initialization
  device_com_ul_ =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_Ultrasound, this);
  device_com_sl_ =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_StaticLoad, this);
  /////////////////////////////////////////////////////////////////////////////
  /// @brief graph time mode pre hour
  opt_graph_time_mode_pre_hour_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_pre_hour")));
  opt_graph_time_mode_pre_hour_->Selected(false);
  opt_graph_time_mode_now_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_current_hour")));
  opt_graph_time_mode_now_->Selected(true);
  /// @brief bind the option button event
  opt_graph_time_mode_pre_hour_->OnNotify += ::MakeDelegate(
      this, &WorkWindowSecondPageGraph::OnOptGraphTimeModeChange);
  opt_graph_time_mode_now_->OnNotify += ::MakeDelegate(
      this, &WorkWindowSecondPageGraph::OnOptGraphTimeModeChange);

  /////////////////////////////////////////////////////////////////////////////
  /// @brief graph time alway show new
  chk_graph_always_show_new_ = static_cast<DuiLib::CCheckBoxUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_always_new")));
  chk_graph_always_show_new_->Selected(true);
  exp_data_info_->mode_ = view_mode_real;
  /// @brief bind the check box event
  chk_graph_always_show_new_->OnNotify += ::MakeDelegate(
      this, &WorkWindowSecondPageGraph::OnChkGraphAlwaysShowNewChange);

  /////////////////////////////////////////////////////////////////
  /// @brief graph time range option
  opt_graph_time_range_1_minitues_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_1_minite")));
  opt_graph_time_range_1_minitues_->Selected(true);
  opt_graph_time_range_5_minitues_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_5_minite")));
  opt_graph_time_range_5_minitues_->Selected(false);
  opt_graph_time_range_10_minitues_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_10_minite")));
  opt_graph_time_range_10_minitues_->Selected(false);
  opt_graph_time_range_30_minitues_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_30_minite")));
  opt_graph_time_range_30_minitues_->Selected(false);
  opt_graph_time_range_60_minitues_ = static_cast<DuiLib::COptionUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_60_minite")));
  opt_graph_time_range_60_minitues_->Selected(false);
  /// @brief bind the option button event
  opt_graph_time_range_1_minitues_->OnNotify += ::MakeDelegate(
      this, &WorkWindowSecondPageGraph::OnOptGraphTimeRangeChange);
  opt_graph_time_range_5_minitues_->OnNotify += ::MakeDelegate(
      this, &WorkWindowSecondPageGraph::OnOptGraphTimeRangeChange);
  opt_graph_time_range_10_minitues_->OnNotify += ::MakeDelegate(
      this, &WorkWindowSecondPageGraph::OnOptGraphTimeRangeChange);
  opt_graph_time_range_30_minitues_->OnNotify += ::MakeDelegate(
      this, &WorkWindowSecondPageGraph::OnOptGraphTimeRangeChange);
  opt_graph_time_range_60_minitues_->OnNotify += ::MakeDelegate(
      this, &WorkWindowSecondPageGraph::OnOptGraphTimeRangeChange);

  btn_graph_amplitude_title_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("graph_amplitude_title")));
  btn_graph_amplitude_canvas_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("graph_amplitude_canvas")));
  btn_graph_amplitude_canvas_->SetEnabled(false);

  btn_graph_stress_title_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("graph_stress_title")));
  btn_graph_stress_canvas_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("graph_stress_canvas")));
  btn_graph_stress_canvas_->SetEnabled(false);

  btn_pre_page_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_pre_page")));
  btn_pre_page_->SetEnabled(false);
  btn_next_page_ = static_cast<DuiLib::CButtonUI*>(
      paint_manager_ui_->FindControl(_T("graph_settings_next_page")));
  btn_next_page_->SetEnabled(false);

  UpdateControlFromSettings();
  // init the graph control
  double x_min = anx::common::GetCurrrentDateTime();
  double x_duration = minutes_to_vartime(graphctrl_sample_total_minutes_);
  {
    CActiveXUI* activex = static_cast<CActiveXUI*>(
        paint_manager_ui_->FindControl(_T("graph_amplitude_canvas")));
    page_graph_amplitude_ctrl_.reset(
        new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
            graph_ctrl_event_.get(), activex, "amp", x_min, x_duration, 15, 6,
            kYAxisAmpInitialValue, exp_data_info_->mode_ ? false : true));
    page_graph_amplitude_ctrl_->Init(std::vector<Element2DPoint>());
  }
  {
    CActiveXUI* activex = static_cast<CActiveXUI*>(
        paint_manager_ui_->FindControl(_T("graph_stress_canvas")));
    page_graph_stress_ctrl_.reset(
        new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
            graph_ctrl_event_.get(), activex, "stress", x_min, x_duration, 8, 6,
            kYAxisStressInitialValue, exp_data_info_->mode_ ? false : true));
    page_graph_stress_ctrl_->Init(std::vector<Element2DPoint>());
  }
  // bind the graph title timer
  btn_graph_amplitude_title_->OnNotify +=
      ::MakeDelegate(this, &WorkWindowSecondPageGraph::OnTimer);
  // bind the graph control event
  paint_manager_ui_->SetTimer(btn_pre_page_, kTimeGraphButtonId, 1000);
  btn_pre_page_->OnNotify +=
      ::MakeDelegate(this, &WorkWindowSecondPageGraph::OnTimer);
}

void WorkWindowSecondPageGraph::Unbind() {
  SaveSettingsFromControl();
  // kill the timer for the graph button
  paint_manager_ui_->KillTimer(btn_pre_page_, kTimeGraphButtonId);
  // release the graph control
  if (page_graph_amplitude_ctrl_ != nullptr) {
    page_graph_amplitude_ctrl_->Release();
    page_graph_amplitude_ctrl_.reset();
  }
  if (page_graph_stress_ctrl_ != nullptr) {
    page_graph_stress_ctrl_->Release();
    page_graph_stress_ctrl_.reset();
  }
  // release the device com interface
  if (device_com_sl_ != nullptr) {
    device_com_sl_->RemoveListener(this);
    device_com_sl_ = nullptr;
  }
  if (device_com_ul_ != nullptr) {
    device_com_ul_->RemoveListener(this);
    device_com_ul_ = nullptr;
  }
}

void WorkWindowSecondPageGraph::UpdateGraphCtrl(
    std::string name,
    const std::vector<std::map<std::string, std::string>>& result) {
  if (name == "amp") {
    if (page_graph_amplitude_ctrl_ != nullptr) {
      page_graph_amplitude_ctrl_->Release();
      page_graph_amplitude_ctrl_.reset();
    }
    std::vector<Element2DPoint> element_list;
    int32_t item_count = result.size();
    for (int32_t i = 0; i < item_count; i++) {
      const std::map<std::string, std::string>& item = result[i];
      double x = std::stod(item.at("date")) / kMultiFactor;
      double y = std::stod(item.at("um")) / kMultiFactor;
      int32_t id = std::stoi(item.at("id"));
      element_list.push_back(Element2DPoint(x, y, id));
    }
    const std::map<std::string, std::string>& item = result[0];
    double x_min = std::stod(item.at("date")) / kMultiFactor;
    double x_duration =
        minutes_to_vartime(this->graphctrl_sample_total_minutes_);
    x_min -= kVartime2Seconds;
    if (x_min < 0) {
      x_min = 0.0f;
    }
    CActiveXUI* activex = static_cast<CActiveXUI*>(
        paint_manager_ui_->FindControl(_T("graph_amplitude_canvas")));
    page_graph_amplitude_ctrl_.reset(
        new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
            graph_ctrl_event_.get(), activex, "amp", x_min, x_duration, 15, 6,
            kYAxisAmpInitialValue, true));
    page_graph_amplitude_ctrl_->Init(element_list);
  } else if (name == "stress") {
    if (page_graph_stress_ctrl_ != nullptr) {
      page_graph_stress_ctrl_->Release();
      page_graph_stress_ctrl_.reset();
    }
    std::vector<Element2DPoint> element_list;
    int32_t item_count = result.size();
    for (int32_t i = 0; i < item_count; i++) {
      const std::map<std::string, std::string>& item = result[i];
      double x = std::stod(item.at("date")) / kMultiFactor;
      double y = std::stod(item.at("MPa")) / kMultiFactor;
      int32_t id = std::stoi(item.at("id"));
      element_list.push_back(Element2DPoint(x, y, id));
    }
    const std::map<std::string, std::string>& item = result[0];
    double x_min = std::stod(item.at("date")) / kMultiFactor;
    double x_duration =
        minutes_to_vartime(this->graphctrl_sample_total_minutes_);
    x_min -= kVartime2Seconds;
    if (x_min < 0) {
      x_min = 0.0f;
    }
    CActiveXUI* activex = static_cast<CActiveXUI*>(
        paint_manager_ui_->FindControl(_T("graph_stress_canvas")));
    page_graph_stress_ctrl_.reset(
        new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
            graph_ctrl_event_.get(), activex, "stress", x_min, x_duration, 8, 6,
            kYAxisStressInitialValue, true));
    page_graph_stress_ctrl_->Init(element_list);
  }
}

void WorkWindowSecondPageGraph::RefreshExpGraphTitleControl(double vartime) {
  // format vartime to timeinfo struct
  struct tm timeinfo;
  VartimeToTimeinfo(vartime, &timeinfo);

  // format the time string like 00:00 and append to the title string
  // like "最大静载 00:00" and set to the title control. %02d:%02d is
  // used to format the time string to 00:00 format from the timeinfo
  // struct. The timeinfo struct is filled with the current time info.
  char time_str[256];
  snprintf(time_str, sizeof(time_str), "%02d:%02d", timeinfo.tm_hour,
           timeinfo.tm_min);

  // get the current time's hour and minute form timeinfo
  std::string tile_time_str = "底端振幅";
  tile_time_str += time_str;
  DuiLib::CDuiString str_title_with_time_amp =
      anx::common::string2wstring(tile_time_str.c_str()).c_str();
  btn_graph_amplitude_title_->SetText(str_title_with_time_amp);

  // update the graph canvas tile with the current time's hour and minute
  tile_time_str = "最大静载";
  tile_time_str += time_str;
  DuiLib::CDuiString str_title_with_time_stress =
      anx::common::string2wstring(tile_time_str.c_str()).c_str();
  btn_graph_stress_title_->SetText(str_title_with_time_stress);
  // update the graph stress canvas title with the current time's hour and
  // minute
}

void WorkWindowSecondPageGraph::RefreshPreNextAlwaysShowNewControl(
    bool is_first_page,
    bool is_last_page) {
  if (is_first_page) {
    btn_pre_page_->SetEnabled(false);
  } else {
    btn_pre_page_->SetEnabled(true);
  }
  if (is_last_page) {
    btn_next_page_->SetEnabled(false);
    chk_graph_always_show_new_->Selected(true);
    exp_data_info_->mode_ = view_mode_real;
    // update the graph to last page data
    if (page_graph_amplitude_ctrl_.get() != nullptr) {
      page_graph_amplitude_ctrl_->SetAutoRefresh(true);
    }
    if (page_graph_stress_ctrl_.get() != nullptr) {
      page_graph_stress_ctrl_->SetAutoRefresh(true);
    }
  } else {
    btn_next_page_->SetEnabled(true);
    chk_graph_always_show_new_->Selected(false);
    exp_data_info_->mode_ = view_mode_history;
    // update the graph to last page data
    if (page_graph_amplitude_ctrl_.get() != nullptr) {
      page_graph_amplitude_ctrl_->SetAutoRefresh(false);
    }
    if (page_graph_stress_ctrl_.get() != nullptr) {
      page_graph_stress_ctrl_->SetAutoRefresh(false);
    }
  }
}

void WorkWindowSecondPageGraph::RefreshPreNextControl() {
  // get the current page no
  int32_t id = exp_data_info_->exp_data_view_current_start_no_;
  // get the total data sample count for one page graph data
  int32_t graph_sample_count_one_page =
      page_graph_amplitude_ctrl_->GetGraphSampleCountOfXDurationAxis();
  int32_t data_sample_count_for_one_graph_sample =
      page_graph_amplitude_ctrl_->GetDataSampleCountForOneGraphSample();
  int32_t total_data_sample_count_one_page =
      graph_sample_count_one_page * data_sample_count_for_one_graph_sample;
  // get the total data sample count
  int32_t total_data_sample_count = exp_data_info_->exp_data_table_no_;
  // get the graph sample display in the graph control min record no
  int32_t min_disaplay_no = exp_data_info_->exp_data_view_current_start_no_;
  bool is_first_page = true;
  if (min_disaplay_no > 1) {
    is_first_page = false;
  }
  // get the graph sample display in the graph control max record no
  int32_t max_display_no = min_disaplay_no + total_data_sample_count_one_page;
  bool is_last_page = true;
  if (max_display_no < total_data_sample_count) {
    is_last_page = false;
  }
  // update the pre next button status
  if (is_first_page) {
    btn_pre_page_->SetEnabled(false);
  } else {
    btn_pre_page_->SetEnabled(true);
  }
  if (is_last_page) {
    btn_next_page_->SetEnabled(false);
  } else {
    btn_next_page_->SetEnabled(true);
  }
}

void WorkWindowSecondPageGraph::UpdateExpClipTimeFromControl() {}

void WorkWindowSecondPageGraph::OnDataReceived(
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
  } else if (device == device_com_sl_.get()) {
    // process the data from static load device
    // 1. parse the data
    // 2. update the data to the graph
    // 3. update the data to the data table
    hex_str = anx::common::ByteArrayToHexString(data, size);
    std::cout << hex_str << std::endl;
  }
  /*
  if (!chk_graph_always_show_new_->IsSelected()) {
  return;
  }*/
  if (exp_data_info_->exp_time_interval_num_ <= exp_time_interval_num_) {
    return;
  }
  exp_time_interval_num_ = exp_data_info_->exp_time_interval_num_;
  // TODO(hhool): update the data to graph
  page_graph_amplitude_ctrl_->ProcessDataSampleIncoming(
      exp_data_info_->amp_um_);  // NOLINT
  page_graph_stress_ctrl_->ProcessDataSampleIncoming(
      exp_data_info_->stress_value_);  // NOLINT
}

void WorkWindowSecondPageGraph::OnDataOutgoing(
    anx::device::DeviceComInterface* device,
    const uint8_t* data,
    int32_t size) {
  // TODO(hhool):
  if (device == device_com_ul_.get()) {
    // process the data from ultrasound device
    // 1. parse the data
    // 2. update the data to the graph
    // 3. update the data to the data table
  } else if (device == device_com_sl_.get()) {
    // process the data from static load device
    // 1. parse the data
    // 2. update the data to the graph
    // 3. update the data to the data table
  }
}

void WorkWindowSecondPageGraph::UpdateControlFromSettings() {
  std::unique_ptr<anx::device::DeviceExpGraphSettings> dcs =
      anx::device::LoadDeviceExpGraphSettingsDefaultResource();
  if (dcs != nullptr) {
    if (dcs->exp_graph_show_time_type_ == 0) {
      opt_graph_time_mode_pre_hour_->Selected(true);
      opt_graph_time_mode_now_->Selected(false);
    } else {
      opt_graph_time_mode_pre_hour_->Selected(false);
      opt_graph_time_mode_now_->Selected(true);
    }
    if (dcs->exp_graph_range_minitues_ == 0) {
      opt_graph_time_range_1_minitues_->Selected(true);
      opt_graph_time_range_5_minitues_->Selected(false);
      opt_graph_time_range_10_minitues_->Selected(false);
      opt_graph_time_range_30_minitues_->Selected(false);
      opt_graph_time_range_60_minitues_->Selected(false);
      graphctrl_sample_total_minutes_ = kGraphCtrlSampleTotalMinutesOne;
    } else if (dcs->exp_graph_range_minitues_ == 1) {
      opt_graph_time_range_1_minitues_->Selected(false);
      opt_graph_time_range_5_minitues_->Selected(true);
      opt_graph_time_range_10_minitues_->Selected(false);
      opt_graph_time_range_30_minitues_->Selected(false);
      opt_graph_time_range_60_minitues_->Selected(false);
      graphctrl_sample_total_minutes_ = kGraphCtrlSampleTotalMinutesFive;
    } else if (dcs->exp_graph_range_minitues_ == 2) {
      opt_graph_time_range_1_minitues_->Selected(false);
      opt_graph_time_range_5_minitues_->Selected(false);
      opt_graph_time_range_10_minitues_->Selected(true);
      opt_graph_time_range_30_minitues_->Selected(false);
      opt_graph_time_range_60_minitues_->Selected(false);
      graphctrl_sample_total_minutes_ = kGraphCtrlSampleTotalMinutesTen;
    } else if (dcs->exp_graph_range_minitues_ == 3) {
      opt_graph_time_range_1_minitues_->Selected(false);
      opt_graph_time_range_5_minitues_->Selected(false);
      opt_graph_time_range_10_minitues_->Selected(false);
      opt_graph_time_range_30_minitues_->Selected(true);
      opt_graph_time_range_60_minitues_->Selected(false);
      graphctrl_sample_total_minutes_ = kGraphCtrlSampleTotalMinutesThirty;
    } else {
      opt_graph_time_range_1_minitues_->Selected(false);
      opt_graph_time_range_5_minitues_->Selected(false);
      opt_graph_time_range_10_minitues_->Selected(false);
      opt_graph_time_range_30_minitues_->Selected(false);
      opt_graph_time_range_60_minitues_->Selected(true);
      graphctrl_sample_total_minutes_ = kGraphCtrlSampleTotalMinutesSixty;
    }
  }
}

void WorkWindowSecondPageGraph::SaveSettingsFromControl() {
  anx::device::DeviceExpGraphSettings dcs;
  if (opt_graph_time_mode_pre_hour_->IsSelected()) {
    dcs.exp_graph_show_time_type_ = 0;
  } else {
    dcs.exp_graph_show_time_type_ = 1;
  }
  if (opt_graph_time_range_1_minitues_->IsSelected()) {
    dcs.exp_graph_range_minitues_ = 0;
  } else if (opt_graph_time_range_5_minitues_->IsSelected()) {
    dcs.exp_graph_range_minitues_ = 1;
  } else if (opt_graph_time_range_10_minitues_->IsSelected()) {
    dcs.exp_graph_range_minitues_ = 2;
  } else if (opt_graph_time_range_30_minitues_->IsSelected()) {
    dcs.exp_graph_range_minitues_ = 3;
  } else {
    dcs.exp_graph_range_minitues_ = 4;
  }
  anx::device::SaveDeviceExpGraphSettingsDefaultResource(dcs);
}

void WorkWindowSecondPageGraph::OnExpStart() {
  /// @note update graph title with the current time
  /// get current var time
  double vartime = anx::common::GetCurrrentDateTime();
  RefreshExpGraphTitleControl(vartime);
  /// @note reset the graph to mode real and set the time interval number to 0
  chk_graph_always_show_new_->SetCheck(true);
  exp_data_info_->mode_ = view_mode_real;
  exp_data_info_->exp_data_view_current_start_no_ = 1;
  exp_time_interval_num_ = 0;
  /// @note update the graph control with the data from the database and
  /// update the graph control.
  double x_min = anx::common::GetCurrrentDateTime();
  double x_duration = minutes_to_vartime(graphctrl_sample_total_minutes_);
  {
    CActiveXUI* activex = static_cast<CActiveXUI*>(
        paint_manager_ui_->FindControl(_T("graph_amplitude_canvas")));
    page_graph_amplitude_ctrl_.reset(
        new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
            graph_ctrl_event_.get(), activex, "amp", x_min, x_duration, 15, 6,
            kYAxisAmpInitialValue, exp_data_info_->mode_ ? false : true));
    page_graph_amplitude_ctrl_->Init(std::vector<Element2DPoint>());
  }
  {
    CActiveXUI* activex = static_cast<CActiveXUI*>(
        paint_manager_ui_->FindControl(_T("graph_stress_canvas")));
    page_graph_stress_ctrl_.reset(
        new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
            graph_ctrl_event_.get(), activex, "stress", x_min, x_duration, 8, 6,
            kYAxisStressInitialValue, exp_data_info_->mode_ ? false : true));
    page_graph_stress_ctrl_->Init(std::vector<Element2DPoint>());
  }
}

void WorkWindowSecondPageGraph::OnExpStop() {
  exp_time_interval_num_ = 0;
}

void WorkWindowSecondPageGraph::OnExpPause() {}

void WorkWindowSecondPageGraph::OnExpResume() {
  chk_graph_always_show_new_->SetCheck(true);
  exp_data_info_->mode_ = view_mode_real;
}

}  // namespace ui
}  // namespace anx
