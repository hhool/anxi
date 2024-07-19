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
  /// @brief Update the main tab page of work window with the file path
  /// @param file_path
  void UpadateTabMainFirstPageWithFilePath(const std::string& file_path);

 protected:
  LRESULT OnCreate(UINT uMsg,
                   WPARAM wParam,
                   LPARAM lParam,
                   BOOL& bHandled) override;
  LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

  /// @brief Load the solution design file with dialog box
  void LoadFileWithDialog();
  /// @brief Save the solution design file with dialog box
  void SaveFileWithDialog();

  /// @brief Read the solution design from control
  void ReadSolutionDesignFromControl();

 private:
  DuiLib::WindowImplBase* pOwner_;
  int32_t solution_type_;
  std::string solution_file_path_;
};
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_H_
