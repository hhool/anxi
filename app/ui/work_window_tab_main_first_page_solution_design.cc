/**
 * @file work_window_tab_main_first_page_solution_design.cc
 * @author hhool (hhool@outlook.com)
 * @brief work window class definition
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/ui/work_window_tab_main_first_page_solution_design.h"

#include <utility>

#include "app/common/logger.h"
#include "app/common/string_utils.h"
#include "app/device/device_exp_amplitude_settings.h"
#include "app/esolution/algorithm/alg.h"
#include "app/esolution/solution_design.h"
#include "app/esolution/solution_design_default.h"
#include "app/esolution/solution_design_helper.h"
#include "app/ui/dialog_about.h"
#include "app/ui/dialog_com_port_settings.h"
#include "app/ui/dialog_common.h"
#include "app/ui/ui_constants.h"
#include "app/ui/ui_num_string_convert.hpp"
#include "app/ui/work_window.h"
#include "app/ui/work_window_menu_design.h"
#include "app/ui/work_window_menu_store.h"
#include "app/ui/work_window_tab_main_page_base.h"

////////////////////////////////////////////////////////////////////////////////
namespace anx {
namespace ui {
namespace {
template <typename R>
R get_value_from_edit_with_prefix(const std::string& ctrl_name,
                                  const std::string& prefix,
                                  DuiLib::CPaintManagerUI* paint_manager_ui) {
  std::string ctrl_name_with_prefix = ctrl_name + prefix;
  DuiLib::CDuiString dui_str_name;
  dui_str_name.Append(
      anx::common::String2WString(ctrl_name_with_prefix.c_str()).c_str());
  DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui->FindControl(dui_str_name));
  return get_value_from_control<R>(edit);
}

void set_value_to_edit_with_prefix(const std::string& ctrl_name,
                                   const std::string& prefix,
                                   DuiLib::CPaintManagerUI* paint_manager_ui,
                                   double value) {
  std::string ctrl_name_with_prefix = ctrl_name + prefix;
  DuiLib::CDuiString dui_str_name;
  dui_str_name.Append(
      anx::common::String2WString(ctrl_name_with_prefix.c_str()).c_str());
  DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui->FindControl(dui_str_name));
  set_value_to_edit(edit, value, 2);
}

void set_value_to_edit_with_prefix(const std::string& ctrl_name,
                                   const std::string& prefix,
                                   DuiLib::CPaintManagerUI* paint_manager_ui,
                                   int value) {
  std::string ctrl_name_with_prefix = ctrl_name + prefix;
  DuiLib::CDuiString dui_str_name;
  dui_str_name.Append(
      anx::common::String2WString(ctrl_name_with_prefix.c_str()).c_str());
  DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui->FindControl(dui_str_name));
  set_value_to_edit(edit, value);
}

void set_enable_to_control_with_prefix(
    const std::string& ctrl_name,
    const std::string& prefix,
    DuiLib::CPaintManagerUI* paint_manager_ui,
    bool enable) {
  std::string ctrl_name_with_prefix = ctrl_name + prefix;
  DuiLib::CDuiString dui_str_name;
  dui_str_name.Append(
      anx::common::String2WString(ctrl_name_with_prefix.c_str()).c_str());
  DuiLib::CControlUI* control = paint_manager_ui->FindControl(dui_str_name);
  control->SetEnabled(enable);
}

}  // namespace

PageSolutionDesignBase::PageSolutionDesignBase(
    DuiLib::CPaintManagerUI* paint_manager_ui,
    int32_t design_type)
    : paint_manager_ui_(paint_manager_ui), design_type_(design_type) {}

PageSolutionDesignBase::~PageSolutionDesignBase() {
  solution_design_.reset();
}

void PageSolutionDesignBase::InitPage() {
  // load default or last document solution design
  std::unique_ptr<anx::esolution::SolutionDesign> design =
      anx::esolution::LoadSolutionDesignDefaultResourceWithType(design_type_);
  if (design == nullptr) {
    // fatal
    assert(false && "Load default solution design failed");
    return;
  }
  if (CheckSolutionDesign(*design) != 0) {
    // fatal
    assert(false && "Load default solution design failed");
    return;
  }

  solution_design_.reset(design.release());
  // update tab_main first page element view header and base param
  UpadateTabMainFirstPageElementViewHeaderAndBaseParam();
  // update tab_main first page element view result
  UpadateTabMainFirstPageElementViewResult();
}

int32_t PageSolutionDesignBase::LoadSolutionDesingFile(
    const std::string& filepath) {
  // load default or last document solution design
  std::unique_ptr<anx::esolution::SolutionDesign> design =
      anx::esolution::LoadSolutionDesignWithFilePath(filepath);
  if (design == nullptr) {
    // fatal
    assert(false && "Load default solution design failed");
    return -1;
  }
  if (CheckSolutionDesign(*design) != 0) {
    // fatal
    assert(false && "Load default solution design failed");
    return -2;
  }
  if (design->header_->solution_type_ != this->design_type_) {
    return -3;
  }
  solution_design_.reset(design.release());
  // update tab_main first page element view header and base param
  UpadateTabMainFirstPageElementViewHeaderAndBaseParam();
  // update tab_main first page element view result
  UpadateTabMainFirstPageElementViewResult();
  return 0;
}

void PageSolutionDesignBase::Dispose() {
  solution_design_.reset();
}

int32_t PageSolutionDesignBase::CheckSolutionDesign(
    const anx::esolution::SolutionDesign& design) {
  anx::esolution::ExpDesignHeader* header = design.header_.get();
  anx::esolution::ExpDesignBaseParam* base_param = design.base_param_.get();
  anx::esolution::ExpDesignResult* result = design.result_.get();
  if (header == nullptr || base_param == nullptr || result == nullptr) {
    return -1;
  }

  // check solution type range
  if (header->solution_type_ >=
          anx::esolution::kSolutionName_Axially_Symmetrical &&
      header->solution_type_ <=
          anx::esolution::kSolutionName_Vibration_Bending) {
  } else {
    return -2;
  }
  return 0;
}

int32_t
PageSolutionDesignBase::UpadateTabMainFirstPageElementViewHeaderAndBaseParam() {
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
  std::string t_prefix =
      anx::esolution::ToTailPrefixName(header->solution_type_);
  DuiLib::CDuiString tail_prefix;
  tail_prefix.Append(anx::common::String2WString(t_prefix).c_str());

  // update header
  {
    DuiLib::CDuiString name_specimensa =
        _T("tm_page_first_left_name_specimens");
    name_specimensa.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_specimensa));
    std::string name;
    name.assign(header->name_, header->name_ + 255);

    std::wstring stemp =
        anx::common::UTF8ToUnicode(name);  // Temporary buffer is required
    LPCWSTR result = stemp.c_str();
    edit->SetText(result);
  }

  // update base param
  {
    DuiLib::CDuiString name_material = _T("tm_page_first_left_material");
    name_material.Append(tail_prefix);
    DuiLib::CComboUI* edit = static_cast<DuiLib::CComboUI*>(
        paint_manager_ui_->FindControl(name_material));
    if (edit != nullptr) {
      std::string material_name;
      material_name.assign(base_param->material_name_,
                           base_param->material_name_ + 255);
      LPCSTR result = material_name.c_str();
      std::wstring result_str(anx::common::String2WString(result));
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
  set_value_to_edit_with_prefix("tm_page_first_left_elastic", t_prefix,
                                paint_manager_ui_,
                                base_param->f_elastic_modulus_GPa_);
  set_value_to_edit_with_prefix("tm_page_first_left_density", t_prefix,
                                paint_manager_ui_,
                                base_param->f_density_kg_m3_);
  set_value_to_edit_with_prefix("tm_page_first_left_max_stress", t_prefix,
                                paint_manager_ui_,
                                base_param->f_max_stress_MPa_);
  set_value_to_edit_with_prefix("tm_page_first_left_ratio_stress", t_prefix,
                                paint_manager_ui_, base_param->f_stress_ratio_);
  return 0;
}

std::unique_ptr<anx::esolution::ExpDesignHeader>
PageSolutionDesignBase::ExpDesignHeaderFromControl() {
  std::string t_prefix = anx::esolution::ToTailPrefixName(design_type_);
  DuiLib::CDuiString name_specimensa = _T("tm_page_first_left_name_specimens");
  DuiLib::CDuiString tail_prefix;
  tail_prefix.Append(
      (LPCTSTR)(anx::common::String2WString(t_prefix.c_str()).c_str()));
  DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_specimensa + tail_prefix));
  DuiLib::CDuiString value = edit->GetText().GetData();

  std::string name = anx::common::UnicodeToUTF8(value.GetData());
  anx::esolution::ExpDesignHeader exp_design_header;
  exp_design_header.solution_type_ = design_type_;
  // copy the name to the name_
  size_t size = name.size() > 255 ? 255 : name.size();
  memset(exp_design_header.name_, 0, sizeof(exp_design_header.name_));
  memcpy(exp_design_header.name_, name.data(), size);
  return std::unique_ptr<anx::esolution::ExpDesignHeader>(
      new anx::esolution::ExpDesignHeader(exp_design_header));
}

std::unique_ptr<anx::esolution::ExpDesignBaseParam>
PageSolutionDesignBase::ExpDesignBaseParamFromControl() {
  std::string t_prefix = anx::esolution::ToTailPrefixName(design_type_);
  DuiLib::CDuiString tail_prefix;
  tail_prefix.Append(
      (LPCTSTR)(anx::common::String2WString(t_prefix.c_str()).c_str()));
  DuiLib::CDuiString name_material = _T("tm_page_first_left_material");
  DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_material + tail_prefix));
  DuiLib::CDuiString value;
  if (edit) {
    value = edit->GetText().GetData();
  } else {
    value = _T("");
  }
  anx::esolution::ExpDesignBaseParam exp_design_base_param;
  std::string material_name = anx::common::WString2String(value.GetData());
  size_t size = material_name.size() > 255 ? 255 : material_name.size();
  memset(exp_design_base_param.material_name_, 0,
         sizeof(exp_design_base_param.material_name_));
  memcpy(exp_design_base_param.material_name_, material_name.data(), size);
  exp_design_base_param.f_elastic_modulus_GPa_ =
      get_value_from_edit_with_prefix<float>("tm_page_first_left_elastic",
                                             t_prefix, paint_manager_ui_);
  exp_design_base_param.f_density_kg_m3_ =
      get_value_from_edit_with_prefix<float>("tm_page_first_left_density",
                                             t_prefix, paint_manager_ui_);
  exp_design_base_param.f_max_stress_MPa_ =
      get_value_from_edit_with_prefix<float>("tm_page_first_left_max_stress",
                                             t_prefix, paint_manager_ui_);
  exp_design_base_param.f_stress_ratio_ =
      get_value_from_edit_with_prefix<float>("tm_page_first_left_ratio_stress",
                                             t_prefix, paint_manager_ui_);
  return std::unique_ptr<anx::esolution::ExpDesignBaseParam>(
      new anx::esolution::ExpDesignBaseParam(exp_design_base_param));
}

////////////////////////////////////////////////////////////////////////////////
DUI_BEGIN_MESSAGE_MAP(WorkWindowFirstPageAxiallySymmetrical,
                      DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_VALUECHANGED, OnValueChanged)
DUI_END_MESSAGE_MAP()
WorkWindowFirstPageAxiallySymmetrical::WorkWindowFirstPageAxiallySymmetrical(
    WorkWindow* pOwner,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : PageSolutionDesignBase(paint_manager_ui,
                             anx::esolution::kSolutionName_Axially_Symmetrical),
      pOwner_(pOwner),
      paint_manager_ui_(paint_manager_ui),
      t_prefix_(anx::esolution::ToTailPrefixName(design_type_)) {
  paint_manager_ui_->AddNotifier(this);
}

WorkWindowFirstPageAxiallySymmetrical::
    ~WorkWindowFirstPageAxiallySymmetrical() {
  paint_manager_ui_->RemoveNotifier(this);
}

void WorkWindowFirstPageAxiallySymmetrical::Notify(TNotifyUI& msg) {
  if (msg.pSender == nullptr) {
    return;
  }
  DuiLib::CDuiString name = msg.pSender->GetName();
  if (name.IsEmpty()) {
    return;
  }
  if (msg.sType == DUI_MSGTYPE_KILLFOCUS) {
    /// TODO(hhool):
  } else if (msg.sType == DUI_MSGTYPE_CLICK) {
    if (msg.pSender->GetName() == _T("btn_exp_start")) {
      exp_running_ = true;
      set_enable_to_control_with_prefix("btn_solution_design_refresh",
                                        t_prefix_, paint_manager_ui_, false);
    } else if (msg.pSender->GetName() == _T("btn_exp_stop")) {
      set_enable_to_control_with_prefix("btn_solution_design_refresh",
                                        t_prefix_, paint_manager_ui_, true);
      exp_running_ = false;
    } else {
      // do nothing
    }
  }
}

void WorkWindowFirstPageAxiallySymmetrical::OnClick(TNotifyUI& msg) {
  if (msg.sType == kClick) {
    if (msg.pSender->GetName() == _T("btn_solution_design_refresh_axially")) {
      // TODO(hhool):
    }
  } else if (msg.sType == DUI_MSGTYPE_CLICK) {
    if (msg.pSender->GetName() == _T("btn_exp_start")) {
      exp_running_ = true;
      set_enable_to_control_with_prefix("btn_solution_design_refresh",
                                        t_prefix_, paint_manager_ui_, false);
    } else if (msg.pSender->GetName() == _T("btn_exp_stop")) {
      set_enable_to_control_with_prefix("btn_solution_design_refresh",
                                        t_prefix_, paint_manager_ui_, true);
      exp_running_ = false;
    } else {
      // do nothing
    }
  }
}

void WorkWindowFirstPageAxiallySymmetrical::OnValueChanged(TNotifyUI& msg) {
  if (msg.sType == DUI_MSGTYPE_VALUECHANGED) {
    if (msg.pSender->GetName() == _T("work_args_area")) {
      ENMsgStruct* enmsg = reinterpret_cast<ENMsgStruct*>(msg.wParam);
      if (enmsg == nullptr) {
        LOG_F(LG_ERROR) << "WorkWindowFirstPageTh3pointBending::OnValueChanged"
                           "enmsg is nullptr";
        return;
      }
      if (enmsg->type_ == enmsg_type_exp_error) {
        set_enable_to_control_with_prefix("btn_solution_design_refresh",
                                          t_prefix_, paint_manager_ui_, true);
        exp_running_ = false;
      } else {
        // do nothing
      }
    } else {
      // do nothing
    }
  }
}

void WorkWindowFirstPageAxiallySymmetrical::InitPage() {
  PageSolutionDesignBase::InitPage();
}

void WorkWindowFirstPageAxiallySymmetrical::
    UpadateTabMainFirstPageElementViewResult() {
  // get header and base param and result from solution_design_
  anx::esolution::ExpDesignHeader* header = solution_design_->header_.get();
  anx::esolution::ExpDesignBaseParam* base_param =
      solution_design_->base_param_.get();
  anx::esolution::ExpDesignResult* result = solution_design_->result_.get();
  // update result
  anx::esolution::ExpDesignResultAxially* result_axially =
      reinterpret_cast<anx::esolution::ExpDesignResultAxially*>(result);
  set_value_to_edit_with_prefix("tm_page_first_left_amplitude", t_prefix_,
                                paint_manager_ui_,
                                result_axially->f_eamplitude_);
  set_value_to_edit_with_prefix("tm_page_first_left_dc_stress", t_prefix_,
                                paint_manager_ui_,
                                result_axially->f_dc_stress_MPa_);
  set_value_to_edit_with_prefix("tm_page_first_left_radius_exp", t_prefix_,
                                paint_manager_ui_,
                                result_axially->f_exp_section_radius_R2_);
  set_value_to_edit_with_prefix("tm_page_first_exp_section_trans", t_prefix_,
                                paint_manager_ui_,
                                result_axially->f_exp_section_length_L2_);
  set_value_to_edit_with_prefix("tm_page_first_left_radus_parallel", t_prefix_,
                                paint_manager_ui_,
                                result_axially->f_parallel_section_radius_R1_);
  set_value_to_edit_with_prefix(
      "tm_page_first_left_radus_trans", t_prefix_, paint_manager_ui_,
      result_axially->f_transition_section_radius_R0_);
  set_value_to_edit_with_prefix(
      "tm_page_first_left_length_trans", t_prefix_, paint_manager_ui_,
      result_axially->f_transition_section_length_L1_);
  set_value_to_edit_with_prefix("tm_page_first_left_length_parallel", t_prefix_,
                                paint_manager_ui_,
                                result_axially->f_parallel_section_length_L0_);
}

std::unique_ptr<anx::esolution::ExpDesignResult>
WorkWindowFirstPageAxiallySymmetrical::ExpDesignResultFromControl() {
  anx::esolution::ExpDesignResultAxially exp_design_result_axially;
  exp_design_result_axially.f_eamplitude_ =
      get_value_from_edit_with_prefix<double>("tm_page_first_left_amplitude",
                                              t_prefix_, paint_manager_ui_);
  exp_design_result_axially.f_dc_stress_MPa_ =
      get_value_from_edit_with_prefix<double>("tm_page_first_left_dc_stress",
                                              t_prefix_, paint_manager_ui_);
  exp_design_result_axially.f_exp_section_radius_R2_ =
      get_value_from_edit_with_prefix<double>("tm_page_first_left_radius_exp",
                                              t_prefix_, paint_manager_ui_);
  exp_design_result_axially.f_parallel_section_radius_R1_ =
      get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_radus_parallel", t_prefix_, paint_manager_ui_);
  exp_design_result_axially.f_transition_section_radius_R0_ =
      get_value_from_edit_with_prefix<double>("tm_page_first_left_radus_trans",
                                              t_prefix_, paint_manager_ui_);
  exp_design_result_axially.f_transition_section_length_L1_ =
      get_value_from_edit_with_prefix<double>("tm_page_first_left_length_trans",
                                              t_prefix_, paint_manager_ui_);
  exp_design_result_axially.f_parallel_section_length_L0_ =
      get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_length_parallel", t_prefix_, paint_manager_ui_);
  exp_design_result_axially.f_exp_section_length_L2_ =
      get_value_from_edit_with_prefix<double>("tm_page_first_exp_section_trans",
                                              t_prefix_, paint_manager_ui_);
  return std::unique_ptr<anx::esolution::ExpDesignResult>(
      new anx::esolution::ExpDesignResultAxially(exp_design_result_axially));
}

std::unique_ptr<anx::esolution::SolutionDesign>
WorkWindowFirstPageAxiallySymmetrical::SolutionDesignFromPage() {
  std::unique_ptr<anx::esolution::ExpDesignHeader> header =
      ExpDesignHeaderFromControl();
  std::unique_ptr<anx::esolution::ExpDesignBaseParam> base_param =
      ExpDesignBaseParamFromControl();
  std::unique_ptr<anx::esolution::ExpDesignResult> result =
      ExpDesignResultFromControl();
  anx::esolution::SolutionDesign solution_design;
  solution_design.header_ = std::move(header);
  solution_design.base_param_ = std::move(base_param);
  solution_design.result_ = std::move(result);
  return std::unique_ptr<anx::esolution::SolutionDesign>(
      new anx::esolution::SolutionDesign(solution_design));
}

////////////////////////////////////////////////////////////////////////////////

DUI_BEGIN_MESSAGE_MAP(WorkWindownFirstPageStressAjustable, DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_VALUECHANGED, OnValueChanged)
DUI_END_MESSAGE_MAP()
WorkWindownFirstPageStressAjustable::WorkWindownFirstPageStressAjustable(
    WorkWindow* pOwner,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : PageSolutionDesignBase(paint_manager_ui,
                             anx::esolution::kSolutionName_Stresses_Adjustable),
      pOwner_(pOwner),
      paint_manager_ui_(paint_manager_ui),
      t_prefix_(anx::esolution::ToTailPrefixName(design_type_)) {}

WorkWindownFirstPageStressAjustable::~WorkWindownFirstPageStressAjustable() {}

void WorkWindownFirstPageStressAjustable::Notify(TNotifyUI& msg) {
  if (msg.pSender == nullptr) {
    return;
  }
  DuiLib::CDuiString name = msg.pSender->GetName();
  if (name.IsEmpty()) {
    return;
  }
  if (msg.sType == DUI_MSGTYPE_KILLFOCUS) {
    /// TODO(hhool):
  } else if (msg.sType == DUI_MSGTYPE_CLICK) {
    if (msg.pSender->GetName() == _T("btn_exp_start")) {
      exp_running_ = true;
      set_enable_to_control_with_prefix("btn_solution_design_refresh",
                                        t_prefix_, paint_manager_ui_, false);
    } else if (msg.pSender->GetName() == _T("btn_exp_stop")) {
      set_enable_to_control_with_prefix("btn_solution_design_refresh",
                                        t_prefix_, paint_manager_ui_, true);
      exp_running_ = false;
    } else {
      // do nothing
    }
  }
}

void WorkWindownFirstPageStressAjustable::OnClick(TNotifyUI& msg) {
  if (msg.sType == kClick) {
    if (msg.pSender->GetName() == _T("btn_solution_design_refresh_stresses")) {
      // TODO(hhool):
    }
  }
}

void WorkWindownFirstPageStressAjustable::OnValueChanged(TNotifyUI& msg) {
  if (msg.sType == DUI_MSGTYPE_VALUECHANGED) {
    if (msg.pSender->GetName() == _T("work_args_area")) {
      ENMsgStruct* enmsg = reinterpret_cast<ENMsgStruct*>(msg.wParam);
      if (enmsg == nullptr) {
        LOG_F(LG_ERROR) << "WorkWindowFirstPageTh3pointBending::OnValueChanged"
                           "enmsg is nullptr";
        return;
      }
      if (enmsg->type_ == enmsg_type_exp_error) {
        set_enable_to_control_with_prefix("btn_solution_design_refresh",
                                          t_prefix_, paint_manager_ui_, true);
        exp_running_ = false;
      } else {
        // do nothing
      }
    } else {
      // do nothing
    }
  }
}

void WorkWindownFirstPageStressAjustable::InitPage() {
  PageSolutionDesignBase::InitPage();
}

void WorkWindownFirstPageStressAjustable::
    UpadateTabMainFirstPageElementViewResult() {
  // get header and base param and result from solution_design_
  anx::esolution::ExpDesignHeader* header = solution_design_->header_.get();
  anx::esolution::ExpDesignBaseParam* base_param =
      solution_design_->base_param_.get();
  anx::esolution::ExpDesignResult* result = solution_design_->result_.get();
  anx::esolution::ExpDesignResultStressesAdjustable* result_stresses =
      reinterpret_cast<anx::esolution::ExpDesignResultStressesAdjustable*>(
          result);
  set_value_to_edit_with_prefix("tm_page_first_left_amplitude", t_prefix_,
                                paint_manager_ui_,
                                result_stresses->f_eamplitude_);
  set_value_to_edit_with_prefix("tm_page_first_left_dc_stress", t_prefix_,
                                paint_manager_ui_,
                                result_stresses->f_dc_stress_MPa_);
  set_value_to_edit_with_prefix("tm_page_first_left_static_load", t_prefix_,
                                paint_manager_ui_,
                                result_stresses->f_static_load_MPa_);
  set_value_to_edit_with_prefix("tm_page_first_left_radius_exp", t_prefix_,
                                paint_manager_ui_,
                                result_stresses->f_exp_section_radius_R2_);
  set_value_to_edit_with_prefix("tm_page_first_exp_section_trans", t_prefix_,
                                paint_manager_ui_,
                                result_stresses->f_exp_section_length_L2_);
  set_value_to_edit_with_prefix("tm_page_first_left_radus_parallel", t_prefix_,
                                paint_manager_ui_,
                                result_stresses->f_parallel_section_radius_R1_);
  set_value_to_edit_with_prefix(
      "tm_page_first_left_radus_trans", t_prefix_, paint_manager_ui_,
      result_stresses->f_transition_section_radius_R0_);
  set_value_to_edit_with_prefix(
      "tm_page_first_left_length_trans", t_prefix_, paint_manager_ui_,
      result_stresses->f_transition_section_length_L1_);
  set_value_to_edit_with_prefix("tm_page_first_left_length_parallel", t_prefix_,
                                paint_manager_ui_,
                                result_stresses->f_parallel_section_length_L0_);
}

std::unique_ptr<anx::esolution::ExpDesignResult>
WorkWindownFirstPageStressAjustable::ExpDesignResultFromControl() {
  anx::esolution::ExpDesignResultStressesAdjustable
      exp_design_result_stresses_adjustable;
  exp_design_result_stresses_adjustable.f_eamplitude_ =
      get_value_from_edit_with_prefix<double>("tm_page_first_left_amplitude",
                                              t_prefix_, paint_manager_ui_);
  exp_design_result_stresses_adjustable.f_dc_stress_MPa_ =
      get_value_from_edit_with_prefix<double>("tm_page_first_left_dc_stress",
                                              t_prefix_, paint_manager_ui_);
  exp_design_result_stresses_adjustable.f_static_load_MPa_ =
      get_value_from_edit_with_prefix<double>("tm_page_first_left_static_load",
                                              t_prefix_, paint_manager_ui_);
  exp_design_result_stresses_adjustable.f_exp_section_radius_R2_ =
      get_value_from_edit_with_prefix<double>("tm_page_first_left_radius_exp",
                                              t_prefix_, paint_manager_ui_);
  exp_design_result_stresses_adjustable.f_parallel_section_radius_R1_ =
      get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_radus_parallel", t_prefix_, paint_manager_ui_);
  exp_design_result_stresses_adjustable.f_transition_section_radius_R0_ =
      get_value_from_edit_with_prefix<double>("tm_page_first_left_radus_trans",
                                              t_prefix_, paint_manager_ui_);
  exp_design_result_stresses_adjustable.f_transition_section_length_L1_ =
      get_value_from_edit_with_prefix<double>("tm_page_first_left_length_trans",
                                              t_prefix_, paint_manager_ui_);
  exp_design_result_stresses_adjustable.f_parallel_section_length_L0_ =
      get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_length_parallel", t_prefix_, paint_manager_ui_);
  exp_design_result_stresses_adjustable.f_exp_section_length_L2_ =
      get_value_from_edit_with_prefix<double>("tm_page_first_exp_section_trans",
                                              t_prefix_, paint_manager_ui_);
  return std::unique_ptr<anx::esolution::ExpDesignResult>(
      new anx::esolution::ExpDesignResultStressesAdjustable(
          exp_design_result_stresses_adjustable));
}

std::unique_ptr<anx::esolution::SolutionDesign>
WorkWindownFirstPageStressAjustable::SolutionDesignFromPage() {
  std::unique_ptr<anx::esolution::ExpDesignHeader> header =
      ExpDesignHeaderFromControl();
  std::unique_ptr<anx::esolution::ExpDesignBaseParam> base_param =
      ExpDesignBaseParamFromControl();
  std::unique_ptr<anx::esolution::ExpDesignResult> result =
      ExpDesignResultFromControl();
  anx::esolution::SolutionDesign solution_design;
  solution_design.header_ = std::move(header);
  solution_design.base_param_ = std::move(base_param);
  solution_design.result_ = std::move(result);
  return std::unique_ptr<anx::esolution::SolutionDesign>(
      new anx::esolution::SolutionDesign(solution_design));
}

////////////////////////////////////////////////////////////////////////////////

DUI_BEGIN_MESSAGE_MAP(WorkWindowFirstPageTh3pointBending, DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_VALUECHANGED, OnValueChanged)
DUI_END_MESSAGE_MAP()
WorkWindowFirstPageTh3pointBending::WorkWindowFirstPageTh3pointBending(
    WorkWindow* pOwner,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : PageSolutionDesignBase(paint_manager_ui,
                             anx::esolution::kSolutionName_Th3point_Bending),
      pOwner_(pOwner),
      paint_manager_ui_(paint_manager_ui),
      t_prefix_(anx::esolution::ToTailPrefixName(design_type_)) {
  paint_manager_ui_->AddNotifier(this);
}

WorkWindowFirstPageTh3pointBending::~WorkWindowFirstPageTh3pointBending() {
  paint_manager_ui_->RemoveNotifier(this);
}

void WorkWindowFirstPageTh3pointBending::Notify(TNotifyUI& msg) {
  if (msg.pSender == nullptr) {
    return;
  }
  DuiLib::CDuiString name = msg.pSender->GetName();
  if (name.IsEmpty()) {
    return;
  }
  if (msg.sType == DUI_MSGTYPE_KILLFOCUS) {
    if (exp_running_)
      return;
    std::string ctrl_name = anx::common::WString2String(name.GetData());
    // check ctrl_name contain tm_page_first_left_elastic
    if (ctrl_name.find("tm_page_first_left_elastic") != std::string::npos) {
      // check the value of edit_elastic_
      double f_elastic_modulus_GPa = get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_elastic", t_prefix_, paint_manager_ui_);
      if (f_elastic_modulus_GPa <= 0.0) {
        // TODO(hhool): show error message, show with tips
        anx::ui::DialogCommon::ShowDialog(
            *pOwner_, "错误", "弹性模量必须大于0.0",
            anx::ui::DialogCommon::kDialogCommonStyleOk);
      }
    } else if (ctrl_name.find("tm_page_first_left_density") !=
               std::string::npos) {
      // check the value of edit_density_
      double f_density_kg_m3 = get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_density", t_prefix_, paint_manager_ui_);
      if (f_density_kg_m3 <= 0.0f) {
        // TODO(hhool): show error message, show with tips
        anx::ui::DialogCommon::ShowDialog(
            *pOwner_, "错误", "密度必须大于0.0",
            anx::ui::DialogCommon::kDialogCommonStyleOk);
      }
    } else if (ctrl_name.find("tm_page_first_left_max_stress") !=
               std::string::npos) {
      // check the value of edit_max_stress_
      double f_max_stress_MPa = get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_max_stress", t_prefix_, paint_manager_ui_);
      if (f_max_stress_MPa <= 0.0f) {
        // TODO(hhool): show error message, show with tips
        anx::ui::DialogCommon::ShowDialog(
            *pOwner_, "错误", "最大应力必须大于0.0",
            anx::ui::DialogCommon::kDialogCommonStyleOk);
      }
    } else if (ctrl_name.find("tm_page_first_left_ratio_stress") !=
               std::string::npos) {
      // check the value of edit_stress_ratio_
      double f_stress_ratio = get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_ratio_stress", t_prefix_, paint_manager_ui_);
      if (f_stress_ratio <= 0.0f) {
        // TODO(hhool): show error message, show with tips
        anx::ui::DialogCommon::ShowDialog(
            *pOwner_, "错误", "应力比必须大于0.0",
            anx::ui::DialogCommon::kDialogCommonStyleOk);
      }
    }
  } else if (msg.sType == DUI_MSGTYPE_CLICK) {
    if (msg.pSender->GetName() == _T("btn_exp_start")) {
      exp_running_ = true;
      set_enable_to_control_with_prefix("btn_solution_design_refresh",
                                        t_prefix_, paint_manager_ui_, false);
    } else if (msg.pSender->GetName() == _T("btn_exp_stop")) {
      set_enable_to_control_with_prefix("btn_solution_design_refresh",
                                        t_prefix_, paint_manager_ui_, true);
      exp_running_ = false;
    } else if (msg.pSender->GetName() == _T("btn_exp_pause")) {
      // do nothing
    } else if (msg.pSender->GetName() == _T("btn_exp_resume")) {
      // do nothing
    }
  }
}

void WorkWindowFirstPageTh3pointBending::OnClick(TNotifyUI& msg) {
  if (msg.sType == kClick) {
    if (msg.pSender->GetName() == _T("btn_solution_design_refresh_th3point")) {
      double f_elastic_modulus_GPa = get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_elastic", t_prefix_, paint_manager_ui_);
      double f_density_kg_m3 = get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_density", t_prefix_, paint_manager_ui_);
      double f_max_stress_MPa = get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_max_stress", t_prefix_, paint_manager_ui_);
      double f_stress_ratio = get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_ratio_stress", t_prefix_, paint_manager_ui_);
      double f_static_load_MPa = get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_static_load", t_prefix_, paint_manager_ui_);
      double f_amplitude = get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_amplitude", t_prefix_, paint_manager_ui_);
      double f_height = get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_speciments_height", t_prefix_, paint_manager_ui_);
      double f_width = get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_speciments_width", t_prefix_, paint_manager_ui_);
      double f_length = anx::esolution::algorithm::CalcTh3Design(
          f_elastic_modulus_GPa, f_height, f_density_kg_m3, 20, f_width,
          f_height, anx::esolution::algorithm::kConstForLenghtOfTh3Design);
      set_value_to_edit_with_prefix("tm_page_first_left_speciments_length",
                                    t_prefix_, paint_manager_ui_, f_length);
      double f_length_parallel = anx::esolution::algorithm::CalcTh3Design(
          f_elastic_modulus_GPa, f_height, f_density_kg_m3, 20, f_width,
          f_height, anx::esolution::algorithm::kConstForLength0OfTh3Design);
      set_value_to_edit_with_prefix("tm_page_first_left_length_parallel",
                                    t_prefix_, paint_manager_ui_,
                                    f_length_parallel);
      LOG_F(LG_INFO) << "CalculateTh3pointBending:f_length:" << f_length
                     << " f_length_parallel:" << f_length_parallel;
      if (f_max_stress_MPa >= 0) {
        f_static_load_MPa = (1 + f_stress_ratio) / 2 * f_max_stress_MPa;
        LOG_F(LG_INFO) << "CalculateTh3pointBending:f_static_load_MPa:"
                       << f_static_load_MPa;
        set_value_to_edit_with_prefix("tm_page_first_left_static_load",
                                      t_prefix_, paint_manager_ui_,
                                      f_static_load_MPa);
      }

      double f_theroy_dc_stress = get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_theory_dc_stress", t_prefix_, paint_manager_ui_);
      double f_theroy_dc_stress_cal =
          sqrt(12 * f_density_kg_m3 * f_elastic_modulus_GPa * f_height *
               f_height) *
          0.000653;
      double f_theory_eamplitude = -1.f;
      // f_theroy_dc_stress_cal is not 0
      if (f_theroy_dc_stress_cal != 0.0) {
        f_theroy_dc_stress = f_theroy_dc_stress_cal;
        // use f_theroy_dc_stress_cal to cal f_theory_eamplitude
        f_theory_eamplitude =
            (1 - f_stress_ratio) / 2 * f_max_stress_MPa / f_theroy_dc_stress;
        set_value_to_edit_with_prefix("tm_page_first_left_theory_amplitude",
                                      t_prefix_, paint_manager_ui_,
                                      f_theory_eamplitude);
        set_value_to_edit_with_prefix("tm_page_first_left_theory_dc_stress",
                                      t_prefix_, paint_manager_ui_,
                                      f_theroy_dc_stress);
        LOG_F(LG_INFO) << "CalculateTh3pointBending:f_theory_dc_stress:"
                       << f_theroy_dc_stress
                       << " f_theory_eamplitude:" << f_theory_eamplitude;
      }

      double f_dc_stress = get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_dc_stress", t_prefix_, paint_manager_ui_);
      // f_dc_stress  is not 0
      if (f_dc_stress != 0.0) {
        // use f_dc_stress_cal to cal f_amplitude
        f_amplitude = (1 - f_stress_ratio) / 2 * f_max_stress_MPa / f_dc_stress;
        LOG_F(LG_INFO) << "CalculateTh3pointBending:f_dc_stress:" << f_dc_stress
                       << " f_amplitude:" << f_amplitude;
        set_value_to_edit_with_prefix("tm_page_first_left_amplitude", t_prefix_,
                                      paint_manager_ui_, f_amplitude);
      } else {
        f_amplitude = -1.0f;
      }
      pOwner_->UpdateArgsArea(-1, -1, f_amplitude, f_static_load_MPa,
                              f_max_stress_MPa, f_stress_ratio);
      if (f_amplitude != -1.0f) {
        // get amplitude from solution design and
        // transform to power step
        std::unique_ptr<anx::device::DeviceExpAmplitudeSettings> deas =
            anx::device::LoadDeviceExpAmplitudeSettingsDefaultResource();
        int32_t exp_power = 0;
        anx::device::Amp2DeviceExpPower(
            *deas, static_cast<float>(f_amplitude * 2), &exp_power);
        LOG_F(LG_INFO) << "exp_power:" << exp_power
                       << " exp_amplitude_:" << f_amplitude;
        /// TODO(hhool): make 20 and 100 to const as 2000C property
        if (exp_power < 20 || exp_power > 100) {
          std::string msg = "超出振幅范围";
          // format the message
          anx::ui::DialogCommon::ShowDialog(
              *pOwner_, "错误", msg.c_str(),
              anx::ui::DialogCommon::kDialogCommonStyleOk);
        }
      }
    }
  }
}

void WorkWindowFirstPageTh3pointBending::OnValueChanged(TNotifyUI& msg) {
  if (msg.sType == DUI_MSGTYPE_VALUECHANGED) {
    if (msg.pSender->GetName() == _T("work_args_area")) {
      ENMsgStruct* enmsg = reinterpret_cast<ENMsgStruct*>(msg.wParam);
      if (enmsg == nullptr) {
        LOG_F(LG_ERROR) << "WorkWindowFirstPageTh3pointBending::OnValueChanged"
                           "enmsg is nullptr";
        return;
      }
      if (enmsg->type_ == enmsg_type_exp_error) {
        set_enable_to_control_with_prefix("btn_solution_design_refresh",
                                          t_prefix_, paint_manager_ui_, true);
        exp_running_ = false;
      } else {
        // do nothing
      }
    } else {
      // do nothing
    }
  }
}

void WorkWindowFirstPageTh3pointBending::InitPage() {
  __super::InitPage();
}

void WorkWindowFirstPageTh3pointBending::
    UpadateTabMainFirstPageElementViewResult() {
  // get header and base param and result from solution_design_
  anx::esolution::ExpDesignHeader* header = solution_design_->header_.get();
  anx::esolution::ExpDesignBaseParam* base_param =
      solution_design_->base_param_.get();
  anx::esolution::ExpDesignResult* result = solution_design_->result_.get();
  // update result
  anx::esolution::ExpDesignResultTh3pointBending* result_th3point =
      reinterpret_cast<anx::esolution::ExpDesignResultTh3pointBending*>(result);
  set_value_to_edit_with_prefix("tm_page_first_left_theory_amplitude",
                                t_prefix_, paint_manager_ui_,
                                result_th3point->f_theory_eamplitude_);
  set_value_to_edit_with_prefix("tm_page_first_left_theory_dc_stress",
                                t_prefix_, paint_manager_ui_,
                                result_th3point->f_theory_dc_stress_MPa_);
  set_value_to_edit_with_prefix("tm_page_first_left_amplitude", t_prefix_,
                                paint_manager_ui_,
                                result_th3point->f_eamplitude_);
  set_value_to_edit_with_prefix("tm_page_first_left_dc_stress", t_prefix_,
                                paint_manager_ui_,
                                result_th3point->f_dc_stress_MPa_);
  set_value_to_edit_with_prefix("tm_page_first_left_static_load", t_prefix_,
                                paint_manager_ui_,
                                result_th3point->f_static_load_MPa_);
  set_value_to_edit_with_prefix("tm_page_first_left_speciments_width",
                                t_prefix_, paint_manager_ui_,
                                result_th3point->f_specimen_width_B_);
  set_value_to_edit_with_prefix("tm_page_first_left_speciments_height",
                                t_prefix_, paint_manager_ui_,
                                result_th3point->f_specimen_thickness_h_);
  set_value_to_edit_with_prefix("tm_page_first_left_speciments_length",
                                t_prefix_, paint_manager_ui_,
                                result_th3point->f_specimen_length_L_);
  set_value_to_edit_with_prefix("tm_page_first_left_length_parallel", t_prefix_,
                                paint_manager_ui_,
                                result_th3point->f_support_distance_L0_);
}

std::unique_ptr<anx::esolution::ExpDesignResult>
WorkWindowFirstPageTh3pointBending::ExpDesignResultFromControl() {
  anx::esolution::ExpDesignResultTh3pointBending exp_design_result_th3point;
  exp_design_result_th3point.f_theory_eamplitude_ =
      get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_theory_amplitude", t_prefix_, paint_manager_ui_);
  exp_design_result_th3point.f_theory_dc_stress_MPa_ =
      get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_theory_dc_stress", t_prefix_, paint_manager_ui_);
  exp_design_result_th3point.f_eamplitude_ =
      get_value_from_edit_with_prefix<double>("tm_page_first_left_amplitude",
                                              t_prefix_, paint_manager_ui_);
  exp_design_result_th3point.f_dc_stress_MPa_ =
      get_value_from_edit_with_prefix<double>("tm_page_first_left_dc_stress",
                                              t_prefix_, paint_manager_ui_);
  exp_design_result_th3point.f_static_load_MPa_ =
      get_value_from_edit_with_prefix<double>("tm_page_first_left_static_load",
                                              t_prefix_, paint_manager_ui_);
  exp_design_result_th3point.f_specimen_width_B_ =
      get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_speciments_width", t_prefix_, paint_manager_ui_);
  exp_design_result_th3point.f_specimen_thickness_h_ =
      get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_speciments_height", t_prefix_, paint_manager_ui_);
  exp_design_result_th3point.f_specimen_length_L_ =
      get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_speciments_length", t_prefix_, paint_manager_ui_);
  exp_design_result_th3point.f_support_distance_L0_ =
      get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_length_parallel", t_prefix_, paint_manager_ui_);
  return std::unique_ptr<anx::esolution::ExpDesignResult>(
      new anx::esolution::ExpDesignResultTh3pointBending(
          exp_design_result_th3point));
}

std::unique_ptr<anx::esolution::SolutionDesign>
WorkWindowFirstPageTh3pointBending::SolutionDesignFromPage() {
  std::unique_ptr<anx::esolution::ExpDesignHeader> header =
      ExpDesignHeaderFromControl();
  std::unique_ptr<anx::esolution::ExpDesignBaseParam> base_param =
      ExpDesignBaseParamFromControl();
  std::unique_ptr<anx::esolution::ExpDesignResult> result =
      ExpDesignResultFromControl();
  anx::esolution::SolutionDesign solution_design;
  solution_design.header_ = std::move(header);
  solution_design.base_param_ = std::move(base_param);
  solution_design.result_ = std::move(result);
  return std::unique_ptr<anx::esolution::SolutionDesign>(
      new anx::esolution::SolutionDesign(solution_design));
}

////////////////////////////////////////////////////////////////////////////////

DUI_BEGIN_MESSAGE_MAP(WorkWindowFirstPageVibrationBending, DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_VALUECHANGED, OnValueChanged)
DUI_END_MESSAGE_MAP()

WorkWindowFirstPageVibrationBending::WorkWindowFirstPageVibrationBending(
    WorkWindow* pOwner,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : PageSolutionDesignBase(paint_manager_ui,
                             anx::esolution::kSolutionName_Vibration_Bending),
      pOwner_(pOwner),
      paint_manager_ui_(paint_manager_ui),
      t_prefix_(anx::esolution::ToTailPrefixName(design_type_)) {
  paint_manager_ui_->AddNotifier(this);
}

WorkWindowFirstPageVibrationBending::~WorkWindowFirstPageVibrationBending() {
  paint_manager_ui_->RemoveNotifier(this);
}

void WorkWindowFirstPageVibrationBending::Notify(TNotifyUI& msg) {
  if (msg.pSender == nullptr) {
    return;
  }
  DuiLib::CDuiString name = msg.pSender->GetName();
  if (name.IsEmpty()) {
    return;
  }
  if (msg.sType == DUI_MSGTYPE_KILLFOCUS) {
    /// TODO(hhool):
  } else if (msg.sType == DUI_MSGTYPE_CLICK) {
    if (msg.pSender->GetName() == _T("btn_exp_start")) {
      exp_running_ = true;
      set_enable_to_control_with_prefix("btn_solution_design_refresh",
                                        t_prefix_, paint_manager_ui_, false);
    } else if (msg.pSender->GetName() == _T("btn_exp_stop")) {
      set_enable_to_control_with_prefix("btn_solution_design_refresh",
                                        t_prefix_, paint_manager_ui_, true);
      exp_running_ = false;
    } else {
      // do nothing
    }
  }
}

void WorkWindowFirstPageVibrationBending::OnClick(TNotifyUI& msg) {
  if (msg.sType == kClick) {
    if (msg.pSender->GetName() == _T("btn_solution_design_refresh_vibration")) {
      // TODO(hhool):
    }
  }
}

void WorkWindowFirstPageVibrationBending::OnValueChanged(TNotifyUI& msg) {
  if (msg.sType == DUI_MSGTYPE_VALUECHANGED) {
    if (msg.pSender->GetName() == _T("work_args_area")) {
      ENMsgStruct* enmsg = reinterpret_cast<ENMsgStruct*>(msg.wParam);
      if (enmsg == nullptr) {
        LOG_F(LG_ERROR) << "WorkWindowFirstPageTh3pointBending::OnValueChanged"
                           "enmsg is nullptr";
        return;
      }
      if (enmsg->type_ == enmsg_type_exp_error) {
        set_enable_to_control_with_prefix("btn_solution_design_refresh",
                                          t_prefix_, paint_manager_ui_, true);
        exp_running_ = false;
      } else {
        // do nothing
      }
    } else {
      // do nothing
    }
  }
}

void WorkWindowFirstPageVibrationBending::InitPage() {
  __super::InitPage();
}

void WorkWindowFirstPageVibrationBending::
    UpadateTabMainFirstPageElementViewResult() {
  // get header and base param and result from solution_design_
  anx::esolution::ExpDesignHeader* header = solution_design_->header_.get();
  anx::esolution::ExpDesignBaseParam* base_param =
      solution_design_->base_param_.get();
  anx::esolution::ExpDesignResult* result =
      solution_design_->result_.get();  // update result
  anx::esolution::ExpDesignResultVibrationBending* result_vibration =
      reinterpret_cast<anx::esolution::ExpDesignResultVibrationBending*>(
          result);
  set_value_to_edit_with_prefix("tm_page_first_left_amplitude", t_prefix_,
                                paint_manager_ui_,
                                result_vibration->f_eamplitude_);
  set_value_to_edit_with_prefix("tm_page_first_left_dc_stress", t_prefix_,
                                paint_manager_ui_,
                                result_vibration->f_dc_stress_MPa_);
  set_value_to_edit_with_prefix(
      "tm_page_first_left_length_parallel_section", t_prefix_,
      paint_manager_ui_,
      result_vibration->f_specimen_length_parallel_section_L1_);
  set_value_to_edit_with_prefix("tm_page_first_left_specimen_radius_arc",
                                t_prefix_, paint_manager_ui_,
                                result_vibration->f_specimen_radius_arc_R1_);
  set_value_to_edit_with_prefix(
      "tm_page_first_left_specimen_radius_transition", t_prefix_,
      paint_manager_ui_, result_vibration->f_specimen_radius_transition_R2_);
  set_value_to_edit_with_prefix("tm_page_first_left_thickness_clamp_section",
                                t_prefix_, paint_manager_ui_,
                                result_vibration->f_thickness_clamping_d1_);
  set_value_to_edit_with_prefix(
      "tm_page_first_left_thickness_exp_section", t_prefix_, paint_manager_ui_,
      result_vibration->f_thickness_exp_section_L0_d2_);
}

std::unique_ptr<anx::esolution::ExpDesignResult>
WorkWindowFirstPageVibrationBending::ExpDesignResultFromControl() {
  anx::esolution::ExpDesignResultVibrationBending exp_design_result_vibration;
  exp_design_result_vibration.f_eamplitude_ =
      get_value_from_edit_with_prefix<double>("tm_page_first_left_amplitude",
                                              t_prefix_, paint_manager_ui_);
  exp_design_result_vibration.f_dc_stress_MPa_ =
      get_value_from_edit_with_prefix<double>("tm_page_first_left_dc_stress",
                                              t_prefix_, paint_manager_ui_);
  exp_design_result_vibration.f_specimen_length_parallel_section_L1_ =
      get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_length_parallel_section", t_prefix_,
          paint_manager_ui_);
  exp_design_result_vibration.f_specimen_radius_arc_R1_ =
      get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_specimen_radius_arc", t_prefix_,
          paint_manager_ui_);
  exp_design_result_vibration.f_specimen_radius_transition_R2_ =
      get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_specimen_radius_transition", t_prefix_,
          paint_manager_ui_);
  exp_design_result_vibration.f_thickness_clamping_d1_ =
      get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_thickness_clamp_section", t_prefix_,
          paint_manager_ui_);
  exp_design_result_vibration.f_thickness_exp_section_L0_d2_ =
      get_value_from_edit_with_prefix<double>(
          "tm_page_first_left_thickness_exp_section", t_prefix_,
          paint_manager_ui_);
  return std::unique_ptr<anx::esolution::ExpDesignResult>(
      new anx::esolution::ExpDesignResultVibrationBending(
          exp_design_result_vibration));
}

std::unique_ptr<anx::esolution::SolutionDesign>
WorkWindowFirstPageVibrationBending::SolutionDesignFromPage() {
  std::unique_ptr<anx::esolution::ExpDesignHeader> header =
      ExpDesignHeaderFromControl();
  std::unique_ptr<anx::esolution::ExpDesignBaseParam> base_param =
      ExpDesignBaseParamFromControl();
  std::unique_ptr<anx::esolution::ExpDesignResult> result =
      ExpDesignResultFromControl();
  anx::esolution::SolutionDesign solution_design;
  solution_design.header_ = std::move(header);
  solution_design.base_param_ = std::move(base_param);
  solution_design.result_ = std::move(result);
  return std::unique_ptr<anx::esolution::SolutionDesign>(
      new anx::esolution::SolutionDesign(solution_design));
}
}  // namespace ui
}  // namespace anx
