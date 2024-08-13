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

#include "app/common/string_utils.h"
#include "app/ui/ui_constants.h"

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

////////////////////////////////////////////////////////////////////////////////、
namespace {
/// @brief data sampling interval value 12 second is one data sample.
/// @note 12 second is one data sample for the graph control. used for the
/// @note graph control data sample incoming. @see data_sampling_interval_ for
/// @note the data sample incoming. @see data_sampling_interval_var_ for the
/// @note data sample
static const double data_sampling_interval_var_ = 1.388888888889e-4;
}  // namespace

WorkWindowSecondWorkWindowSecondPageGraphCtrl::
    WorkWindowSecondWorkWindowSecondPageGraphCtrl() {}
WorkWindowSecondWorkWindowSecondPageGraphCtrl::
    WorkWindowSecondWorkWindowSecondPageGraphCtrl(
        CActiveXUI* activex,
        const std::string name,
        int32_t sampling_total_minutes,
        int32_t y_grid_step,
        int32_t x_grid_step)
    : activex_(activex),
      name_(name),
      sampling_total_minutes_(sampling_total_minutes),
      y_grid_step_(y_grid_step),
      x_grid_step_(x_grid_step) {
  if (sampling_total_minutes_ == 5) {
    x_duration_ = 5.0 / (24.0 * 60.0);
  } else if (sampling_total_minutes_ == 10) {
    x_duration_ = 10.0 / (24.0 * 60.0);
  } else if (sampling_total_minutes_ == 30) {
    x_duration_ = 30.0 / (24.0 * 60.0);
  } else if (sampling_total_minutes_ == 60) {
    x_duration_ = 60.0 / (24.0 * 60.0);
  }
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
  hr = spGraphElement_->put_PointSize(3);

  ///////////////////////////////////////////////////////////////////////////
  /// @note plot the element list to the graph element.
  /// @note calculate the element start index, end index, and start x axis
  /// @note value
  double x_min_axis_value = 0.0f;
  bool has_x_min_axis_value = false;
  int32_t start_index = element_list.size() - (sampling_total_minutes_ * 5);
  int32_t end_index = element_list.size();
  if (start_index < 0) {
    start_index = 0;
    if (end_index > 0) {
      has_x_min_axis_value = true;
      x_min_axis_value = element_list[start_index].x_;
    }
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @note Update plot graph x axis time.
  // graph_ctrl_->AutoRange();
  if (!has_x_min_axis_value) {
    this->UpateXTimeValue();
  } else {
    x_min_ = x_min_axis_value;
  }
  graph_ctrl_->SetRange(x_min_, x_min_ + x_duration_, y_min_, y_max_);
  ///////////////////////////////////////////////////////////////////////////

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
  for (int i = 0; i < sampling_total_minutes_ * 5; i++) {
    spGraphElement_->PlotXY(x_min_ + data_sampling_interval_var_ * (i), 0);
  }
  plot_index_ = plot_resvered_index_for_y_axis_;
  for (int32_t i = start_index; i < end_index; i++) {
    spGraphElement_->put_XValue(plot_index_, element_list[i - start_index].x_);
    spGraphElement_->put_YValue(plot_index_, element_list[i].y_);
    plot_index_++;
  }
}

void WorkWindowSecondWorkWindowSecondPageGraphCtrl::ProcessDataSampleIncoming(
    int32_t sample) {
  /// @note 100 ms is one sample, 120 sample is 12 second, 120 sample is
  /// one data sample, take average of the 120 sample as one data sample
  /// and put it to the graph.
  data_incomming_count_++;
  data_incomming_total_ += sample;
  /// @todo 100 ms is one sample, 120 sample is 12 second, 120 sample is
  /// one data sample, take average of the 120 sample as one data sample
  /// and put it to the graph.
  /// TODO(hhool): will be removed after the real data incoming.
#if !defined(_DEBUG)
  if (data_incomming_count_ % 120 == 0) {
#else
  if (data_incomming_count_ % 10 == 0) {
#endif
    int32_t average = static_cast<int32_t>(
        static_cast<double>(data_incomming_total_) / data_incomming_count_);
    average %= y_grid_step_;

    int32_t plot_data_sample_index =
        plot_index_ - plot_resvered_index_for_y_axis_;
    if (plot_data_sample_index > 0 &&
        plot_data_sample_index % (sampling_total_minutes_ * 5) == 0) {
      ///////////////////////////////////////////////
      /// @note Update plot graph x axis time.
      // graph_ctrl_->AutoRange();
      this->UpateXTimeValue();
#if 0
        /// @note for debug simulator
        x_min_ += x_duration_;
#endif
      graph_ctrl_->SetRange(x_min_, x_min_ + x_duration_, y_min_, y_max_);
      ///////////////////////////////////////////////
      /// @note Reset value after plot_index and x axis time to new x_min_;
      for (int32_t i = plot_resvered_index_for_y_axis_;
           i < sampling_total_minutes_ * 5 + plot_resvered_index_for_y_axis_;
           i++) {
        spGraphElement_->put_YValue(i, 0);
        spGraphElement_->put_XValue(
            i, x_min_ + data_sampling_interval_var_ *
                            (i - plot_resvered_index_for_y_axis_));
      }
      /// @note Reset the plot index to 2
      plot_index_ = plot_resvered_index_for_y_axis_;
    }
    spGraphElement_->put_YValue(plot_index_, average);
    plot_index_++;

    data_incomming_total_ = 0;
    data_incomming_count_ = 0;
  }
}

void WorkWindowSecondWorkWindowSecondPageGraphCtrl::Release() {
  if (graph_ctrl_ != nullptr) {
    graph_ctrl_->ClearGraph();
  }
  SAFE_RELEASE(spGraphElement_);
  SAFE_RELEASE(spDispatch_);
  SAFE_RELEASE(spElements_);
  SAFE_RELEASE(graph_ctrl_);
}

void WorkWindowSecondWorkWindowSecondPageGraphCtrl::UpateXTimeValue() {
  SYSTEMTIME st;
  GetLocalTime(&st);
  /// @note use SystemTimeToVariantTime to convert the time to double
  /// @note only the time part is used, the date part is ignored.
  DATE dt;
  st.wYear = 1899;
  st.wMonth = 12;
  st.wDay = 30;
  st.wHour = 0;
  st.wMilliseconds = 0;
  st.wDayOfWeek = 0;

  SystemTimeToVariantTime(&st, &dt);
  x_min_ = dt;
}

}  // namespace ui
}  // namespace anx
