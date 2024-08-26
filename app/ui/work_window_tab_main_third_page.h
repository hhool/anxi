/**
 * @file work_window_tab_main_third_page.h
 * @author hhool (hhool@outlook.com)
 * @brief work window third page ui
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_WORK_WINDOW_TAB_MAIN_THIRD_PAGE_H_
#define APP_UI_WORK_WINDOW_TAB_MAIN_THIRD_PAGE_H_

#include <memory>
#include <string>
#include <vector>

#include "app/device/device_com.h"
#include "app/ui/ui_virtual_wnd_base.h"

#include "third_party\duilib\source\DuiLib\UIlib.h"

// for DUI_DECLARE_MESSAGE_MAP
using namespace DuiLib;  // NOLINT

namespace anx {
namespace device {
class DeviceComInterface;
class DeviceComListener;
}  // namespace device
namespace ui {
class WorkWindow;
}  // namespace ui
}  // namespace anx

namespace anx {
namespace ui {
class WorkWindowThirdPage : public DuiLib::CNotifyPump,
                            public UIVirtualWndBase,
                            public anx::device::DeviceComListener {
 public:
  WorkWindowThirdPage(WorkWindow* pOwner,
                      DuiLib::CPaintManagerUI* paint_manager_ui);
  ~WorkWindowThirdPage();

 public:
  DUI_DECLARE_MESSAGE_MAP()
  void OnClick(TNotifyUI& msg);  // NOLINT
  void OnTimer(TNotifyUI& msg);  // NOLINT

 public:
  // implement the base class UIVirtualWndBase virtual function
  void Bind() override;
  void Unbind() override;

 protected:
  void UpdateControlFromSettings();

 protected:
  // impliment anx::device::DeviceComListener;
  void OnDataReceived(anx::device::DeviceComInterface* device,
                      const uint8_t* data,
                      int32_t size) override;
  void OnDataOutgoing(anx::device::DeviceComInterface* device,
                      const uint8_t* data,
                      int32_t size) override;

 private:
  WorkWindow* pWorkWindow_;
  DuiLib::CPaintManagerUI* paint_manager_ui_;
  std::shared_ptr<anx::device::DeviceComInterface> device_com_ul_;
  std::shared_ptr<anx::device::DeviceComInterface> device_com_sl_;
  std::vector<std::string> send_data_items_;
  uint32_t send_table_no_ = 0;
  std::vector<std::string> recv_data_items_;
  uint32_t recv_table_no_ = 0;
  std::vector<std::string> recv_notify_data_items_;
  uint32_t recv_notify_table_no_ = 0;

  COptionUI* opt_direct_up_;
  COptionUI* opt_direct_down_;
  COptionUI* opt_action_pull_;
  COptionUI* opt_action_push_;

  CEditUI* edit_speed_;
  CEditUI* edit_retention_;

  CLabelUI* label_displacement_;
  CLabelUI* label_strength_;
  CCheckBoxUI* check_box_display_send_;
  CCheckBoxUI* check_box_display_stop_recv_notify_;

  CListUI* list_send_;
  CListUI* list_recv_;
  CListUI* list_recv_notify_;

  class ListSendGetter;
  class ListRecvGetter;
  class ListRecvNotifyGetter;

  std::unique_ptr<DuiLib::IListCallbackUI> list_send_getter_;
  std::unique_ptr<DuiLib::IListCallbackUI> list_recv_getter_;
  std::unique_ptr<DuiLib::IListCallbackUI> list_recv_notify_getter_;
};
}  // namespace ui
}  // namespace anx

#endif  // APP_UI_WORK_WINDOW_TAB_MAIN_THIRD_PAGE_H_
