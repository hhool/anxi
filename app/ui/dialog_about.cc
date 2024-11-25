/**
 * @file dialog_about.cc
 * @author hhool (hhool@outlook.com)
 * @brief dialog about
 * @version 0.1
 * @date 2024-07-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/dialog_about.h"

#include <map>
#include <string>

#include "app/common/logger.h"
#include "app/common/module_utils.h"
#include "app/common/string_utils.h"
#include "app/ui/ui_constants.h"

#include "third_party/tinyxml2/source/tinyxml2.h"

DUI_BEGIN_MESSAGE_MAP(anx::ui::DialogAbout, DuiLib::WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {
DialogAbout::DialogAbout() {}

DialogAbout::~DialogAbout() {}

void DialogAbout::InitWindow() {
  __super::InitWindow();
  std::map<std::string, std::string> about_config = LoadAboutConfig();
  CLabelUI* lb_version =
      static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("ver")));
  lb_version->SetText(anx::common::UTF8ToUnicode(about_config["ver"]).c_str());
  CLabelUI* lb_in_ver =
      static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("in_ver")));
  lb_in_ver->SetText(
      anx::common::UTF8ToUnicode(about_config["in_ver"]).c_str());
  CLabelUI* lb_soft_name =
      static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("soft_name")));
  lb_soft_name->SetText(
      anx::common::UTF8ToUnicode(about_config["soft_name"]).c_str());
  CLabelUI* lb_copy_right =
      static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("copyright")));
  lb_copy_right->SetText(
      anx::common::UTF8ToUnicode(about_config["copyright"]).c_str());
  CLabelUI* lb_phone =
      static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("phone")));
  lb_phone->SetText(anx::common::UTF8ToUnicode(about_config["phone"]).c_str());
}

void DialogAbout::Notify(DuiLib::TNotifyUI& msg) {
  if (msg.sType == kWindowInit) {
    OnPrepare(msg);
  } else if (msg.sType == kClick) {
    if (msg.pSender->GetName() == kCloseButtonControlName) {
      this->Close();
      return;
    }
  }
}

void DialogAbout::OnFinalMessage(HWND hWnd) {
  WindowImplBase::OnFinalMessage(hWnd);
  delete this;
}

LRESULT DialogAbout::ResponseDefaultKeyEvent(WPARAM wParam) {
  if (wParam == VK_RETURN) {
    this->Close();
    return TRUE;
  } else if (wParam == VK_ESCAPE) {
    return TRUE;
  }
  return FALSE;
}

void DialogAbout::OnPrepare(const DuiLib::TNotifyUI& msg) {
  ::SetWindowLong(
      m_hWnd, GWL_STYLE,
      ::GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);
}

std::map<std::string, std::string> DialogAbout::LoadAboutConfig() {
  std::map<std::string, std::string> about_config;
  std::string app_data_dir = anx::common::GetApplicationDataPath();
#if defined(_WIN32) || defined(_WIN64)
  app_data_dir += "\\anxi\\default\\config_about.xm";
#else
  app_data_dir += "/anxi/default/config_about.xm";
#endif
  std::string module_path = app_data_dir;
  tinyxml2::XMLDocument doc;
  if (doc.LoadFile(module_path.c_str()) != tinyxml2::XML_SUCCESS) {
    return about_config;
  }
  tinyxml2::XMLElement* root = doc.RootElement();
  if (root == nullptr) {
    return about_config;
  }
  tinyxml2::XMLElement* ele = root->FirstChildElement("about");
  if (ele == nullptr) {
    return about_config;
  }
  do {
    if (ele == nullptr) {
      break;
    }
    tinyxml2::XMLElement* ele_soft_name = ele->FirstChildElement("soft_name");
    if (ele_soft_name != nullptr) {
      about_config["soft_name"] = ele_soft_name->GetText();
    }
    tinyxml2::XMLElement* ele_version = ele->FirstChildElement("ver");
    if (ele_version != nullptr) {
      about_config["ver"] = ele_version->GetText();
    }
    tinyxml2::XMLElement* ele_in_ver = ele->FirstChildElement("in_ver");
    if (ele_in_ver != nullptr) {
      about_config["in_ver"] = ele_in_ver->GetText();
    }
    tinyxml2::XMLElement* ele_copyright = ele->FirstChildElement("copyright");
    if (ele_copyright != nullptr) {
      about_config["copyright"] = ele_copyright->GetText();
    }
    tinyxml2::XMLElement* ele_phone = ele->FirstChildElement("phone");
    if (ele_phone != nullptr) {
      about_config["phone"] = ele_phone->GetText();
    }
  } while (false);
  return about_config;
}

}  // namespace ui
}  // namespace anx
