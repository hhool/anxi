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

#include <memory>
#include <string>

#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP
using namespace DuiLib;

namespace anx {
namespace esolution {
class SolutionDesign;
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

  DuiLib::CDuiString GetSkinFolder() override;
  DuiLib::CDuiString GetSkinFile() override;
  DuiLib::UILIB_RESOURCETYPE GetResourceType() const override;
  LPCTSTR GetWindowClassName(void) const override;

  LRESULT OnSysCommand(UINT uMsg,
                       WPARAM wParam,
                       LPARAM lParam,
                       BOOL& bHandled) override;

 protected:
  void OnPrepare(DuiLib::TNotifyUI& msg);

  /// @brief Update the main tab page of work window with the solution type
  /// @param solution_type
  void UpdateTabMainFirstPageWithSolution(int32_t solution_type);
  /// @brief Update the work window tile with the solution type
  /// @param solution_type
  void UpdateWorkWindowTileWithSolution(int32_t solution_type);
  /// @brief Update the main tab page of work window with
  /// solution_design_.header_ and solution_design_.base_param_
  int32_t UpadateTabMainFirstPageElementViewHeaderAndBaseParam();
  /// @brief Update the main tab page of work window with
  /// solution_design_.result_
  void UpadateTabMainFirstPageElementViewResult();

 protected:
  /// @brief Default solution design xml filepath with solution type
  /// for solution design created with default value
  /// @param solution_type
  std::string DefaultSolutionDesignXml(int32_t solution_type);

  /// @brief Load the solution design file
  /// @param file_path the file path of the solution design file
  /// @return 0 if success, -1 if failed
  int32_t LoadSolutionDesignFile(const std::string& file_path);

  /// @brief Save the solution design file
  /// @param file_path the file path of the solution design file
  /// @return 0 if success, -1 if failed
  int32_t SaveSolutionDesignFile(const std::string& file_path);

  /// @brief Load the solution design file with dialog box
  /// @return 0 if success, -1 if failed
  /// @note the file path will be saved in solution_file_path_
  /// update solution_design_ with the file content.
  int32_t LoadFileWithDialog();

  /// @brief Save the solution design file with dialog box
  int32_t SaveFileWithDialog();

  /// @brief Update the solution design control with the solution design
  int32_t UpdateSolutionDesignControl();

  /// @brief Calculate the solution design with control and update the result
  int32_t SolutionDesignCalculateWithControl();

  /// @brief Read the solution design from control
  std::string SolutionDesignXmlFromControl();

 private:
  DuiLib::WindowImplBase* pOwner_;
  int32_t solution_type_;
  std::string solution_file_path_;
  std::unique_ptr<anx::esolution::SolutionDesign> solution_design_;

  CButtonUI* btn_close_;
  CButtonUI* btn_max_;
  CButtonUI* btn_restore_;
  CButtonUI* btn_min_;

  CButtonUI* btn_menu_design_manager_;
  CButtonUI* btn_menu_design_store_;
  CButtonUI* btn_menu_back_;
  CButtonUI* btn_menu_about_;

  CButtonUI* btn_solution_refresh_;

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
