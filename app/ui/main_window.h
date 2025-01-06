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

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP
using namespace DuiLib;  // NOLINT

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

  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(DuiLib::TNotifyUI& msg) override;

  DuiLib::CDuiString GetSkinFolder() override { return _T("skin\\"); }
  DuiLib::CDuiString GetSkinFile() override { return _T("main_window.xml"); }
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
  LPCTSTR GetWindowClassName(void) const override { return _T("main_window"); }

  LRESULT OnClose(UINT uMsg,
                  WPARAM wParam,
                  LPARAM lParam,
                  BOOL& bHandled) override;

  LRESULT OnSetFocus(UINT /*uMsg*/,
                     WPARAM /*wParam*/,
                     LPARAM /*lParam*/,
                     BOOL& bHandled) override;

  LRESULT
  OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

  LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

 protected:
  void Switch_Axially_Symmetrical();
  void Switch_Stresses_Adjustable();
  void Switch_Th3point_Bending();
  void Switch_Vibration_Bending();
  void Switch_ThirdApp();
  void OnBtnAppSettings();

 protected:
  std::map<std::string, std::string> LoadAppConfig();

 private:
  CButtonUI* btn_close_;
  CButtonUI* btn_work_axially_symmetrical_;
  CButtonUI* btn_work_stresses_adjustable_;
  CButtonUI* btn_work_th3point_bending_;
  CButtonUI* btn_work_vibration_bending_;
  CHorizontalLayoutUI* hlayout_work_pilot_e10c_;
  CButtonUI* btn_work_pilot_e10c_;
  CButtonUI* btn_app_settings_;
  bool is_deal_with_quit_ = false;
};
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_MAIN_WINDOW_H_
