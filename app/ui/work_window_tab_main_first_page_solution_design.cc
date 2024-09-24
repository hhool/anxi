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

#include "app/common/defines.h"
#include "app/common/string_utils.h"
#include "app/esolution/solution_design.h"
#include "app/esolution/solution_design_default.h"
#include "app/esolution/solution_design_helper.h"
#include "app/ui/dialog_about.h"
#include "app/ui/dialog_com_port_settings.h"
#include "app/ui/ui_constants.h"
#include "app/ui/work_window.h"
#include "app/ui/work_window_menu_design.h"
#include "app/ui/work_window_menu_store.h"

////////////////////////////////////////////////////////////////////////////////
namespace anx {
namespace ui {
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
  DuiLib::CDuiString tail_prefix;
  tail_prefix.Append(
      anx::common::string2wstring(
          anx::esolution::ToTailPrefixName(header->solution_type_))
          .c_str());

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
      std::wstring result_str(anx::common::string2wstring(result));
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
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_elastic));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), base_param->f_elastic_modulus_GPa_);
    edit->SetText(value);
  }
  {
    DuiLib::CDuiString name_density = _T("tm_page_first_left_density");
    name_density.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_density));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), base_param->f_density_kg_m3_);
    edit->SetText(value);
  }

  {
    DuiLib::CDuiString name_max_stress = _T("tm_page_first_left_max_stress");
    name_max_stress.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_max_stress));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), base_param->f_max_stress_MPa_);
    edit->SetText(value);
  }

  {
    DuiLib::CDuiString name_stress_ratio =
        _T("tm_page_first_left_ratio_stress");
    name_stress_ratio.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_stress_ratio));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), base_param->f_stress_ratio_);
    edit->SetText(value);
  }
  return 0;
}

std::unique_ptr<anx::esolution::ExpDesignHeader>
PageSolutionDesignBase::ExpDesignHeaderFromControl() {
  std::string t_prefix = anx::esolution::ToTailPrefixName(design_type_);
  DuiLib::CDuiString name_specimensa = _T("tm_page_first_left_name_specimens");
  DuiLib::CDuiString tail_prefix;
  tail_prefix.Append(
      (LPCTSTR)(anx::common::string2wstring(t_prefix.c_str()).c_str()));
  DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_specimensa + tail_prefix));
  DuiLib::CDuiString value = edit->GetText().GetData();

  std::string name = anx::common::UnicodeToUTF8(value.GetData());
  anx::esolution::ExpDesignHeader exp_design_header;
  exp_design_header.solution_type_ = design_type_;
  memcpy(exp_design_header.name_, name.data(), 255);
  return std::unique_ptr<anx::esolution::ExpDesignHeader>(
      new anx::esolution::ExpDesignHeader(exp_design_header));
}

