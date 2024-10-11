/**
 * @file work_window_tab_main_second_page_graph_ctrl.cc
 * @author hhool (hhool@outlook.com)
 * @brief work window tab main second page graph control class implementation
 * @version 0.1
 * @date 2024-08-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/work_window_tab_main_second_page_graph_ctrl.h"

#include <cfloat>
#include <string>

#include "app/common/logger.h"
#include "app/common/string_utils.h"
#include "app/common/time_utils.h"
#include "app/ui/ui_constants.h"
#include "app/ui/work_window_tab_main_second_page_base.h"

namespace anx {
namespace ui {
#define CHECK_HROBJ(hr, obj) \
  if (FAILED(hr)) {          \
    return;                  \
  } else {                   \
  }                          \
  if (obj == nullptr) {      \
    return;                  \
  } else {                   \
  }

#define CHECK_HROBJ_RETURN(hr, obj, ret) \
  if (FAILED(hr)) {                      \
    return ret;                          \
  } else {                               \
  }                                      \
  if (obj == nullptr) {                  \
    return ret;                          \
  } else {                               \
  }

#define SAFE_RELEASE(p) \
  if (p != nullptr) {   \
    p->Release();       \
    p = nullptr;        \
  }

////////////////////////////////////////////////////////////////////////////////
const double kVarTimeDuration1min = 1.0f / (24.0 * 60.0);
const double kVarTimeDuration5min = 5.0f / (24.0 * 60.0);
const double kVarTimeDuration10min = 10.0f / (24.0 * 60.0);
const double kVarTimeDuration30min = 30.0f / (24.0 * 60.0);
const double kVarTimeDuration60min = 60.0f / (24.0 * 60.0);

const double kVartime2Seconds = 2.3148148148148147e-05;
const double kVartime4Seconds = 4.6296296296296294e-05;
const double kVartime6Seconds = 6.944444444444444e-05;
const double kVartime8Seconds = 9.2592592592592588e-05;
const double kVartime10Seconds = 1.1574074074074075e-04;
////////////////////////////////////////////////////////////////////////////////
namespace {
/// @brief data sampling interval value 12 second is one data sample.
/// @note 12 second is one data sample for the graph control. used for the
/// @note graph control data sample incoming. @see data_sampling_interval_ for
/// @note the data sample incoming. @see data_sampling_interval_var_ for the
/// @note data sample
static const double data_sampling_interval_var_ = kVartime10Seconds;

int32_t plot_point_size_with_sampling_total_minutes(
    int32_t sampling_total_minutes) {
  int32_t point_size = 5;
  if (sampling_total_minutes == 1) {
    point_size = 5;
  } else if (sampling_total_minutes == 5) {
    point_size = 4;
  } else if (sampling_total_minutes == 10) {
    point_size = 3;
  } else if (sampling_total_minutes == 30) {
    point_size = 2;
  } else if (sampling_total_minutes == 60) {
    point_size = 1;
  }
  return point_size;
}

}  // namespace

double minutes_to_vartime(int32_t minutes) {
  double duration = kVarTimeDuration5min;
  if (minutes == 1) {
    duration = kVarTimeDuration1min;
  } else if (minutes == 5) {
    duration = kVarTimeDuration5min;
  } else if (minutes == 10) {
    duration = kVarTimeDuration10min;
  } else if (minutes == 30) {
    duration = kVarTimeDuration30min;
  } else if (minutes == 60) {
    duration = kVarTimeDuration60min;
  }
  return duration;
}

uint32_t vartime_to_minutes(double duration) {
  uint32_t sampling_total_minutes = 5;
  if (std::fabs(duration - kVarTimeDuration1min) < DBL_EPSILON) {
    sampling_total_minutes = 1;
  } else if (std::fabs(duration - kVarTimeDuration5min) < DBL_EPSILON) {
    sampling_total_minutes = 5;
  } else if (std::fabs(duration - kVarTimeDuration10min) < DBL_EPSILON) {
    sampling_total_minutes = 10;
  } else if (std::fabs(duration - kVarTimeDuration30min) < DBL_EPSILON) {
    sampling_total_minutes = 30;
  } else if (std::fabs(duration - kVarTimeDuration60min) < DBL_EPSILON) {
    sampling_total_minutes = 60;
  }
  return sampling_total_minutes;
}

uint32_t minutes_to_data_sample_count(int32_t minutes) {
  uint32_t data_sample_count = 1;
  if (minutes == 1) {
    data_sample_count = 5 * 6 * minutes;
  } else if (minutes == 5) {
    data_sample_count = 5 * 6 * minutes;
  } else if (minutes == 10) {
    data_sample_count = 5 * 6 * minutes;
  } else if (minutes == 30) {
    data_sample_count = 5 * 6 * minutes;
  } else if (minutes == 60) {
    data_sample_count = 5 * 6 * minutes;
  }
  return data_sample_count;
}

uint32_t minutes_to_graph_sample_count(int32_t minutes) {
  uint32_t graph_sample_count = 1;
  if (minutes == 1) {
    graph_sample_count = 6 * minutes;
  } else if (minutes == 5) {
    graph_sample_count = 6 * minutes;
  } else if (minutes == 10) {
    graph_sample_count = 6 * minutes;
  } else if (minutes == 30) {
    graph_sample_count = 6 * minutes;
  } else if (minutes == 60) {
    graph_sample_count = 6 * minutes;
  }
  return graph_sample_count;
}

WorkWindowSecondWorkWindowSecondPageGraphCtrl::
    WorkWindowSecondWorkWindowSecondPageGraphCtrl() {}
WorkWindowSecondWorkWindowSecondPageGraphCtrl::
    WorkWindowSecondWorkWindowSecondPageGraphCtrl(
        GraphCtrlEventInterface* event_interface,
        CActiveXUI* activex,
        const std::string name,
        double x_min,
        double x_duration,
        double y_min,
        double y_max,
        bool auto_refresh)
    : event_interface_(event_interface),
      activex_(activex),
      name_(name),
      x_min_(x_min),
      x_duration_(x_duration),
      y_min_(y_min),
      y_max_(y_max),
      auto_refresh_(auto_refresh) {
  y_grid_step_ = 10;
  x_grid_step_ = 6;
  y_axis_intial_value_ = y_max / 100 * 0.3;
  sampling_total_minutes_ = vartime_to_minutes(x_duration);
  data_sample_count_for_one_graph_sample_ = 5;
  data_sample_incoming_first_time_ = -1.0f;
}

WorkWindowSecondWorkWindowSecondPageGraphCtrl::
    ~WorkWindowSecondWorkWindowSecondPageGraphCtrl() {
  Release();
  LOG_F(LG_INFO);
}

void WorkWindowSecondWorkWindowSecondPageGraphCtrl::SetAutoRefresh(
    bool auto_refresh) {
  auto_refresh_ = auto_refresh;
}

void WorkWindowSecondWorkWindowSecondPageGraphCtrl::Init(
    std::vector<Element2DPoint> element_list) {
  HRESULT hr = activex_->GetControl(IID_IUnknown,
                                    reinterpret_cast<void**>(&graph_ctrl_));
  CHECK_HROBJ(hr, graph_ctrl_);
  graph_ctrl_->ClearGraph();

  ///////////////////////////////////////////////////////////////////////////
  hr = S_OK;
  CHECK_HROBJ(hr, graph_ctrl_);
  FrameStyle style = FrameStyle::Scope;
  graph_ctrl_->put_FrameStyle(style);
  graph_ctrl_->put_TrackMode(TrackModeState::TrackElement);
  IDMGraphAxis* x_spXAxis = nullptr;
  hr = graph_ctrl_->get_Axis(Orientation::HorizontalX, &x_spXAxis);
  CHECK_HROBJ(hr, x_spXAxis);

  hr = x_spXAxis->put_Format(L"%M:%S");
  CHECK_HROBJ(hr, x_spXAxis);
  x_spXAxis->put_Time(VARIANT_TRUE);
  x_spXAxis->put_GridNumber(x_grid_step_);
  x_spXAxis->Release();

  IDMGraphAxis* y_spXAxis = nullptr;
  hr = graph_ctrl_->get_Axis(Orientation::VerticalY, &y_spXAxis);
  CHECK_HROBJ(hr, y_spXAxis);
  hr = y_spXAxis->put_GridNumber(y_grid_step_);
  y_spXAxis->Release();

  hr = graph_ctrl_->put_LabelColor(RGB(0, 0, 0));
  hr = graph_ctrl_->get_Elements(&spElements_);
  CHECK_HROBJ(hr, spElements_);
  hr = spElements_->Add(&spDispatch_);
  CHECK_HROBJ(hr, spDispatch_);
  hr = spDispatch_->QueryInterface(&spGraphElement_);
  CHECK_HROBJ(hr, spGraphElement_);
  hr = spGraphElement_->put_Name(
      (BSTR)anx::common::string2wstring(name_).c_str());

  hr = spGraphElement_->put_LineType(LineType::XYStep);
  hr = spGraphElement_->put_PointSymbol(SymbolType::Dots);
  int point_size =
      plot_point_size_with_sampling_total_minutes(sampling_total_minutes_);
  hr = spGraphElement_->put_PointSize(point_size);

  ///////////////////////////////////////////////////////////////////////////
  /// @note plot the element list to the graph element.
  /// @note calculate the element start index, end index, and start x axis
  /// @note value
  int32_t start_index =
      element_list.size() - (sampling_total_minutes_ * (x_grid_step_)) *
                                data_sample_count_for_one_graph_sample_;
  int32_t end_index = element_list.size();
  if (start_index <= 0) {
    start_index = 0;
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @note Update plot graph x axis time.
  LOG_F_INFO_TAG(name_) << " " << "x_min:" << x_min_ << " " << "SetRange";
  graph_ctrl_->SetRange(x_min_, x_min_ + x_duration_, y_min_, y_max_);
  ///////////////////////////////////////////////////////////////////////////
  /// @note the real plot index is 2 for the graph element.
  /// @note 0 and 1 is the x and y axis.
  /// @note plot the x and y axis
  spGraphElement_->PlotXY(x_min_, 0);
  spGraphElement_->PlotXY(x_min_, static_cast<double>(y_grid_step_));
  spGraphElement_->put_Show(true);

  /// @note plot the x axis time value.
  /// @note intial point 6 plot point for 1 minute
  /// @note intial point 30 plot point for 5 minute
  /// @note intial point 60 plot point for 10 minute
  /// @note intial point 180 plot point for 30 minute
  /// @note intial point 360 plot point for 60 minute
  for (int i = 0; i < sampling_total_minutes_ * x_grid_step_; i++) {
    spGraphElement_->PlotXY(x_min_ + data_sampling_interval_var_ * (i),
                            y_axis_intial_value_);
  }
#if 0
  // dump the x and y value of the graph element.
  DumpGraphDataList();
  // dump element list
  for (uint32_t i = 0; i < element_list.size(); i++) {
    LOG_F_INFO_TAG(name_) << "element:" << i << " "
                          << "x:" << element_list[i].x_ << " "
                          << "y:" << element_list[i].y_ << " "
                          << "no:" << element_list[i].no_;
  }
#endif
  plot_index_ = plot_graph_x_axis_reserve_count_for_y_axis_;
  double y_value = 0.0f;
  int32_t data_sample_index = 1;
  for (int32_t i = start_index; i < end_index; i++) {
    if ((data_sample_index % data_sample_count_for_one_graph_sample_) != 0) {
      y_value += element_list[i].y_;
      data_sample_index++;
      continue;
    } else {
      y_value += element_list[i].y_;
    }
    LOG_F_INFO_TAG(name_)
        << "x:"
        << element_list[((plot_index_ -
                          plot_graph_x_axis_reserve_count_for_y_axis_) *
                         data_sample_count_for_one_graph_sample_)]
               .x_
        << " "
        << "y:" << (y_value / data_sample_index) << " "
        << "plot_index:" << plot_index_ << " "
        << "data_sample_index:" << data_sample_index;
    spGraphElement_->put_YValue(plot_index_, y_value / data_sample_index);
    plot_index_++;

    y_value = 0.0f;
    data_sample_index = 1;
  }

  data_sample_incoming_count_ = 0;
  data_sample_incoming_value_total_ = 0;
  LOG_F(LG_INFO) << "data_sample_index:" << data_sample_index << " "
                 << "y_value:" << y_value;
  if (data_sample_index > 1) {
    data_sample_incoming_count_ = data_sample_index - 1;
    data_sample_incoming_value_total_ = y_value;
    LOG_F(LG_INFO) << "data_sample_incoming_count_:"
                   << data_sample_incoming_count_ << " "
                   << "data_sample_incoming_value_total_:"
                   << data_sample_incoming_value_total_;
  }
}

void WorkWindowSecondWorkWindowSecondPageGraphCtrl::ProcessDataSampleIncoming(
    double sample) {
  if (!auto_refresh_)
    return;
  if (data_sample_incoming_first_time_ < 0) {
    data_sample_incoming_first_time_ = 1;
    LOG_F(LG_INFO) << "data_sample_incoming_count_:"
                   << data_sample_incoming_count_ << "  "
                   << "data_sample_incoming_value_total_:"
                   << data_sample_incoming_value_total_;
  }
  /// @note 100 ms is one sample, 120 sample is 12 second, 120 sample is
  /// one data sample, take average of the 120 sample as one data sample
  /// and put it to the graph.
  data_sample_incoming_count_++;
  data_sample_incoming_value_total_ += sample;
  /// @todo 100 ms is one sample, 120 sample is 12 second, 120 sample is
  /// one data sample, take average of the 120 sample as one data sample
  /// and put it to the graph.
  if (data_sample_incoming_count_ % data_sample_count_for_one_graph_sample_ ==
      0) {
    double average =
        data_sample_incoming_value_total_ / (data_sample_incoming_count_);

    int32_t plot_graph_sample_index =
        plot_index_ - plot_graph_x_axis_reserve_count_for_y_axis_;
    if (plot_graph_sample_index <
        (sampling_total_minutes_ * (this->x_grid_step_))) {
      LOG_F_INFO_TAG(name_) << " " << "plot_index:" << plot_index_;
      spGraphElement_->put_YValue(plot_index_, average);
    }
    plot_index_++;
    LOG_F_INFO_TAG(name_) << " " << "value:" << average << " "
                          << "data_sample_incoming_value_total_:"
                          << data_sample_incoming_value_total_ << " "
                          << "data_sample_incoming_count_:"
                          << data_sample_incoming_count_ << " "
                          << "plot_graph_sample_index:"
                          << plot_graph_sample_index << " "
                          << "sampling_total_minutes_*x_grid_step:"
                          << (sampling_total_minutes_ * (this->x_grid_step_));

    if (plot_graph_sample_index > 0 &&
        plot_graph_sample_index %
                (sampling_total_minutes_ * (this->x_grid_step_ - 1)) ==
            0) {
#if 0
      DumpGraphDataList();
#endif
      ///////////////////////////////////////////////
      /// @note Update plot graph x axis time.
      this->UpateXTimeValue();
      LOG_F(LG_INFO) << "\r\n=================================================="
                        "======================================================"
                        "=============="
                        "======================================================"
                        "==========\r\n";
      LOG_F_INFO_TAG(name_) << " " << "x_min:" << x_min_ << " " << "SetRange";
      graph_ctrl_->SetRange(x_min_, x_min_ + x_duration_, y_min_, y_max_);
      ///////////////////////////////////////////////
      /// @note Reset value after plot_index and x axis time to new x_min_;
      for (int32_t i = plot_graph_x_axis_reserve_count_for_y_axis_;
           i < sampling_total_minutes_ * x_grid_step_ +
                   plot_graph_x_axis_reserve_count_for_y_axis_;
           i++) {
        spGraphElement_->put_YValue(i, y_axis_intial_value_);
        spGraphElement_->put_XValue(
            i, x_min_ + data_sampling_interval_var_ *
                            (i - plot_graph_x_axis_reserve_count_for_y_axis_));
      }
      /// @note Reset the plot index to 2
      plot_index_ = plot_graph_x_axis_reserve_count_for_y_axis_;
      ///////////////////////////////////////////////
      if (event_interface_ != nullptr) {
        event_interface_->OnGraphCtrlEvent(this, 1);
      }
    }

    data_sample_incoming_value_total_ = 0;
    data_sample_incoming_count_ = 0;
  }
}

void WorkWindowSecondWorkWindowSecondPageGraphCtrl::Release() {
  if (graph_ctrl_ != nullptr) {
#if 0
    DumpGraphDataList();
#endif
    graph_ctrl_->ClearGraph();
  }
  SAFE_RELEASE(spGraphElement_);
  SAFE_RELEASE(spDispatch_);
  SAFE_RELEASE(spElements_);
  SAFE_RELEASE(graph_ctrl_);
}

double WorkWindowSecondWorkWindowSecondPageGraphCtrl::GetXMinOfAxis() const {
  LOG_F_INFO_TAG(name_) << " " << "x_min:" << x_min_;
  return x_min_;
}

double WorkWindowSecondWorkWindowSecondPageGraphCtrl::GetXDurationOfAxis()
    const {
  LOG_F_INFO_TAG(name_) << " " << "x_min:" << x_min_;
  return x_duration_;
}

void WorkWindowSecondWorkWindowSecondPageGraphCtrl::UpateXTimeValue() {
  x_min_ = anx::common::GetCurrrentDateTime();
  LOG_F_INFO_TAG(name_) << " " << "x_min:" << x_min_;
}

void WorkWindowSecondWorkWindowSecondPageGraphCtrl::DumpGraphDataList() {
  for (int i = plot_graph_x_axis_reserve_count_for_y_axis_;
       i < sampling_total_minutes_ * x_grid_step_ +
               plot_graph_x_axis_reserve_count_for_y_axis_;
       i++) {
    double x = 0, y = 0;
    spGraphElement_->get_XValue(i, &x);
    spGraphElement_->get_YValue(i, &y);
    LOG_F_INFO_TAG(name_) << "plot:" << i << " " << "x:" << x << " "
                          << "y:" << y;
  }
}

}  // namespace ui
}  // namespace anx
