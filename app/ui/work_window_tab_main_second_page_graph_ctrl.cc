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
////////////////////////////////////////////////////////////////////////////////
namespace {
static const double var_time_2_seconds_ = 2.3148148148148147e-05;
static const double var_time_4_seconds_ = 4.6296296296296294e-05;
static const double var_time_6_seconds_ = 6.944444444444444e-05;
static const double var_time_8_seconds_ = 9.2592592592592588e-05;
static const double var_time_10_seconds_ = 1.1574074074074075e-04;
/// @brief data sampling interval value 12 second is one data sample.
/// @note 12 second is one data sample for the graph control. used for the
/// @note graph control data sample incoming. @see data_sampling_interval_ for
/// @note the data sample incoming. @see data_sampling_interval_var_ for the
/// @note data sample
static const double data_sampling_interval_var_ = var_time_10_seconds_;

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

double duration_with_sampling_total_minutes(int32_t sampling_total_minutes) {
  double duration = kVarTimeDuration5min;
  if (sampling_total_minutes == 1) {
    duration = kVarTimeDuration1min;
  } else if (sampling_total_minutes == 5) {
    duration = kVarTimeDuration5min;
  } else if (sampling_total_minutes == 10) {
    duration = kVarTimeDuration10min;
  } else if (sampling_total_minutes == 30) {
    duration = kVarTimeDuration30min;
  } else if (sampling_total_minutes == 60) {
    duration = kVarTimeDuration60min;
  }
  return duration;
}
}  // namespace

WorkWindowSecondWorkWindowSecondPageGraphCtrl::
    WorkWindowSecondWorkWindowSecondPageGraphCtrl() {}
WorkWindowSecondWorkWindowSecondPageGraphCtrl::
    WorkWindowSecondWorkWindowSecondPageGraphCtrl(
        GraphCtrlEventInterface* event_interface,
        CActiveXUI* activex,
        const std::string name,
        int32_t sampling_total_minutes,
        int32_t y_grid_step,
        int32_t x_grid_step,
        double y_axis_intial_value,
        bool auto_refresh)
    : event_interface_(event_interface),
      activex_(activex),
      name_(name),
      sampling_total_minutes_(sampling_total_minutes),
      y_grid_step_(y_grid_step),
      x_grid_step_(x_grid_step),
      y_axis_intial_value_(y_axis_intial_value),
	  auto_refresh_(auto_refresh) {
  data_sample_count_for_one_graph_sample_ = 5;
  x_duration_ = duration_with_sampling_total_minutes(sampling_total_minutes_);
  y_min_ = 0.0f;
  y_max_ = y_grid_step * 1.0f;
}

WorkWindowSecondWorkWindowSecondPageGraphCtrl::
    ~WorkWindowSecondWorkWindowSecondPageGraphCtrl() {
  Release();
}

int32_t WorkWindowSecondWorkWindowSecondPageGraphCtrl::GetSamplingTotalMinutes()
    const {
  return sampling_total_minutes_;
}

std::vector<Element2DPoint>
WorkWindowSecondWorkWindowSecondPageGraphCtrl::GetGraphDataList() {
  IDMGraphCtrl* graph_ctrl = nullptr;
  HRESULT hr =
      activex_->GetControl(IID_IUnknown, reinterpret_cast<void**>(&graph_ctrl));
  CHECK_HROBJ_RETURN(hr, graph_ctrl, std::vector<Element2DPoint>());
  std::vector<Element2DPoint> element_list;

  IDMGraphCollection* spElements = nullptr;
  hr = graph_ctrl->get_Elements(&spElements);
  CHECK_HROBJ_RETURN(hr, spElements, std::vector<Element2DPoint>());
  IDispatch* spDispatch = nullptr;
  hr = spElements->get_Item(0, &spDispatch);
  if (SUCCEEDED(hr)) {
    if (spDispatch != nullptr) {
      CHECK_HROBJ_RETURN(hr, spDispatch, std::vector<Element2DPoint>());
      IDMGraphElement* spGraphElement = nullptr;
      hr = spDispatch->QueryInterface(&spGraphElement);
      CHECK_HROBJ_RETURN(hr, spGraphElement, std::vector<Element2DPoint>());
      int32_t plot_num = 0;
      spGraphElement->get_Count(reinterpret_cast<long*>(&plot_num));  // NOLINT
      /// @note copy the plot elements to the new plot elements.
      for (int32_t i = plot_resvered_index_for_y_axis_; i < plot_index_; i++) {
        double x = 0, y = 0;
        spGraphElement->get_XValue(i, &x);
        spGraphElement->get_YValue(i, &y);
        /// @note store the x and y value to the array list.
        element_list.push_back(Element2DPoint(x, y));
      }
      SAFE_RELEASE(spGraphElement);
      SAFE_RELEASE(spDispatch);
    }
  }
  SAFE_RELEASE(spElements);
  SAFE_RELEASE(graph_ctrl);
  return element_list;
}

