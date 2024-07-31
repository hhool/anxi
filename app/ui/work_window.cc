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
#include "app/esolution/solution_design.h"
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

namespace {

std::string WCharToMByte(LPCWSTR lpcwszStr) {
  string str;
  DWORD dwMinSize = 0;
  LPSTR lpszStr = NULL;
  dwMinSize =
      WideCharToMultiByte(CP_OEMCP, NULL, lpcwszStr, -1, NULL, 0, NULL, FALSE);
  if (0 == dwMinSize) {
    return FALSE;
  }
  lpszStr = reinterpret_cast<LPSTR>(malloc(sizeof(char) * dwMinSize));
  WideCharToMultiByte(CP_OEMCP, NULL, lpcwszStr, -1, lpszStr, dwMinSize, NULL,
                      FALSE);
  str = lpszStr;
  free(lpszStr);
  return str;
}

std::wstring string2wstring(const std::string& s) {
  int len;
  size_t slength = s.length() + 1;
  len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
  wchar_t* buf = reinterpret_cast<wchar_t*>(malloc(sizeof(wchar_t) * len));
  MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
  std::wstring r = buf;
  free(buf);
  return r;
}
}  // namespace

WorkWindow::WorkWindow(DuiLib::WindowImplBase* pOwner, int32_t solution_type)
    : pOwner_(pOwner), solution_type_(solution_type) {}

WorkWindow::~WorkWindow() {}

void WorkWindow::InitWindow() {
  __super::InitWindow();
  btn_close_ =
      static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("closebtn")));
  btn_max_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("maxbtn")));
  btn_restore_ =
      static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("restorebtn")));
  btn_min_ = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("minbtn")));

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

  btn_solution_refresh_ = static_cast<CButtonUI*>(
      m_PaintManager.FindControl(_T("solution_design_refresh")));
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
    OnClick(msg);
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
    int32_t ret = LoadFileWithDialog();
    if (ret < 0) {
      MessageBox(*this, _T("加载失败"), _T("加载失败"), MB_OK);
      return;
    } else if (ret == 0) {
      if (UpdateSolutionDesignControl() != 0) {
        MessageBox(*this, _T("更新失败"), _T("更新失败"), MB_OK);
        return;
      }
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
    CMenuDesignWnd* pMenu = new CMenuDesignWnd();
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
  } else if (msg.pSender == btn_solution_refresh_) {
    if (SolutionDesignCalculateWithControl() == 0) {
      std::string xml = SolutionDesignXmlFromControl();
    } else {
      // TODO(hhoo): do nothing;
    }
  } else {
    // TODO(hhool):
  }
}

DuiLib::CDuiString WorkWindow::GetSkinFolder() {
#ifdef _DEBUG
  return _T("skin\\");
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

  return 0;
}

void WorkWindow::OnPrepare(DuiLib::TNotifyUI& msg) {
  if (solution_type_ >= anx::esolution::kSolutionName_Axially_Symmetrical &&
      solution_type_ <= anx::esolution::kSolutionName_Vibration_Bending) {
    // update tab_main first page for different solution.
    UpdateTabMainFirstPageWithSolution(solution_type_);
    // update work_frame window titile for different solution
    UpdateWorkWindowTileWithSolution(solution_type_);
  } else {
    assert(false && "Invalid solution type");
  }
}