std::unique_ptr<anx::esolution::ExpDesignBaseParam>
PageSolutionDesignBase::ExpDesignBaseParamFromControl() {
  std::string t_prefix = anx::esolution::ToTailPrefixName(design_type_);
  DuiLib::CDuiString tail_prefix;
  tail_prefix.Append(
      (LPCTSTR)(anx::common::string2wstring(t_prefix.c_str()).c_str()));
  DuiLib::CDuiString name_material = _T("tm_page_first_left_material");
  DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_material + tail_prefix));
  DuiLib::CDuiString value;
  if (edit) {
    value = edit->GetText().GetData();
  } else {
    value = _T("");
  }
  std::string material_name = anx::common::wstring2string(value.GetData());

  DuiLib::CDuiString name_elastic = _T("tm_page_first_left_elastic");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_elastic + tail_prefix));
  value = edit->GetText().GetData();
  float f_elastic_modulus_GPa_ = static_cast<float>(_ttof(value.GetData()));

  DuiLib::CDuiString name_density = _T("tm_page_first_left_density");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_density + tail_prefix));
  value = edit->GetText().GetData();
  float f_density_kg_m3_ = static_cast<float>(_ttof(value.GetData()));

  DuiLib::CDuiString name_max_stress = _T("tm_page_first_left_max_stress");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_max_stress + tail_prefix));
  value = edit->GetText().GetData();
  float f_max_stress_MPa_ = static_cast<float>(_ttof(value.GetData()));

  DuiLib::CDuiString name_ratio_stress = _T("tm_page_first_left_ratio_stress");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_ratio_stress + tail_prefix));
  value = edit->GetText().GetData();
  float f_stress_ratio_ = static_cast<float>(_ttof(value.GetData()));

  anx::esolution::ExpDesignBaseParam exp_design_base_param;
  memcpy(exp_design_base_param.material_name_, material_name.data(), 255);
  exp_design_base_param.f_elastic_modulus_GPa_ = f_elastic_modulus_GPa_;
  exp_design_base_param.f_density_kg_m3_ = f_density_kg_m3_;
  exp_design_base_param.f_max_stress_MPa_ = f_max_stress_MPa_;
  exp_design_base_param.f_stress_ratio_ = f_stress_ratio_;
  return std::unique_ptr<anx::esolution::ExpDesignBaseParam>(
      new anx::esolution::ExpDesignBaseParam(exp_design_base_param));
}
}  // namespace ui
}  // namespace anx

DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindowFirstPageAxiallySymmetrical,
                      DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {

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
    ~WorkWindowFirstPageAxiallySymmetrical() {}

void WorkWindowFirstPageAxiallySymmetrical::Notify(TNotifyUI& msg) {}

void WorkWindowFirstPageAxiallySymmetrical::OnClick(TNotifyUI& msg) {
  if (msg.sType == kClick) {
    if (msg.pSender->GetName() == _T("solution_design_refresh")) {
      // TODO(hhool):
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
  // get tail prefix
  DuiLib::CDuiString tail_prefix = _T("");
  tail_prefix = _T("_axially");
  // update result
  anx::esolution::ExpDesignResultAxially* result_axially =
      reinterpret_cast<anx::esolution::ExpDesignResultAxially*>(result);
  {
    DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
    name_amplitude.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_amplitude));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_axially->f_eamplitude_);
    edit->SetText(value);
  }
  {
    DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
    name_dc_stress.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_dc_stress));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_axially->f_dc_stress_MPa_);
    edit->SetText(value);
  }
  {
    DuiLib::CDuiString name_radius_exp = _T("tm_page_first_left_radius_exp");
    name_radius_exp.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_radius_exp));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_axially->f_exp_section_radius_R2_);
    edit->SetText(value);
  }
  {
    DuiLib::CDuiString name_radius_parallel =
        _T("tm_page_first_left_radus_parallel");
    name_radius_parallel.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_radius_parallel));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_axially->f_parallel_section_radius_R1_);
    edit->SetText(value);
  }
  {
    DuiLib::CDuiString name_radus_trans = _T("tm_page_first_left_radus_trans");
    name_radus_trans.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_radus_trans));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_axially->f_transition_section_radius_R0_);
    edit->SetText(value);
  }
  {
    DuiLib::CDuiString name_length_trans =
        _T("tm_page_first_left_length_trans");
    name_length_trans.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_length_trans));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_axially->f_transition_section_length_L1_);
    edit->SetText(value);
  }
  {
    DuiLib::CDuiString name_length_parallel =
        _T("tm_page_first_left_length_parallel");
    name_length_parallel.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_length_parallel));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_axially->f_parallel_section_length_L0_);
    edit->SetText(value);
  }
}