void WorkWindowSecondWorkWindowSecondPageGraphCtrl::SetAutoRefresh(
    bool auto_refresh) {
  auto_refresh_ = auto_refresh;
}

void WorkWindowSecondWorkWindowSecondPageGraphCtrl::Init(
    std::vector<Element2DPoint> element_list,
    int32_t data_sample_count_for_one_graph_sample) {
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
  double x_min_axis_value = 0.0f;
  bool has_x_min_axis_value = false;
  int32_t start_index =
      element_list.size() - (sampling_total_minutes_ * x_grid_step_) *
                                data_sample_count_for_one_graph_sample;
  int32_t end_index = element_list.size();
  if (start_index <= 0) {
    start_index = 0;
    if (end_index > 0) {
      has_x_min_axis_value = true;
      x_min_axis_value = element_list[start_index].x_;
      if (data_sample_count_for_one_graph_sample > 1) {
        x_min_axis_value -= var_time_2_seconds_;
      }
    }
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @note Update plot graph x axis time.
  if (!has_x_min_axis_value) {
    this->UpateXTimeValue();
    LOG_F_INFO_TAG(name_) << " " << "x_min:" << x_min_;
  } else {
    x_min_ = x_min_axis_value;
    LOG_F_INFO_TAG(name_) << " " << "x_min:" << x_min_;
  }
  LOG_F_INFO_TAG(name_) << " " << "x_min:" << x_min_ << " " << "SetRange";
  graph_ctrl_->SetRange(x_min_, x_min_ + x_duration_, y_min_, y_max_);
  ///////////////////////////////////////////////////////////////////////////
  /// @note the real plot index is 2 for the graph element.
  /// @note 0 and 1 is the x and y axis.
  /// @note plot the x and y axis
  spGraphElement_->PlotXY(x_min_, 0);
  spGraphElement_->PlotXY(x_min_, static_cast<double>(y_grid_step_));
  spGraphElement_->put_Show(true);

  /// @note intial point 25 plot point for 5 minute
  /// @note intial point 50 plot point for 10 minute
  /// @note intial point 150 plot point for 30 minute
  /// @note intial point 300 plot point for 60 minute
  for (int i = 0; i < sampling_total_minutes_ * x_grid_step_; i++) {
    spGraphElement_->PlotXY(x_min_ + data_sampling_interval_var_ * (i),
                            y_axis_intial_value_);
  }
#if 1
  // dump the x and y value of the graph element.
  DumpGraphDataList();
  // dump element list
  for (uint32_t i = 0; i < element_list.size(); i++) {
    LOG_F_INFO_TAG(name_) << "element:" << i << " "
                          << "x:" << element_list[i].x_ << " "
                          << "y:" << element_list[i].y_;
  }
#endif
  plot_index_ = plot_resvered_index_for_y_axis_;
  double y_value = 0.0f;
  int32_t data_sample_index = 1;
  for (int32_t i = start_index; i < end_index; i++) {
    if ((data_sample_index % data_sample_count_for_one_graph_sample) != 0) {
      y_value += element_list[i].y_;
      data_sample_index++;
      continue;
    } else {
      y_value += element_list[i].y_;
    }
    LOG_F_INFO_TAG(name_)
        << "x:"
        << element_list[((plot_index_ - plot_resvered_index_for_y_axis_) *
                         data_sample_count_for_one_graph_sample)]
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
}

void WorkWindowSecondWorkWindowSecondPageGraphCtrl::ProcessDataSampleIncoming(
    double sample) {
  if (!auto_refresh_)
    return;
  /// @note 100 ms is one sample, 120 sample is 12 second, 120 sample is
  /// one data sample, take average of the 120 sample as one data sample
  /// and put it to the graph.
  data_sample_incoming_count_++;
  data_sample_incoming_value_total_ += sample;
  /// @todo 100 ms is one sample, 120 sample is 12 second, 120 sample is
  /// one data sample, take average of the 120 sample as one data sample
  /// and put it to the graph.
  /// TODO(hhool): will be removed after the real data incoming.
  if (data_sample_incoming_count_ % data_sample_count_for_one_graph_sample_ ==
      0) {
    double average =
        data_sample_incoming_value_total_ / (data_sample_incoming_count_);
    double average_n = average;
    average = average_n / kMultiFactor;
    LOG_F_INFO_TAG(name_) << " " << "value:" << average;

    int32_t plot_data_sample_index =
        plot_index_ - plot_resvered_index_for_y_axis_;
    if (plot_data_sample_index > 0 &&
        plot_data_sample_index %
                (sampling_total_minutes_ * this->x_grid_step_) ==
            0) {
#if 1
      DumpGraphDataList();
#endif
      ///////////////////////////////////////////////
      /// @note Update plot graph x axis time.
      // graph_ctrl_->AutoRange();
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
      for (int32_t i = plot_resvered_index_for_y_axis_;
           i < sampling_total_minutes_ * x_grid_step_ +
                   plot_resvered_index_for_y_axis_;
           i++) {
        spGraphElement_->put_YValue(i, y_axis_intial_value_);
        spGraphElement_->put_XValue(
            i, x_min_ + data_sampling_interval_var_ *
                            (i - plot_resvered_index_for_y_axis_));
      }
      /// @note Reset the plot index to 2
      plot_index_ = plot_resvered_index_for_y_axis_;
      ///////////////////////////////////////////////
      if (event_interface_ != nullptr) {
        event_interface_->OnGraphCtrlEvent(this, 1);
      }
    }
    LOG_F_INFO_TAG(name_) << " " << "plot_index:" << plot_index_;
    spGraphElement_->put_YValue(plot_index_, average);
    plot_index_++;

    data_sample_incoming_value_total_ = 0;
    data_sample_incoming_count_ = 0;
  }
}

void WorkWindowSecondWorkWindowSecondPageGraphCtrl::Release() {
  if (graph_ctrl_ != nullptr) {
#if 1
    DumpGraphDataList();
#endif
    graph_ctrl_->ClearGraph();
  }
  SAFE_RELEASE(spGraphElement_);
  SAFE_RELEASE(spDispatch_);
  SAFE_RELEASE(spElements_);
  SAFE_RELEASE(graph_ctrl_);
}

double WorkWindowSecondWorkWindowSecondPageGraphCtrl::GetCurrentXMin() const {
  LOG_F_INFO_TAG(name_) << " " << "x_min:" << x_min_;
  return x_min_;
}

double WorkWindowSecondWorkWindowSecondPageGraphCtrl::GetCurrentXDuration()
    const {
  LOG_F_INFO_TAG(name_) << " " << "x_min:" << x_min_;
  return x_duration_;
}

int32_t WorkWindowSecondWorkWindowSecondPageGraphCtrl::
    GetCurrentXDurationGraphSampleCount() const {
  LOG_F_INFO_TAG(name_) << " " << "x_min:" << x_min_;
  return sampling_total_minutes_ * this->x_grid_step_;
}

int32_t WorkWindowSecondWorkWindowSecondPageGraphCtrl::
    GetDataSampleCountForOneGraphSample() const {
  return data_sample_count_for_one_graph_sample_;
}

void WorkWindowSecondWorkWindowSecondPageGraphCtrl::UpateXTimeValue() {
  x_min_ = anx::common::GetCurrrentDateTime();
  LOG_F_INFO_TAG(name_) << " " << "x_min:" << x_min_;
}

void WorkWindowSecondWorkWindowSecondPageGraphCtrl::DumpGraphDataList() {
  for (int i = plot_resvered_index_for_y_axis_;
       i <
       sampling_total_minutes_ * x_grid_step_ + plot_resvered_index_for_y_axis_;
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
