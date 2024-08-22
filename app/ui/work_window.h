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

#include <map>
#include <memory>
#include <string>

#include "app/device/device_com.h"
#include "app/ui/ui_virtual_wnd_base.h"
#include "app/ui/work_window_tab_main_first_page_solution_design_base.h"

#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP
using namespace DuiLib;  // NOLINT

namespace anx {
namespace device {
class DeviceComInterface;
class DeviceComListener;
}  // namespace device
namespace esolution {
class SolutionDesign;
class PageSolutionDesignBase;
}  // namespace esolution
namespace ui {
class ChartLabelUI;
class CMenuDesignWnd;
class CMenuStoreWnd;
class WorkWindowStatusBar;
class WorkWindowSecondPage;
class WorkWindowThirdPage;
}  // namespace ui
}  // namespace anx

namespace anx {
namespace ui {
class WorkWindow : public DuiLib::WindowImplBase,
                   public anx::device::DeviceComListener {
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
  virtual void OnSelectChanged(DuiLib::TNotifyUI& msg);  // NOLINT

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

  CControlUI* CreateControl(LPCTSTR pstrClass) override;

  LRESULT OnDestroy(UINT /*uMsg*/,
                    WPARAM /*wParam*/,
                    LPARAM /*lParam*/,
                    BOOL& bHandled) override;

 protected:
  void OnPrepare(DuiLib::TNotifyUI& msg);  // NOLINT

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

 protected:
  /// @breif On menu device connect clicked
  void OnMenuDeviceConnectClicked(DuiLib::TNotifyUI& msg);  // NOLINT
  /// @breif On menu device disconnect clicked
  void OnMenuDeviceDisconnectClicked(DuiLib::TNotifyUI& msg);  // NOLINT

 protected:
  friend class CMenuDesignWnd;
  friend class CMenuStoreWnd;
  friend class WorkWindowStatusBar;
  friend class WorkWindowSecondPage;
  friend class WorkWindowThirdPage;
  /// @brief Is device com interface connected
  bool IsDeviceComInterfaceConnected() const;
  /// @brief is sl device com interface connected
  bool IsSLDeviceComInterfaceConnected() const;
  /// @brief is ul device com interface connected
  bool IsULDeviceComInterfaceConnected() const;
  /// @brief Open device com interface
  int32_t OpenDeviceCom(int32_t device_type);
  /// @brief Close all device com interface
  void CloseDeviceCom(int32_t device_type);

 protected:
  // impliment anx::device::DeviceComListener;
  void OnDataReceived(anx::device::DeviceComInterface* device,
                      const uint8_t* data,
                      int32_t size) override;
  void OnDataOutgoing(anx::device::DeviceComInterface* device,
                      const uint8_t* data,
                      int32_t size) override;

 protected:
  void OnExpStart();
  void OnExpStop();
  void OnExpPause();
  void OnExpResume();

 protected:
  void ClearArgsFreqNum();

 private:
  DuiLib::WindowImplBase* pOwner_;
  int32_t solution_type_;
  std::map<std::string, std::unique_ptr<DuiLib::CNotifyPump>> tab_main_pages_;
  PageSolutionDesignBase* solution_design_base_;
  UIVirtualWndBase* work_window_second_page_virtual_wnd_;
  UIVirtualWndBase* work_window_third_page_virtual_wnd_;
  std::unique_ptr<DuiLib::CNotifyPump> work_window_status_bar_;
  UIVirtualWndBase* work_window_status_bar_virtual_wnd_;
  std::shared_ptr<anx::device::DeviceComInterface> device_com_ul_;
  std::shared_ptr<anx::device::DeviceComInterface> device_com_sl_;
  std::vector<ChartLabelUI*> label_chart_uis_;

  CButtonUI* btn_close_;
  CButtonUI* btn_max_;
  CButtonUI* btn_restore_;
  CButtonUI* btn_min_;
  CButtonUI* btn_title_;

  CControlUI* icon_menu_design_manager_;
  CButtonUI* btn_menu_design_manager_;
  CControlUI* icon_menu_design_store_;
  CButtonUI* btn_menu_design_store_;
  CButtonUI* btn_menu_back_;

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