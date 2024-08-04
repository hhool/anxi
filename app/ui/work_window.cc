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

#include <utility>

#include "app/common/defines.h"
#include "app/common/string_utils.h"
#include "app/device/device_com_factory.h"
#include "app/device/device_com_settings.h"
#include "app/esolution/solution_design.h"
#include "app/esolution/solution_design_default.h"
#include "app/esolution/solution_design_helper.h"
#include "app/ui/dialog_about.h"
#include "app/ui/dialog_com_port_settings.h"
#include "app/ui/ui_constants.h"
#include "app/ui/work_window_menu_design.h"
#include "app/ui/work_window_menu_store.h"
#include "app/ui/work_window_status_bar.h"
#include "app/ui/work_window_tab_main_first_page_solution_design.h"
#include "app/ui/work_window_tab_main_second_page.h"
#include "app/ui/work_window_tab_main_third_page.h"

DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindow, DuiLib::WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_ON_MSGTYPE(DUI_MSGTYPE_TIMER, OnTimer)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {

namespace {
const int32_t kTimerCurrentTimeMsgId = 1;
const int32_t kTimerCurrentTimePeriod = 50;
}  // namespace

WorkWindow::WorkWindow(DuiLib::WindowImplBase* pOwner, int32_t solution_type)
    : pOwner_(pOwner), solution_type_(solution_type) {
  if (solution_type == anx::esolution::kSolutionName_Axially_Symmetrical) {
    anx::ui::WorkWindowFirstPageAxiallySymmetrical* page_axially =
        new anx::ui::WorkWindowFirstPageAxiallySymmetrical(this,
                                                           &m_PaintManager);
    solution_design_base_ = page_axially;
    tab_main_pages_["WorkWindowFirstPageAxiallySymmetrical"].reset(
        page_axially);
    this->AddVirtualWnd(_T("WorkWindowFirstPageAxiallySymmetrical"),
                        page_axially);
  } else if (solution_type ==
             anx::esolution::kSolutionName_Stresses_Adjustable) {
    anx::ui::WorkWindownFirstPageStressAjustable* page_stress =
        new anx::ui::WorkWindownFirstPageStressAjustable(this, &m_PaintManager);
    solution_design_base_ = page_stress;
    tab_main_pages_["WorkWindowFirstPageStressesAdjustable"].reset(page_stress);
    this->AddVirtualWnd(
        _T("WorkWindowFirstPageStressesAdjustable"),
        tab_main_pages_["WorkWindowFirstPageStressesAdjustable"].get());
  } else if (solution_type == anx::esolution::kSolutionName_Th3point_Bending) {
    anx::ui::WorkWindowFirstPageTh3pointBending* page_th3point =
        new anx::ui::WorkWindowFirstPageTh3pointBending(this, &m_PaintManager);
    solution_design_base_ = page_th3point;
    tab_main_pages_["WorkWindowFirstPageTh3pointBending"].reset(page_th3point);
    this->AddVirtualWnd(
        _T("WorkWindowFirstPageTh3pointBending"),
        tab_main_pages_["WorkWindowFirstPageTh3pointBending"].get());
  } else if (solution_type == anx::esolution::kSolutionName_Vibration_Bending) {
    anx::ui::WorkWindowFirstPageVibrationBending* page_vibration =
        new anx::ui::WorkWindowFirstPageVibrationBending(this, &m_PaintManager);
    solution_design_base_ = page_vibration;
    tab_main_pages_["WorkWindowFirstPageVibrationBending"].reset(
        page_vibration);
    this->AddVirtualWnd(
        _T("WorkWindowFirstPageVibrationBending"),
        tab_main_pages_["WorkWindowFirstPageVibrationBending"].get());
  } else {
    assert(false && "Invalid solution type");
  }
  anx::ui::WorkWindowSecondPage* second_page =
      new anx::ui::WorkWindowSecondPage(this, &m_PaintManager);
  work_window_second_page_virtual_wnd_ = second_page;
  tab_main_pages_["WorkWindowSecondPage"].reset(second_page);
  this->AddVirtualWnd(_T("WorkWindowSecondPage"),
                      tab_main_pages_["WorkWindowSecondPage"].get());

  anx::ui::WorkWindowThirdPage* third_page =
      new anx::ui::WorkWindowThirdPage(this, &m_PaintManager);
  work_window_third_page_virtual_wnd_ = third_page;
  tab_main_pages_["WorkWindowThirdPage"].reset(third_page);
  this->AddVirtualWnd(_T("WorkWindowThirdPage"),
                      tab_main_pages_["WorkWindowThirdPage"].get());

  anx::ui::WorkWindowStatusBar* work_windows_status_bar =
      new anx::ui::WorkWindowStatusBar(this, &m_PaintManager);
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

  work_window_second_page_virtual_wnd_->Unbind();
  work_window_second_page_virtual_wnd_ = nullptr;
  // remove tab_main_pages_
  tab_main_pages_.clear();

  work_window_status_bar_virtual_wnd_->Unbind();
  work_window_status_bar_virtual_wnd_ = nullptr;
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

  btn_menu_design_manager_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(_T("menu_btn_design_manager")));
  btn_menu_design_store_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(_T("menu_btn_design_store")));
  btn_menu_back_ =
      static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("menu_btn_back")));
  btn_menu_about_ =
      static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("menu_btn_about")));

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
  } else if (msg.sType == kMenu_Design_Modify_Addr) {
    // TODO(hhool):
    MessageBox(*this, msg.sType, msg.sType, MB_OK);
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
    }
  } else if (msg.sType == kMenu_Design_Exit) {
    // TODO(hhool):kMenu_Store_ExpRecord}
  } else if (msg.sType == kMenu_Store_ExpRecord) {
    // TODO(hhool):
    MessageBox(*this, msg.sType, msg.sType, MB_OK);
  } else if (msg.sType == kMenu_Store_ComRecord) {
    // TODO(hhool):
    MessageBox(*this, msg.sType, msg.sType, MB_OK);
  } else {
    // TODO(hhool):
  }
}

