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

#ifndef ANXI_APP_UI_MAIN_WINDOW_H_
#define ANXI_APP_UI_MAIN_WINDOW_H_

#include <memory>
#include <string>

#include "third_party\duilib\source\DuiLib\UIlib.h"

using namespace DuiLib;

namespace anx {
namespace ui {

class MainWindow : public DuiLib::WindowImplBase {
 public:
  MainWindow();
  ~MainWindow() override;

  void Show();
  void Hide();
  void Close();

  void SetTitle(const std::string& title);
  void SetSize(int width, int height);
  void SetPosition(int x, int y);

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
  void OnExit(DuiLib::TNotifyUI& msg);
  void OnTimer(DuiLib::TNotifyUI& msg);

 private:
  std::string title_;
  int width_;
  int height_;
  int x_;
  int y_;
};
}  // namespace ui
}  // namespace anx

#endif  // ANXI_APP_UI_MAIN_WINDOW_H_