void WorkWindow::UpdateTabMainFirstPageWithSolution(int32_t solution_type) {
  if (solution_type == anx::esolution::kSolutionName_Axially_Symmetrical) {
    DuiLib::CTabLayoutUI* pControl = static_cast<DuiLib::CTabLayoutUI*>(
        m_PaintManager.FindControl(_T("tab_main")));
    if (pControl == nullptr)
      return;
    pControl->RemoveAt(1);
    pControl->RemoveAt(1);
    pControl->RemoveAt(1);

    pControl->GetItemAt(0)->SetVisible();
  } else if (solution_type ==
             anx::esolution::kSolutionName_Stresses_Adjustable) {
    DuiLib::CTabLayoutUI* pControl = static_cast<DuiLib::CTabLayoutUI*>(
        m_PaintManager.FindControl(_T("tab_main")));
    if (pControl == nullptr)
      return;
    pControl->RemoveAt(0);
    pControl->RemoveAt(1);
    pControl->RemoveAt(1);

    pControl->GetItemAt(0)->SetVisible();
  } else if (solution_type == anx::esolution::kSolutionName_Th3point_Bending) {
    DuiLib::CTabLayoutUI* pControl = static_cast<DuiLib::CTabLayoutUI*>(
        m_PaintManager.FindControl(_T("tab_main")));
    if (pControl == nullptr)
      return;
    pControl->RemoveAt(0);
    pControl->RemoveAt(0);
    pControl->RemoveAt(1);

    pControl->GetItemAt(0)->SetVisible();
  } else if (solution_type == anx::esolution::kSolutionName_Vibration_Bending) {
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

void WorkWindow::UpdateWorkWindowTileWithSolution(int32_t solution_type) {
  if (solution_type == anx::esolution::kSolutionName_Axially_Symmetrical) {
    CControlUI* desciption = m_PaintManager.FindControl(kMenu_Button_ExpTitle);
    if (desciption != NULL) {
      desciption->SetText(_T("轴向对称拉压疲劳试验"));
    }
  } else if (solution_type ==
             anx::esolution::kSolutionName_Stresses_Adjustable) {
    CControlUI* desciption = m_PaintManager.FindControl(kMenu_Button_ExpTitle);
    if (desciption != NULL) {
      desciption->SetText(_T("可调应力轴向拉压疲劳试验"));
    }
  } else if (solution_type == anx::esolution::kSolutionName_Th3point_Bending) {
    CControlUI* desciption = m_PaintManager.FindControl(kMenu_Button_ExpTitle);
    if (desciption != NULL) {
      desciption->SetText(_T("三点弯曲疲劳试验"));
    }
  } else if (solution_type == anx::esolution::kSolutionName_Vibration_Bending) {
    CControlUI* desciption = m_PaintManager.FindControl(kMenu_Button_ExpTitle);
    if (desciption != NULL) {
      desciption->SetText(_T("振动弯曲疲劳试验"));
    }
  } else {
    assert(false && "Invalid solution type");
  }
  // load default solution design
  std::string default_xml = DefaultSolutionDesignXml(solution_type);
  // get module path
  char module_path[MAX_PATH] = {0};
  GetModuleFileNameA(NULL, module_path, MAX_PATH);
  std::string module_dir = module_path;
  size_t pos = module_dir.find_last_of("\\/");
  module_dir = module_dir.substr(0, pos);
  default_xml = module_dir + "\\" + default_xml;
  if (0 == LoadSolutionDesignFile(default_xml)) {
    UpdateSolutionDesignControl();
  }
}

int32_t WorkWindow::UpadateTabMainFirstPageElementViewHeaderAndBaseParam() {
  if (solution_design_ == nullptr) {
    return -1;
  }

  anx::esolution::ExpDesignHeader* header = solution_design_->header_.get();
  if (header == nullptr) {
    return -2;
  }

  anx::esolution::ExpDesignBaseParam* base_param =
      solution_design_->base_param_.get();
  if (base_param == nullptr) {
    return -3;
  }
  // checkout header->solution_type_
  if (header->solution_type_ <
          anx::esolution::kSolutionName_Axially_Symmetrical ||
      header->solution_type_ >
          anx::esolution::kSolutionName_Vibration_Bending) {
    return -4;
  }

  // get tail prefix
  DuiLib::CDuiString tail_prefix;
  tail_prefix.Append(
      string2wstring(anx::esolution::ToTailPrefixName(header->solution_type_))
          .c_str());

  // update header
  {
    DuiLib::CDuiString name_specimensa =
        _T("tm_page_first_left_name_specimens");
    name_specimensa.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_specimensa));
    std::string name;
    name.assign(header->name_, header->name_ + 255);

    std::wstring stemp = string2wstring(name);  // Temporary buffer is required
    LPCWSTR result = stemp.c_str();
    edit->SetText(result);
  }

  // update base param
  {
    DuiLib::CDuiString name_material = _T("tm_page_first_left_material");
    name_material.Append(tail_prefix);
    DuiLib::CComboUI* edit = static_cast<DuiLib::CComboUI*>(
        m_PaintManager.FindControl(name_material));
    if (edit != nullptr) {
      std::string material_name;
      material_name.assign(base_param->material_name_,
                           base_param->material_name_ + 255);
      LPCSTR result = material_name.c_str();
      std::wstring result_str(string2wstring(result));
      int count = edit->GetCount();
      for (int i = 0; i < count; i++) {
        DuiLib::CDuiString text = edit->GetItemAt(i)->GetText();
        // compare text with result_str
        // convert text to wstring
        std::wstring text_str(text.GetData());
        // compare text_str with result_str
        if (text_str == result_str) {
          edit->SelectItem(i);
          break;
        }
      }
    }
  }
  {
    DuiLib::CDuiString name_elastic = _T("tm_page_first_left_elastic");
    name_elastic.Append(tail_prefix);
    DuiLib::CEditUI* edit =
        static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl(name_elastic));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), base_param->f_elastic_modulus_GPa_);
    edit->SetText(value);
  }
  {
    DuiLib::CDuiString name_density = _T("tm_page_first_left_density");
    name_density.Append(tail_prefix);
    DuiLib::CEditUI* edit =
        static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl(name_density));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), base_param->f_density_kg_m3_);
    edit->SetText(value);
  }

  {
    DuiLib::CDuiString name_max_stress = _T("tm_page_first_left_max_stress");
    name_max_stress.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_max_stress));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), base_param->f_max_stress_MPa_);
    edit->SetText(value);
  }

  {
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), base_param->f_max_stress_MPa_);
    btn_args_area_value_max_stress_->SetText(value);
  }

  {
    DuiLib::CDuiString name_stress_ratio =
        _T("tm_page_first_left_ratio_stress");
    name_stress_ratio.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_stress_ratio));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), base_param->f_stress_ratio_);
    edit->SetText(value);
  }

  {
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), base_param->f_stress_ratio_);
    btn_args_area_value_stress_ratio_->SetText(value);
  }

  return 0;
}

