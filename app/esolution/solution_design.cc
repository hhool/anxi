/**
 * @file solution_design.cc
 * @author hhool (hhool@outlook.com)
 * @brief sloution exp design struct for the solution
 * @version 0.1
 * @date 2024-07-27
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/esolution/solution_design.h"

#include <utility>

#include "third_party/tinyxml2/source/tinyxml2.h"

namespace anx {
namespace esolution {

////////////////////////////////////////////////////////////
// constants
const int32_t kSolutionName_Axially_Symmetrical = 0;
const int32_t kSolutionName_Stresses_Adjustable = 1;
const int32_t kSolutionName_Th3point_Bending = 2;
const int32_t kSolutionName_Vibration_Bending = 3;

std::string ToSolutionName(int32_t solution_type) {
  switch (solution_type) {
    case kSolutionName_Axially_Symmetrical:
      return "axially";
    case kSolutionName_Stresses_Adjustable:
      return "stresses";
    case kSolutionName_Th3point_Bending:
      return "th3point";
    case kSolutionName_Vibration_Bending:
      return "vibration";
    default:
      return "unknown";
  }
}

std::string ToTailPrefixName(int32_t solution_type) {
  switch (solution_type) {
    case kSolutionName_Axially_Symmetrical:
      return "_axially";
    case kSolutionName_Stresses_Adjustable:
      return "_stresses";
    case kSolutionName_Th3point_Bending:
      return "_th3point";
    case kSolutionName_Vibration_Bending:
      return "_vibration";
    default:
      return "unknown";
  }
}

////////////////////////////////////////////////////////////
// ExpDesignHeader
//////////////////////////
ExpDesignHeader::ExpDesignHeader() : solution_type_(0), version_(1) {
  memset(name_, 0, sizeof(name_));
  memset(copy_right_, 0, sizeof(copy_right_));
}

std::string ExpDesignHeader::ToXml(bool close_tag) const {
  std::string xml;
  if (close_tag) {
    xml += "<header>";
  }
  xml +=
      "<solution_type>" + std::to_string(solution_type_) + "</solution_type>";
  xml += "<version>" + std::to_string(version_) + "</version>";
  xml +=
      "<name>" + std::string(reinterpret_cast<const char*>(name_)) + "</name>";
  if (close_tag) {
    xml += "</header>";
  }
  return xml;
}

////////////////////////////////////////////////////////////
// ExpDesignBaseParam

ExpDesignBaseParam::ExpDesignBaseParam()
    : f_elastic_modulus_GPa_(0.0),
      f_density_kg_m3_(0.0),
      f_max_stress_MPa_(0.0),
      f_stress_ratio_(0.0) {
  // do something
  memset(material_name_, 0, sizeof(material_name_));
}

std::string ExpDesignBaseParam::ToXml(bool close_tag) const {
  std::string xml;
  if (close_tag) {
    xml += "<base_param>";
  }
  xml += "<material_name>" +
         std::string(reinterpret_cast<const char*>(material_name_)) +
         "</material_name>";
  xml += "<elastic_modulus>" + std::to_string(f_elastic_modulus_GPa_) +
         "</elastic_modulus>";
  xml += "<density>" + std::to_string(f_density_kg_m3_) + "</density>";
  xml += "<max_stress>" + std::to_string(f_max_stress_MPa_) + "</max_stress>";
  xml += "<stress_ratio>" + std::to_string(f_stress_ratio_) + "</stress_ratio>";
  if (close_tag) {
    xml += "</base_param>";
  }
  return xml;
}

//////////////////////////
// ExpDesignParamAxially
ExpDesignParamAxially::ExpDesignParamAxially() : ExpDesignBaseParam() {
  // do something
}

//////////////////////////
// ExpDesignParamStressesAdjustable
ExpDesignParamStressesAdjustable::ExpDesignParamStressesAdjustable()
    : ExpDesignBaseParam() {
  // do something
}

//////////////////////////
// ExpDesignParamTh3pointBending
ExpDesignParamTh3pointBending::ExpDesignParamTh3pointBending()
    : ExpDesignBaseParam() {
  // do something
}

//////////////////////////
// ExpDesignParamVibrationBending
ExpDesignParamVibrationBending::ExpDesignParamVibrationBending()
    : ExpDesignBaseParam() {
  // do something
}

//////////////////////////
// ExpDesignResult
ExpDesignResult::ExpDesignResult(int32_t solution_type)
    : solution_type_(solution_type) {
  // do something
}

std::string ExpDesignResult::ToXml(bool close_tag) const {
  std::string xml;
  return xml;
}

//////////////////////////
// ExpDesignResult1
ExpDesignResult1::ExpDesignResult1(int32_t solution_type)
    : ExpDesignResult(solution_type) {
  // do something
}

std::string ExpDesignResult1::ToXml(bool close_tag) const {
  std::string xml;
  if (close_tag) {
    xml += "<result type=\"axially\">";
  }
  xml += ExpDesignResult::ToXml(false);
  xml += "<f_eamplitude>" + std::to_string(f_eamplitude_) + "</f_eamplitude>";
  xml += "<f_dc_stress_MPa>" + std::to_string(f_dc_stress_MPa_) +
         "</f_dc_stress_MPa>";
  xml += "<f_exp_section_radius_R2>" +
         std::to_string(f_exp_section_radius_R2_) +
         "</f_exp_section_radius_R2>";
  xml += "<f_parallel_section_radius_R1>" +
         std::to_string(f_parallel_section_radius_R1_) +
         "</f_parallel_section_radius_R1>";
  xml += "<f_transition_section_radius_R0>" +
         std::to_string(f_transition_section_radius_R0_) +
         "</f_transition_section_radius_R0>";
  xml += "<f_transition_section_length_L1>" +
         std::to_string(f_transition_section_length_L1_) +
         "</f_transition_section_length_L1>";
  xml += "<f_parallel_section_length_L0>" +
         std::to_string(f_parallel_section_length_L0_) +
         "</f_parallel_section_length_L0>";
  xml += "<f_exp_section_length_L2>" +
         std::to_string(f_exp_section_length_L2_) +
         "</f_exp_section_length_L2>";
  if (close_tag) {
    xml += "</result>";
  }

  return xml;
}

//////////////////////////
// ExpDesignResultAxially
ExpDesignResultAxially::ExpDesignResultAxially() : ExpDesignResult1(0) {
  // do something
}

std::string ExpDesignResultAxially::ToXml(bool close_tag) const {
  std::string xml;
  if (close_tag) {
    xml += "<result type=\"axially\">";
  }
  xml += ExpDesignResult1::ToXml(false);
  if (close_tag) {
    xml += "</result>";
  }

  return xml;
}

//////////////////////////
// ExpDesignResultStressesAdjustable
ExpDesignResultStressesAdjustable::ExpDesignResultStressesAdjustable()
    : ExpDesignResult1(0) {
  // do something
}

std::string ExpDesignResultStressesAdjustable::ToXml(bool close_tag) const {
  std::string xml;
  if (close_tag) {
    xml += "<result type=\"stresses\">";
  }
  xml += ExpDesignResult1::ToXml(false);
  xml += "<f_static_load_MPa>" + std::to_string(f_static_load_MPa_) +
         "</f_static_load_MPa>";
  if (close_tag) {
    xml += "</result>";
  }
  return xml;
}

//////////////////////////
// ExpDesignResult2
ExpDesignResult2::ExpDesignResult2(int32_t solution_type)
    : ExpDesignResult(solution_type) {
  // do something
}

std::string ExpDesignResult2::ToXml(bool close_tag) const {
  std::string xml;
  xml += "<f_eamplitude_um>" + std::to_string(f_eamplitude_um_) +
         "</f_eamplitude_um>";
  xml += "<f_dc_stress_MPa>" + std::to_string(f_dc_stress_MPa_) +
         "</f_dc_stress_MPa>";
  return xml;
}

//////////////////////////
// ExpDesignResultTh3pointBending
ExpDesignResultTh3pointBending::ExpDesignResultTh3pointBending()
    : ExpDesignResult2(2) {
  // do something
}

std::string ExpDesignResultTh3pointBending::ToXml(bool close_tag) const {
  std::string xml;
  if (close_tag) {
    xml += "<result type=\"th3point\">";
  }
  xml += ExpDesignResult2::ToXml(false);
  xml += "<f_static_load_MPa>" + std::to_string(f_static_load_MPa_) +
         "</f_static_load_MPa>";
  xml += "<f_specimen_width_B>" + std::to_string(f_specimen_width_B_) +
         "</f_specimen_width_B>";
  xml += "<f_specimen_thickness_h>" + std::to_string(f_specimen_thickness_h_) +
         "</f_specimen_thickness_h>";
  xml += "<f_specimen_length_L>" + std::to_string(f_specimen_length_L_) +
         "</f_specimen_length_L>";
  xml += "<f_support_distance_L0>" + std::to_string(f_support_distance_L0_) +
         "</f_support_distance_L0>";
  if (close_tag) {
    xml += "</result>";
  }
  return xml;
}
//////////////////////////
// ExpDesignResultVibrationBending
ExpDesignResultVibrationBending::ExpDesignResultVibrationBending()
    : ExpDesignResult2(3) {
  // do something
}

std::string ExpDesignResultVibrationBending::ToXml(bool close_tag) const {
  std::string xml;
  if (close_tag) {
    xml += "<result type=\"vibration\">";
  }
  xml += ExpDesignResult2::ToXml(false);
  xml += "<f_specimen_length_parallel_section_L1>" +
         std::to_string(f_specimen_length_parallel_section_L1_) +
         "</f_specimen_length_parallel_section_L1>";
  xml += "<f_specimen_radius_arc_R1>" +
         std::to_string(f_specimen_radius_arc_R1_) +
         "</f_specimen_radius_arc_R1>";
  xml += "<f_specimen_radius_transition_R2>" +
         std::to_string(f_specimen_radius_transition_R2_) +
         "</f_specimen_radius_transition_R2>";
  xml += "<f_thickness_clamping_d1>" +
         std::to_string(f_thickness_clamping_d1_) +
         "</f_thickness_clamping_d1>";
  xml += "<f_thickness_exp_section_L0_d2>" +
         std::to_string(f_thickness_exp_section_L0_d2_) +
         "</f_thickness_exp_section_L0_d2>";
  if (close_tag) {
    xml += "</result>";
  }
  return xml;
}

////////////////////////// SolutionDesign //////////////////////////////
SolutionDesign::SolutionDesign()
    : header_(nullptr), base_param_(nullptr), result_(nullptr) {
  // do something
}

SolutionDesign::SolutionDesign(const SolutionDesign& design)
    : header_(std::make_unique<ExpDesignHeader>(*design.header_)),
      base_param_(std::make_unique<ExpDesignBaseParam>(*design.base_param_)) {
  // do something
  if (header_->solution_type_ == kSolutionName_Axially_Symmetrical) {
    result_.reset(new ExpDesignResultAxially(
        *static_cast<ExpDesignResultAxially*>(design.result_.get())));
  } else if (header_->solution_type_ == kSolutionName_Stresses_Adjustable) {
    result_.reset(new ExpDesignResultStressesAdjustable(
        *static_cast<ExpDesignResultStressesAdjustable*>(
            design.result_.get())));
  } else if (header_->solution_type_ == kSolutionName_Th3point_Bending) {
    result_.reset(new ExpDesignResultTh3pointBending(
        *static_cast<ExpDesignResultTh3pointBending*>(design.result_.get())));
  } else if (header_->solution_type_ == kSolutionName_Vibration_Bending) {
    result_.reset(new ExpDesignResultVibrationBending(
        *static_cast<ExpDesignResultVibrationBending*>(design.result_.get())));
  }
}

std::string SolutionDesign::ToXml() {
  std::string xml;
  xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
  xml += header_->ToXml();
  xml += base_param_->ToXml();
  xml += result_->ToXml();
  return xml;
}

int32_t SolutionDesign::FromXml(const std::string& xml,
                                SolutionDesign* solution_design) {
  ExpDesignHeader header;
  ExpDesignBaseParam base_param;

  tinyxml2::XMLDocument doc;
  if (doc.Parse(xml.c_str()) != tinyxml2::XML_SUCCESS) {
    return -1;
  }

  tinyxml2::XMLElement* header_element = doc.FirstChildElement("header");

  tinyxml2::XMLElement* base_param_element =
      doc.FirstChildElement("base_param");

  tinyxml2::XMLElement* result_element = doc.FirstChildElement("result");
  if (!header_element || !base_param_element || !result_element) {
    return -2;
  }

  // parse header[solution_type_element] element
  {
    tinyxml2::XMLElement* solution_type_element =
        header_element->FirstChildElement("solution_type");
    if (solution_type_element) {
      header.solution_type_ = solution_type_element->IntText();
    }

    tinyxml2::XMLElement* version_element =
        header_element->FirstChildElement("version");
    if (version_element) {
      header.version_ = version_element->IntText();
    }

    tinyxml2::XMLElement* name_element =
        header_element->FirstChildElement("name");
    if (name_element) {
      const char* name = name_element->GetText();
      if (name) {
        memcpy(header.name_, name, strlen(name));
      }
    }
  }

  // parse base param[base_param_element]
  {
    tinyxml2::XMLElement* material_name_element =
        base_param_element->FirstChildElement("material_name");
    if (material_name_element) {
      const char* material_name = material_name_element->GetText();
      if (material_name) {
        memcpy(base_param.material_name_, material_name, strlen(material_name));
      }
    }

    tinyxml2::XMLElement* elastic_modulus_element =
        base_param_element->FirstChildElement("elastic_modulus");
    if (elastic_modulus_element) {
      base_param.f_elastic_modulus_GPa_ = elastic_modulus_element->FloatText();
    }

    tinyxml2::XMLElement* density_element =
        base_param_element->FirstChildElement("density");
    if (density_element) {
      base_param.f_density_kg_m3_ = density_element->FloatText();
    }

    tinyxml2::XMLElement* max_stress_element =
        base_param_element->FirstChildElement("max_stress");
    if (max_stress_element) {
      base_param.f_max_stress_MPa_ = max_stress_element->FloatText();
    }

    tinyxml2::XMLElement* stress_ratio_element =
        base_param_element->FirstChildElement("stress_ratio");
    if (stress_ratio_element) {
      base_param.f_stress_ratio_ = stress_ratio_element->FloatText();
    }
  }

  // parse result[result_element]
  {
    if (header.solution_type_ == kSolutionName_Axially_Symmetrical) {
      ExpDesignResultAxially result_axially;
      tinyxml2::XMLElement* f_eamplitude_element =
          result_element->FirstChildElement("f_eamplitude");
      if (f_eamplitude_element) {
        result_axially.f_eamplitude_ = f_eamplitude_element->FloatText();
      }

      tinyxml2::XMLElement* f_dc_stress_MPa_element =
          result_element->FirstChildElement("f_dc_stress_MPa");
      if (f_dc_stress_MPa_element) {
        result_axially.f_dc_stress_MPa_ = f_dc_stress_MPa_element->FloatText();
      }

      tinyxml2::XMLElement* f_exp_section_radius_R2_element =
          result_element->FirstChildElement("f_exp_section_radius_R2");
      if (f_exp_section_radius_R2_element) {
        result_axially.f_exp_section_radius_R2_ =
            f_exp_section_radius_R2_element->FloatText();
      }

      tinyxml2::XMLElement* f_parallel_section_radius_R1_element =
          result_element->FirstChildElement("f_parallel_section_radius_R1");
      if (f_parallel_section_radius_R1_element) {
        result_axially.f_parallel_section_radius_R1_ =
            f_parallel_section_radius_R1_element->FloatText();
      }

      tinyxml2::XMLElement* f_transition_section_radius_R0_element =
          result_element->FirstChildElement("f_transition_section_radius_R0");
      if (f_transition_section_radius_R0_element) {
        result_axially.f_transition_section_radius_R0_ =
            f_transition_section_radius_R0_element->FloatText();
      }

      tinyxml2::XMLElement* f_transition_section_length_L1_element =
          result_element->FirstChildElement("f_transition_section_length_L1");
      if (f_transition_section_length_L1_element) {
        result_axially.f_transition_section_length_L1_ =
            f_transition_section_length_L1_element->FloatText();
      }

      tinyxml2::XMLElement* f_parallel_section_length_L0_element =
          result_element->FirstChildElement("f_parallel_section_length_L0");
      if (f_parallel_section_length_L0_element) {
        result_axially.f_parallel_section_length_L0_ =
            f_parallel_section_length_L0_element->FloatText();
      }

      tinyxml2::XMLElement* f_exp_section_length_L2_element =
          result_element->FirstChildElement("f_exp_section_length_L2");
      if (f_exp_section_length_L2_element) {
        result_axially.f_exp_section_length_L2_ =
            f_exp_section_length_L2_element->FloatText();
      }
      solution_design->result_.reset(
          new ExpDesignResultAxially(result_axially));
    } else if (header.solution_type_ == kSolutionName_Stresses_Adjustable) {
      ExpDesignResultStressesAdjustable result_stresses;
      tinyxml2::XMLElement* f_eamplitude_element =
          result_element->FirstChildElement("f_eamplitude");
      if (f_eamplitude_element) {
        result_stresses.f_eamplitude_ = f_eamplitude_element->FloatText();
      }

      tinyxml2::XMLElement* f_dc_stress_MPa_element =
          result_element->FirstChildElement("f_dc_stress_MPa");
      if (f_dc_stress_MPa_element) {
        result_stresses.f_dc_stress_MPa_ = f_dc_stress_MPa_element->FloatText();
      }

      tinyxml2::XMLElement* f_static_load_MPa_element =
          result_element->FirstChildElement("f_static_load_MPa");
      if (f_static_load_MPa_element) {
        result_stresses.f_static_load_MPa_ =
            f_static_load_MPa_element->FloatText();
      }
      tinyxml2::XMLElement* f_exp_section_radius_R2_element =
          result_element->FirstChildElement("f_exp_section_radius_R2");
      if (f_exp_section_radius_R2_element) {
        result_stresses.f_exp_section_radius_R2_ =
            f_exp_section_radius_R2_element->FloatText();
      }

      tinyxml2::XMLElement* f_parallel_section_radius_R1_element =
          result_element->FirstChildElement("f_parallel_section_radius_R1");
      if (f_parallel_section_radius_R1_element) {
        result_stresses.f_parallel_section_radius_R1_ =
            f_parallel_section_radius_R1_element->FloatText();
      }

      tinyxml2::XMLElement* f_transition_section_radius_R0_element =
          result_element->FirstChildElement("f_transition_section_radius_R0");
      if (f_transition_section_radius_R0_element) {
        result_stresses.f_transition_section_radius_R0_ =
            f_transition_section_radius_R0_element->FloatText();
      }

      tinyxml2::XMLElement* f_transition_section_length_L1_element =
          result_element->FirstChildElement("f_transition_section_length_L1");
      if (f_transition_section_length_L1_element) {
        result_stresses.f_transition_section_length_L1_ =
            f_transition_section_length_L1_element->FloatText();
      }

      tinyxml2::XMLElement* f_parallel_section_length_L0_element =
          result_element->FirstChildElement("f_parallel_section_length_L0");
      if (f_parallel_section_length_L0_element) {
        result_stresses.f_parallel_section_length_L0_ =
            f_parallel_section_length_L0_element->FloatText();
      }

      tinyxml2::XMLElement* f_exp_section_length_L2_element =
          result_element->FirstChildElement("f_exp_section_length_L2");
      if (f_exp_section_length_L2_element) {
        result_stresses.f_exp_section_length_L2_ =
            f_exp_section_length_L2_element->FloatText();
      }
      solution_design->result_.reset(
          new ExpDesignResultStressesAdjustable(result_stresses));
    } else if (header.solution_type_ == kSolutionName_Th3point_Bending) {
      ExpDesignResultTh3pointBending result_th3point;

      tinyxml2::XMLElement* f_eamplitude_um_element =
          result_element->FirstChildElement("f_eamplitude_um");
      if (f_eamplitude_um_element) {
        result_th3point.f_eamplitude_um_ = f_eamplitude_um_element->FloatText();
      }

      tinyxml2::XMLElement* f_dc_stress_MPa_element =
          result_element->FirstChildElement("f_dc_stress_MPa");
      if (f_dc_stress_MPa_element) {
        result_th3point.f_dc_stress_MPa_ = f_dc_stress_MPa_element->FloatText();
      }
      tinyxml2::XMLElement* f_static_load_MPa_element =
          result_element->FirstChildElement("f_static_load_MPa");
      if (f_static_load_MPa_element) {
        result_th3point.f_static_load_MPa_ =
            f_static_load_MPa_element->FloatText();
      }
      tinyxml2::XMLElement* f_specimen_width_B_element =
          result_element->FirstChildElement("f_specimen_width_B");
      if (f_specimen_width_B_element) {
        result_th3point.f_specimen_width_B_ =
            f_specimen_width_B_element->FloatText();
      }

      tinyxml2::XMLElement* f_specimen_thickness_h_element =
          result_element->FirstChildElement("f_specimen_thickness_h");
      if (f_specimen_thickness_h_element) {
        result_th3point.f_specimen_thickness_h_ =
            f_specimen_thickness_h_element->FloatText();
      }

      tinyxml2::XMLElement* f_specimen_length_L_element =
          result_element->FirstChildElement("f_specimen_length_L");
      if (f_specimen_length_L_element) {
        result_th3point.f_specimen_length_L_ =
            f_specimen_length_L_element->FloatText();
      }

      tinyxml2::XMLElement* f_support_distance_L0_element =
          result_element->FirstChildElement("f_support_distance_L0");
      if (f_support_distance_L0_element) {
        result_th3point.f_support_distance_L0_ =
            f_support_distance_L0_element->FloatText();
      }

      solution_design->result_.reset(
          new ExpDesignResultTh3pointBending(result_th3point));
    } else if (header.solution_type_ == kSolutionName_Vibration_Bending) {
      ExpDesignResultVibrationBending result_vibration;

	  tinyxml2::XMLElement* f_eamplitude_um_element =
		  result_element->FirstChildElement("f_eamplitude_um");
	  if (f_eamplitude_um_element) {
		  result_vibration.f_eamplitude_um_ = f_eamplitude_um_element->FloatText();
	  }

      tinyxml2::XMLElement* f_dc_stress_MPa_element =
          result_element->FirstChildElement("f_dc_stress_MPa");
      if (f_dc_stress_MPa_element) {
        result_vibration.f_dc_stress_MPa_ =
            f_dc_stress_MPa_element->FloatText();
      }

      tinyxml2::XMLElement* f_specimen_length_parallel_section_L1_element =
          result_element->FirstChildElement(
              "f_specimen_length_parallel_section_L1");
      if (f_specimen_length_parallel_section_L1_element) {
        result_vibration.f_specimen_length_parallel_section_L1_ =
            f_specimen_length_parallel_section_L1_element->FloatText();
      }

      tinyxml2::XMLElement* f_specimen_radius_arc_R1_element =
          result_element->FirstChildElement("f_specimen_radius_arc_R1");
      if (f_specimen_radius_arc_R1_element) {
        result_vibration.f_specimen_radius_arc_R1_ =
            f_specimen_radius_arc_R1_element->FloatText();
      }

      tinyxml2::XMLElement* f_specimen_radius_transition_R2_element =
          result_element->FirstChildElement("f_specimen_radius_transition_R2");
      if (f_specimen_radius_transition_R2_element) {
        result_vibration.f_specimen_radius_transition_R2_ =
            f_specimen_radius_transition_R2_element->FloatText();
      }

      tinyxml2::XMLElement* f_thickness_clamping_d1_element =
          result_element->FirstChildElement("f_thickness_clamping_d1");
      if (f_thickness_clamping_d1_element) {
        result_vibration.f_thickness_clamping_d1_ =
            f_thickness_clamping_d1_element->FloatText();
      }

      tinyxml2::XMLElement* f_thickness_exp_section_L0_d2_element =
          result_element->FirstChildElement("f_thickness_exp_section_L0_d2");
      if (f_thickness_exp_section_L0_d2_element) {
        result_vibration.f_thickness_exp_section_L0_d2_ =
            f_thickness_exp_section_L0_d2_element->FloatText();
      }

      solution_design->result_.reset(
          new ExpDesignResultVibrationBending(result_vibration));
    }

    solution_design->header_.reset(new ExpDesignHeader(header));
    solution_design->base_param_.reset(new ExpDesignBaseParam(base_param));
    return 0;
  }
}
}  // namespace esolution
}  // namespace anx