std::unique_ptr<anx::esolution::ExpDesignResult>
anx::ui::WorkWindowFirstPageAxiallySymmetrical::ExpDesignResultFromControl() {
  std::string t_prefix = anx::esolution::ToTailPrefixName(design_type_);
  DuiLib::CDuiString tail_prefix;
  tail_prefix.Append(
      (LPCTSTR)(anx::common::string2wstring(t_prefix.c_str()).c_str()));
  anx::esolution::ExpDesignResultAxially exp_design_result_axially;
  DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
  DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_amplitude + tail_prefix));
  DuiLib::CDuiString value = edit->GetText().GetData();
  exp_design_result_axially.f_eamplitude_ = _ttof(value.GetData());

  DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_dc_stress + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_axially.f_dc_stress_MPa_ = _ttof(value.GetData());

  DuiLib::CDuiString name_radius_exp = _T("tm_page_first_left_radius_exp");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_radius_exp + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_axially.f_exp_section_radius_R2_ = _ttof(value.GetData());

  DuiLib::CDuiString name_radius_parallel =
      _T("tm_page_first_left_radus_parallel");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_radius_parallel + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_axially.f_parallel_section_radius_R1_ =
      _ttof(value.GetData());

  DuiLib::CDuiString name_radus_trans = _T("tm_page_first_left_radus_trans");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_radus_trans + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_axially.f_transition_section_radius_R0_ =
      _ttof(value.GetData());

  DuiLib::CDuiString name_length_trans = _T("tm_page_first_left_length_trans");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_length_trans + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_axially.f_transition_section_length_L1_ =
      _ttof(value.GetData());

  DuiLib::CDuiString name_length_parallel =
      _T("tm_page_first_left_length_parallel");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_length_parallel + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_axially.f_parallel_section_length_L0_ =
      _ttof(value.GetData());

  DuiLib::CDuiString name_exp_section = _T("tm_page_first_exp_section_trans");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_exp_section + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_axially.f_exp_section_length_L2_ = _ttof(value.GetData());

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
}  // namespace ui
}  // namespace anx

////////////////////////////////////////////////////////////////////////////////

DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindownFirstPageStressAjustable,
                      DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {
WorkWindownFirstPageStressAjustable::WorkWindownFirstPageStressAjustable(
    WorkWindow* pOwner,
    DuiLib::CPaintManagerUI* paint_manager_ui)
    : PageSolutionDesignBase(paint_manager_ui,
                             anx::esolution::kSolutionName_Stresses_Adjustable),
      pOwner_(pOwner),
      paint_manager_ui_(paint_manager_ui) {}

WorkWindownFirstPageStressAjustable::~WorkWindownFirstPageStressAjustable() {}

void WorkWindownFirstPageStressAjustable::Notify(TNotifyUI& msg) {}

void WorkWindownFirstPageStressAjustable::OnClick(TNotifyUI& msg) {
  if (msg.sType == kClick) {
    if (msg.pSender->GetName() == _T("solution_design_refresh")) {
      // TODO(hhool):
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
  // get tail prefix
  DuiLib::CDuiString tail_prefix = _T("");
  tail_prefix = _T("_stresses");
  anx::esolution::ExpDesignResultStressesAdjustable* result_stresses =
      reinterpret_cast<anx::esolution::ExpDesignResultStressesAdjustable*>(
          result);
  {
    DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
    name_amplitude.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_amplitude));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_stresses->f_eamplitude_);
    edit->SetText(value);
  }
  {
    DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
    name_dc_stress.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_dc_stress));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_stresses->f_dc_stress_MPa_);
    edit->SetText(value);
  }
  {
    DuiLib::CDuiString name_static_load = _T("tm_page_first_left_static_load");
    name_static_load.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_static_load));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_stresses->f_static_load_MPa_);
    edit->SetText(value);
  }
  {
    DuiLib::CDuiString name_radius_exp = _T("tm_page_first_left_radius_exp");
    name_radius_exp.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_radius_exp));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_stresses->f_exp_section_radius_R2_);
    edit->SetText(value);
  }
  {
    DuiLib::CDuiString name_radius_load = _T("tm_page_first_exp_section_trans");
    name_radius_load.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_radius_load));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_stresses->f_exp_section_length_L2_);
    edit->SetText(value);
  }
}

