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
#include "app/esolution/solution_design.h"
#include "app/esolution/solution_design_default.h"
#include "app/esolution/solution_design_helper.h"
#include "app/ui/dialog_com_port_settings.h"
#include "app/ui/dialog_exp_data_record.h"
#include "app/ui/ui_chart_label.h"
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
  device_com_ul_ =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_Ultrasound, this);

  device_com_sl_ =
      anx::device::DeviceComFactory::Instance()->CreateOrGetDeviceComWithType(
          anx::device::kDeviceCom_StaticLoad, this);

  // database initial
  // remove database
  anx::db::helper::ClearDatabaseFile(anx::db::helper::kDefaultDatabasePathname);
  // initial database
  std::vector<std::string> sqls;
  sqls.push_back(anx::db::helper::sql::kCreateTableAmpSqlFormat);
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

  work_window_second_page_virtual_wnd_->Unbind();
  work_window_second_page_virtual_wnd_ = nullptr;
  WorkWindowSecondPage* work_window_second_page =
      reinterpret_cast<WorkWindowSecondPage*>(
          tab_main_pages_["WorkWindowSecondPage"].release());
  delete work_window_second_page;

  work_window_status_bar_virtual_wnd_->Unbind();
  work_window_status_bar_virtual_wnd_ = nullptr;
  WorkWindowStatusBar* work_window_status_bar =
      reinterpret_cast<WorkWindowStatusBar*>(work_window_status_bar_.release());
  delete work_window_status_bar;
  // remove tab_main_pages_
  tab_main_pages_.clear();

  // close devices all
  CloseDeviceCom(anx::device::kDeviceCom_Ultrasound);
  CloseDeviceCom(anx::device::kDeviceCom_StaticLoad);
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

LRESULT WorkWindow::ResponseDefaultKeyEvent(WPARAM wParam, bool& bHandled) {
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
    // TODO(hhool): kMenu_Store_ExpRecord
  } else if (msg.sType == kMenu_Store_ExpRecord) {
    DialogExpDataRecord* dialog_exp_data_record = new DialogExpDataRecord();
    dialog_exp_data_record->Create(*this, _T("dialog_exp_data_record"),
                                   UI_WNDSTYLE_FRAME,
                                   WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
    dialog_exp_data_record->CenterWindow();
    dialog_exp_data_record->ShowModal();
  } else if (msg.sType == kMenu_Store_ComRecord) {
    // TODO(hhool):
    MessageBox(*this, msg.sType, msg.sType, MB_OK);
  } else {
    // DuiLib::WindowImplBase::Notify(msg);
    __super::Notify(msg);
  }
}

