/**
 * @file solution_design_helper.cc
 * @author  hhool (hhool@outlook.com)
 * @brief solution design helper for the solution design
 * @version 0.1
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/esolution/solution_design_helper.h"

#include <cassert>

#include "app/common/file_utils.h"
#include "app/common/logger.h"
#include "app/common/module_utils.h"
#include "app/esolution/solution_design.h"
#include "app/esolution/solution_design_default.h"

namespace anx {
namespace esolution {

std::string DefaultSolutionDesignXmlFilePath(int32_t solution_type) {
  if (solution_type == anx::esolution::kSolutionName_Axially_Symmetrical) {
#if defined(_WIN32) || defined(_WIN64)
    return "default\\default_axially_symmetrical.xml";
#else
    return "default/default_axially_symmetrical.xml";
#endif
  } else if (solution_type ==
             anx::esolution::kSolutionName_Stresses_Adjustable) {
#if defined(_WIN32) || defined(_WIN64)
    return "default\\default_stresses_adjustable.xml";
#else
    return "default/default_stresses_adjustable.xml";
#endif
  } else if (solution_type == anx::esolution::kSolutionName_Th3point_Bending) {
#if defined(_WIN32) || defined(_WIN64)
    return "default\\default_th3point_bending.xml";
#else
    return "default/default_th3point_bending.xml";
#endif
  } else if (solution_type == anx::esolution::kSolutionName_Vibration_Bending) {
#if defined(_WIN32) || defined(_WIN64)
    return "default\\default_vibration_bending.xml";
#else
    return "default/default_vibration_bending.xml";
#endif
  } else {
    assert(false && "Invalid solution type");
    return "";
  }
}

std::unique_ptr<SolutionDesign> LoadSolutionDesignWithFilePath(
    const std::string& file_path) {
  std::string file_content;
  if (!anx::common::ReadFile(file_path, &file_content, true)) {
    LOG_F(LG_ERROR) << "Failed to read file: " << file_path;
    return nullptr;
  }
  // parse xml
  std::string xml = file_content;
  std::unique_ptr<anx::esolution::SolutionDesign> solution_design(
      new anx::esolution::SolutionDesign());
  if (anx::esolution::SolutionDesign::FromXml(xml, solution_design.get()) !=
      0) {
    return nullptr;
  }
  return solution_design;
}

std::unique_ptr<SolutionDesign> LoadSolutionDesignDefaultResourceWithType(
    int32_t solution_type) {
  std::string default_xml = DefaultSolutionDesignXmlFilePath(solution_type);
  if (default_xml.length() == 0) {
    return nullptr;
  }

  // get module path
  std::string app_data_dir = anx::common::GetApplicationDataPath();
#ifdef _WIN32
  default_xml = app_data_dir + "\\anxi\\" + default_xml;
#else
  default_xml = app_data_dir + "/anxi/" + default_xml;
#endif

  std::unique_ptr<SolutionDesign> design =
      LoadSolutionDesignWithFilePath(default_xml);
  if (design != nullptr) {
    return design;
  }
  design = LoadSolutionDesignDefaultWithType(solution_type);
  if (design != nullptr) {
    return design;
  }
  return nullptr;
}

int32_t SaveSolutionDesignFile(const std::string& file_path,
                               const SolutionDesign& design) {
  std::string xml = design.ToXml();
  if (xml.length() == 0) {
    LOG_F(LG_ERROR) << "ToXml failed";
    return -1;
  }

  if (!anx::common::WriteFile(file_path, xml, true)) {
    LOG_F(LG_ERROR) << "write file failed:" << file_path;
    return -2;
  }
  return 0;
}

int32_t ResetSolutionDesignDefaultResourceWithType(int32_t solution_type) {
  std::string default_xml = DefaultSolutionDesignXmlFilePath(solution_type);
  if (default_xml.length() == 0) {
    return -1;
  }
  // get module path
  std::string app_data_dir = anx::common::GetApplicationDataPath();
#ifdef _WIN32
  std::string dst_xml = app_data_dir + "\\anxi\\" + default_xml;
  std::string origin_xml = anx::common::GetModuleDir() + "\\" + default_xml;
#else
  std::string dst_xml = app_data_dir + "/anxi/" + default_xml;
  std::string origin_xml = anx::common::GetModuleDir() + "\\" + default_xml;
#endif
  if (!anx::common::FileExists(origin_xml)) {
    LOG_F(LG_ERROR) << "origin file not exists:" << origin_xml;
    return -1;
  }
  if (!anx::common::CCopyFile(origin_xml, dst_xml)) {
    LOG_F(LG_ERROR) << "CopyFile failed:" << origin_xml << " to " << dst_xml;
    return -2;
  }
  return 0;
}

}  // namespace esolution
}  // namespace anx