std::unique_ptr<anx::esolution::ExpDesignResult>
anx::ui::WorkWindownFirstPageStressAjustable::ExpDesignResultFromControl() {
  std::string t_prefix = anx::esolution::ToTailPrefixName(design_type_);
  DuiLib::CDuiString tail_prefix;
  tail_prefix.Append(
      (LPCTSTR)(anx::common::string2wstring(t_prefix.c_str()).c_str()));
  anx::esolution::ExpDesignResultStressesAdjustable
      exp_design_result_stresses_adjustable;

  DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
  DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_amplitude + tail_prefix));
  DuiLib::CDuiString value = edit->GetText().GetData();
  exp_design_result_stresses_adjustable.f_eamplitude_ = _ttof(value.GetData());

  DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_dc_stress + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_stresses_adjustable.f_dc_stress_MPa_ =
      _ttof(value.GetData());

  DuiLib::CDuiString name_static_load = _T("tm_page_first_left_static_load");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_static_load + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_stresses_adjustable.f_static_load_MPa_ =
      _ttof(value.GetData());

  DuiLib::CDuiString name_radius_exp = _T("tm_page_first_left_radius_exp");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_radius_exp + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_stresses_adjustable.f_exp_section_radius_R2_ =
      _ttof(value.GetData());

  DuiLib::CDuiString name_radius_parallel =
      _T("tm_page_first_left_radus_parallel");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_radius_parallel + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_stresses_adjustable.f_parallel_section_radius_R1_ =
      _ttof(value.GetData());

  DuiLib::CDuiString name_radus_trans = _T("tm_page_first_left_radus_trans");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_radus_trans + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_stresses_adjustable.f_transition_section_radius_R0_ =
      _ttof(value.GetData());

  DuiLib::CDuiString name_length_trans = _T("tm_page_first_left_length_trans");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_length_trans + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_stresses_adjustable.f_transition_section_length_L1_ =
      _ttof(value.GetData());

  DuiLib::CDuiString name_length_parallel =
      _T("tm_page_first_left_length_parallel");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_length_parallel + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_stresses_adjustable.f_parallel_section_length_L0_ =
      _ttof(value.GetData());

  DuiLib::CDuiString name_exp_section = _T("tm_page_first_exp_section_trans");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_exp_section + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_stresses_adjustable.f_exp_section_length_L2_ =
      _ttof(value.GetData());

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
}  // namespace ui
}  // namespace anx
////////////////////////////////////////////////////////////////////////////////

DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindowFirstPageTh3pointBending,
                      DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()
