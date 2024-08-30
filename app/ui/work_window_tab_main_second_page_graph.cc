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

/// @brief timer id for the graph update
const int32_t kTimerGraphId = 1;

/// @brief period for the graph update in ms
const int32_t kTimerGraphIdPeriod = 1000;

const double kYAxisAmpInitialValue = 0.05f;

const double kYAxisStressInitialValue = 0.03f;
/////////////////////////////////////////////////////////////////////////////
/// @brief the graph control sample total minutes for the graph control.

double minutes_to_var_time_duration(int32_t minutes) {
  if (minutes == 1) {
    return kVarTimeDuration5min;
  } else if (minutes == 5) {
    return kVarTimeDuration5min;
  } else if (minutes == 10) {
    return kVarTimeDuration10min;
  } else if (minutes == 30) {
    return kVarTimeDuration30min;
  } else if (minutes == 60) {
    return kVarTimeDuration60min;
  } else {
    return kVarTimeDuration5min;
  }
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
        pWorkWindowSecondPageGraph_->page_graph_stress_ctrl_
            ->GetCurrentXDurationGraphSampleCount();
    int32_t data_sample_count_for_one_graph_sample =
        pWorkWindowSecondPageGraph_->page_graph_stress_ctrl_
            ->GetDataSampleCountForOneGraphSample();
    int32_t total_data_sample_count_one_page =
        graph_sample_count_one_page * data_sample_count_for_one_graph_sample;

    pWorkWindowSecondPageGraph_->exp_data_info_->exp_data_current_no_ =
        (pWorkWindowSecondPageGraph_->exp_data_info_->exp_data_table_no_ /
         total_data_sample_count_one_page) *
            total_data_sample_count_one_page +
        1;
    pWorkWindowSecondPageGraph_->btn_next_page_->SetEnabled(false);
    pWorkWindowSecondPageGraph_->btn_pre_page_->SetEnabled(true);
    LOG_F(LG_INFO) << "OnGraphCtrlEvent: " << action << " "
                   << pWorkWindowSecondPageGraph_->exp_data_info_->ToString();
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
  paint_manager_ui_->KillTimer(btn_graph_amplitude_title_, kTimerGraphId);
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
  // TODO(hhool):
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr) {
    return false;
  }
  if (pMsg->pSender == chk_graph_always_show_new_) {
    if (chk_graph_always_show_new_->IsSelected()) {
      // always show new
      LOG_F(LG_INFO) << "chk_graph_always_show_new_ selected";
      exp_data_info_->mode_ = 0;
      // update the graph to last page data
      if (page_graph_amplitude_ctrl_.get() != nullptr) {
        page_graph_amplitude_ctrl_->SetAutoRefresh(true);
      }
      if (page_graph_stress_ctrl_.get() != nullptr) {
        page_graph_stress_ctrl_->SetAutoRefresh(true);
      }
    } else {
      // not always show new
      LOG_F(LG_INFO) << "chk_graph_always_show_new_ not selected";
      exp_data_info_->mode_ = 1;
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
  graphctrl_sample_total_minutes_ = graphctrl_sample_total_minutes;

  if (page_graph_amplitude_ctrl_ != nullptr) {
    if (page_graph_amplitude_ctrl_->GetSamplingTotalMinutes() !=
        graphctrl_sample_total_minutes) {
      CActiveXUI* activex = static_cast<CActiveXUI*>(
          paint_manager_ui_->FindControl(_T("graph_amplitude_canvas")));
      std::vector<Element2DPoint> element_list =
          page_graph_amplitude_ctrl_->GetGraphDataList();
      int32_t data_sample_count_for_one_graph_sample =
          page_graph_amplitude_ctrl_->GetSamplingCount();
      double data_sample_value_one_graph_sample =
          page_graph_amplitude_ctrl_->GetSamplingValueTotal();
      page_graph_amplitude_ctrl_->Release();
      page_graph_amplitude_ctrl_.reset();
      page_graph_amplitude_ctrl_.reset(
          new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
              graph_ctrl_event_.get(), activex, "amp",
              graphctrl_sample_total_minutes, 15, 6, kYAxisAmpInitialValue,
              true));
      page_graph_amplitude_ctrl_->Init(element_list);
      page_graph_amplitude_ctrl_->SetSamplingCount(
          data_sample_count_for_one_graph_sample);
      page_graph_amplitude_ctrl_->SetSamplingValueTotal(
          data_sample_value_one_graph_sample);
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  if (page_graph_stress_ctrl_ != nullptr) {
    if (page_graph_stress_ctrl_->GetSamplingTotalMinutes() !=
        graphctrl_sample_total_minutes) {
      CActiveXUI* activex = static_cast<CActiveXUI*>(
          paint_manager_ui_->FindControl(_T("graph_stress_canvas")));
      std::vector<Element2DPoint> element_list =
          page_graph_stress_ctrl_->GetGraphDataList();
      int32_t data_sample_count_for_one_graph_sample =
          page_graph_stress_ctrl_->GetSamplingCount();
      double data_sample_value_one_graph_sample =
          page_graph_stress_ctrl_->GetSamplingValueTotal();
      page_graph_stress_ctrl_->Release();
      page_graph_stress_ctrl_.reset();
      page_graph_stress_ctrl_.reset(
          new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
              graph_ctrl_event_.get(), activex, "stress",
              graphctrl_sample_total_minutes, 8, 6, kYAxisStressInitialValue,
              true));
      page_graph_stress_ctrl_->Init(element_list);
      page_graph_stress_ctrl_->SetSamplingCount(
          data_sample_count_for_one_graph_sample);
      page_graph_stress_ctrl_->SetSamplingValueTotal(
          data_sample_value_one_graph_sample);
    }
  }

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

  /// @note update auto refresh check box to false
  /// @note update the mode to 1 history view for the graph control
  chk_graph_always_show_new_->Selected(false);
  exp_data_info_->mode_ = 1;

  /// @note update the auto refresh of graph control to false
  if (page_graph_amplitude_ctrl_.get() != nullptr) {
    page_graph_amplitude_ctrl_->SetAutoRefresh(false);
  }
  if (page_graph_stress_ctrl_.get() != nullptr) {
    page_graph_stress_ctrl_->SetAutoRefresh(false);
  }

  /// @note amp and stress graph control has the same sample count for one page
  /// graph data. so we only need to get the sample count for one page graph
  /// data from the stress graph control.
  int32_t graph_sample_count_one_page =
      page_graph_stress_ctrl_->GetCurrentXDurationGraphSampleCount();
  int32_t data_sample_count_for_one_graph_sample =
      page_graph_stress_ctrl_->GetDataSampleCountForOneGraphSample();
  int32_t total_data_sample_count_one_page =
      graph_sample_count_one_page * data_sample_count_for_one_graph_sample;

  /// @note update the current no to the pre page no and update the graph data
  /// from the database and update the graph control. and update the pre page
  /// button status.
  if (exp_data_info_->exp_data_current_no_ < 1) {
    exp_data_info_->exp_data_current_no_ = 1;
  } else if (exp_data_info_->exp_data_current_no_ >
             total_data_sample_count_one_page) {
    exp_data_info_->exp_data_current_no_ -= total_data_sample_count_one_page;
  } else {
    exp_data_info_->exp_data_current_no_ = 1;
  }
  assert(exp_data_info_->exp_data_current_no_ % 10 == 1);
  int32_t id = exp_data_info_->exp_data_current_no_;
  if (id < 1) {
    assert(false);
    btn_pre_page_->SetEnabled(false);
    return false;
  }
  /// @note update next page button status false temporarily. will update it
  /// later.
  btn_next_page_->SetEnabled(false);
  bool enable_next_btn = true;
  int32_t no = id;
  LOG_F(LG_INFO) << "no: " << no
                 << " exp_data_info: " << exp_data_info_->ToString();
  /////////////////////////////////////////////////////////////////////////////
  /// @note clear current graph data and update the graph data from the exp_data
  /// of the database. and update the graph control.
  {
    /// @note query the data from the exp_data of the database and update the
    /// graph control.
    std::string sql_str = " SELECT* FROM ";
    sql_str += anx::db::helper::kTableExpData;
    sql_str += " WHERE ";
    sql_str += " id >= ";
    sql_str += std::to_string(id);
    sql_str += " AND";
    sql_str += " id <";
    sql_str += std::to_string(id + graph_sample_count_one_page *
                                       data_sample_count_for_one_graph_sample);
    sql_str += " ORDER BY date ASC";
    sql_str += ";";
    std::vector<std::map<std::string, std::string>> result;
    anx::db::helper::QueryDataBase(anx::db::helper::kDefaultDatabasePathname,
                                   anx::db::helper::kTableExpData, sql_str,
                                   &result);
    std::string id_str = (result.size() > 0) ? result[0]["id"] : "-1";
    int32_t id_result_zero = std::stoi(id_str);
    if (id_result_zero <= 1) {
      btn_pre_page_->SetEnabled(false);
    }
    /// @note calculate the next page button status. if the result size is less
    /// than the total data sample count for one page graph sample or the next
    /// page no is greater than the total data sample count for one page graph
    /// sample, then the next page button status is false.
    if (static_cast<int32_t>(result.size()) <
            graph_sample_count_one_page *
                data_sample_count_for_one_graph_sample ||
        (id + graph_sample_count_one_page *
                  data_sample_count_for_one_graph_sample) >=
            static_cast<int32_t>(exp_data_info_->exp_data_table_no_)) {
      enable_next_btn = false;
    }
    /// @note update the graph control with the data from the database.
    /// reverse result data to the element list
    std::vector<Element2DPoint> element_list;
    for (auto& item : result) {
      double x = std::stod(item["date"]) / kMultiFactor;
      double y = std::stod(item["um"]) / kMultiFactor;
      element_list.push_back(Element2DPoint(x, y));
    }
    page_graph_amplitude_ctrl_->Release();
    page_graph_amplitude_ctrl_.reset();
    CActiveXUI* activex = static_cast<CActiveXUI*>(
        paint_manager_ui_->FindControl(_T("graph_amplitude_canvas")));
    page_graph_amplitude_ctrl_.reset(
        new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
            graph_ctrl_event_.get(), activex, "amp",
            graphctrl_sample_total_minutes_, 15, 6, kYAxisAmpInitialValue,
            false));
    page_graph_amplitude_ctrl_->Init(element_list,
                                     data_sample_count_for_one_graph_sample);
  }
  /////////////////////////////////////////////////////////////////////////////
  /// @note clear current graph data and update the graph data from the exp_data
  /// of the database. and update the graph control.

  {
    /// @note query the data from the exp_data of the database and update the
    /// graph control.
    std::string sql_str = " SELECT* FROM ";
    sql_str += anx::db::helper::kTableExpData;
    sql_str += " WHERE ";
    sql_str += " id >= ";
    sql_str += std::to_string(id);
    sql_str += " AND";
    sql_str += " id <";
    sql_str += std::to_string(id + graph_sample_count_one_page *
                                       data_sample_count_for_one_graph_sample);
    sql_str += " ORDER BY date ASC";
    sql_str += ";";
    std::vector<std::map<std::string, std::string>> result;
    anx::db::helper::QueryDataBase(anx::db::helper::kDefaultDatabasePathname,
                                   anx::db::helper::kTableExpData, sql_str,
                                   &result);
    std::string id_str = (result.size() > 0) ? result[0]["id"] : "-1";
    int32_t id_result_zero = std::stoi(id_str);
    if (id_result_zero <= 1) {
      btn_pre_page_->SetEnabled(false);
    }
    /// @note calculate the next page button status. if the result size is less
    /// than the total data sample count for one page graph sample or the next
    /// page no is greater than the total data sample count for one page graph
    /// sample, then the next page button status is false.
    if (static_cast<int32_t>(result.size()) <
            graph_sample_count_one_page *
                data_sample_count_for_one_graph_sample ||
        (id + graph_sample_count_one_page *
                  data_sample_count_for_one_graph_sample) >=
            static_cast<int32_t>(exp_data_info_->exp_data_table_no_)) {
      enable_next_btn = false;
    }
    /// @note update the graph control with the data from the database.
    /// reverse result data to the element list.
    std::vector<Element2DPoint> element_list;
    for (auto& item : result) {
      double x = std::stod(item["date"]) / kMultiFactor;
      double y = std::stod(item["MPa"]) / kMultiFactor;
      element_list.push_back(Element2DPoint(x, y));
    }

    page_graph_stress_ctrl_->Release();
    page_graph_stress_ctrl_.reset();
    CActiveXUI* activex_stress = static_cast<CActiveXUI*>(
        paint_manager_ui_->FindControl(_T("graph_stress_canvas")));
    page_graph_stress_ctrl_.reset(
        new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
            graph_ctrl_event_.get(), activex_stress, "stress",
            graphctrl_sample_total_minutes_, 8, 6, kYAxisStressInitialValue,
            false));
    page_graph_stress_ctrl_->Init(element_list,
                                  data_sample_count_for_one_graph_sample);
  }
  if (enable_next_btn) {
    btn_next_page_->SetEnabled(true);
  }
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

  /// @note update auto refresh check box to false
  /// @note update the mode to 1 history view for the graph control
  chk_graph_always_show_new_->Selected(false);
  exp_data_info_->mode_ = 1;

  /// @note update the auto refresh of graph control to false
  if (page_graph_amplitude_ctrl_.get() != nullptr) {
    page_graph_amplitude_ctrl_->SetAutoRefresh(false);
  }
  if (page_graph_stress_ctrl_.get() != nullptr) {
    page_graph_stress_ctrl_->SetAutoRefresh(false);
  }

  /// @note amp and stress graph control has the same sample count for one page
  /// graph data. so we only need to get the sample count for one page graph
  /// data from the stress graph control.
  int32_t graph_sample_count_one_page =
      page_graph_stress_ctrl_->GetCurrentXDurationGraphSampleCount();
  int32_t data_sample_count_for_one_graph_sample =
      page_graph_stress_ctrl_->GetDataSampleCountForOneGraphSample();
  int32_t total_data_sample_count_one_page =
      graph_sample_count_one_page * data_sample_count_for_one_graph_sample;
  int32_t graph_sample_count = graph_sample_count_one_page;

  int32_t id = exp_data_info_->exp_data_current_no_;
  assert(id % 10 == 1);
  if ((id + total_data_sample_count_one_page) <=
      static_cast<int32_t>(exp_data_info_->exp_data_table_no_)) {
    /// @note next page has 1 more page data than the current page data.
    id += total_data_sample_count_one_page;
  } else if ((id + total_data_sample_count_one_page) >
             static_cast<int32_t>(exp_data_info_->exp_data_table_no_)) {
    /// @note current page is the last page data.
    btn_next_page_->SetEnabled(false);
  }
  // update pre page button status
  int32_t no = id;
  LOG_F(LG_INFO) << "no: " << no << " exp_data_current_no_: "
                 << exp_data_info_->exp_data_current_no_
                 << " total: " << exp_data_info_->exp_data_table_no_;

  /////////////////////////////////////////////////////////////////////////////
  /// @note clear current graph data and update the graph data from the
  /// exp_data of the database. and update the graph control.
  {
    /// @note query the data from the exp_data of the database and update the
    /// graph control.
    std::string sql_str = " SELECT* FROM ";
    sql_str += anx::db::helper::kTableExpData;
    sql_str += " WHERE ";
    sql_str += " id >= ";
    sql_str += std::to_string(id);
    sql_str += " AND";
    sql_str += " id <";
    sql_str += std::to_string(id + graph_sample_count *
                                       data_sample_count_for_one_graph_sample);
    sql_str += " ORDER BY date ASC";
    sql_str += ";";
    std::vector<std::map<std::string, std::string>> result;
    anx::db::helper::QueryDataBase(anx::db::helper::kDefaultDatabasePathname,
                                   anx::db::helper::kTableExpData, sql_str,
                                   &result);
    if (result.size() == total_data_sample_count_one_page) {
      exp_data_info_->exp_data_current_no_ = id;
      if ((static_cast<int32_t>(exp_data_info_->exp_data_table_no_) -
           exp_data_info_->exp_data_current_no_) > 0) {
        btn_next_page_->SetEnabled(true);
        btn_pre_page_->SetEnabled(true);
      } else {
        btn_next_page_->SetEnabled(false);
        btn_pre_page_->SetEnabled(true);
      }
    } else {
      exp_data_info_->exp_data_current_no_ =
          ((id + result.size()) / total_data_sample_count_one_page *
           total_data_sample_count_one_page) +
          1;
      btn_next_page_->SetEnabled(false);
      chk_graph_always_show_new_->SetCheck(true);
      exp_data_info_->mode_ = 0;
      btn_pre_page_->SetEnabled(true);
    }
    /// @note update the graph control with the data from the database.
    /// reverse result data to the element list
    std::vector<Element2DPoint> element_list;
    for (auto& item : result) {
      double x = std::stod(item["date"]) / kMultiFactor;
      double y = std::stod(item["um"]) / kMultiFactor;
      element_list.push_back(Element2DPoint(x, y));
    }

    page_graph_amplitude_ctrl_->Release();
    page_graph_amplitude_ctrl_.reset();
    CActiveXUI* activex = static_cast<CActiveXUI*>(
        paint_manager_ui_->FindControl(_T("graph_amplitude_canvas")));
    page_graph_amplitude_ctrl_.reset(
        new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
            graph_ctrl_event_.get(), activex, "amp",
            graphctrl_sample_total_minutes_, 15, 6, kYAxisAmpInitialValue,
            false));
    page_graph_amplitude_ctrl_->Init(element_list,
                                     data_sample_count_for_one_graph_sample);
  }
  /////////////////////////////////////////////////////////////////////////////
  /// @note clear current graph data and update the graph data from the
  /// exp_data of the database. and update the graph control.
  {
    /// @note query the data from the exp_data of the database and update the
    /// graph control.
    std::string sql_str = " SELECT* FROM ";
    sql_str += anx::db::helper::kTableExpData;
    sql_str += " WHERE ";
    sql_str += " id >= ";
    sql_str += std::to_string(id);
    sql_str += " AND";
    sql_str += " id <";
    sql_str += std::to_string(id + graph_sample_count_one_page *
                                       data_sample_count_for_one_graph_sample);
    sql_str += " ORDER BY date ASC";
    sql_str += ";";
    std::vector<std::map<std::string, std::string>> result;
    anx::db::helper::QueryDataBase(anx::db::helper::kDefaultDatabasePathname,
                                   anx::db::helper::kTableExpData, sql_str,
                                   &result);
    // TODO(hhool): deal with more
    if (result.size() < 0) {
      btn_next_page_->SetEnabled(false);
      return false;
    }
    std::string id_str = (result.size() > 0) ? result[0]["id"] : "-1";
    int32_t id_result_zero = std::stoi(id_str);
    if (id_result_zero >=
        static_cast<int32_t>(exp_data_info_->exp_data_table_no_)) {
      btn_next_page_->SetEnabled(false);
    }
    /// @note update the graph control with the data from the database.
    /// reverse result data to the element list.
    std::vector<Element2DPoint> element_list;
    for (auto& item : result) {
      double x = std::stod(item["date"]) / kMultiFactor;
      double y = std::stod(item["MPa"]) / kMultiFactor;
      element_list.push_back(Element2DPoint(x, y));
    }

    page_graph_stress_ctrl_->Release();
    page_graph_stress_ctrl_.reset();
    CActiveXUI* activex_stress = static_cast<CActiveXUI*>(
        paint_manager_ui_->FindControl(_T("graph_stress_canvas")));
    page_graph_stress_ctrl_.reset(
        new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
            graph_ctrl_event_.get(), activex_stress, "stress",
            graphctrl_sample_total_minutes_, 8, 6, kYAxisStressInitialValue,
            exp_data_info_->mode_ ? false : true));
    page_graph_stress_ctrl_->Init(element_list,
                                  data_sample_count_for_one_graph_sample);
  }
  return true;
}