void WorkWindow::UpadateTabMainFirstPageElementViewResult() {
  // get header and base param and result from solution_design_
  anx::esolution::ExpDesignHeader* header = solution_design_->header_.get();
  anx::esolution::ExpDesignBaseParam* base_param =
      solution_design_->base_param_.get();
  anx::esolution::ExpDesignResult* result = solution_design_->result_.get();
  // checkout header->solution_type_
  if (header->solution_type_ <
          anx::esolution::kSolutionName_Axially_Symmetrical ||
      header->solution_type_ >
          anx::esolution::kSolutionName_Vibration_Bending) {
    return;
  }
  // get tail prefix
  DuiLib::CDuiString tail_prefix = _T("");
  if (header->solution_type_ ==
      anx::esolution::kSolutionName_Axially_Symmetrical) {
    tail_prefix = _T("_axially");
    // update result
    anx::esolution::ExpDesignResultAxially* result_axially =
        reinterpret_cast<anx::esolution::ExpDesignResultAxially*>(result);
    {
      DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
      name_amplitude.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_amplitude));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_axially->f_eamplitude_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_axially->f_eamplitude_);
      btn_args_area_value_amplitude_->SetText(value);
    }
    {
      DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
      name_dc_stress.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_dc_stress));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_axially->f_dc_stress_MPa_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_radius_exp = _T("tm_page_first_left_radius_exp");
      name_radius_exp.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_radius_exp));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_axially->f_exp_section_radius_R2_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_radius_parallel =
          _T("tm_page_first_left_radus_parallel");
      name_radius_parallel.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_radius_parallel));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_axially->f_parallel_section_radius_R1_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_radus_trans =
          _T("tm_page_first_left_radus_trans");
      name_radus_trans.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_radus_trans));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_axially->f_transition_section_radius_R0_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_length_trans =
          _T("tm_page_first_left_length_trans");
      name_length_trans.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_length_trans));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_axially->f_transition_section_length_L1_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_length_parallel =
          _T("tm_page_first_left_length_parallel");
      name_length_parallel.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_length_parallel));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_axially->f_parallel_section_length_L0_);
      edit->SetText(value);
    }
  } else if (header->solution_type_ ==
             anx::esolution::kSolutionName_Stresses_Adjustable) {
    tail_prefix = _T("_stresses");
    // update result
    anx::esolution::ExpDesignResultStressesAdjustable* result_stresses =
        reinterpret_cast<anx::esolution::ExpDesignResultStressesAdjustable*>(
            result);
    {
      DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
      name_amplitude.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_amplitude));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_stresses->f_eamplitude_);
      edit->SetText(value);
    }

    {
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_stresses->f_eamplitude_);
      btn_args_area_value_amplitude_->SetText(value);
    }

    {
      DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
      name_dc_stress.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_dc_stress));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_stresses->f_dc_stress_MPa_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_radius_exp = _T("tm_page_first_left_radius_exp");
      name_radius_exp.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_radius_exp));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_stresses->f_exp_section_radius_R2_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_radius_parallel =
          _T("tm_page_first_left_radus_parallel");
      name_radius_parallel.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_radius_parallel));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_stresses->f_parallel_section_radius_R1_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_radus_trans =
          _T("tm_page_first_left_radus_trans");
      name_radus_trans.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_radus_trans));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"),
                   result_stresses->f_transition_section_radius_R0_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_length_trans =
          _T("tm_page_first_left_length_trans");
      name_length_trans.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_length_trans));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"),
                   result_stresses->f_transition_section_length_L1_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_length_parallel =
          _T("tm_page_first_left_length_parallel");
      name_length_parallel.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_length_parallel));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_stresses->f_parallel_section_length_L0_);
      edit->SetText(value);
    }
  } else if (header->solution_type_ ==
             anx::esolution::kSolutionName_Stresses_Adjustable) {
    tail_prefix = _T("_stresses");
    // update result
    anx::esolution::ExpDesignResultStressesAdjustable* result_stresses =
        reinterpret_cast<anx::esolution::ExpDesignResultStressesAdjustable*>(
            result);
    {
      DuiLib::CDuiString name_amplitude_load =
          _T("tm_page_first_left_amplitude_load");
      name_amplitude_load.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_amplitude_load));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_stresses->f_eamplitude_);
      edit->SetText(value);
    }

    {
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_stresses->f_eamplitude_);
      btn_args_area_value_amplitude_->SetText(value);
    }
    {
      DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
      name_dc_stress.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_dc_stress));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_stresses->f_dc_stress_MPa_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_static_load =
          _T("tm_page_first_left_static_load");
      name_static_load.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_static_load));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_stresses->f_static_load_MPa_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_radius_exp = _T("tm_page_first_left_radius_exp");
      name_radius_exp.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_radius_exp));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_stresses->f_exp_section_radius_R2_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_radius_load = _T("tm_page_first_exp_section");
      name_radius_load.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_radius_load));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_stresses->f_exp_section_length_L2_);
      edit->SetText(value);
    }

  } else if (header->solution_type_ ==
             anx::esolution::kSolutionName_Th3point_Bending) {
    tail_prefix = _T("_th3point");
    // update result
    anx::esolution::ExpDesignResultTh3pointBending* result_th3point =
        reinterpret_cast<anx::esolution::ExpDesignResultTh3pointBending*>(
            result);
    {
      DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
      name_amplitude.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_amplitude));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_th3point->f_eamplitude_um_);
      edit->SetText(value);
    }

    {
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_th3point->f_eamplitude_um_);
      btn_args_area_value_amplitude_->SetText(value);
    }
    {
      DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
      name_dc_stress.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_dc_stress));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_th3point->f_dc_stress_MPa_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_static_load =
          _T("tm_page_first_left_static_load");
      name_static_load.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_static_load));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_th3point->f_static_load_MPa_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_speciments_width =
          _T("tm_page_first_left_speciments_width");
      name_speciments_width.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_speciments_width));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_th3point->f_specimen_width_B_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_speciments_height =
          _T("tm_page_first_left_speciments_height");
      name_speciments_height.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_speciments_height));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_th3point->f_specimen_thickness_h_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_speciments_length =
          _T("tm_page_first_left_speciments_length");
      name_speciments_length.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_speciments_length));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_th3point->f_specimen_length_L_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_length_parallel =
          _T("tm_page_first_left_length_parallel");
      name_length_parallel.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_length_parallel));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_th3point->f_support_distance_L0_);
      edit->SetText(value);
    }
  } else if (header->solution_type_ ==
             anx::esolution::kSolutionName_Vibration_Bending) {
    tail_prefix = _T("_vibration");
    // update result
    anx::esolution::ExpDesignResultVibrationBending* result_vibration =
        reinterpret_cast<anx::esolution::ExpDesignResultVibrationBending*>(
            result);
    {
      DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
      name_amplitude.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_amplitude));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_vibration->f_eamplitude_um_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_vibration->f_eamplitude_um_);
      btn_args_area_value_amplitude_->SetText(value);
    }
    {
      DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
      name_dc_stress.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_dc_stress));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_vibration->f_dc_stress_MPa_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_radius_exp =
          _T("tm_page_first_left_length_parallel_section");
      name_radius_exp.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_radius_exp));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"),
                   result_vibration->f_specimen_length_parallel_section_L1_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_radius_load =
          _T("tm_page_first_left_specimen_radius_arc");
      name_radius_load.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_radius_load));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_vibration->f_specimen_radius_arc_R1_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_radius_trans =
          _T("tm_page_first_left_specimen_radius_transition");
      name_radius_trans.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_radius_trans));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"),
                   result_vibration->f_specimen_radius_transition_R2_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_thickness_clamp =
          _T("tm_page_first_left_thickness_clamp_section");
      name_thickness_clamp.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_thickness_clamp));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"), result_vibration->f_thickness_clamping_d1_);
      edit->SetText(value);
    }
    {
      DuiLib::CDuiString name_thickness_exp =
          _T("tm_page_first_left_thickness_exp_section");
      name_thickness_exp.Append(tail_prefix);
      DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
          m_PaintManager.FindControl(name_thickness_exp));
      DuiLib::CDuiString value;
      value.Format(_T("%.2f"),
                   result_vibration->f_thickness_exp_section_L0_d2_);
      edit->SetText(value);
    }
  } else {
    assert(false && "Invalid solution type");
    return;
  }
}

