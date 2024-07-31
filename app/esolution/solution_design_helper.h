/**
 * @file solution_design_helper.h
 * @author  hhool (hhool@outlook.com)
 * @brief solution design helper for the solution design
 * @version 0.1
 * @date 2024-08-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef APP_ESOLUTION_SOLUTION_DESIGN_HELPER_H_
#define APP_ESOLUTION_SOLUTION_DESIGN_HELPER_H_

#include <memory>
#include <string>

namespace anx {
namespace esolution {
class SolutionDesign;
}  // namespace esolution
}  // namespace anx

namespace anx {
namespace esolution {
/// @brief Default solution design xml filepath with solution type
/// for solution design created with default value
/// @param solution_type
std::string DefaultSolutionDesignXmlFilePath(int32_t solution_type);

/// @brief Load the solution design with the file path
/// @param file_path the file path of the solution design file
/// @return the solution design pointer
std::unique_ptr<SolutionDesign> LoadSolutionDesignWithFilePath(
    const std::string& file_path);

/// @brief Load default the solution design from resource with the solution type
/// @param solution_type the solution type
/// @return the solution design pointer
std::unique_ptr<SolutionDesign> LoadSolutionDesignDefaultResourceWithType(
    int32_t solution_type);

/// @brief Save the solution design file
/// @param file_path the file path of the solution design file
/// @param design the solution design
/// @return 0 if success, -1 if failed
int32_t SaveSolutionDesignFile(const std::string& file_path,
                               const SolutionDesign& design);

}  // namespace esolution
}  // namespace anx

#endif  // APP_ESOLUTION_SOLUTION_DESIGN_HELPER_H_