void WorkWindow::OnClick(DuiLib::TNotifyUI& msg) {
  if (msg.pSender == btn_close_) {
    if (pOwner_ != nullptr) {
      CloseDeviceCom(anx::device::kDeviceCom_Ultrasound);
      CloseDeviceCom(anx::device::kDeviceCom_StaticLoad);
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
      CloseDeviceCom(anx::device::kDeviceCom_Ultrasound);
      CloseDeviceCom(anx::device::kDeviceCom_StaticLoad);
      pOwner_->ShowWindow(true, true);
      this->Close();
    } else {
      PostQuitMessage(0);
    }
  } else {
    // TODO(hhool):
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
      CloseDeviceCom(anx::device::kDeviceCom_Ultrasound);
      CloseDeviceCom(anx::device::kDeviceCom_StaticLoad);
      pOwner_->ShowWindow(true, true);
      this->Close();
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

CControlUI* WorkWindow::CreateControl(LPCTSTR pstrClass) {
  // compare pstrClass with the control class name
  if (_tcscmp(pstrClass, _T("ChartLabel")) == 0) {
    ChartLabelUI* chart_label_ui = new ChartLabelUI();
    HWND hWnd = CreateWindow(_T("static"), _T("win32"),
                             WS_VISIBLE | WS_CHILD | WS_BORDER, 0, 0, 0, 0,
                             m_PaintManager.GetPaintWindow(), NULL, NULL, NULL);
    chart_label_ui->AttachGraph(hWnd);
    label_chart_uis_.push_back(chart_label_ui);
    return chart_label_ui;
  } else if (_tcscmp(pstrClass, _T("ChartLabel")) == 0) {
    ChartLabelUI* chart_label_ui = new ChartLabelUI();
    HWND hWnd = CreateWindow(_T("static"), _T("win32"),
                             WS_VISIBLE | WS_CHILD | WS_BORDER, 0, 0, 0, 0,
                             m_PaintManager.GetPaintWindow(), NULL, NULL, NULL);
    chart_label_ui->AttachGraph(hWnd);
    return chart_label_ui;
  }
  return __super::CreateControl(pstrClass);
}

LRESULT WorkWindow::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled) {
  for (auto label_chart_ui : label_chart_uis_) {
    HWND hwnd = label_chart_ui->DetachGraph();
    if (hwnd != nullptr) {
      ::DestroyWindow(hwnd);
    }
  }
  label_chart_uis_.clear();
  bHandled = FALSE;
  return __super::OnDestroy(0, 0, 0, bHandled);
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
  int32_t ret = OpenDeviceCom(anx::device::kDeviceCom_Ultrasound);
  if (ret == -1) {
    // TODO(hhool): show status bar message
  } else if (ret == -2) {
    // TODO(hhool): show status bar message
  }
  // open static load device.
  ret = OpenDeviceCom(anx::device::kDeviceCom_StaticLoad);
  if (ret == -1) {
    // TODO(hhool): show status bar message
  } else if (ret == -2) {
    // TODO(hhool): show status bar message
  }
}

void WorkWindow::OnMenuDeviceDisconnectClicked(DuiLib::TNotifyUI& msg) {
  CloseDeviceCom(anx::device::kDeviceCom_Ultrasound);
  CloseDeviceCom(anx::device::kDeviceCom_StaticLoad);
  // TODO(hhooll): show status bar message
}

bool WorkWindow::IsDeviceComInterfaceConnected() const {
  return (device_com_ul_ != nullptr && device_com_ul_->isOpened()) ||
         (device_com_sl_ != nullptr && device_com_sl_->isOpened());
}

bool WorkWindow::IsSLDeviceComInterfaceConnected() const {
  return device_com_sl_ != nullptr && device_com_sl_->isOpened();
}

bool WorkWindow::IsULDeviceComInterfaceConnected() const {
  return device_com_ul_ != nullptr && device_com_ul_->isOpened();
}

int32_t WorkWindow::OpenDeviceCom(int32_t device_type) {
  if (device_type == anx::device::kDeviceCom_Ultrasound) {
    if (!device_com_ul_->isOpened()) {
      std::unique_ptr<anx::device::ComSettings> com_settings =
          anx::device::LoadDeviceComSettingsDefaultResourceWithType(
              device_type);
      if (com_settings == nullptr) {
        return -1;
      }
      if (device_com_ul_->Open(*(reinterpret_cast<anx::device::ComPortDevice*>(
              com_settings.get()))) != 0) {
        // show status bar message
        return -2;
      }
    } else {
      // show status bar message
      return -3;
    }
  } else if (device_type == anx::device::kDeviceCom_StaticLoad) {
    if (!device_com_sl_->isOpened()) {
      std::unique_ptr<anx::device::ComSettings> com_settings =
          anx::device::LoadDeviceComSettingsDefaultResourceWithType(
              device_type);
      if (com_settings == nullptr) {
        return -1;
      }
      if (device_com_sl_->Open(*(reinterpret_cast<anx::device::ComPortDevice*>(
              com_settings.get()))) != 0) {
        // show status bar message
        return -2;
      }
    } else {
      // show status bar message
      return -3;
    }
  } else {
    assert(false && "Invalid device type");
  }
  return 0;
}

void WorkWindow::CloseDeviceCom(int32_t device_type) {
  if (device_type == anx::device::kDeviceCom_Ultrasound) {
    if (device_com_ul_ != nullptr) {
      device_com_ul_->Close();
      device_com_ul_->RemoveListener(this);
    }
  } else if (device_type == anx::device::kDeviceCom_StaticLoad) {
    if (device_com_sl_ != nullptr) {
      device_com_sl_->Close();
      device_com_sl_->RemoveListener(this);
    }
  } else {
    assert(false && "Invalid device type");
  }
}

void WorkWindow::OnDataReceived(anx::device::DeviceComInterface* device,
                                const uint8_t* data,
                                int32_t size) {
  if (is_exp_state_) {
    // TODO(hhool):
  }
}

void WorkWindow::OnDataOutgoing(anx::device::DeviceComInterface* device,
                                const uint8_t* data,
                                int32_t size) {
  // TODO(hhool):
}

void WorkWindow::OnExpStart() {
  // TODO(hhool):
  // 1. get solution design from control
  // 2. start experiment
  // 3. show status bar message
  is_exp_state_ = 1;
}

void WorkWindow::OnExpStop() {
  // TODO(hhool):
  // 1. stop experiment
  // 2. show status bar message
  is_exp_state_ = 0;
}

void WorkWindow::OnExpPause() {
  // TODO(hhool):
  // 1. pause experiment
  // 2. show status bar message

  is_exp_state_ = 2;
}

void WorkWindow::OnExpResume() {
  // TODO(hhool):
  // 1. resume experiment
  // 2. show status bar message
  is_exp_state_ = 1;
}

void WorkWindow::ClearArgsFreqNum() {
  // clear freq num
  btn_args_area_value_freq_num_->SetText(_T("0"));
}

}  // namespace ui
}  // namespace anx