std::string WorkWindow::DefaultSolutionDesignXml(int32_t solution_type) {
  if (solution_type == anx::esolution::kSolutionName_Axially_Symmetrical) {
    return "default/default_axially_symmetrical.xml";
  } else if (solution_type ==
             anx::esolution::kSolutionName_Stresses_Adjustable) {
    return "default/default_stresses_adjustable.xml";
  } else if (solution_type == anx::esolution::kSolutionName_Th3point_Bending) {
    return "default/default_th3point_bending.xml";
  } else if (solution_type == anx::esolution::kSolutionName_Vibration_Bending) {
    return "default/default_vibration_bending.xml";
  } else {
    assert(false && "Invalid solution type");
    return "";
  }
}

int32_t WorkWindow::LoadSolutionDesignFile(const std::string& file_path) {
  // open file with file_path
  FILE* file = fopen(file_path.c_str(), "rb");
  if (file == NULL) {
    return -1;
  }
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);
  // read file
  std::unique_ptr<char[]> buffer(new char[size]);
  fread(buffer.get(), size, 1, file);
  fclose(file);
  // parse xml
  std::string xml(buffer.get(), size);
  anx::esolution::SolutionDesign solution_design;
  if (anx::esolution::SolutionDesign::FromXml(xml, &solution_design) != 0) {
    return -2;
  }
  if (solution_design.header_->solution_type_ != this->solution_type_) {
    return -3;
  }
  solution_design_.reset(new anx::esolution::SolutionDesign(solution_design));
  return 0;
}

int32_t WorkWindow::SaveSolutionDesignFile(const std::string& file_path) {
  std::string xml = SolutionDesignXmlFromControl();
  if (xml.length() == 0) {
    return -1;
  }
  FILE* file = fopen(file_path.c_str(), "wb");
  if (file == NULL) {
    return -2;
  }
  fwrite(xml.c_str(), xml.size(), 1, file);
  fclose(file);
  return 0;
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
  std::string filepath = WCharToMByte(ofn.lpstrFile);
  return LoadSolutionDesignFile(filepath);
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
  std::string filepath = WCharToMByte(ofn.lpstrFile);
  // get file extension
  std::string extension = filepath.substr(filepath.find_last_of(".") + 1);
  if (extension != "xml") {
    // append .xml to filepath
    filepath += ".xml";
  }
  if (SaveSolutionDesignFile(filepath) != 0) {
    return -1;
  }
  return 0;
}