bool WorkWindowSecondPageGraph::OnTimer(void* param) {
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(param);
  if (pMsg == nullptr) {
    return false;
  }
  if (pMsg->pSender != btn_graph_amplitude_title_) {
    return false;
  }
  if (pMsg->wParam == kTimerGraphId) {
    // update the graph
    RefreshExpGraphTitelControl();
    RefreshPreNextControl();
    return true;
  } else {
    // TODO(hhool): do nothing;
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
  exp_data_info_->mode_ = 0;
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
  {
    CActiveXUI* activex = static_cast<CActiveXUI*>(
        paint_manager_ui_->FindControl(_T("graph_amplitude_canvas")));
    page_graph_amplitude_ctrl_.reset(
        new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
            graph_ctrl_event_.get(), activex, "amp",
            graphctrl_sample_total_minutes_, 15, 6, kYAxisAmpInitialValue,
            exp_data_info_->mode_ ? false : true));
    page_graph_amplitude_ctrl_->Init(std::vector<Element2DPoint>());
  }
  {
    CActiveXUI* activex = static_cast<CActiveXUI*>(
        paint_manager_ui_->FindControl(_T("graph_stress_canvas")));
    page_graph_stress_ctrl_.reset(
        new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
            graph_ctrl_event_.get(), activex, "stress",
            graphctrl_sample_total_minutes_, 8, 6, kYAxisStressInitialValue,
            exp_data_info_->mode_ ? false : true));
    page_graph_stress_ctrl_->Init(std::vector<Element2DPoint>());
  }
  btn_graph_amplitude_title_->OnNotify +=
      ::MakeDelegate(this, &WorkWindowSecondPageGraph::OnTimer);
}

