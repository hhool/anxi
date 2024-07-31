/**
 * @file solution_design_default.h
 * @author hhool (hhool@outlook.com)
 * @brief solution design default xml content
 * @version 0.1
 * @date 2024-07-31
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_ESOLUTION_SOLUTION_DESIGN_DEFAULT_H_
#define APP_ESOLUTION_SOLUTION_DESIGN_DEFAULT_H_

#include <memory>

namespace anx {
namespace esolution {
class SolutionDesign;
}  // namespace esolution
}  // namespace anx

namespace anx {
namespace esolution {

extern const char kSolutionDesign_Axially_Symmetrical_DefaultXml[];
extern const char kSolutionDesign_Stresses_Adjustable_DefaultXml[];
extern const char kSolutionDesign_Th3point_Bending_DefaultXml[];
extern const char kSolutionDesign_Vibration_Bending_DefaultXml[];

std::unique_ptr<SolutionDesign> LoadSolutionDesignDefaultWithType(
    int32_t solution_type);
}  // namespace esolution
}  // namespace anx

#endif  // APP_ESOLUTION_SOLUTION_DESIGN_DEFAULT_H_