namespace anx {
namespace ui {
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

WorkWindowFirstPageTh3pointBending::~WorkWindowFirstPageTh3pointBending() {}

void WorkWindowFirstPageTh3pointBending::Notify(TNotifyUI& msg) {
  if (msg.pSender == edit_elastic_) {
    if (msg.sType == DUI_MSGTYPE_KILLFOCUS) {
      // check the value of edit_elastic_
      DuiLib::CDuiString value = edit_elastic_->GetText();
      float f_elastic_modulus_GPa_ = static_cast<float>(_ttof(value.GetData()));
      if (f_elastic_modulus_GPa_ <= 0.0f) {
        // show error message
        MessageBox(paint_manager_ui_->GetPaintWindow(),
                   _T("Elastic modulus must be greater than 0.0"), _T("Error"),
                   MB_OK);
      }
    }
  } else if (msg.pSender == edit_density_) {
    if (msg.sType == DUI_MSGTYPE_KILLFOCUS) {
      // check the value of edit_density_
      DuiLib::CDuiString value = edit_density_->GetText();
      float f_density_kg_m3_ = static_cast<float>(_ttof(value.GetData()));
      if (f_density_kg_m3_ <= 0.0f) {
        // show error message
        MessageBox(paint_manager_ui_->GetPaintWindow(),
                   _T("Density must be greater than 0.0"), _T("Error"), MB_OK);
      }
    }
  } else if (msg.pSender == edit_max_stress_) {
    if (msg.sType == DUI_MSGTYPE_KILLFOCUS) {
      // check the value of edit_max_stress_
      DuiLib::CDuiString value = edit_max_stress_->GetText();
      float f_max_stress_MPa_ = static_cast<float>(_ttof(value.GetData()));
      if (f_max_stress_MPa_ <= 0.0f) {
        // show error message
        MessageBox(paint_manager_ui_->GetPaintWindow(),
                   _T("Max stress must be greater than 0.0"), _T("Error"),
                   MB_OK);
      }
    }
  } else if (msg.pSender == edit_stress_ratio_) {
    if (msg.sType == DUI_MSGTYPE_KILLFOCUS) {
      // check the value of edit_stress_ratio_
      DuiLib::CDuiString value = edit_stress_ratio_->GetText();
      float f_stress_ratio_ = static_cast<float>(_ttof(value.GetData()));
      if (f_stress_ratio_ <= 0.0f) {
        // show error message
        MessageBox(paint_manager_ui_->GetPaintWindow(),
                   _T("Stress ratio must be greater than 0.0"), _T("Error"),
                   MB_OK);
      }
    }
  }
}

void WorkWindowFirstPageTh3pointBending::OnClick(TNotifyUI& msg) {
  if (msg.sType == kClick) {
    if (msg.pSender->GetName() == _T("solution_design_refresh")) {
      // TODO(hhool):
    }
  }
}

void WorkWindowFirstPageTh3pointBending::InitPage() {
  __super::InitPage();

  DuiLib::CDuiString tail_prefix;
  tail_prefix.Append(
      (LPCTSTR)(anx::common::string2wstring(t_prefix_.c_str()).c_str()));
  {
    DuiLib::CDuiString name_elastic = _T("tm_page_first_left_elastic");
    name_elastic.Append(tail_prefix);
    edit_elastic_ = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_elastic));
  }
  {
    DuiLib::CDuiString name_density = _T("tm_page_first_left_density");
    name_density.Append(tail_prefix);
    edit_density_ = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_density));
  }
  {
    DuiLib::CDuiString name_max_stress = _T("tm_page_first_left_max_stress");
    name_max_stress.Append(tail_prefix);
    edit_max_stress_ = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_max_stress));
  }
  {
    DuiLib::CDuiString name_stress_ratio =
        _T("tm_page_first_left_ratio_stress");
    name_stress_ratio.Append(tail_prefix);
    edit_stress_ratio_ = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_stress_ratio));
  }
  {
    DuiLib::CDuiString name_static_load = _T("tm_page_first_left_static_load");
    name_static_load.Append(tail_prefix);
    edit_static_load_ = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_static_load));
  }
  {
    DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
    name_amplitude.Append(tail_prefix);
    edit_amplitude_ = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_amplitude));
  }
}