int32_t WorkWindow::UpdateSolutionDesignControl() {
  if (solution_design_ == nullptr) {
    return -1;
  }
  anx::esolution::ExpDesignHeader* header = solution_design_->header_.get();
  anx::esolution::ExpDesignBaseParam* base_param =
      solution_design_->base_param_.get();
  anx::esolution::ExpDesignResult* result = solution_design_->result_.get();
  if (header == nullptr || base_param == nullptr || result == nullptr) {
    return -2;
  }

  // check solution type range
  if (header->solution_type_ >=
          anx::esolution::kSolutionName_Axially_Symmetrical &&
      header->solution_type_ <=
          anx::esolution::kSolutionName_Vibration_Bending) {
    // update tab_main first page element view header and base param
    UpadateTabMainFirstPageElementViewHeaderAndBaseParam();
    // update tab_main first page element view result
    UpadateTabMainFirstPageElementViewResult();
  } else {
    assert(false && "Invalid solution type");
    return -3;
  }
  return 0;
}

int32_t WorkWindow::SolutionDesignCalculateWithControl() {
  // TODO(hhool): calculate solution design from control
  // check control value
  if (solution_type_ == anx::esolution::kSolutionName_Axially_Symmetrical) {
    DuiLib::CDuiString name_specimensa =
        _T("tm_page_first_left_name_specimens");
    DuiLib::CDuiString tail_prefix = _T("_axially");
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_specimensa + tail_prefix));
    DuiLib::CDuiString value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("试验名称不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_material = _T("tm_page_first_left_material");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_material + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("材料名称不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_elastic = _T("tm_page_first_left_elastic");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_elastic + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("弹性模量不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_density = _T("tm_page_first_left_density");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_density + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("密度不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_max_stress = _T("tm_page_first_left_max_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_max_stress + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("最大应力不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_ratio_stress =
        _T("tm_page_first_left_ratio_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_ratio_stress + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("应力比不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_amplitude + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("应变幅值不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_dc_stress + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("直流应力不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_radius_exp = _T("tm_page_first_left_radius_exp");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radius_exp + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("试验段半径不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_radius_parallel =
        _T("tm_page_first_left_radus_parallel");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radius_parallel + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("平行段半径不能为空"), _T("错误"), NULL);
      return -1;
    }
  } else if (solution_type_ ==
             anx::esolution::kSolutionName_Stresses_Adjustable) {
    // read stresses adjustable solution design from control
    DuiLib::CDuiString name_specimensa =
        _T("tm_page_first_left_name_specimens");
    DuiLib::CDuiString tail_prefix = _T("_stresses");
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_specimensa + tail_prefix));
    DuiLib::CDuiString value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("试验名称不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_material = _T("tm_page_first_left_material");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_material + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("材料名称不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_elastic = _T("tm_page_first_left_elastic");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_elastic + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("弹性模量不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_density = _T("tm_page_first_left_density");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_density + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("密度不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_max_stress = _T("tm_page_first_left_max_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_max_stress + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("最大应力不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_ratio_stress =
        _T("tm_page_first_left_ratio_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_ratio_stress + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("应力比不能为空"), _T("错误"), NULL);
      return -1;
    }

    //////////////////////////
    DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_amplitude + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("应变幅值不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_dc_stress + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("直流应力不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_radius_exp = _T("tm_page_first_left_radius_exp");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radius_exp + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("试验段半径不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_radius_parallel =
        _T("tm_page_first_left_radus_parallel");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radius_parallel + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("平行段半径不能为空"), _T("错误"), NULL);
      return -1;
    }

  } else if (solution_type_ == anx::esolution::kSolutionName_Th3point_Bending) {
    // read th3point bending solution design from control
    DuiLib::CDuiString name_specimensa =
        _T("tm_page_first_left_name_specimens");
    DuiLib::CDuiString tail_prefix = _T("_th3point");
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_specimensa + tail_prefix));
    DuiLib::CDuiString value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("试验名称不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_elastic = _T("tm_page_first_left_elastic");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_elastic + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("弹性模量不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_density = _T("tm_page_first_left_density");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_density + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("密度不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_max_stress = _T("tm_page_first_left_max_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_max_stress + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("最大应力不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_ratio_stress =
        _T("tm_page_first_left_ratio_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_ratio_stress + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("应力比不能为空"), _T("错误"), NULL);
      return -1;
    }

    //////////////////////////
    DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_amplitude + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("应变幅值不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_dc_stress + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("直流应力不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_speciments_width =
        _T("tm_page_first_left_speciments_width");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_speciments_width + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("试样宽度不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_speciments_height =
        _T("tm_page_first_left_speciments_height");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_speciments_height + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("试样厚度不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_speciments_length =
        _T("tm_page_first_left_speciments_length");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_speciments_length + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("试样长度不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_length_parallel =
        _T("tm_page_first_left_length_parallel");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_length_parallel + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("平行段长度不能为空"), _T("错误"), NULL);
      return -1;
    }

  } else if (solution_type_ ==
             anx::esolution::kSolutionName_Vibration_Bending) {
    // read vibration bending solution design from control
    DuiLib::CDuiString name_specimensa =
        _T("tm_page_first_left_name_specimens");
    DuiLib::CDuiString tail_prefix = _T("_vibration");
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_specimensa + tail_prefix));
    DuiLib::CDuiString value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("试验名称不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_elastic = _T("tm_page_first_left_elastic");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_elastic + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("弹性模量不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_density = _T("tm_page_first_left_density");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_density + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("密度不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_max_stress = _T("tm_page_first_left_max_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_max_stress + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("最大应力不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_ratio_stress =
        _T("tm_page_first_left_ratio_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_ratio_stress + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("应力比不能为空"), _T("错误"), NULL);
      return -1;
    }

    //////////////////////////
    DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_amplitude + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("应变幅值不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_dc_stress + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("直流应力不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_radius_exp =
        _T("tm_page_first_left_length_parallel_section");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radius_exp + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("试验段长度不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_radius_load =
        _T("tm_page_first_left_specimen_radius_arc");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radius_load + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("试验段半径不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_radius =
        _T("tm_page_first_left_specimen_radius_transition");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radius + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("过渡段半径不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_thickness_clamp =
        _T("tm_page_first_left_thickness_clamp_section");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_thickness_clamp + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("夹持段厚度不能为空"), _T("错误"), NULL);
      return -1;
    }

    DuiLib::CDuiString name_thickness_exp =
        _T("tm_page_first_left_thickness_exp_section");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_thickness_exp + tail_prefix));
    value = edit->GetText().GetData();
    if (value.IsEmpty()) {
      MessageBox(NULL, _T("试验段厚度不能为空"), _T("错误"), NULL);
      return -1;
    }

  } else {
    assert(false && "Invalid solution type");
  }
  return 0;
}

std::string WorkWindow::SolutionDesignXmlFromControl() {
  anx::esolution::ExpDesignHeader header;
  header.solution_type_ = this->solution_type_;
  anx::esolution::ExpDesignBaseParam exp_design_base_param;
  std::string t_prefix = anx::esolution::ToTailPrefixName(solution_type_);
  if (solution_type_ == anx::esolution::kSolutionName_Axially_Symmetrical) {
    // read axially symmetrical solution design from control
    DuiLib::CDuiString name_specimensa =
        _T("tm_page_first_left_name_specimens");
    DuiLib::CDuiString tail_prefix;
    tail_prefix.Append((LPCTSTR)(string2wstring(t_prefix.c_str()).c_str()));
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_specimensa + tail_prefix));
    DuiLib::CDuiString value = edit->GetText().GetData();

    std::string name = WCharToMByte(value.GetData());
    memcpy(header.name_, name.data(), 255);

    DuiLib::CDuiString name_material = _T("tm_page_first_left_material");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_material + tail_prefix));
    value = edit->GetText().GetData();
    std::string material_name = WCharToMByte(value.GetData());
    memcpy(exp_design_base_param.material_name_, material_name.data(), 255);

    DuiLib::CDuiString name_elastic = _T("tm_page_first_left_elastic");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_elastic + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_base_param.f_elastic_modulus_GPa_ = _ttof(value.GetData());

    DuiLib::CDuiString name_density = _T("tm_page_first_left_density");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_density + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_base_param.f_density_kg_m3_ = _ttof(value.GetData());

    DuiLib::CDuiString name_max_stress = _T("tm_page_first_left_max_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_max_stress + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_base_param.f_max_stress_MPa_ = _ttof(value.GetData());

    DuiLib::CDuiString name_ratio_stress =
        _T("tm_page_first_left_ratio_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_ratio_stress + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_base_param.f_stress_ratio_ = _ttof(value.GetData());

    anx::esolution::ExpDesignResultAxially exp_design_result_axially;
    DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_amplitude + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_axially.f_eamplitude_ = _ttof(value.GetData());

    DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_dc_stress + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_axially.f_dc_stress_MPa_ = _ttof(value.GetData());

    DuiLib::CDuiString name_radius_exp = _T("tm_page_first_left_radius_exp");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radius_exp + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_axially.f_exp_section_radius_R2_ = _ttof(value.GetData());

    DuiLib::CDuiString name_radius_parallel =
        _T("tm_page_first_left_radus_parallel");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radius_parallel + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_axially.f_parallel_section_radius_R1_ =
        _ttof(value.GetData());

    DuiLib::CDuiString name_radus_trans = _T("tm_page_first_left_radus_trans");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radus_trans + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_axially.f_transition_section_radius_R0_ =
        _ttof(value.GetData());

    DuiLib::CDuiString name_length_trans =
        _T("tm_page_first_left_length_trans");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_length_trans + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_axially.f_transition_section_length_L1_ =
        _ttof(value.GetData());

    DuiLib::CDuiString name_length_parallel =
        _T("tm_page_first_left_length_parallel");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_length_parallel + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_axially.f_parallel_section_length_L0_ =
        _ttof(value.GetData());

    DuiLib::CDuiString name_exp_section = _T("tm_page_first_exp_section_trans");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_exp_section + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_axially.f_exp_section_length_L2_ = _ttof(value.GetData());

    // to xml
    anx::esolution::SolutionDesign solution_design;
    solution_design.header_.reset(new anx::esolution::ExpDesignHeader(header));
    solution_design.base_param_.reset(
        new anx::esolution::ExpDesignBaseParam(exp_design_base_param));
    solution_design.result_.reset(
        new anx::esolution::ExpDesignResultAxially(exp_design_result_axially));
    std::string xml = solution_design.ToXml();
    return xml;
  } else if (solution_type_ ==
             anx::esolution::kSolutionName_Stresses_Adjustable) {
    // read stresses adjustable solution design from control
    DuiLib::CDuiString name_specimensa =
        _T("tm_page_first_left_name_specimens");
    DuiLib::CDuiString tail_prefix;
    tail_prefix.Append((LPCTSTR)(string2wstring(t_prefix.c_str()).c_str()));
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_specimensa + tail_prefix));
    DuiLib::CDuiString value = edit->GetText().GetData();
    std::string name = WCharToMByte(value.GetData());
    memcpy(header.name_, name.data(), 255);

    DuiLib::CDuiString name_material = _T("tm_page_first_left_material");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_material + tail_prefix));
    value = edit->GetText().GetData();
    std::string material_name = WCharToMByte(value.GetData());
    memcpy(exp_design_base_param.material_name_, material_name.data(), 255);

    DuiLib::CDuiString name_elastic = _T("tm_page_first_left_elastic");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_elastic + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_base_param.f_elastic_modulus_GPa_ = _ttof(value.GetData());

    DuiLib::CDuiString name_density = _T("tm_page_first_left_density");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_density + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_base_param.f_density_kg_m3_ = _ttof(value.GetData());

    DuiLib::CDuiString name_max_stress = _T("tm_page_first_left_max_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_max_stress + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_base_param.f_max_stress_MPa_ = _ttof(value.GetData());

    DuiLib::CDuiString name_ratio_stress =
        _T("tm_page_first_left_ratio_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_ratio_stress + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_base_param.f_stress_ratio_ = _ttof(value.GetData());

    anx::esolution::ExpDesignResultStressesAdjustable
        exp_design_result_stresses_adjustable;

    DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_amplitude + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_stresses_adjustable.f_eamplitude_ =
        _ttof(value.GetData());

    DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_dc_stress + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_stresses_adjustable.f_dc_stress_MPa_ =
        _ttof(value.GetData());

    DuiLib::CDuiString name_static_load = _T("tm_page_first_left_static_load");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_static_load + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_stresses_adjustable.f_static_load_MPa_ =
        _ttof(value.GetData());

    DuiLib::CDuiString name_radius_exp = _T("tm_page_first_left_radius_exp");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radius_exp + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_stresses_adjustable.f_exp_section_radius_R2_ =
        _ttof(value.GetData());

    DuiLib::CDuiString name_radius_parallel =
        _T("tm_page_first_left_radus_parallel");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radius_parallel + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_stresses_adjustable.f_parallel_section_radius_R1_ =
        _ttof(value.GetData());

    DuiLib::CDuiString name_radus_trans = _T("tm_page_first_left_radus_trans");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radus_trans + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_stresses_adjustable.f_transition_section_radius_R0_ =
        _ttof(value.GetData());

    DuiLib::CDuiString name_length_trans =
        _T("tm_page_first_left_length_trans");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_length_trans + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_stresses_adjustable.f_transition_section_length_L1_ =
        _ttof(value.GetData());

    DuiLib::CDuiString name_length_parallel =
        _T("tm_page_first_left_length_parallel");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_length_parallel + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_stresses_adjustable.f_parallel_section_length_L0_ =
        _ttof(value.GetData());

    DuiLib::CDuiString name_exp_section = _T("tm_page_first_exp_section_trans");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_exp_section + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_stresses_adjustable.f_exp_section_length_L2_ =
        _ttof(value.GetData());

    // to xml
    anx::esolution::SolutionDesign solution_design;
    solution_design.header_.reset(new anx::esolution::ExpDesignHeader(header));
    solution_design.base_param_.reset(
        new anx::esolution::ExpDesignBaseParam(exp_design_base_param));
    solution_design.result_.reset(
        new anx::esolution::ExpDesignResultStressesAdjustable(
            exp_design_result_stresses_adjustable));
    std::string xml = solution_design.ToXml();
    return xml;
  } else if (solution_type_ == anx::esolution::kSolutionName_Th3point_Bending) {
    // read th3point bending solution design from control
    DuiLib::CDuiString name_specimensa =
        _T("tm_page_first_left_name_specimens");
    DuiLib::CDuiString tail_prefix;
    tail_prefix.Append((LPCTSTR)(string2wstring(t_prefix.c_str()).c_str()));
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_specimensa + tail_prefix));
    DuiLib::CDuiString value = edit->GetText().GetData();
    std::string name = WCharToMByte(value.GetData());
    memcpy(header.name_, name.data(), 255);

    DuiLib::CDuiString name_elastic = _T("tm_page_first_left_elastic");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_elastic + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_base_param.f_elastic_modulus_GPa_ = _ttof(value.GetData());

    DuiLib::CDuiString name_density = _T("tm_page_first_left_density");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_density + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_base_param.f_density_kg_m3_ = _ttof(value.GetData());

    DuiLib::CDuiString name_max_stress = _T("tm_page_first_left_max_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_max_stress + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_base_param.f_max_stress_MPa_ = _ttof(value.GetData());

    DuiLib::CDuiString name_ratio_stress =
        _T("tm_page_first_left_ratio_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_ratio_stress + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_base_param.f_stress_ratio_ = _ttof(value.GetData());

    //////////////////////////////////////////////////////////////////////////
    anx::esolution::ExpDesignResultTh3pointBending exp_design_result_th3point;
    DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_amplitude + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_th3point.f_eamplitude_um_ = _ttof(value.GetData());

    DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_dc_stress + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_th3point.f_dc_stress_MPa_ = _ttof(value.GetData());

    DuiLib::CDuiString name_static_load = _T("tm_page_first_left_static_load");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_static_load + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_th3point.f_static_load_MPa_ = _ttof(value.GetData());

    DuiLib::CDuiString name_specimens_width =
        _T("tm_page_first_left_speciments_width");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_specimens_width + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_th3point.f_specimen_width_B_ = _ttof(value.GetData());

    DuiLib::CDuiString name_specimens_height =
        _T("tm_page_first_left_speciments_height");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_specimens_height + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_th3point.f_specimen_thickness_h_ = _ttof(value.GetData());

    DuiLib::CDuiString name_specimens_length =
        _T("tm_page_first_left_speciments_length");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_specimens_length + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_th3point.f_specimen_length_L_ = _ttof(value.GetData());

    DuiLib::CDuiString name_length_parallel =
        _T("tm_page_first_left_length_parallel");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_length_parallel + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_th3point.f_support_distance_L0_ = _ttof(value.GetData());

    // to xml
    anx::esolution::SolutionDesign solution_design;
    solution_design.header_.reset(new anx::esolution::ExpDesignHeader(header));
    solution_design.base_param_.reset(
        new anx::esolution::ExpDesignBaseParam(exp_design_base_param));
    solution_design.result_.reset(
        new anx::esolution::ExpDesignResultTh3pointBending(
            exp_design_result_th3point));
    std::string xml = solution_design.ToXml();
    return xml;
  } else if (solution_type_ ==
             anx::esolution::kSolutionName_Vibration_Bending) {
    // read th3point bending solution design from control
    DuiLib::CDuiString name_specimensa =
        _T("tm_page_first_left_name_specimens");
    DuiLib::CDuiString tail_prefix;
    tail_prefix.Append((LPCTSTR)(string2wstring(t_prefix.c_str()).c_str()));
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_specimensa + tail_prefix));
    DuiLib::CDuiString value = edit->GetText().GetData();
    std::string name = WCharToMByte(value.GetData());
    memcpy(header.name_, name.data(), 255);

    DuiLib::CDuiString name_elastic = _T("tm_page_first_left_elastic");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_elastic + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_base_param.f_elastic_modulus_GPa_ = _ttof(value.GetData());

    DuiLib::CDuiString name_density = _T("tm_page_first_left_density");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_density + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_base_param.f_density_kg_m3_ = _ttof(value.GetData());

    DuiLib::CDuiString name_max_stress = _T("tm_page_first_left_max_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_max_stress + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_base_param.f_max_stress_MPa_ = _ttof(value.GetData());

    DuiLib::CDuiString name_ratio_stress =
        _T("tm_page_first_left_ratio_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_ratio_stress + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_base_param.f_stress_ratio_ = _ttof(value.GetData());

    //////////////////////////////////////////////////////////////////////////
    anx::esolution::ExpDesignResultVibrationBending exp_design_result_vibration;
    DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_amplitude + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_vibration.f_eamplitude_um_ = _ttof(value.GetData());

    DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_dc_stress + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_vibration.f_dc_stress_MPa_ = _ttof(value.GetData());

    DuiLib::CDuiString name_radius_exp =
        _T("tm_page_first_left_length_parallel_section");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radius_exp + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_vibration.f_specimen_length_parallel_section_L1_ =
        _ttof(value.GetData());

    DuiLib::CDuiString name_radius_arc =
        _T("tm_page_first_left_specimen_radius_arc");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radius_arc + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_vibration.f_specimen_radius_arc_R1_ =
        _ttof(value.GetData());

    DuiLib::CDuiString name_radius_parallel =
        _T("tm_page_first_left_specimen_radius_transition");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radius_parallel + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_vibration.f_specimen_radius_transition_R2_ =
        _ttof(value.GetData());

    DuiLib::CDuiString name_radus_trans =
        _T("tm_page_first_left_thickness_clamp_section");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_radus_trans + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_vibration.f_thickness_clamping_d1_ =
        _ttof(value.GetData());

    DuiLib::CDuiString name_length_trans =
        _T("tm_page_first_left_thickness_exp_section");
    edit = static_cast<DuiLib::CEditUI*>(
        m_PaintManager.FindControl(name_length_trans + tail_prefix));
    value = edit->GetText().GetData();
    exp_design_result_vibration.f_thickness_exp_section_L0_d2_ =
        _ttof(value.GetData());

    // to xml
    anx::esolution::SolutionDesign solution_design;
    solution_design.header_.reset(new anx::esolution::ExpDesignHeader(header));
    solution_design.base_param_.reset(
        new anx::esolution::ExpDesignBaseParam(exp_design_base_param));
    solution_design.result_.reset(
        new anx::esolution::ExpDesignResultVibrationBending(
            exp_design_result_vibration));
    std::string xml = solution_design.ToXml();
    return xml;
  } else {
    assert(false && "Invalid solution type");
  }
  return "";
}

}  // namespace ui
}  // namespace anx
