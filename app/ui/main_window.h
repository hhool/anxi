/**
 * @file main_window.h
 * @author hhool (hhool@outlook.com)
 * @brief main window class definition
 * @version 0.1
 * @date 2024-07-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_MAIN_WINDOW_H_
#define APP_UI_MAIN_WINDOW_H_

#include <memory>
#include <string>

#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP
using namespace DuiLib;

namespace anx {
namespace ui {

class MainWindow : public DuiLib::WindowImplBase {
 public:
  MainWindow();
  ~MainWindow() override;

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
  void OnPrepare(const DuiLib::TNotifyUI& msg);
  void OnExit(const DuiLib::TNotifyUI& msg);
  void OnTimer(const DuiLib::TNotifyUI& msg);

 protected:
  void Switch_Axially_Symmetrical();
  void Switch_Stresses_Adjustable();
  void Switch_th3point_Bending();
  void Switch_Vibration_Bending();

 private:
};
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_MAIN_WINDOW_H_