void WorkWindowFirstPageTh3pointBending::
    UpadateTabMainFirstPageElementViewResult() {
  // get header and base param and result from solution_design_
  anx::esolution::ExpDesignHeader* header = solution_design_->header_.get();
  anx::esolution::ExpDesignBaseParam* base_param =
      solution_design_->base_param_.get();
  anx::esolution::ExpDesignResult* result = solution_design_->result_.get();

  // update result
  DuiLib::CDuiString tail_prefix;
  tail_prefix.Append(
      (LPCTSTR)(anx::common::string2wstring(t_prefix_.c_str()).c_str()));
  anx::esolution::ExpDesignResultTh3pointBending* result_th3point =
      reinterpret_cast<anx::esolution::ExpDesignResultTh3pointBending*>(result);
  {
    DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
    name_amplitude.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_amplitude));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_th3point->f_eamplitude_);
    edit->SetText(value);
  }

  {
    DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
    name_dc_stress.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_dc_stress));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_th3point->f_dc_stress_MPa_);
    edit->SetText(value);
  }
  {
    DuiLib::CDuiString name_static_load = _T("tm_page_first_left_static_load");
    name_static_load.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_static_load));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_th3point->f_static_load_MPa_);
    edit->SetText(value);
  }
  {
    DuiLib::CDuiString name_speciments_width =
        _T("tm_page_first_left_speciments_width");
    name_speciments_width.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_speciments_width));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_th3point->f_specimen_width_B_);
    edit->SetText(value);
  }
  {
    DuiLib::CDuiString name_speciments_height =
        _T("tm_page_first_left_speciments_height");
    name_speciments_height.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_speciments_height));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_th3point->f_specimen_thickness_h_);
    edit->SetText(value);
  }
  {
    DuiLib::CDuiString name_speciments_length =
        _T("tm_page_first_left_speciments_length");
    name_speciments_length.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_speciments_length));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_th3point->f_specimen_length_L_);
    edit->SetText(value);
  }
  {
    DuiLib::CDuiString name_length_parallel =
        _T("tm_page_first_left_length_parallel");
    name_length_parallel.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_length_parallel));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_th3point->f_support_distance_L0_);
    edit->SetText(value);
  }
}

std::unique_ptr<anx::esolution::ExpDesignResult>
anx::ui::WorkWindowFirstPageTh3pointBending::ExpDesignResultFromControl() {
  DuiLib::CDuiString tail_prefix;
  tail_prefix.Append(
      (LPCTSTR)(anx::common::string2wstring(t_prefix_.c_str()).c_str()));
  anx::esolution::ExpDesignResultTh3pointBending exp_design_result_th3point;
  DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
  DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_amplitude + tail_prefix));
  DuiLib::CDuiString value = edit->GetText().GetData();
  exp_design_result_th3point.f_eamplitude_ = _ttof(value.GetData());

  DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_dc_stress + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_th3point.f_dc_stress_MPa_ = _ttof(value.GetData());

  DuiLib::CDuiString name_static_load = _T("tm_page_first_left_static_load");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_static_load + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_th3point.f_static_load_MPa_ = _ttof(value.GetData());

  DuiLib::CDuiString name_specimens_width =
      _T("tm_page_first_left_speciments_width");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_specimens_width + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_th3point.f_specimen_width_B_ = _ttof(value.GetData());

  DuiLib::CDuiString name_specimens_height =
      _T("tm_page_first_left_speciments_height");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_specimens_height + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_th3point.f_specimen_thickness_h_ = _ttof(value.GetData());

  DuiLib::CDuiString name_specimens_length =
      _T("tm_page_first_left_speciments_length");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_specimens_length + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_th3point.f_specimen_length_L_ = _ttof(value.GetData());

  DuiLib::CDuiString name_length_parallel =
      _T("tm_page_first_left_length_parallel");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_length_parallel + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_th3point.f_support_distance_L0_ = _ttof(value.GetData());

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
}  // namespace ui
}  // namespace anx
////////////////////////////////////////////////////////////////////////////////
DUI_BEGIN_MESSAGE_MAP(anx::ui::WorkWindowFirstPageVibrationBending,
                      DuiLib::CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

namespace anx {
namespace ui {
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

WorkWindowFirstPageVibrationBending::~WorkWindowFirstPageVibrationBending() {}

void WorkWindowFirstPageVibrationBending::Notify(TNotifyUI& msg) {}

void WorkWindowFirstPageVibrationBending::OnClick(TNotifyUI& msg) {
  if (msg.sType == kClick) {
    if (msg.pSender->GetName() == _T("solution_design_refresh")) {
      // TODO(hhool):
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
  anx::esolution::ExpDesignResult* result = solution_design_->result_.get();
  // get tail prefix
  DuiLib::CDuiString tail_prefix = _T("");
  tail_prefix = _T("_vibration");
  // update result
  anx::esolution::ExpDesignResultVibrationBending* result_vibration =
      reinterpret_cast<anx::esolution::ExpDesignResultVibrationBending*>(
          result);
  {
    DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
    name_amplitude.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_amplitude));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_vibration->f_eamplitude_);
    edit->SetText(value);
  }
  {
    DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
    name_dc_stress.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_dc_stress));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_vibration->f_dc_stress_MPa_);
    edit->SetText(value);
  }
  {
    DuiLib::CDuiString name_radius_exp =
        _T("tm_page_first_left_length_parallel_section");
    name_radius_exp.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_radius_exp));
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
        paint_manager_ui_->FindControl(name_radius_load));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_vibration->f_specimen_radius_arc_R1_);
    edit->SetText(value);
  }
  {
    DuiLib::CDuiString name_radius_trans =
        _T("tm_page_first_left_specimen_radius_transition");
    name_radius_trans.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_radius_trans));
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
        paint_manager_ui_->FindControl(name_thickness_clamp));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_vibration->f_thickness_clamping_d1_);
    edit->SetText(value);
  }
  {
    DuiLib::CDuiString name_thickness_exp =
        _T("tm_page_first_left_thickness_exp_section");
    name_thickness_exp.Append(tail_prefix);
    DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
        paint_manager_ui_->FindControl(name_thickness_exp));
    DuiLib::CDuiString value;
    value.Format(_T("%.2f"), result_vibration->f_thickness_exp_section_L0_d2_);
    edit->SetText(value);
  }
}

