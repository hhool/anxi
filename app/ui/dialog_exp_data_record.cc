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
#include "app/common/logger.h"
#include "app/common/string_utils.h"
#include "app/expdata/experiment_data_base.h"
#include "app/ui/ui_constants.h"

DUI_BEGIN_MESSAGE_MAP(anx::ui::DialogExpDataRecord, DuiLib::WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {

namespace {
uint64_t FileTimeToUnixTime(const FILETIME& file_time) {
  ULARGE_INTEGER ull;
  ull.LowPart = file_time.dwLowDateTime;
  ull.HighPart = file_time.dwHighDateTime;
  return (ull.QuadPart - 116444736000000000) / 10000000;
}

void UnixTimeToFileTime(uint64_t unix_time, FILETIME* file_time) {
  ULARGE_INTEGER ull;
  ull.QuadPart = unix_time * 10000000 + 116444736000000000;
  file_time->dwLowDateTime = ull.LowPart;
  file_time->dwHighDateTime = ull.HighPart;
}

SYSTEMTIME UnixTimeToSystemTime(uint64_t unix_time) {
  SYSTEMTIME sys_time;
  FILETIME file_time;
  UnixTimeToFileTime(unix_time, &file_time);
  FileTimeToSystemTime(&file_time, &sys_time);
  return sys_time;
}

uint64_t SystemTimeToUnixTime(const SYSTEMTIME& sys_time) {
  FILETIME file_time;
  SystemTimeToFileTime(&sys_time, &file_time);
  return FileTimeToUnixTime(file_time);
}

SYSTEMTIME StringToSystemTime(const std::string& time_str) {
  SYSTEMTIME sys_time;
  sscanf(time_str.c_str(), "%04d-%02d-%02d %02d:%02d:%02d",
         reinterpret_cast<int*>(&sys_time.wYear),
         reinterpret_cast<int*>(&sys_time.wMonth),
         reinterpret_cast<int*>(&sys_time.wDay),
         reinterpret_cast<int*>(&sys_time.wHour),
         reinterpret_cast<int*>(&sys_time.wMinute),
         reinterpret_cast<int*>(&sys_time.wSecond));
  return sys_time;
}

std::string SystemTimeToString(const SYSTEMTIME& sys_time) {
  char time_str[64];
  sprintf(time_str, "%04d-%02d-%02d %02d:%02d:%02d", sys_time.wYear,
          sys_time.wMonth, sys_time.wDay, sys_time.wHour, sys_time.wMinute,
          sys_time.wSecond);
  return time_str;
}

uint64_t StringToUnixTime(const std::string& time_str) {
  SYSTEMTIME sys_time = StringToSystemTime(time_str);
  return SystemTimeToUnixTime(sys_time);
}

std::string UnixTimeToString(uint64_t unix_time) {
  SYSTEMTIME sys_time = UnixTimeToSystemTime(unix_time);
  return SystemTimeToString(sys_time);
}
}  // namespace

DialogExpDataRecord::DialogExpDataRecord() {}

DialogExpDataRecord::~DialogExpDataRecord() {}

void DialogExpDataRecord::InitWindow() {
  close_button_ = static_cast<DuiLib::CButtonUI*>(
      this->m_PaintManager.FindControl(kCloseButtonControlName));
  refresh_button_ = static_cast<DuiLib::CButtonUI*>(
      this->m_PaintManager.FindControl(_T("btn_refresh")));
  start_date_time_ = static_cast<DuiLib::CDateTimeUI*>(
      this->m_PaintManager.FindControl(_T("date_time_start")));
  end_date_time_ = static_cast<DuiLib::CDateTimeUI*>(
      this->m_PaintManager.FindControl(_T("date_time_end")));
  /// set the start time and end time
  SYSTEMTIME sys_time;
  GetLocalTime(&sys_time);
  start_date_time_->SetTime(&sys_time);
  end_date_time_->SetTime(&sys_time);

  /// bind the click event
  refresh_button_->OnNotify +=
      ::MakeDelegate(this, &DialogExpDataRecord::OnRefreshButtonClick);

  /// Traverse the directory expdata folder and get all the csv files
  anx::expdata::TraverseExpDataFolder(&exp_data_summary_list_);
  /// revert summarys
  std::reverse(exp_data_summary_list_.begin(), exp_data_summary_list_.end());
  exp_data_list_ = static_cast<DuiLib::CListUI*>(
      this->m_PaintManager.FindControl(_T("exp_data_record_list")));
  for (const auto& summary : exp_data_summary_list_) {
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
    DuiLib::CTextUI* pLabelUI = new DuiLib::CTextUI();
    pLabelUI->SetAttributeList(_T("font=\"11\" algin=\"right\""));
    pLabelUI->SetText(
        anx::common::string2wstring(summary.file_name_.c_str()).c_str());
    new_h_lay->Add(new_control);
    new_h_lay->Add(new_container);
    new_h_lay->Add(pLabelUI);
    new_node->Add(new_h_lay);
    exp_data_list_->Add(new_node);
  }

  /// get min start time from the summary list and set it to the start time
  /// picker and max time is the current time and set it to the end time picker
  uint64_t min_time = UINT64_MAX;
  // set max time to now time
  uint64_t max_time = time(nullptr);
  if (exp_data_summary_list_.size() == 0) {
    min_time = max_time;
  }
  for (const auto& summary : exp_data_summary_list_) {
    if (summary.start_time_ < min_time) {
      min_time = summary.start_time_;
    }
  }
  SYSTEMTIME min_sys_time = UnixTimeToSystemTime(min_time);
  SYSTEMTIME max_sys_time = UnixTimeToSystemTime(max_time);
  start_date_time_->SetTime(&min_sys_time);
  end_date_time_->SetTime(&max_sys_time);

  /// show the first item detail
  if (exp_data_list_->GetCount() > 0) {
    exp_data_list_->SelectItem(0);
    ShowSummaryItemDetail(0);
  } else {
    ShowSummaryEmptyDetail();
  }
  /// bind list item click event
  exp_data_list_->OnNotify +=
      ::MakeDelegate(this, &DialogExpDataRecord::OnListItemClick);
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

LRESULT DialogExpDataRecord::ResponseDefaultKeyEvent(WPARAM wParam) {
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

bool DialogExpDataRecord::OnRefreshButtonClick(void* msg) {
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(msg);
  if (pMsg == nullptr) {
    return false;
  }
  if (pMsg->pSender != refresh_button_) {
    return false;
  }
  /// get start time and end time from the date time picker
  SYSTEMTIME start_time = start_date_time_->GetTime();
  SYSTEMTIME end_time = end_date_time_->GetTime();
  /// make start time bengin 00:00:00 end time end 23:59:59
  start_time.wHour = 0;
  start_time.wMinute = 0;
  start_time.wSecond = 0;
  end_time.wHour = 23;
  end_time.wMinute = 59;
  end_time.wSecond = 59;
  /// convert the time to unix time
  uint64_t start_time_unix = SystemTimeToUnixTime(start_time);
  uint64_t end_time_unix = SystemTimeToUnixTime(end_time);
  /// find the data from exp_data_summary_list_ to new list by the time
  std::vector<anx::expdata::ExperimentFileSummary> new_list;
  for (const auto& summary : exp_data_summary_list_) {
    if (summary.start_time_ >= start_time_unix &&
        summary.end_time_ <= end_time_unix) {
      new_list.push_back(summary);
    }
  }
  /// no need revert new list
  DuiLib::CListUI* pList = static_cast<DuiLib::CListUI*>(
      this->m_PaintManager.FindControl(_T("exp_data_record_list")));
  pList->RemoveAll();

  for (const auto& summary : new_list) {
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
    DuiLib::CTextUI* pLabelUI = new DuiLib::CTextUI();
    pLabelUI->SetAttributeList(_T("font=\"11\" algin=\"right\""));
    pLabelUI->SetText(
        anx::common::string2wstring(summary.file_name_.c_str()).c_str());
    new_h_lay->Add(new_control);
    new_h_lay->Add(new_container);
    new_h_lay->Add(pLabelUI);
    new_node->Add(new_h_lay);
    pList->Add(new_node);
  }

  return true;
}

bool DialogExpDataRecord::OnListItemClick(void* msg) {
  TNotifyUI* pMsg = reinterpret_cast<TNotifyUI*>(msg);
  if (pMsg == nullptr) {
    return false;
  }
  if (pMsg->pSender != exp_data_list_) {
    return false;
  }
  int index = exp_data_list_->GetCurSel();
  if (index < 0) {
    return false;
  }
  return ShowSummaryItemDetail(index);
}

bool DialogExpDataRecord::ShowSummaryItemDetail(int index) {
  if (index < 0) {
    return false;
  }
  if (index >= static_cast<int>(exp_data_summary_list_.size())) {
    return false;
  }
  const auto& summary = exp_data_summary_list_[index];
  LOG_F(LG_INFO) << "file name: " << summary.file_name_;
  DuiLib::CLabelUI* pLabelUI = static_cast<DuiLib::CTextUI*>(
      this->m_PaintManager.FindControl(_T("file_name")));
  pLabelUI->SetText(
      anx::common::string2wstring(summary.file_name_.c_str()).c_str());

  /// splite file name to get the start time and end time of the file
  /// file name format: 2024-08-12_08-00-00_2024-08-12_08-00-00.csv
  std::string file_name = summary.file_name_;
  // find file extension and if it is csv file then remove it
  std::string extension = file_name.substr(file_name.size() - 4, 4);
  if (extension != ".csv") {
    return false;
  }
  // remove the extension
  file_name = file_name.substr(0, file_name.size() - 4);
  std::vector<std::string> file_name_parts = anx::common::Split(file_name, "_");
  if (file_name_parts.size() != 4) {
    return false;
  }
  // file_name_parts[1] is "08-00-00" transform to "08:00:00" format
  std::string start_time_str_format = file_name_parts[1];
  std::vector<std::string> start_time_parts =
      anx::common::Split(start_time_str_format, "-");
  if (start_time_parts.size() != 3) {
    return false;
  }
  start_time_str_format = start_time_parts[0] + ":" + start_time_parts[1] +
                          ":" + start_time_parts[2];
  // file_name_parts[3] is "08-00-00" transform to "08:00:00" format
  std::string end_time_str_format = file_name_parts[3];
  std::vector<std::string> end_time_parts =
      anx::common::Split(end_time_str_format, "-");
  if (end_time_parts.size() != 3) {
    return false;
  }
  end_time_str_format =
      end_time_parts[0] + ":" + end_time_parts[1] + ":" + end_time_parts[2];

  std::string start_time_str = file_name_parts[0] + " " + start_time_str_format;
  std::string end_time_str = file_name_parts[2] + " " + end_time_str_format;
  // get duration time from start time and end time
  uint64_t start_time_unix = StringToUnixTime(start_time_str);
  uint64_t end_time_unix = StringToUnixTime(end_time_str);
  uint64_t duration_time = end_time_unix - start_time_unix;
  LOG_F(LG_INFO) << "start time: " << start_time_str
                 << " end time: " << end_time_str
                 << " duration time: " << duration_time;
  pLabelUI = static_cast<DuiLib::CTextUI*>(
      this->m_PaintManager.FindControl(_T("start_time")));
  pLabelUI->SetText(
      anx::common::string2wstring(start_time_str.c_str()).c_str());
  pLabelUI = static_cast<DuiLib::CTextUI*>(
      this->m_PaintManager.FindControl(_T("end_time")));
  pLabelUI->SetText(anx::common::string2wstring(end_time_str.c_str()).c_str());
  // format duration time to string
  uint64_t duration_hour = duration_time / 3600;
  uint64_t duration_minute = (duration_time % 3600) / 60;
  uint64_t duration_second = duration_time % 60;
  std::string duration_time_str;
  if (duration_hour != 0) {
    duration_time_str = std::to_string(duration_hour) + "时 ";
  }
  if (duration_minute != 0) {
    duration_time_str += std::to_string(duration_minute) + "分 ";
  }
  duration_time_str += std::to_string(duration_second) + "秒";
  pLabelUI = static_cast<DuiLib::CTextUI*>(
      this->m_PaintManager.FindControl(_T("duration_time")));
  pLabelUI->SetText(
      anx::common::string2wstring(duration_time_str.c_str()).c_str());
  return true;
}

void DialogExpDataRecord::ShowSummaryEmptyDetail() {
  DuiLib::CLabelUI* pLabelUI = static_cast<DuiLib::CTextUI*>(
      this->m_PaintManager.FindControl(_T("file_name")));
  pLabelUI->SetText(_T(""));
  pLabelUI = static_cast<DuiLib::CTextUI*>(
      this->m_PaintManager.FindControl(_T("start_time")));
  pLabelUI->SetText(_T(""));
  pLabelUI = static_cast<DuiLib::CTextUI*>(
      this->m_PaintManager.FindControl(_T("end_time")));
  pLabelUI->SetText(_T(""));
  pLabelUI = static_cast<DuiLib::CTextUI*>(
      this->m_PaintManager.FindControl(_T("duration_time")));
  pLabelUI->SetText(_T(""));
}

}  // namespace ui
}  // namespace anx
