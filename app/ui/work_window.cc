/**
 * @file work_window.cc
 * @author hhool (hhool@outlook.com)
 * @brief work window class definition
 * @version 0.1
 * @date 2024-07-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/work_window.h"

#include <iostream>
#include <utility>
#include <vector>

#include "app/common/defines.h"
#include "app/common/file_utils.h"
#include "app/common/string_utils.h"
#include "app/common/time_utils.h"
#include "app/db/database_helper.h"
#include "app/device/device_com_factory.h"
#include "app/device/device_com_settings.h"
#include "app/device/device_com_settings_helper.h"
#include "app/device/device_exp_data_sample_settings.h"
#include "app/device/stload/stload_helper.h"
#include "app/device/ultrasonic/ultra_helper.h"
#include "app/esolution/solution_design.h"
#include "app/esolution/solution_design_default.h"
#include "app/esolution/solution_design_helper.h"
#include "app/ui/dialog_com_port_settings.h"
#include "app/ui/dialog_com_record_2000c.h"
#include "app/ui/dialog_exp_data_record.h"
#include "app/ui/ui_constants.h"
#include "app/ui/work_window_menu_design.h"
#include "app/ui/work_window_menu_store.h"
#include "app/ui/work_window_status_bar.h"
#include "app/ui/work_window_tab_main_first_page_solution_design.h"
#include "app/ui/work_window_tab_main_page_base.h"
#include "app/ui/work_window_tab_main_second_page.h"
#include "app/ui/work_window_tab_main_third_page.h"

#include <powrprof.h>
#pragma comment(lib, "Powrprof.lib")

DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindow, DuiLib::WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {

namespace {
const int32_t kTimerCurrentTimeMsgId = 1;
const int32_t kTimerCurrentTimePeriod = 50;
}  // namespace

WorkWindow::WorkWindow(DuiLib::WindowImplBase* pOwner, int32_t solution_type)
    : pOwner_(pOwner), solution_type_(solution_type) {
  // initial device com
  std::shared_ptr<anx::device::DeviceComInterface> device_com_ul =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_Ultrasound, this);
  ultra_device_.reset(new anx::device::UltraDevice(device_com_ul.get()));
  is_device_stload_connected_ = false;
  is_device_ultra_connected_ = false;

  anx::device::stload::STLoadHelper::InitStLoad();
  anx::device::ultrasonic::UltrasonicHelper::InitUltrasonic();
  // database initial
  // remove database
  anx::db::helper::ClearDatabaseFile(anx::db::helper::kDefaultDatabasePathname);
  // initial database
  std::vector<std::string> sqls;
  sqls.push_back(anx::db::helper::sql::kCreateTableExpDataGraphSqlFormat);
  sqls.push_back(anx::db::helper::sql::kCreateTableExpDataListSqlFormat);
  sqls.push_back(anx::db::helper::sql::kCreateTableSendDataSqlFormat);
  sqls.push_back(anx::db::helper::sql::kCreateTableNotificationSqlFormat);
  sqls.push_back(anx::db::helper::sql::kCreateTableSendNotifySqlFormat);

  anx::db::helper::InitializeDataBase(anx::db::helper::kDefaultDatabasePathname,
                                      sqls);
  // initial solution design
  if (solution_type == anx::esolution::kSolutionName_Axially_Symmetrical) {
    WorkWindowFirstPageAxiallySymmetrical* page_axially =
        new WorkWindowFirstPageAxiallySymmetrical(this, &m_PaintManager);
    solution_design_base_ = page_axially;
    tab_main_pages_["WorkWindowFirstPageAxiallySymmetrical"].reset(
        page_axially);
    this->AddVirtualWnd(_T("WorkWindowFirstPageAxiallySymmetrical"),
                        page_axially);
  } else if (solution_type ==
             anx::esolution::kSolutionName_Stresses_Adjustable) {
    WorkWindownFirstPageStressAjustable* page_stress =
        new WorkWindownFirstPageStressAjustable(this, &m_PaintManager);
    solution_design_base_ = page_stress;
    tab_main_pages_["WorkWindowFirstPageStressesAdjustable"].reset(page_stress);
    this->AddVirtualWnd(
        _T("WorkWindowFirstPageStressesAdjustable"),
        tab_main_pages_["WorkWindowFirstPageStressesAdjustable"].get());
  } else if (solution_type == anx::esolution::kSolutionName_Th3point_Bending) {
    WorkWindowFirstPageTh3pointBending* page_th3point =
        new WorkWindowFirstPageTh3pointBending(this, &m_PaintManager);
    solution_design_base_ = page_th3point;
    tab_main_pages_["WorkWindowFirstPageTh3pointBending"].reset(page_th3point);
    this->AddVirtualWnd(
        _T("WorkWindowFirstPageTh3pointBending"),
        tab_main_pages_["WorkWindowFirstPageTh3pointBending"].get());
  } else if (solution_type == anx::esolution::kSolutionName_Vibration_Bending) {
    WorkWindowFirstPageVibrationBending* page_vibration =
        new WorkWindowFirstPageVibrationBending(this, &m_PaintManager);
    solution_design_base_ = page_vibration;
    tab_main_pages_["WorkWindowFirstPageVibrationBending"].reset(
        page_vibration);
    this->AddVirtualWnd(
        _T("WorkWindowFirstPageVibrationBending"),
        tab_main_pages_["WorkWindowFirstPageVibrationBending"].get());
  } else {
    assert(false && "Invalid solution type");
  }
  WorkWindowSecondPage* second_page =
      new WorkWindowSecondPage(this, &m_PaintManager);
  work_window_second_page_virtual_wnd_ = second_page;
  tab_main_pages_["WorkWindowSecondPage"].reset(second_page);
  this->AddVirtualWnd(_T("WorkWindowSecondPage"),
                      tab_main_pages_["WorkWindowSecondPage"].get());

  WorkWindowThirdPage* third_page =
      new WorkWindowThirdPage(this, &m_PaintManager);
  work_window_third_page_virtual_wnd_ = third_page;
  tab_main_pages_["WorkWindowThirdPage"].reset(third_page);
  this->AddVirtualWnd(_T("WorkWindowThirdPage"),
                      tab_main_pages_["WorkWindowThirdPage"].get());

  WorkWindowStatusBar* work_windows_status_bar =
      new WorkWindowStatusBar(this, &m_PaintManager);
  work_window_status_bar_.reset(work_windows_status_bar);
  this->AddVirtualWnd(_T("WorkWindowStatusBar"), work_window_status_bar_.get());
  work_window_status_bar_virtual_wnd_ = work_windows_status_bar;
}

WorkWindow::~WorkWindow() {
  solution_design_base_->Dispose();
  for (auto& tab_main_page : tab_main_pages_) {
    DuiLib::CDuiString name(
        anx::common::string2wstring(tab_main_page.first).c_str());
    this->RemoveVirtualWnd(name);
  }

  work_window_third_page_virtual_wnd_->Unbind();
  work_window_third_page_virtual_wnd_ = nullptr;
  WorkWindowThirdPage* work_window_third_page =
      reinterpret_cast<WorkWindowThirdPage*>(
          tab_main_pages_["WorkWindowThirdPage"].release());
  delete work_window_third_page;
  {
    auto it = tab_main_pages_.find("WorkWindowThirdPage");
    tab_main_pages_.erase(it);
  }

  work_window_second_page_virtual_wnd_->Unbind();
  work_window_second_page_virtual_wnd_ = nullptr;
  WorkWindowSecondPage* work_window_second_page =
      reinterpret_cast<WorkWindowSecondPage*>(
          tab_main_pages_["WorkWindowSecondPage"].release());
  delete work_window_second_page;
  {
    auto it = tab_main_pages_.find("WorkWindowSecondPage");
    tab_main_pages_.erase(it);
  }

  work_window_status_bar_virtual_wnd_->Unbind();
  work_window_status_bar_virtual_wnd_ = nullptr;
  WorkWindowStatusBar* work_window_status_bar =
      reinterpret_cast<WorkWindowStatusBar*>(work_window_status_bar_.release());
  delete work_window_status_bar;

  // remove tab_main_pages_
  for (auto& tab_main_page : tab_main_pages_) {
    PageSolutionDesignBase* base = reinterpret_cast<PageSolutionDesignBase*>(
        tab_main_page.second.release());
    if (solution_type_ == anx::esolution::kSolutionName_Axially_Symmetrical) {
      WorkWindowFirstPageAxiallySymmetrical* page_axially =
          reinterpret_cast<WorkWindowFirstPageAxiallySymmetrical*>(base);
      delete page_axially;
    } else if (solution_type_ ==
               anx::esolution::kSolutionName_Stresses_Adjustable) {
      WorkWindownFirstPageStressAjustable* page_stress =
          reinterpret_cast<WorkWindownFirstPageStressAjustable*>(base);
      delete page_stress;
    } else if (solution_type_ ==
               anx::esolution::kSolutionName_Th3point_Bending) {
      WorkWindowFirstPageTh3pointBending* page_th3point =
          reinterpret_cast<WorkWindowFirstPageTh3pointBending*>(base);
      delete page_th3point;
    } else if (solution_type_ ==
               anx::esolution::kSolutionName_Vibration_Bending) {
      WorkWindowFirstPageVibrationBending* page_vibration =
          reinterpret_cast<WorkWindowFirstPageVibrationBending*>(base);
      delete page_vibration;
    } else {
      assert(false && "Invalid solution type");
    }
  }
  tab_main_pages_.clear();

  // close devices all
  CloseDeviceCom(anx::device::kDeviceCom_StaticLoad);
  CloseDeviceCom(anx::device::kDeviceCom_Ultrasound);

  anx::device::stload::STLoadHelper::UnInitStLoad();
}

void WorkWindow::InitWindow() {
  __super::InitWindow();

  btn_close_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(kCloseButtonControlName));
  btn_max_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(kMaxButtonControlName));
  btn_restore_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(kRestoreButtonControlName));
  btn_min_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(kMinButtonControlName));
  btn_title_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(kMenu_Button_ExpTitle));

  icon_menu_design_manager_ =
      m_PaintManager.FindControl(_T("menu_icon_design_manager"));
  btn_menu_design_manager_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(_T("menu_btn_design_manager")));
  icon_menu_design_store_ =
      m_PaintManager.FindControl(_T("menu_icon_design_store"));
  btn_menu_design_store_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(_T("menu_btn_design_store")));
  btn_menu_back_ =
      static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("menu_btn_back")));

  h_layout_args_area_ = static_cast<CHorizontalLayoutUI*>(
      m_PaintManager.FindControl(_T("work_args_area")));
  btn_args_area_value_freq_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(_T("args_area_value_freq")));
  btn_args_area_value_freq_num_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(_T("args_area_value_freq_num")));
  btn_args_area_value_amplitude_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(_T("args_area_value_amplitude")));
  btn_args_area_value_max_stress_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(_T("args_area_value_max_stress")));
  btn_args_area_value_static_load_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(_T("args_area_value_static_load")));
  btn_args_area_value_stress_ratio_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(_T("args_area_value_stress_ratio")));

  BOOL ret =
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.set_dest_wnd(
          this->GetHWND());
  LOG_F(LG_INFO) << "set dest wnd ret:" << ret;
}

void WorkWindow::OnFinalMessage(HWND hWnd) {
  __super::OnFinalMessage(hWnd);
  delete this;
}

LRESULT WorkWindow::ResponseDefaultKeyEvent(WPARAM wParam) {
  if (wParam == VK_RETURN) {
    return FALSE;
  } else if (wParam == VK_ESCAPE) {
    return TRUE;
  }
  return FALSE;
}

void WorkWindow::Notify(DuiLib::TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
    OnPrepare(msg);
  } else if (msg.sType == kClick || msg.sType == kSelectChanged) {
    if (msg.pSender->GetName() == _T("btn_exp_start")) {
      OnExpStart();
    } else if (msg.pSender->GetName() == _T("btn_exp_stop")) {
      OnExpStop();
    } else if (msg.pSender->GetName() == _T("btn_exp_pause")) {
      OnExpPause();
    } else if (msg.pSender->GetName() == _T("btn_exp_resume")) {
      OnExpResume();
    }
    return DuiLib::WindowImplBase::Notify(msg);
  } else if (msg.sType == kTimer) {
    return DuiLib::WindowImplBase::Notify(msg);
  } else if (msg.sType == kMenu_Design_Connect) {
    OnMenuDeviceConnectClicked(msg);
  } else if (msg.sType == kMenu_Design_DisConnect) {
    OnMenuDeviceDisconnectClicked(msg);
  } else if (msg.sType == kMenu_Design_Device_Settings) {
    DialogComPortSettings* dialog_comport_settings =
        new DialogComPortSettings();
    dialog_comport_settings->Create(*this, _T("dialog_comport_settings"),
                                    UI_WNDSTYLE_FRAME,
                                    WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
    dialog_comport_settings->CenterWindow();
    dialog_comport_settings->ShowModal();
  } else if (msg.sType == kMenu_Design_Read_Solution) {
    int32_t ret = LoadFileWithDialog();
    if (ret < 0) {
      if (ret == -3) {
        MessageBox(*this, _T("与当前试件设计类型不匹配"), _T("加载失败"),
                   MB_OK);
      } else {
        MessageBox(*this, _T("试件设计文件错误"), _T("加载失败"), MB_OK);
      }
      return;
    } else {
      // TODO(hhool): do nothing;
    }
  } else if (msg.sType == kMenu_Design_Save_Solution) {
    int32_t ret = SaveFileWithDialog();
    if (ret < 0) {
      MessageBox(*this, _T("保存失败"), _T("保存失败"), MB_OK);
      return;
    } else if (ret == 0) {
      MessageBox(*this, _T("保存成功"), _T("保存成功"), MB_OK);
    } else {
      // do nothing
    }
  } else if (msg.sType == kMenu_Store_ExpRecord) {
    DialogExpDataRecord* dialog_exp_data_record = new DialogExpDataRecord();
    dialog_exp_data_record->Create(*this, _T("dialog_exp_data_record"),
                                   UI_WNDSTYLE_FRAME,
                                   WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
    dialog_exp_data_record->CenterWindow();
    dialog_exp_data_record->ShowModal();
  } else if (msg.sType == kMenu_Store_ComRecord) {
    DialogComRecord2000C* dialog_com_record_2000c = new DialogComRecord2000C();
    dialog_com_record_2000c->Create(*this, _T("dialog_com_record_2000c"),
                                    UI_WNDSTYLE_FRAME,
                                    WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
    dialog_com_record_2000c->CenterWindow();
    dialog_com_record_2000c->ShowWindow(true, true);
  } else if (msg.sType == DUI_MSGTYPE_KILLFOCUS) {
    DuiLib::CDuiString name = msg.pSender->GetName();
    std::string name_str = anx::common::wstring2string(name.GetData());
    LOG_F(LG_INFO) << "name:" << name_str;
    // compare name_str partion compare with
    // "tm_page_first_left_max_stress" and "tm_page_first_left_ratio_stress"
    if (name_str.find("tm_page_first_left_max_stress") != std::string::npos) {
      DuiLib::CDuiString max_stress = msg.pSender->GetText();
      btn_args_area_value_max_stress_->SetText(max_stress);
    } else if (name_str.find("tm_page_first_left_ratio_stress") !=
               std::string::npos) {
      DuiLib::CDuiString stress_ratio = msg.pSender->GetText();
      btn_args_area_value_stress_ratio_->SetText(stress_ratio);
    } else {
      // do nothing
    }
  } else {
    // DuiLib::WindowImplBase::Notify(msg);
    __super::Notify(msg);
  }
}

void WorkWindow::OnClick(DuiLib::TNotifyUI& msg) {
  if (msg.pSender == btn_close_) {
    if (pOwner_ != nullptr) {
      CloseDeviceCom(anx::device::kDeviceCom_StaticLoad);
      CloseDeviceCom(anx::device::kDeviceCom_Ultrasound);
      this->Close();
      pOwner_->ShowWindow(true, true);
    } else {
      PostQuitMessage(0);
    }
    return;
  } else if (msg.pSender == btn_max_) {
    SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
  } else if (msg.pSender == btn_restore_) {
    SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
  } else if (msg.pSender == btn_min_) {
    SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
  } else if (msg.pSender == btn_menu_design_manager_) {
    RECT rt = icon_menu_design_manager_->GetClientPos();
    POINT pt = {0};
    pt.y = rt.bottom;
    pt.x = rt.left;
    ::ClientToScreen(*this, &pt);
    CMenuDesignWnd* pMenu = new CMenuDesignWnd(this);
    if (pMenu == NULL) {
      return;
    }
    pMenu->Init(msg.pSender, pt);
  } else if (msg.pSender == btn_menu_design_store_) {
    RECT rt = icon_menu_design_store_->GetClientPos();
    POINT pt = {0};
    pt.y = rt.bottom;
    pt.x = rt.left;
    ::ClientToScreen(*this, &pt);
    CMenuStoreWnd* pMenu = new CMenuStoreWnd();
    if (pMenu == NULL) {
      return;
    }
    pMenu->Init(msg.pSender, pt);
  } else if (msg.pSender == btn_menu_back_) {
    if (pOwner_ != nullptr) {
      CloseDeviceCom(anx::device::kDeviceCom_StaticLoad);
      CloseDeviceCom(anx::device::kDeviceCom_Ultrasound);
      Close();
      pOwner_->ShowWindow(true, true);
    } else {
      PostQuitMessage(0);
    }
  } else {
    // TODO(hhool): do nothing
  }
}

void WorkWindow::OnSelectChanged(DuiLib::TNotifyUI& msg) {
  DuiLib::CDuiString name = msg.pSender->GetName();
  DuiLib::CTabLayoutUI* pControl_tb_main = static_cast<DuiLib::CTabLayoutUI*>(
      m_PaintManager.FindControl(_T("tab_main")));
  CControlUI* pControl_design =
      m_PaintManager.FindControl(_T("body_tab_button_specimen_design"));
  CLabelUI* pLabel_design = static_cast<CLabelUI*>(
      m_PaintManager.FindControl(_T("body_tab_label_specimen_design")));
  CControlUI* pControl_exec =
      m_PaintManager.FindControl(_T("body_tab_button_exp_exec"));
  CLabelUI* pLabel_exec = static_cast<CLabelUI*>(
      m_PaintManager.FindControl(_T("body_tab_label_exp_exec")));
  CControlUI* pControl_static_carrier_aircraft =
      m_PaintManager.FindControl(_T("body_tab_button_static_carrier_aircraft"));
  CLabelUI* pLabel_static_carrier_aircraft = static_cast<CLabelUI*>(
      m_PaintManager.FindControl(_T("body_tab_label_static_carrier_aircraft")));
  if (name == _T("body_tab_button_specimen_design")) {
    pControl_tb_main->SelectItem(0);
    pLabel_design->SetVisible(true);
    pLabel_exec->SetVisible(false);
    pLabel_static_carrier_aircraft->SetVisible(false);
  } else if (name == _T("body_tab_button_exp_exec")) {
    pControl_tb_main->SelectItem(1);
    pLabel_design->SetVisible(false);
    pLabel_exec->SetVisible(true);
    pLabel_static_carrier_aircraft->SetVisible(false);
  } else if (name == _T("body_tab_button_static_carrier_aircraft")) {
    pControl_tb_main->SelectItem(2);
    pLabel_design->SetVisible(false);
    pLabel_exec->SetVisible(false);
    pLabel_static_carrier_aircraft->SetVisible(true);
  } else {
    // do nothing;
  }
}

LRESULT WorkWindow::OnSysCommand(UINT uMsg,
                                 WPARAM wParam,
                                 LPARAM lParam,
                                 BOOL& bHandled) {
  if (wParam == SC_CLOSE) {
    if (pOwner_ != nullptr) {
      CloseDeviceCom(anx::device::kDeviceCom_StaticLoad);
      CloseDeviceCom(anx::device::kDeviceCom_Ultrasound);
      this->Close();
      pOwner_->ShowWindow(true, true);
    } else {
      PostQuitMessage(0);
    }
    bHandled = TRUE;
    return 0;
  }
  BOOL bZoomed = ::IsZoomed(m_hWnd);
  LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
  if (::IsZoomed(m_hWnd) != bZoomed) {
    if (!bZoomed) {
      btn_max_->SetVisible(false);
      btn_restore_->SetVisible(true);

    } else {
      btn_max_->SetVisible(true);
      btn_restore_->SetVisible(false);
    }
  }
  return 0;
}

ULONG WorkWindow::DeviceCallback(PVOID Context, ULONG Type, PVOID Setting) {
  WorkWindow* pthis = reinterpret_cast<WorkWindow*>(Context);
  if (Type == PBT_APMSUSPEND) {
    if (pthis->is_exp_state_ != 0) {
      DuiLib::TNotifyUI msg;
      msg.sType = kValueChanged;
      msg.pSender = pthis->btn_args_area_value_amplitude_;
      msg.wParam = PBT_APMQUERYSUSPEND;
      pthis->tab_main_pages_["WorkWindowThirdPage"]->NotifyPump(msg);
      pthis->tab_main_pages_["WorkWindowSecondPage"]->NotifyPump(msg);
      return 0;
    }
  } else if (Type == PBT_APMRESUMESUSPEND) {
    cout << "open" << endl;
  }
  return ERROR_SUCCESS;
}

static HPOWERNOTIFY g_power_notify_handle = NULL;
LRESULT WorkWindow::OnCreate(UINT uMsg,
                             WPARAM wParam,
                             LPARAM lParam,
                             BOOL& bHandled) {
  DEVICE_NOTIFY_SUBSCRIBE_PARAMETERS params;
  params.Callback = DeviceCallback;
  params.Context = this;
  PowerRegisterSuspendResumeNotification(DEVICE_NOTIFY_CALLBACK, &params,
                                         &g_power_notify_handle);
  return __super::OnCreate(uMsg, wParam, lParam, bHandled);
}

LRESULT WorkWindow::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled) {
  PowerUnregisterSuspendResumeNotification(g_power_notify_handle);
  bHandled = FALSE;
  return __super::OnDestroy(0, 0, 0, bHandled);
}

LRESULT WorkWindow::OnNcHitTest(UINT uMsg,
                                WPARAM wParam,
                                LPARAM lParam,
                                BOOL& bHandled) {
  POINT pt;
  pt.x = GET_X_LPARAM(lParam);
  pt.y = GET_Y_LPARAM(lParam);
  ::ScreenToClient(*this, &pt);

  RECT rcClient;
  ::GetClientRect(*this, &rcClient);
  if (!::IsZoomed(*this)) {
    RECT rcSizeBox = m_PaintManager.GetSizeBox();
    if (pt.y < rcClient.top + rcSizeBox.top) {
      if (pt.x < rcClient.left + rcSizeBox.left)
        return HTTOPLEFT;
      if (pt.x > rcClient.right - rcSizeBox.right)
        return HTTOPRIGHT;
      return HTTOP;

    } else if (pt.y > rcClient.bottom - rcSizeBox.bottom) {
      if (pt.x < rcClient.left + rcSizeBox.left)
        return HTBOTTOMLEFT;
      if (pt.x > rcClient.right - rcSizeBox.right)
        return HTBOTTOMRIGHT;
      return HTBOTTOM;
    }
    if (pt.x < rcClient.left + rcSizeBox.left)
      return HTLEFT;
    if (pt.x > rcClient.right - rcSizeBox.right)
      return HTRIGHT;
  }

  RECT rcCaption = m_PaintManager.GetCaptionRect();
  if (pt.x >= rcClient.left + rcCaption.left &&
      pt.x < rcClient.right - rcCaption.right && pt.y >= rcCaption.top &&
      pt.y < rcCaption.bottom) {
    CControlUI* pControl =
        static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
    if (pControl &&
        (_tcsicmp(pControl->GetClass(), _T("HorizontalLayout")) == 0 ||
         _tcsicmp(pControl->GetClass(), _T("Container")) == 0)) {
      return HTCAPTION;
    }
  }

  return HTCLIENT;
}

LRESULT WorkWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (uMsg == DLLMSG) {
    if (lParam == DLL_SAMPLE) {
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_
          .before_get_sample();
      double load =
          anx::device::stload::STLoadHelper::st_load_loader_.st_api_.get_load();
      double pos =
          anx::device::stload::STLoadHelper::st_load_loader_.st_api_.get_posi();
      double exten =
          anx::device::stload::STLoadHelper::st_load_loader_.st_api_.get_extn();
      uint32_t status = anx::device::stload::STLoadHelper::st_load_loader_
                            .st_api_.get_test_status();
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_
          .after_get_sample();

      if (anx::device::stload::STLoadHelper::Is_Stload_Simulation()) {
        pos = static_cast<double>(rand() % 100) * 1.0f;
        load = static_cast<double>(rand() % 100) * 1.0f;
        std::unique_ptr<anx::esolution::SolutionDesign> design =
            solution_design_base_->SolutionDesignFromPage();
        if (design != nullptr) {
          if (design->result_->solution_type_ ==
              anx::esolution::kSolutionName_Stresses_Adjustable) {
            load = reinterpret_cast<
                       anx::esolution::ExpDesignResultStressesAdjustable*>(
                       design->result_.get())
                       ->f_static_load_MPa_;
          } else if (design->result_->solution_type_ ==
                     anx::esolution::kSolutionName_Th3point_Bending) {
            load = reinterpret_cast<
                       anx::esolution::ExpDesignResultTh3pointBending*>(
                       design->result_.get())
                       ->f_static_load_MPa_;
          }
        }
      }
      // notify third page to update the data
      // notify second page to update the chart
      DuiLib::TNotifyUI msg;
      msg.pSender = this->h_layout_args_area_;
      msg.sType = kValueChanged;

      ENMsgStruct enmsg;

      anx::device::stload::STResult result;
      result.load_ = load;
      result.pos_ = pos;
      result.status_ = status;

      enmsg.ptr_ = &result;
      enmsg.type_ = enmsg_type_stload_value_cur;
      msg.wParam = reinterpret_cast<WPARAM>(&enmsg);
      tab_main_pages_["WorkWindowThirdPage"]->NotifyPump(msg);
      tab_main_pages_["WorkWindowSecondPage"]->NotifyPump(msg);
    }
    return 0;
  } else {
    // TODO(hhool): do nothing
  }
  return __super::HandleMessage(uMsg, wParam, lParam);
}

void WorkWindow::OnPrepare(DuiLib::TNotifyUI& msg) {
  if (solution_type_ >= anx::esolution::kSolutionName_Axially_Symmetrical &&
      solution_type_ <= anx::esolution::kSolutionName_Vibration_Bending) {
    // update work_frame window titile for different solution
    UpdateWorkWindowTileWithSolution(solution_type_);
    // update tab_main first page for different solution.
    UpdateTabMainFirstPageWithSolution(solution_type_);
    // load default or last document solution design
    solution_design_base_->InitPage();
    UpdateArgsAreaWithSolution();
    // init third page
    work_window_third_page_virtual_wnd_->Bind();
    // init second page
    work_window_second_page_virtual_wnd_->Bind();
    // init status bar
    work_window_status_bar_virtual_wnd_->Bind();
  } else {
    assert(false && "Invalid solution type");
  }
}

void WorkWindow::UpdateWorkWindowTileWithSolution(int32_t solution_type) {
  if (solution_type == anx::esolution::kSolutionName_Axially_Symmetrical) {
    btn_title_->SetText(_T("轴向对称拉压疲劳试验"));
  } else if (solution_type ==
             anx::esolution::kSolutionName_Stresses_Adjustable) {
    btn_title_->SetText(_T("可调应力轴向拉压疲劳试验"));
  } else if (solution_type == anx::esolution::kSolutionName_Th3point_Bending) {
    btn_title_->SetText(_T("三点弯曲疲劳试验"));
  } else if (solution_type == anx::esolution::kSolutionName_Vibration_Bending) {
    btn_title_->SetText(_T("振动弯曲疲劳试验"));
  } else {
    assert(false && "Invalid solution type");
  }
}

void WorkWindow::UpdateTabMainFirstPageWithSolution(int32_t solution_type) {
  DuiLib::CTabLayoutUI* pControl = static_cast<DuiLib::CTabLayoutUI*>(
      m_PaintManager.FindControl(_T("tab_main")));
  if (pControl == nullptr)
    return;
  if (solution_type == anx::esolution::kSolutionName_Axially_Symmetrical) {
    pControl->RemoveAt(1);
    pControl->RemoveAt(1);
    pControl->RemoveAt(1);

    pControl->GetItemAt(0)->SetVisible();
  } else if (solution_type ==
             anx::esolution::kSolutionName_Stresses_Adjustable) {
    pControl->RemoveAt(0);
    pControl->RemoveAt(1);
    pControl->RemoveAt(1);

    pControl->GetItemAt(0)->SetVisible();
  } else if (solution_type == anx::esolution::kSolutionName_Th3point_Bending) {
    pControl->RemoveAt(0);
    pControl->RemoveAt(0);
    pControl->RemoveAt(1);

    pControl->GetItemAt(0)->SetVisible();
  } else if (solution_type == anx::esolution::kSolutionName_Vibration_Bending) {
    pControl->RemoveAt(0);
    pControl->RemoveAt(0);
    pControl->RemoveAt(0);

    pControl->GetItemAt(0)->SetVisible();
  } else {
    assert(false && "Invalid solution type");
  }
}

void WorkWindow::UpdateArgsAreaWithSolution() {
  std::unique_ptr<anx::esolution::SolutionDesign> design =
      solution_design_base_->SolutionDesignFromPage();
  if (design == nullptr) {
    return;
  }
  {
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), design->base_param_->f_max_stress_MPa_);
    btn_args_area_value_max_stress_->SetText(value);
  }
  {
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), design->base_param_->f_stress_ratio_);
    btn_args_area_value_stress_ratio_->SetText(value);
  }
  {
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"),
                 reinterpret_cast<anx::esolution::ExpDesignResult0*>(
                     design->result_.get())
                     ->f_eamplitude_);
    btn_args_area_value_amplitude_->SetText(value);
  }
  {
    if (design->result_->solution_type_ ==
        anx::esolution::kSolutionName_Stresses_Adjustable) {
      DuiLib::CDuiString value;
      value.Format(
          _T("%.2f"),
          reinterpret_cast<anx::esolution::ExpDesignResultStressesAdjustable*>(
              design->result_.get())
              ->f_static_load_MPa_);
      btn_args_area_value_static_load_->SetText(value);
    } else if (design->result_->solution_type_ ==
               anx::esolution::kSolutionName_Th3point_Bending) {
      DuiLib::CDuiString value;
      value.Format(
          _T("%.2f"),
          reinterpret_cast<anx::esolution::ExpDesignResultTh3pointBending*>(
              design->result_.get())
              ->f_static_load_MPa_);
      btn_args_area_value_static_load_->SetText(value);
    } else {
      // TODO(hhool): do nothing;
    }
  }
  DuiLib::TNotifyUI msg;
  msg.pSender = this->h_layout_args_area_;
  msg.sType = kValueChanged;
  ENMsgStruct enmsg;
  enmsg.ptr_ = design.get();
  enmsg.type_ = enmsg_type_exp_stress_amp;
  msg.wParam = reinterpret_cast<WPARAM>(&enmsg);
  tab_main_pages_["WorkWindowThirdPage"]->NotifyPump(msg);
  tab_main_pages_["WorkWindowSecondPage"]->NotifyPump(msg);
}

int32_t WorkWindow::LoadFileWithDialog() {
  OPENFILENAME ofn;
  TCHAR FileName[MAX_PATH];
  memset(&ofn, 0, sizeof(OPENFILENAME));
  memset(FileName, 0, sizeof(char) * MAX_PATH);
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = m_hWnd;
  ofn.lpstrFilter = _T("*.xml\0\0");
  ofn.lpstrFile = FileName;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_FILEMUSTEXIST;
  if (!GetOpenFileName(&ofn)) {
    return 1;
  }
  std::string filepath = anx::common::wstring2string(ofn.lpstrFile);
  int32_t ret = solution_design_base_->LoadSolutionDesingFile(filepath);
  if (ret != 0) {
    return ret;
  }
  return 0;
}

int32_t WorkWindow::SaveFileWithDialog() {
  OPENFILENAME ofn;
  TCHAR FileName[MAX_PATH];
  memset(&ofn, 0, sizeof(OPENFILENAME));
  memset(FileName, 0, sizeof(char) * MAX_PATH);
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = m_hWnd;
  ofn.lpstrFilter = _T("*.xml\0\0");
  ofn.lpstrFile = FileName;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_FILEMUSTEXIST;
  if (!GetSaveFileName(&ofn)) {
    return 1;
  }

  // create file with ofn.lpstrFile
  std::string filepath = anx::common::wstring2string(ofn.lpstrFile);
  // get file extension
  std::string extension = filepath.substr(filepath.find_last_of(".") + 1);
  if (extension != "xml") {
    // append .xml to filepath
    filepath += ".xml";
  }
  // save solution design file
  // 1. get solution design from control
  // 2. save solution design file
  // 3. return 0
  std::unique_ptr<anx::esolution::SolutionDesign> design =
      solution_design_base_->SolutionDesignFromPage();
  if (design == nullptr) {
    return -1;
  }

  if (anx::esolution::SaveSolutionDesignFile(filepath, *design) != 0) {
    return -2;
  }

  return 0;
}

namespace {
/// @brief transform port name to int32_t
///        COM1 -> 1
///        COM2 -> 2
/// @param port_name
/// @return
int32_t PortNameToInt32(const std::string& port_name) {
  if (port_name.size() < 4) {
    return -1;
  }
  return std::stoi(port_name.substr(3));
}
}  // namespace

void WorkWindow::OnMenuDeviceConnectClicked(DuiLib::TNotifyUI& msg) {
  // create or get ultrasound device.
  int32_t ret = OpenDeviceCom(anx::device::kDeviceCom_Ultrasound);
  if (ret == 0) {
    is_device_ultra_connected_ = true;
  } else if (ret < 0) {
    is_device_ultra_connected_ = false;
    MessageBox(*this, _T("超声连接失败"), _T("连接失败"), MB_OK);
  }
  ret = OpenDeviceCom(anx::device::kDeviceCom_StaticLoad);
  if (ret == 0) {
    is_device_stload_connected_ = true;
  } else if (ret < 0) {
    is_device_stload_connected_ = false;
    MessageBox(*this, _T("静载机连接失败"), _T("连接失败"), MB_OK);
  }
}

void WorkWindow::OnMenuDeviceDisconnectClicked(DuiLib::TNotifyUI& msg) {
  CloseDeviceCom(anx::device::kDeviceCom_StaticLoad);
  CloseDeviceCom(anx::device::kDeviceCom_Ultrasound);
  is_device_stload_connected_ = false;
  is_device_ultra_connected_ = false;
}

bool WorkWindow::IsDeviceComInterfaceConnected() const {
  return (ultra_device_ != nullptr && ultra_device_->isOpened() &&
          is_device_ultra_connected_) ||
         (is_device_stload_connected_);
}

bool WorkWindow::IsSLDeviceComInterfaceConnected() const {
  return is_device_stload_connected_;
}

bool WorkWindow::IsULDeviceComInterfaceConnected() const {
  return ultra_device_ != nullptr && ultra_device_->isOpened() &&
         is_device_ultra_connected_;
}

int32_t WorkWindow::OpenDeviceCom(int32_t device_type) {
  if (device_type == anx::device::kDeviceCom_Ultrasound) {
    if (!ultra_device_->isOpened()) {
      std::unique_ptr<anx::device::ComSettings> com_settings =
          anx::device::LoadDeviceComSettingsDefaultResourceWithType(
              device_type);
      if (com_settings == nullptr) {
        return -1;
      }
      if (ultra_device_->Open(*com_settings.get()) != 0) {
        return -2;
      }
      int32_t max_power = ultra_device_->GetMaxPower();
      if (max_power < 0) {
        return -3;
      }
      int32_t current_power = ultra_device_->GetCurrentPower();
      if (current_power < 0) {
        return -4;
      }
      int32_t initial_freq = ultra_device_->GetCurrentFreq();
      if (initial_freq < 0) {
        return -5;
      }
      DuiLib::CDuiString value;
      value.Format(_T("%.3f"), initial_freq / 1000.0f);
      btn_args_area_value_freq_->SetText(value);
    } else {
      return -3;
    }
  } else if (device_type == anx::device::kDeviceCom_StaticLoad) {
    std::unique_ptr<anx::device::ComSettings> com_settings =
        anx::device::LoadDeviceComSettingsDefaultResourceWithType(
            anx::device::kDeviceCom_StaticLoad);
    if (com_settings == nullptr) {
      return -1;
    }
    int32_t port = PortNameToInt32(com_settings->GetComName());
    if (port >= 0) {
      bool bSuccess =
          anx::device::stload::STLoadHelper::st_load_loader_.st_api_
                  .open_device(PortNameToInt32(com_settings->GetComName()))
              ? true
              : false;
      if (!bSuccess) {
        LOG_F(LG_ERROR) << "OpenDevice failed";
        return -1;
      }
      bSuccess = (anx::device::stload::STLoadHelper::STLoadSetup() == 0)
                     ? true
                     : false;
      if (!bSuccess) {
        LOG_F(LG_ERROR) << "STLoadSetup failed";
        return -1;
      }
    } else {
      return -2;
    }
  } else {
    assert(false && "Invalid device type");
  }
  return 0;
}

void WorkWindow::CloseDeviceCom(int32_t device_type) {
  if (device_type == anx::device::kDeviceCom_Ultrasound) {
    if (ultra_device_ != nullptr) {
      ultra_device_->GetPortDevice()->RemoveListener(this);
      ultra_device_->Close();
    }
  } else if (device_type == anx::device::kDeviceCom_StaticLoad) {
    LOG_F(LG_INFO) << "CloseDeviceCom StaticLoad";
    if (anx::device::stload::STLoadHelper::st_load_loader_.st_api_.stop_run !=
        nullptr) {
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.stop_run();
    }
    if (anx::device::stload::STLoadHelper::st_load_loader_.st_api_.off_line !=
        nullptr) {
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.off_line();
    }
    if (anx::device::stload::STLoadHelper::st_load_loader_.st_api_
            .close_device != nullptr) {
      anx::device::stload::STLoadHelper::st_load_loader_.st_api_.close_device();
    }
  } else {
    assert(false && "Invalid device type");
  }
}

void WorkWindow::OnExpStart() {
  is_exp_state_ = 1;

  std::unique_ptr<anx::esolution::SolutionDesign> design =
      solution_design_base_->SolutionDesignFromPage();
  if (design == nullptr) {
    LOG_F(LG_ERROR) << "SolutionDesignFromPage failed";
    return;
  }
  DuiLib::TNotifyUI msg;
  msg.pSender = this->h_layout_args_area_;
  msg.sType = kValueChanged;
  ENMsgStruct enmsg;
  enmsg.ptr_ = design.get();
  enmsg.type_ = enmsg_type_exp_stress_amp;
  msg.wParam = reinterpret_cast<WPARAM>(&enmsg);
  tab_main_pages_["WorkWindowThirdPage"]->NotifyPump(msg);
  tab_main_pages_["WorkWindowSecondPage"]->NotifyPump(msg);
}

void WorkWindow::ClearArgsFreqNum() {
  // clear freq num
  btn_args_area_value_freq_num_->SetText(_T("0"));
}

void WorkWindow::UpdateArgsArea(int64_t cycle_count,
                                double freq,
                                double amplitude,
                                double static_load) {
  // update cycle count
  DuiLib::CDuiString value;
  value.Format(_T("%d"), cycle_count);
  btn_args_area_value_freq_num_->SetText(value);
}
}  // namespace ui
}  // namespace anx
