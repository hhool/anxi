/**
 * @file dialog_exp_data_record.cc
 * @author hhool (hhool@outlook.com)
 * @brief dialog about
 * @version 0.1
 * @date 2024-08-12
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/dialog_exp_data_record.h"

#include <string>
#include <vector>

#include "app/common/defines.h"
#include "app/common/string_utils.h"
#include "app/expdata/experiment_data_base.h"
#include "app/ui/ui_constants.h"

DUI_BEGIN_MESSAGE_MAP(anx::ui::DialogExpDataRecord, DuiLib::WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {
DialogExpDataRecord::DialogExpDataRecord() {}

DialogExpDataRecord::~DialogExpDataRecord() {}

void DialogExpDataRecord::InitWindow() {
  close_button_ = static_cast<DuiLib::CButtonUI*>(
      this->m_PaintManager.FindControl(kCloseButtonControlName));
  refresh_button_ = static_cast<DuiLib::CButtonUI*>(
      this->m_PaintManager.FindControl(_T("btn_refresh")));
  start_date_time_ = static_cast<DuiLib::CDateTimeUI*>(
      this->m_PaintManager.FindControl(_T("start_date_time")));
  end_date_time_ = static_cast<DuiLib::CDateTimeUI*>(
      this->m_PaintManager.FindControl(_T("end_date_time")));

  /// Traverse the directory expdata folder and get all the csv files
  std::vector<anx::expdata::ExperimentFileSummary> summarys;
  anx::expdata::TraverseExpDataFolder(&summarys);
  DuiLib::CListUI* pList = static_cast<DuiLib::CListUI*>(
      this->m_PaintManager.FindControl(_T("exp_data_record_list")));
  if (pList) {
    for (const auto& summary : summarys) {
      CListContainerElementUI* new_node = new CListContainerElementUI;
      new_node->SetAttributeList(
          _T("height=\"22\" width=\"360\" align=\"left\""));
      CHorizontalLayoutUI* new_h_lay = new CHorizontalLayoutUI;
      CContainerUI* new_container = new CContainerUI();
      CControlUI* new_control = new CControlUI();
      new_control->SetAttributeList(
          _T("width=\"20\" height=\"20\" ")
          _T("padding=\"24,0,0,0\" ")
          _T("bkimage=\"icon_csv.png\""));
      new_container->SetAttributeList(
          _T("width=\"1\" ")
          _T("padding=\"2,0,2,0\" "));
      DuiLib::CTextUI* pText = new DuiLib::CTextUI();
      pText->SetAttributeList(_T("font=\"11\" algin=\"right\""));
      pText->SetText(
          anx::common::string2wstring(summary.file_name_.c_str()).c_str());
      new_h_lay->Add(new_control);
      new_h_lay->Add(new_container);
      new_h_lay->Add(pText);
      new_node->Add(new_h_lay);
      pList->Add(new_node);
    }
    if (pList->GetCount() > 0) {
      pList->SelectItem(0);
    }
  }
}

void DialogExpDataRecord::Notify(DuiLib::TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
    OnPrepare(msg);
  } else if (msg.sType == kClick) {
    if (msg.pSender->GetName() == kCloseButtonControlName) {
      this->Close();
      return;
    } else if (msg.pSender->GetName() == _T("ok")) {
      this->Close();
      return;
    } else if (msg.pSender->GetName() == _T("cancel")) {
      this->Close();
      return;
    }
  }
}

void DialogExpDataRecord::OnFinalMessage(HWND hWnd) {
  WindowImplBase::OnFinalMessage(hWnd);
  delete this;
}

LRESULT DialogExpDataRecord::ResponseDefaultKeyEvent(WPARAM wParam,
                                                     bool& bHandled) {
  if (wParam == VK_RETURN) {
    this->Close();
    return TRUE;
  } else if (wParam == VK_ESCAPE) {
    return TRUE;
  }
  return FALSE;
}

void DialogExpDataRecord::OnPrepare(const DuiLib::TNotifyUI& msg) {
  ::SetWindowLong(
      m_hWnd, GWL_STYLE,
      ::GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);
}
}  // namespace ui
}  // namespace anx
