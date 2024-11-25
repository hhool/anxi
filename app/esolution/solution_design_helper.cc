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
  FILE* file = fopen(file_path.c_str(), "rb");
  if (file == NULL) {
    return nullptr;
  }
  fseek(file, 0, SEEK_END);
  int32_t file_size = static_cast<int32_t>(ftell(file));
  fseek(file, 0, SEEK_SET);
  // read file
  std::unique_ptr<char[]> buffer(new char[file_size]);
  size_t size = fread(buffer.get(), 1, file_size, file);
  if (size != file_size) {
    fclose(file);
    return nullptr;
  }
  fclose(file);
  // parse xml
  std::string xml(buffer.get(), size);
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
    return -1;
  }

  FILE* file = fopen(file_path.c_str(), "wb");
  if (file == NULL) {
    return -2;
  }

  size_t size = fwrite(xml.c_str(), xml.size(), 1, file);
  fclose(file);

  if (size != 1) {
    return -3;
  }
  return 0;
}

}  // namespace esolution
}  // namespace anx
