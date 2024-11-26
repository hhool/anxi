/**
 * @file dialog_exp_data_record.h
 * @author hhool (hhool@outlook.com)
 * @brief dialog about
 * @version 0.1
 * @date 2024-07-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_DIALOG_EXP_DATA_RECORD_H_
#define APP_UI_DIALOG_EXP_DATA_RECORD_H_

#include <memory>
#include <string>
#include <vector>

#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP
using namespace DuiLib;  // NOLINT

using DuiLib::CControlUI;
using DuiLib::CPaintManagerUI;
using DuiLib::CWindowWnd;
using DuiLib::INotifyUI;

namespace anx {
namespace expdata {
class ExperimentFileSummary;
}  // namespace expdata
}  // namespace anx
namespace anx {
namespace ui {
class DialogExpDataRecord : public DuiLib::WindowImplBase {
 public:
  DialogExpDataRecord();
  ~DialogExpDataRecord();

  DUI_DECLARE_MESSAGE_MAP()

  void InitWindow() override;
  void Notify(DuiLib::TNotifyUI& msg) override;
  void OnFinalMessage(HWND hWnd) override;
  LRESULT ResponseDefaultKeyEvent(WPARAM wParam) override;

  DuiLib::CDuiString GetSkinFolder() override { return _T("skin\\"); }
  DuiLib::CDuiString GetSkinFile() override {
    return _T("dialog_experiment_data_record.xml");
  }
  DuiLib::UILIB_RESOURCETYPE GetResourceType() const override {
#if _DEBUG
    return DuiLib::UILIB_FILE;
#else
    return DuiLib::UILIB_ZIP;
#endif
  }
  DuiLib::CDuiString GetZIPFileName() const override {
    return _T("skin_default.zip");
  }
  LPCTSTR GetWindowClassName(void) const override {
    return _T("dialog_experiment_data_record");
  }

 protected:
  void OnPrepare(const TNotifyUI& msg);

  bool OnRefreshButtonClick(void* msg);  // NOLINT
  bool OnListItemClick(void* msg);       // NOLINT
  bool ShowSummaryItemDetail(int index);
  bool OnOpenFolderButtonClick(void* msg);  // NOLINT
  bool OnToReportButtonClick(void* msg);    // NOLINT

 private:
  DuiLib::CButtonUI* close_button_;
  DuiLib::CButtonUI* refresh_button_;
  DuiLib::CDateTimeUI* start_date_time_;
  DuiLib::CDateTimeUI* end_date_time_;
  DuiLib::CButtonUI* btn_open_folder_;
  DuiLib::CListUI* exp_data_list_;
  DuiLib::CVerticalLayoutUI* exp_data_detail_layout_;
  DuiLib::CButtonUI* btn_to_report_;
  DuiLib::CHorizontalLayoutUI* to_report_layout_;

  std::vector<anx::expdata::ExperimentFileSummary> exp_data_summary_list_;
};
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_DIALOG_EXP_DATA_RECORD_H_
