/**
 * @file work_window_tab_main_second_page_graph_ctrl.h
 * @author hhool (hhool@outlook.com)
 * @brief work window tab main second page graph control class implementation
 * @version 0.1
 * @date 2024-08-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_GRAPH_CTRL_H_
#define APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_GRAPH_CTRL_H_

#include <string>
#include <vector>

#include "app/common/string_utils.h"
#include "app/ui/dmgraph.tlh"
#include "app/ui/ui_constants.h"

#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP
using namespace DuiLib;  // NOLINT

namespace anx {
namespace ui {
////////////////////////////////////////////////////////////////////////////////
class Element2DPoint {
 public:
  Element2DPoint() {}
  Element2DPoint(double x, double y, int32_t no) : x_(x), y_(y), no_(no) {}
  ~Element2DPoint() = default;

 public:
  double x_ = 0;
  double y_ = 0;
  int32_t no_ = 1;
};

extern const double kVarTimeDuration5min;
extern const double kVarTimeDuration10min;
extern const double kVarTimeDuration15min;
extern const double kVarTimeDuration30min;
extern const double kVarTimeDuration60min;

extern const double kVartime2Seconds;
extern const double kVartime4Seconds;
extern const double kVartime6Seconds;
extern const double kVartime8Seconds;
extern const double kVartime10Seconds;

double minutes_to_vartime(int32_t minutes);
uint32_t vartime_to_minutes(double duration);
uint32_t minutes_to_data_sample_count(int32_t minutes);
uint32_t minutes_to_graph_sample_count(int32_t minutes);

class WorkWindowSecondWorkWindowSecondPageGraphCtrl;
class GraphCtrlEventInterface {
 public:
  virtual void OnGraphCtrlEvent(
      WorkWindowSecondWorkWindowSecondPageGraphCtrl* pOwner,
      int32_t action) = 0;
};

////////////////////////////////////////////////////////////////////////////////
class WorkWindowSecondWorkWindowSecondPageGraphCtrl {
 public:
  WorkWindowSecondWorkWindowSecondPageGraphCtrl();
  WorkWindowSecondWorkWindowSecondPageGraphCtrl(
      GraphCtrlEventInterface* event_interface,
      DuiLib::CActiveXUI* activex,
      const std::string name,
      double x_min,
      double x_duration,
      int32_t y_grid_step = 1,
      int32_t x_grid_step = 1,
      double y_axis_intial_value = 0.05f,
      bool auto_refresh = true);
  ~WorkWindowSecondWorkWindowSecondPageGraphCtrl();

 public:
  /// @brief  set auto refresh for the graph control
  /// @param auto_refresh the auto refresh flag for the graph control
  void SetAutoRefresh(bool auto_refresh = true);

  ////////////////////////////////////////////////////////////////////////////
  /// @brief  init the graph data list to the graph control element list
  ///         for the graph control, used for graph total duration change
  ///         copy old duration data to the new duration data.
  /// @param element_list the element list for the graph control
  void Init(std::vector<Element2DPoint> element_list);
  /// @brief  update the graph plot point to the graph control element list
  /// @param element_list the element list for the graph control
  void ProcessDataSampleIncoming(double sample);
  /// @brief  Release the graph control element list
  void Release();
  ////////////////////////////////////////////////////////////////////////////
  /// @brief  get x-axis min value for the graph control, x min value is
  ///         the start time for the graph control.
  /// @return  the current x min value for the graph control
  double GetXMinOfAxis() const;
  /// @brief  get x-axis duration value for the graph control, x duration
  ///         value is the total duration for the graph control.
  /// @return  the current x duration value for the graph control
  double GetXDurationOfAxis() const;
  /// @brief  get x-axis duration sample count for the graph control element
  /// list that is displayed on the graph control.
  /// @return  the current duration sample count for the graph control element
  /// list that is displayed on the graph control. type is int32_t.
  int32_t GetGraphSampleCountOfXDurationAxis() const {
    return sampling_total_minutes_ * this->x_grid_step_;
  }
  /// @brief data incoming count for the one graic data sample
  /// @return  the data incoming count for the one graic data sample
  int32_t GetDataSampleCountForOneGraphSample() const {
    return data_sample_count_for_one_graph_sample_;
  }
  /// @brief get current sampling count of one graph sample for the graph
  /// control
  /// @return int32_t
  int32_t GetCurrentDataSampleCountOfOneGraphPlot() const {
    return data_sample_incoming_count_;
  }

  /// @brief update y_max_ value for the graph control
  /// @param y_max the y_max_ value for the graph control
  void UpdateYMaxValue(double yMaxValue, bool update=true) {
     y_max_ = yMaxValue;
    if (update && graph_ctrl_ != nullptr) {
      graph_ctrl_->SetRange(x_min_, x_min_ + x_duration_, y_min_, y_max_);
    }
  }
 protected:
  void UpateXTimeValue();
  void DumpGraphDataList();

 private:
  CActiveXUI* activex_ = nullptr;
  GraphCtrlEventInterface* event_interface_ = nullptr;
  std::string name_;
  IDMGraphCtrl* graph_ctrl_ = nullptr;
  IDMGraphCollection* spElements_ = nullptr;
  IDispatch* spDispatch_ = nullptr;
  IDMGraphElement* spGraphElement_ = nullptr;
  /// @brief x min value for the graph control.
  /// @note default is 0.0, 0.0 var time is 1899-12-30 00:00:00.
  double x_min_ = 0.0;
  /// @brief x duration value for the graph control.
  /// @note default is 5 minutes, 5 minutes is the total duration for the
  /// graph control.
  double x_duration_ = 5.0 / (24.0 * 60.0);
  /// @brief  y grid step for the graph control.
  /// @note default is 1, 1 is the y grid step for the graph control.
  int32_t y_grid_step_ = 1;
  /// @brief  x grid step for the graph control.
  /// @note default is 1, 1 is the x grid step for the graph control.
  int32_t x_grid_step_ = 1;
  /// @brief y min value for the graph control.
  /// @note default is 0, 0 is the min value for the y axis.
  /// @note 0 is the min value for the y axis. it's mean the y axis start
  /// @note from 0.  unit is um at amplitude graph. MPa at stress graph.
  double y_min_ = 0.0;
  /// @brief y max value for the graph control.
  /// @note default is 15, 15 is the max value for the y axis.
  double y_max_ = 15.0;
  /// @brief y intial value for the graph control.
  /// @note default is 0.05, 0.05 is the intial value for the y axis.
  double y_axis_intial_value_ = 0.05f;
  /// @brief auto refresh flag for the graph control.
  /// @note default is false, false is the auto refresh flag for the graph
  bool auto_refresh_ = true;
  /// @brief data sampling total duration in minutes,
  /// @note default is 5 minutes graph control.
  /// @note 5 * 60 * 1000 = 5 minutes. 5 minutes is the total duration
  /// @note for the graph control.
  /// @note it's has 25 data sample for the graph control.
  /// @note 25 * 12 second = 5 minutes.
  int32_t sampling_total_minutes_ = 5;
  /// @brief data incoming count for the data sample
  /// @note 2000 millisecond one data sample, 2000 * 5 sample is 10 second for
  /// graph sample. it's mean 5 @see data_sample_count_for_one_graph_sample_
  /// sample is one data sample, take average of the 5 sample and put it to the
  /// graph.
  int32_t data_sample_incoming_count_ = 0;
  double data_sample_incoming_value_total_ = 0;
  int32_t data_sample_count_for_one_graph_sample_ = 5;
  /// @brief plot index for the graph control, plot index start from 2 for
  /// the graph element. 0 and 1 is the x and y axis. 2 is the first plot
  /// element for data sample. 3 is the second plot element.
  int32_t plot_index_ = 0;
  /// @brief just for the first time incoming data sample. for the first
  /// time incoming data sample, need to set the first time incoming data
  /// sample time. just for debug.
  double data_sample_incoming_first_time_ = 0;

 public:
  /// @brief plot graph x axis reserve count for y axis value range[y_min_,
  /// y_max_]
  static const int32_t plot_graph_x_axis_reserve_count_for_y_axis_ = 2;
};

}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_GRAPH_CTRL_H_