void WorkWindow::OnClick(DuiLib::TNotifyUI& msg) {
  if (msg.pSender == btn_close_) {
    if (pOwner_ != nullptr) {
      pOwner_->ShowWindow(true, true);
      this->Close();
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
    RECT rt = msg.pSender->GetClientPos();
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
    RECT rt = msg.pSender->GetClientPos();
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
      pOwner_->ShowWindow(true, true);
      this->Close();
    } else {
      PostQuitMessage(0);
    }
  } else if (msg.pSender == btn_menu_about_) {
    DialogAbout* about = new DialogAbout();
    about->Create(*this, _T("dialog_about"), UI_WNDSTYLE_FRAME,
                  WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
    about->CenterWindow();
    about->ShowModal();
  } else {
    // TODO(hhool):
  }
}

void anx::ui::WorkWindow::OnSelectChanged(DuiLib::TNotifyUI& msg) {
  DuiLib::CDuiString name = msg.pSender->GetName();
  DuiLib::CTabLayoutUI* pControl = static_cast<DuiLib::CTabLayoutUI*>(
      m_PaintManager.FindControl(_T("tab_main")));
  if (name == _T("body_tab_buton_specimen_design")) {
    pControl->SelectItem(0);
  } else if (name == _T("body_tab_buton_exp_exec")) {
    pControl = static_cast<DuiLib::CTabLayoutUI*>(
        m_PaintManager.FindControl(_T("tab_main")));
    pControl->SelectItem(1);
  } else if (name == _T("body_tab_button_static_carrier_aircraft")) {
    pControl = static_cast<DuiLib::CTabLayoutUI*>(
        m_PaintManager.FindControl(_T("tab_main")));
    pControl->SelectItem(2);
  } else {
    // do nothing;
  }
}

void anx::ui::WorkWindow::OnTimer(DuiLib::TNotifyUI& msg) {}

LRESULT WorkWindow::OnSysCommand(UINT uMsg,
                                 WPARAM wParam,
                                 LPARAM lParam,
                                 BOOL& bHandled) {
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

void WorkWindow::OnMenuDeviceConnectClicked(DuiLib::TNotifyUI& msg) {
  // create or get ultrasound device.
  if (device_com_ul_ == nullptr) {
    device_com_ul_ =
        anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
            anx::device::kDeviceCom_Ultrasound);
    if (device_com_ul_ != nullptr) {
      if (device_com_ul_->Open() != 0) {
        // show status bar message
        MessageBox(*this, _T("打开超声设备失败"), _T("打开失败"), MB_OK);
      }
    } else {
      // TODO(hhool): show status bar message
      MessageBox(*this, _T("创建超声设备失败"), _T("打开失败"), MB_OK);
    }
  }
  // open static load device.
  if (device_com_sl_ == nullptr) {
    device_com_sl_ =
        anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
            anx::device::kDeviceCom_StaticLoad);
    if (device_com_sl_ != nullptr) {
      if (device_com_sl_->Open() != 0) {
        // show status bar message
        MessageBox(*this, _T("打开静载设备失败"), _T("打开失败"), MB_OK);
      }
    } else {
      // TODO(hhool): show status bar message
      MessageBox(*this, _T("创建静载设备失败"), _T("打开失败"), MB_OK);
    }
  }
  if (device_com_ul_ == nullptr || device_com_sl_ == nullptr) {
    // show status bar message
    MessageBox(*this, _T("打开设备失败"), _T("打开失败"), MB_OK);
  } else {
    // status bar
  }
  // TODO(hhool): show status bar message
}

void WorkWindow::OnMenuDeviceDisconnectClicked(DuiLib::TNotifyUI& msg) {
  if (device_com_sl_ != nullptr) {
    device_com_sl_->Close();
  }
  if (device_com_ul_ != nullptr) {
    device_com_ul_->Close();
  }
  // TODO(hhooll): show status bar message
}

bool WorkWindow::IsDeviceComInterfaceConnected() const {
  return device_com_ul_ != nullptr && device_com_sl_ != nullptr;
}

bool WorkWindow::IsSLDeviceComInterfaceConnected() const {
  return device_com_sl_ != nullptr;
}

bool WorkWindow::IsULDeviceComInterfaceConnected() const {
  return device_com_ul_ != nullptr;
}

}  // namespace ui
}  // namespace anx
