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

#include "app/common/defines.h"
#include "app/ui/dialog_about.h"
#include "app/ui/dialog_com_port_settings.h"
#include "app/ui/ui_constants.h"
#include "app/ui/work_window_menu_design.h"
#include "app/ui/work_window_menu_store.h"

DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindow, DuiLib::WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {

namespace {}

WorkWindow::WorkWindow(DuiLib::WindowImplBase* pOwner, int32_t solution_type)
    : pOwner_(pOwner), solution_type_(solution_type) {}

WorkWindow::~WorkWindow() {}

void WorkWindow::InitWindow() {
  __super::InitWindow();
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
  } else if (msg.sType == kClick) {
    if (msg.pSender->GetName() == kCloseButtonControlName) {
      if (pOwner_ != nullptr) {
        pOwner_->ShowWindow(true, true);
        this->Close();
      } else {
        PostQuitMessage(0);
      }
      return;
    } else if (msg.pSender->GetName() == kMaxButtonControlName) {
#if defined(UNDER_CE)
      ::ShowWindow(m_hWnd, SW_MAXIMIZE);
      CControlUI* pControl = static_cast<CControlUI*>(
          m_PaintManager.FindControl(kMaxButtonControlName));
      if (pControl)
        pControl->SetVisible(false);
      pControl = static_cast<CControlUI*>(
          m_PaintManager.FindControl(kRestoreButtonControlName));
      if (pControl)
        pControl->SetVisible(true);
#else
      SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
#endif
    } else if (msg.pSender->GetName() == kRestoreButtonControlName) {
#if defined(UNDER_CE)
      ::ShowWindow(m_hWnd, SW_RESTORE);
      CControlUI* pControl = static_cast<CControlUI*>(
          m_PaintManager.FindControl(kMaxButtonControlName));
      if (pControl)
        pControl->SetVisible(true);
      pControl = static_cast<CControlUI*>(
          m_PaintManager.FindControl(kRestoreButtonControlName));
      if (pControl)
        pControl->SetVisible(false);
#else
      SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
#endif
    } else if (msg.pSender->GetName() == kMinButtonControlName) {
#if defined(UNDER_CE)
      ::ShowWindow(m_hWnd, SW_MINIMIZE);
#else
      SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
#endif
    } else if (msg.pSender->GetName() == _T("menu_btn_design_manager")) {
      RECT rt = msg.pSender->GetClientPos();
      POINT pt = {0};
      pt.y = rt.bottom;
      pt.x = rt.left;
      ::ClientToScreen(*this, &pt);
      CMenuDesignWnd* pMenu = new CMenuDesignWnd();
      if (pMenu == NULL) {
        return;
      }
      pMenu->Init(msg.pSender, pt);
    } else if (msg.pSender->GetName() == _T("menu_btn_design_store")) {
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
    } else if (msg.pSender->GetName() == _T("menu_btn_back")) {
      if (pOwner_ != nullptr) {
        pOwner_->ShowWindow(true, true);
        this->Close();
      } else {
        PostQuitMessage(0);
      }
    } else if (msg.pSender->GetName() == _T("menu_btn_help")) {
      DuiLib::CDuiPoint point = msg.ptMouse;
      ClientToScreen(m_hWnd, &point);
    } else if (msg.pSender->GetName() == _T("menu_btn_about")) {
      DialogAbout* about = new DialogAbout();
      about->Create(*this, _T("dialog_about"), UI_WNDSTYLE_FRAME,
                    WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0);
      about->CenterWindow();
      about->ShowModal();
    } else if (msg.pSender->GetName() == _T("solution_design_refresh")) {
      ReadSolutionDesignFromControl();
    } else {
      // TODO(hhool):
    }
  } else if (msg.sType == _T("selectchanged")) {
    DuiLib::CDuiString name = msg.pSender->GetName();
    DuiLib::CTabLayoutUI* pControl = nullptr;

    if (name == _T("body_tab_buton_specimen_design")) {
      pControl = static_cast<DuiLib::CTabLayoutUI*>(
          m_PaintManager.FindControl(_T("tab_main")));
      pControl->SelectItem(0);
    } else if (name == _T("body_tab_buton_exp_exec")) {
      pControl = static_cast<DuiLib::CTabLayoutUI*>(
          m_PaintManager.FindControl(_T("tab_main")));
      pControl->SelectItem(1);
    } else if (name == _T("body_tab_button_static_carrier_aircraft")) {
      pControl = static_cast<DuiLib::CTabLayoutUI*>(
          m_PaintManager.FindControl(_T("tab_main")));
      pControl->SelectItem(2);
    } else if (name == _T("tab_graph")) {
      pControl = static_cast<DuiLib::CTabLayoutUI*>(
          m_PaintManager.FindControl(_T("tab_graph_data")));
      pControl->SelectItem(0);
    } else if (name == _T("tab_data")) {
      pControl = static_cast<DuiLib::CTabLayoutUI*>(
          m_PaintManager.FindControl(_T("tab_graph_data")));
      pControl->SelectItem(1);
    } else {
      // TODO(hhool): do nothing;
    }
  } else if (msg.sType == kMenu_Design_Connect) {
    // TODO(hhool):
    MessageBox(*this, msg.sType, msg.sType, MB_OK);
  } else if (msg.sType == kMenu_Design_DisConnect) {
    // TODO(hhool):
    MessageBox(*this, msg.sType, msg.sType, MB_OK);
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
    LoadFileWithDialog();
  } else if (msg.sType == kMenu_Design_Save_Solution) {
    SaveFileWithDialog();
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

void WorkWindow::OnClick(DuiLib::TNotifyUI& msg) {}

DuiLib::CDuiString WorkWindow::GetSkinFolder() {
#ifdef _DEBUG
  return _T("skin\\anxi\\used");
#else
  return _T("skin\\");
#endif
}

DuiLib::CDuiString WorkWindow::GetSkinFile() {
  return _T("work_window.xml");
}

DuiLib::UILIB_RESOURCETYPE WorkWindow::GetResourceType() const {
#ifdef _DEBUG
  return DuiLib::UILIB_FILE;
#else
  return DuiLib::UILIB_ZIP;
#endif
}

LPCTSTR WorkWindow::GetWindowClassName(void) const {
  return _T("work_window");
}

LRESULT WorkWindow::OnSysCommand(UINT uMsg,
                                 WPARAM wParam,
                                 LPARAM lParam,
                                 BOOL& bHandled) {
#if defined(WIN32) && !defined(UNDER_CE)
  BOOL bZoomed = ::IsZoomed(m_hWnd);
  LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
  if (::IsZoomed(m_hWnd) != bZoomed) {
    if (!bZoomed) {
      DuiLib::CControlUI* pControl = static_cast<DuiLib::CControlUI*>(
          m_PaintManager.FindControl(kMaxButtonControlName));
      if (pControl)
        pControl->SetVisible(false);
      pControl = static_cast<DuiLib::CControlUI*>(
          m_PaintManager.FindControl(kRestoreButtonControlName));
      if (pControl)
        pControl->SetVisible(true);
    } else {
      DuiLib::CControlUI* pControl = static_cast<DuiLib::CControlUI*>(
          m_PaintManager.FindControl(kMaxButtonControlName));
      if (pControl)
        pControl->SetVisible(true);
      pControl = static_cast<DuiLib::CControlUI*>(
          m_PaintManager.FindControl(kRestoreButtonControlName));
      if (pControl)
        pControl->SetVisible(false);
    }
  }
#else
  return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
#endif

  return 0;
}

void anx::ui::WorkWindow::OnPrepare(DuiLib::TNotifyUI& msg) {
  if (solution_type_ >= anx::common::kSolutionName_Axially_Symmetrical &&
      solution_type_ <= anx::common::kSolutionName_Vibration_Bending) {
    // update tab_main first page for different solution.
    UpdateTabMainFirstPageWithSolution(solution_type_);
    // update work_frame window titile for different solution
    UpdateWorkWindowTileWithSolution(solution_type_);
  } else {
    assert(false && "Invalid solution type");
  }
}

void anx::ui::WorkWindow::UpdateTabMainFirstPageWithSolution(
    int32_t solution_type) {
  if (solution_type == anx::common::kSolutionName_Axially_Symmetrical) {
    DuiLib::CTabLayoutUI* pControl = static_cast<DuiLib::CTabLayoutUI*>(
        m_PaintManager.FindControl(_T("tab_main")));
    if (pControl == nullptr)
      return;
    pControl->RemoveAt(1);
    pControl->RemoveAt(1);
    pControl->RemoveAt(1);

    pControl->GetItemAt(0)->SetVisible();
  } else if (solution_type == anx::common::kSolutionName_Stresses_Adjustable) {
    DuiLib::CTabLayoutUI* pControl = static_cast<DuiLib::CTabLayoutUI*>(
        m_PaintManager.FindControl(_T("tab_main")));
    if (pControl == nullptr)
      return;
    pControl->RemoveAt(0);
    pControl->RemoveAt(1);
    pControl->RemoveAt(1);

    pControl->GetItemAt(0)->SetVisible();
  } else if (solution_type == anx::common::kSolutionName_3point_Bending) {
    DuiLib::CTabLayoutUI* pControl = static_cast<DuiLib::CTabLayoutUI*>(
        m_PaintManager.FindControl(_T("tab_main")));
    if (pControl == nullptr)
      return;
    pControl->RemoveAt(0);
    pControl->RemoveAt(0);
    pControl->RemoveAt(1);

    pControl->GetItemAt(0)->SetVisible();
  } else if (solution_type == anx::common::kSolutionName_Vibration_Bending) {
    DuiLib::CTabLayoutUI* pControl = static_cast<DuiLib::CTabLayoutUI*>(
        m_PaintManager.FindControl(_T("tab_main")));
    if (pControl == nullptr)
      return;
    pControl->RemoveAt(0);
    pControl->RemoveAt(0);
    pControl->RemoveAt(0);

    pControl->GetItemAt(0)->SetVisible();
  } else {
    assert(false && "Invalid solution type");
  }
}

void anx::ui::WorkWindow::UpdateWorkWindowTileWithSolution(
    int32_t solution_type) {
  if (solution_type == anx::common::kSolutionName_Axially_Symmetrical) {
    CControlUI* desciption = m_PaintManager.FindControl(kMenu_Button_ExpTitle);
    if (desciption != NULL) {
      desciption->SetText(_T("轴向对称拉压疲劳试验"));
    }
  } else if (solution_type == anx::common::kSolutionName_Stresses_Adjustable) {
    CControlUI* desciption = m_PaintManager.FindControl(kMenu_Button_ExpTitle);
    if (desciption != NULL) {
      desciption->SetText(_T("应力可调疲劳试验"));
    }
  } else if (solution_type == anx::common::kSolutionName_3point_Bending) {
    CControlUI* desciption = m_PaintManager.FindControl(kMenu_Button_ExpTitle);
    if (desciption != NULL) {
      desciption->SetText(_T("三点弯曲疲劳试验"));
    }
  } else if (solution_type == anx::common::kSolutionName_Vibration_Bending) {
    CControlUI* desciption = m_PaintManager.FindControl(kMenu_Button_ExpTitle);
    if (desciption != NULL) {
      desciption->SetText(_T("振动弯曲疲劳试验"));
    }
  } else {
    assert(false && "Invalid solution type");
  }
}

void WorkWindow::UpadateTabMainFirstPageWithFilePath(
    const std::string& file_path) {}

LRESULT WorkWindow::OnCreate(UINT uMsg,
                             WPARAM wParam,
                             LPARAM lParam,
                             BOOL& bHandled) {
  return DuiLib::WindowImplBase::OnCreate(uMsg, wParam, lParam, bHandled);
}

LRESULT WorkWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  LRESULT lRes = 0;
  BOOL bHandled = TRUE;
  switch (uMsg) {
    case WM_CREATE:
      lRes = OnCreate(uMsg, wParam, lParam, bHandled);
      break;
    case WM_CLOSE:
      lRes = OnClose(uMsg, wParam, lParam, bHandled);
      break;
    default:
      bHandled = FALSE;
  }
  if (bHandled)
    return lRes;
  return DuiLib::WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

void WorkWindow::LoadFileWithDialog() {
  OPENFILENAME ofn;
  TCHAR FileName[MAX_PATH];
  memset(&ofn, 0, sizeof(OPENFILENAME));
  memset(FileName, 0, sizeof(char) * MAX_PATH);
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = m_hWnd;
  ofn.lpstrFilter = _T("*.dat");
  ofn.lpstrFile = FileName;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_FILEMUSTEXIST;
  if (GetOpenFileName(&ofn)) {
    // TODO(hhool): load file
    MessageBox(NULL, FileName, _T("加载成功"), NULL);
  }
}

void WorkWindow::SaveFileWithDialog() {
  OPENFILENAME ofn;
  TCHAR FileName[MAX_PATH];
  memset(&ofn, 0, sizeof(OPENFILENAME));
  memset(FileName, 0, sizeof(char) * MAX_PATH);
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = m_hWnd;
  ofn.lpstrFilter = _T("*.dat");
  ofn.lpstrFile = FileName;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_FILEMUSTEXIST;
  if (GetSaveFileName(&ofn)) {
    // TODO(hhool): save file
    MessageBox(NULL, FileName, _T("保存成功"), NULL);
  }
}

void WorkWindow::ReadSolutionDesignFromControl() {
  if (solution_type_ == anx::common::kSolutionName_Axially_Symmetrical) {
    // read axially symmetrical solution design from control
    DuiLib::CDuiString name_specimensa =
        _T("tm_page_first_left_name_specimens");
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_specimensa + _T("_axially")));
    DuiLib::CDuiString value = edit->GetText().GetData();

    DuiLib::CDuiString name_material = _T("tm_page_first_left_material");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_material + _T("_axially")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_elastic = _T("tm_page_first_left_elastic");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_elastic + _T("_axially")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_density = _T("tm_page_first_left_density");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_density + _T("_axially")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_max_stress = _T("tm_page_first_left_max_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_max_stress + _T("_axially")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_ratio_stress =
        _T("tm_page_first_left_ratio_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_ratio_stress + _T("_axially")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_amplitude + _T("_axially")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_dc_stress + _T("_axially")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_radius_exp = _T("tm_page_first_left_radius_exp");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radius_exp + _T("_axially")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_radius_parallel =
        _T("tm_page_first_left_radus_parallel");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radius_parallel + _T("_axially")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_radus_trans = _T("tm_page_first_left_radus_trans");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radus_trans + _T("_axially")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_length_trans =
        _T("tm_page_first_left_length_trans");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_length_trans + _T("_axially")));

    DuiLib::CDuiString name_length_parallel =
        _T("tm_page_first_left_length_parallel");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_length_parallel + _T("_axially")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_exp_section = _T("tm_page_first_exp_section_trans");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_exp_section + _T("_axially")));
    value = edit->GetText().GetData();
  } else if (solution_type_ == anx::common::kSolutionName_Stresses_Adjustable) {
    // read stresses adjustable solution design from control
    DuiLib::CDuiString name_specimensa =
        _T("tm_page_first_left_name_specimens");
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_specimensa + _T("_stresses")));
    DuiLib::CDuiString value = edit->GetText().GetData();

    DuiLib::CDuiString name_material = _T("tm_page_first_left_material");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_material + _T("_stresses")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_elastic = _T("tm_page_first_left_elastic");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_elastic + _T("_stresses")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_density = _T("tm_page_first_left_density");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_density + _T("_stresses")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_max_stress = _T("tm_page_first_left_max_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_max_stress + _T("_stresses")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_ratio_stress =
        _T("tm_page_first_left_ratio_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_ratio_stress + _T("_stresses")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_amplitude + _T("_stresses")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_dc_stress + _T("_stresses")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_static_load = _T("tm_page_first_left_static_load");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_static_load + _T("_stresses")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_radius_exp = _T("tm_page_first_left_radius_exp");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radius_exp + _T("_stresses")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_radius_parallel =
        _T("tm_page_first_left_radus_parallel");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radius_parallel + _T("_stresses")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_radus_trans = _T("tm_page_first_left_radus_trans");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radus_trans + _T("_stresses")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_length_trans =
        _T("tm_page_first_left_length_trans");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_length_trans + _T("_stresses")));

    DuiLib::CDuiString name_length_parallel =
        _T("tm_page_first_left_length_parallel");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_length_parallel + _T("_stresses")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_exp_section = _T("tm_page_first_exp_section_trans");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_exp_section + _T("_stresses")));
    value = edit->GetText().GetData();
  } else if (solution_type_ == anx::common::kSolutionName_3point_Bending) {
    // read th3point bending solution design from control
    DuiLib::CDuiString name_specimensa =
        _T("tm_page_first_left_name_specimens");
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_specimensa + _T("_th3point")));
    DuiLib::CDuiString value = edit->GetText().GetData();

    DuiLib::CDuiString name_elastic = _T("tm_page_first_left_elastic");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_elastic + _T("_th3point")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_density = _T("tm_page_first_left_density");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_density + _T("_th3point")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_max_stress = _T("tm_page_first_left_max_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_max_stress + _T("_th3point")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_ratio_stress =
        _T("tm_page_first_left_ratio_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_ratio_stress + _T("_th3point")));
    value = edit->GetText().GetData();

    //////////////////////////////////////////////////////////////////////////
    DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_amplitude + _T("_th3point")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_dc_stress + _T("_th3point")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_static_load = _T("tm_page_first_left_static_load");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_static_load + _T("_th3point")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_specimens_width =
        _T("tm_page_first_left_specimens_width");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_specimens_width + _T("_th3point")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_specimens_height =
        _T("tm_page_first_left_specimens_height");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_specimens_height + _T("_th3point")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_specimens_length =
        _T("tm_page_first_left_specimens_length");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_specimens_length + _T("_th3point")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_length_parallel =
        _T("tm_page_first_left_length_parallel");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_length_parallel + _T("_th3point")));
    value = edit->GetText().GetData();
  } else if (solution_type_ == anx::common::kSolutionName_Vibration_Bending) {
    // read th3point bending solution design from control
    DuiLib::CDuiString name_specimensa =
        _T("tm_page_first_left_name_specimens");
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_specimensa + _T("_vibration")));
    DuiLib::CDuiString value = edit->GetText().GetData();

    DuiLib::CDuiString name_elastic = _T("tm_page_first_left_elastic");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_elastic + _T("_vibration")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_density = _T("tm_page_first_left_density");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_density + _T("_vibration")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_max_stress = _T("tm_page_first_left_max_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_max_stress + _T("_vibration")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_ratio_stress =
        _T("tm_page_first_left_ratio_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_ratio_stress + _T("_vibration")));
    value = edit->GetText().GetData();

    //////////////////////////////////////////////////////////////////////////
    DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_amplitude + _T("_vibration")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_dc_stress + _T("_vibration")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_radius_exp = _T("tm_page_first_left_radius_exp");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radius_exp + _T("_vibration")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_radius_parallel =
        _T("tm_page_first_left_radus_parallel");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radius_parallel + _T("_vibration")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_radus_trans = _T("tm_page_first_left_radus_trans");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radus_trans + _T("_vibration")));
    value = edit->GetText().GetData();

    DuiLib::CDuiString name_length_trans =
        _T("tm_page_first_left_length_trans");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_length_trans + _T("_vibration")));
    value = edit->GetText().GetData();
  } else {
    assert(false && "Invalid solution type");
  }
}

}  // namespace ui
}  // namespace anx