void WorkWindowSecondPageGraph::Unbind() {
  SaveSettingsFromControl();

  paint_manager_ui_->KillTimer(btn_graph_amplitude_title_, kTimerGraphId);
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

void WorkWindowSecondPageGraph::CheckDeviceComConnectedStatus() {}

void WorkWindowSecondPageGraph::RefreshExpGraphTitelControl() {
  // get current system time get current hour and minute
  struct tm timeinfo;
  anx::common::GetLocalTime(&timeinfo);

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

void WorkWindowSecondPageGraph::RefreshPreNextControl() {
  if (page_graph_amplitude_ctrl_ != nullptr) {
    double var_time_amp_current_x_min =
        page_graph_amplitude_ctrl_->GetCurrentXMin();
    double diff = var_time_amp_current_x_min - amp_start_time_;
    if (!exp_data_info_->mode_) {
      if (diff > 0) {
        btn_pre_page_->SetEnabled(true);
      } else {
        btn_pre_page_->SetEnabled(false);
      }
    }
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

  RefreshExpGraphTitelControl();
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
  RefreshExpGraphTitelControl();
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
  RefreshExpGraphTitelControl();
  exp_time_interval_num_ = 0;

  paint_manager_ui_->SetTimer(btn_graph_amplitude_title_, kTimerGraphId,
                              kTimerGraphIdPeriod);
  {
    CActiveXUI* activex = static_cast<CActiveXUI*>(
        paint_manager_ui_->FindControl(_T("graph_amplitude_canvas")));
    page_graph_amplitude_ctrl_.reset(
        new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
            graph_ctrl_event_.get(), activex, "amp",
            graphctrl_sample_total_minutes_, 15, 6, kYAxisAmpInitialValue,
            exp_data_info_->mode_ ? false : true));
    page_graph_amplitude_ctrl_->Init(std::vector<Element2DPoint>());
    amp_start_time_ = page_graph_amplitude_ctrl_->GetCurrentXMin();
  }
  {
    CActiveXUI* activex = static_cast<CActiveXUI*>(
        paint_manager_ui_->FindControl(_T("graph_stress_canvas")));
    page_graph_stress_ctrl_.reset(
        new WorkWindowSecondWorkWindowSecondPageGraphCtrl(
            graph_ctrl_event_.get(), activex, "stress",
            graphctrl_sample_total_minutes_, 8, 6, kYAxisStressInitialValue,
            exp_data_info_->mode_ ? false : true));
    page_graph_stress_ctrl_->Init(std::vector<Element2DPoint>());
    stress_start_time_ = page_graph_stress_ctrl_->GetCurrentXMin();
  }
}

void WorkWindowSecondPageGraph::OnExpStop() {
  paint_manager_ui_->KillTimer(btn_graph_amplitude_title_, kTimerGraphId);
  exp_time_interval_num_ = 0;
}

void WorkWindowSecondPageGraph::OnExpPause() {
  paint_manager_ui_->KillTimer(btn_graph_amplitude_title_, kTimerGraphId);
}

void WorkWindowSecondPageGraph::OnExpResume() {
  paint_manager_ui_->SetTimer(btn_graph_amplitude_title_, kTimerGraphId,
                              kTimerGraphIdPeriod);
}

}  // namespace ui
}  // namespace anx