std::unique_ptr<anx::esolution::ExpDesignResult>
anx::ui::WorkWindowFirstPageVibrationBending::ExpDesignResultFromControl() {
  std::string t_prefix = anx::esolution::ToTailPrefixName(design_type_);
  DuiLib::CDuiString tail_prefix;
  tail_prefix.Append(
      (LPCTSTR)(anx::common::string2wstring(t_prefix.c_str()).c_str()));
  anx::esolution::ExpDesignResultVibrationBending exp_design_result_vibration;
  DuiLib::CDuiString name_amplitude = _T("tm_page_first_left_amplitude");
  DuiLib::CEditUI* edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_amplitude + tail_prefix));
  DuiLib::CDuiString value = edit->GetText().GetData();
  exp_design_result_vibration.f_eamplitude_ = _ttof(value.GetData());

  DuiLib::CDuiString name_dc_stress = _T("tm_page_first_left_dc_stress");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_dc_stress + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_vibration.f_dc_stress_MPa_ = _ttof(value.GetData());

  DuiLib::CDuiString name_radius_exp =
      _T("tm_page_first_left_length_parallel_section");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_radius_exp + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_vibration.f_specimen_length_parallel_section_L1_ =
      _ttof(value.GetData());

  DuiLib::CDuiString name_radius_arc =
      _T("tm_page_first_left_specimen_radius_arc");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_radius_arc + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_vibration.f_specimen_radius_arc_R1_ =
      _ttof(value.GetData());

  DuiLib::CDuiString name_radius_parallel =
      _T("tm_page_first_left_specimen_radius_transition");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_radius_parallel + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_vibration.f_specimen_radius_transition_R2_ =
      _ttof(value.GetData());

  DuiLib::CDuiString name_radus_trans =
      _T("tm_page_first_left_thickness_clamp_section");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_radus_trans + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_vibration.f_thickness_clamping_d1_ = _ttof(value.GetData());

  DuiLib::CDuiString name_length_trans =
      _T("tm_page_first_left_thickness_exp_section");
  edit = static_cast<DuiLib::CEditUI*>(
      paint_manager_ui_->FindControl(name_length_trans + tail_prefix));
  value = edit->GetText().GetData();
  exp_design_result_vibration.f_thickness_exp_section_L0_d2_ =
      _ttof(value.GetData());

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
