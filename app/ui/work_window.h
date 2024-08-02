/**
 * @file work_window.h
 * @author hhool (hhool@outlook.com)
 * @brief work window class definition
 * @version 0.1
 * @date 2024-07-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_WORK_WINDOW_H_
#define APP_UI_WORK_WINDOW_H_

#include "app/ui/ui_virtual_wnd_base.h"
#include "app/ui/work_window_tab_main_first_page_solution_design_base.h"
#include "app/ui/work_window_tab_main_second_page_base.h"

#include <map>
#include <memory>
#include <string>

#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP
using namespace DuiLib;

namespace anx {
namespace esolution {
class SolutionDesign;
class PageSolutionDesignBase;
}  // namespace esolution
}  // namespace anx

namespace anx {
namespace ui {
class WorkWindow : public DuiLib::WindowImplBase {
 public:
  explicit WorkWindow(DuiLib::WindowImplBase* pOwner, int32_t solution_type);
  ~WorkWindow() override;

 public:
  // impliment the pure virtual function of DuiLib::WindowImplBase
  void InitWindow() override;
  void OnFinalMessage(HWND hWnd) override;
  LRESULT ResponseDefaultKeyEvent(WPARAM wParam) override;
  void Notify(DuiLib::TNotifyUI& msg) override;

  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(DuiLib::TNotifyUI& msg) override;
  virtual void OnSelectChanged(DuiLib::TNotifyUI& msg);
  virtual void OnTimer(DuiLib::TNotifyUI& msg);

  DuiLib::CDuiString GetSkinFolder() override {
#ifdef _DEBUG
    return _T("skin\\");
#else
    return _T("skin\\");
#endif
  }
  DuiLib::CDuiString GetSkinFile() override { return _T("work_window.xml"); }
  DuiLib::UILIB_RESOURCETYPE GetResourceType() const override {
#ifdef _DEBUG
    return DuiLib::UILIB_FILE;
#else
    return DuiLib::UILIB_ZIP;
#endif
  }
  LPCTSTR GetWindowClassName(void) const override { return _T("work_window"); }

  LRESULT OnSysCommand(UINT uMsg,
                       WPARAM wParam,
                       LPARAM lParam,
                       BOOL& bHandled) override;

 protected:
  void OnPrepare(DuiLib::TNotifyUI& msg);

  /// @brief Update the work window tile with the solution type
  /// @param solution_type
  void UpdateWorkWindowTileWithSolution(int32_t solution_type);
  /// @brief Update the main tab page of work window with the solution type
  /// @param solution_type
  void UpdateTabMainFirstPageWithSolution(int32_t solution_type);
  /// @brief Update args area
  void UpdateArgsAreaWithSolution();

 protected:
  /// @brief Load the solution design file with dialog box
  /// @return 0 if success, -1 if failed
  /// @note the file path will be saved in solution_file_path_
  /// update solution_design_ with the file content.
  int32_t LoadFileWithDialog();

  /// @brief Save the solution design file with dialog box
  int32_t SaveFileWithDialog();

 private:
  DuiLib::WindowImplBase* pOwner_;
  int32_t solution_type_;
  std::map<std::string, std::unique_ptr<DuiLib::CNotifyPump>> tab_main_pages_;
  PageSolutionDesignBase* solution_design_base_;
  WorkWindowSecondPageBase* work_window_second_page_base_;
  std::unique_ptr<DuiLib::CNotifyPump> work_window_status_bar_;
  UIVirtualWndBase* work_window_status_bar_virtual_wnd_;

  CButtonUI* btn_close_;
  CButtonUI* btn_max_;
  CButtonUI* btn_restore_;
  CButtonUI* btn_min_;
  CButtonUI* btn_title_;

  CButtonUI* btn_menu_design_manager_;
  CButtonUI* btn_menu_design_store_;
  CButtonUI* btn_menu_back_;
  CButtonUI* btn_menu_about_;

  CButtonUI* btn_args_area_value_freq_;
  CButtonUI* btn_args_area_value_freq_num_;
  CButtonUI* btn_args_area_value_amplitude_;
  CButtonUI* btn_args_area_value_max_stress_;
  CButtonUI* btn_args_area_value_static_load_;
  CButtonUI* btn_args_area_value_stress_ratio_;
};
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_H_
