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
  Element2DPoint(double x, double y) : x_(x), y_(y) {}
  ~Element2DPoint() = default;

 public:
  double x_ = 0;
  double y_ = 0;
};

extern const double kVarTimeDuration5min;
extern const double kVarTimeDuration10min;
extern const double kVarTimeDuration15min;
extern const double kVarTimeDuration30min;
extern const double kVarTimeDuration60min;

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
      int32_t sampling_total_minutes = 1,
      int32_t y_grid_step = 1,
      int32_t x_grid_step = 1,
      double y_axis_intial_value = 0.05f,
      bool auto_refresh = true);
  ~WorkWindowSecondWorkWindowSecondPageGraphCtrl();

 public:
  /// @brief  get the sampling total minutes
  /// @return  the sampling total minutes
  int32_t GetSamplingTotalMinutes() const;

  /// @brief  set the sampling count of one graph sample for the graph control
  /// @param count the sampling count of one graph sample for the graph control
  /// @return void
  void SetSamplingCount(int32_t count) { data_sample_incoming_count_ = count; }
  /// @brief get current sampling count of one graph sample for the graph
  /// control
  /// @return int32_t
  int32_t GetSamplingCount() const { return data_sample_incoming_count_; }

  /// @brief  set the sampling value total of one graph sample for the graph
  /// control
  /// @param total the sampling value total of one graph sample for the graph
  /// control
  /// @return void
  void SetSamplingValueTotal(double total) {
    data_sample_incoming_value_total_ = total;
  }

  /// @brief get the sampling value total of one graph sample for the graph
  /// control
  /// @return double
  double GetSamplingValueTotal() const {
    return data_sample_incoming_value_total_;
  }

  /// @brief  get the graph data list from the graph control
  /// @param pActiveXUI the active x ui pointer for the graph control
  /// @return  the graph data list from the graph control element list
  std::vector<Element2DPoint> GetGraphDataList();

  /// @brief  set auto refresh for the graph control
  /// @param auto_refresh the auto refresh flag for the graph control
  void SetAutoRefresh(bool auto_refresh = true);

 public:
  ////////////////////////////////////////////////////////////////////////////
  /// @brief  init the graph data list to the graph control element list
  ///         for the graph control, used for graph total duration change
  ///         copy old duration data to the new duration data.
  /// @param element_list the element list for the graph control
  void Init(std::vector<Element2DPoint> element_list,
            int32_t data_sample_count_for_one_graph_sample = 1);
  /// @brief  update the graph plot point to the graph control element list
  /// @param element_list the element list for the graph control
  void ProcessDataSampleIncoming(double sample);
  /// @brief  Release the graph control element list
  void Release();
  ////////////////////////////////////////////////////////////////////////////
  /// @brief  get current x min value for the graph control, x min value is
  ///         the start time for the graph control.
  /// @return  the current x min value for the graph control
  double GetCurrentXMin() const;
  /// @brief  get current x duration value for the graph control, x duration
  ///         value is the total duration for the graph control.
  /// @return  the current x duration value for the graph control
  double GetCurrentXDuration() const;
  /// @brief  get current duration sample count for the graph control element
  /// list that is displayed on the graph control.
  /// @return  the current duration sample count for the graph control element
  /// list that is displayed on the graph control. type is int32_t.
  int32_t GetCurrentXDurationGraphSampleCount() const;
  /// @brief data incoming count for the one graic data sample
  /// @return  the data incoming count for the one graic data sample
  int32_t GetDataSampleCountForOneGraphSample() const;

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
  int32_t y_grid_step_ = 1;
  int32_t x_grid_step_ = 1;
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
  /// @brief x min value for the graph control.
  /// @note default is 0.0, 0.0 var time is 1899-12-30 00:00:00.
  double x_min_ = 0.0;
  /// @brief x duration value for the graph control.
  /// @note default is 5 minutes, 5 minutes is the total duration for the
  /// graph control.
  double x_duration_ = 5.0 / (24.0 * 60.0);
  /// @brief y min value for the graph control.
  /// @note default is 0, 0 is the min value for the y axis.
  /// @note 0 is the min value for the y axis. it's mean the y axis start
  /// @note from 0.  unit is um at amplitude graph. MPa at stress graph.
  double y_min_ = 0.0;
  double y_max_ = 15.0;
  /// @brief data incoming count for the data sample
  /// @note 100 millisecond one sample, 120 sample is 12 second,
  /// @note 120 sample is one data sample, take average of the 120 sample.
  /// @note as one data sample and put it to the graph.
  int32_t data_sample_incoming_count_ = 0;
  double data_sample_incoming_value_total_ = 0;
  int32_t data_sample_count_for_one_graph_sample_ = 5;
  /// @brief plot index for the graph control, plot index start from 2 for
  /// the graph element. 0 and 1 is the x and y axis. 2 is the first plot
  /// element for data sample. 3 is the second plot element.
  int32_t plot_index_ = 0;

 public:
  static const int32_t plot_resvered_index_for_y_axis_ = 2;
};

}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_TAB_MAIN_SECOND_PAGE_GRAPH_CTRL_H_
