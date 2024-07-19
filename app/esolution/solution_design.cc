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

namespace anx {
namespace esolution {

////////////////////////////////////////////////////////////
// ExpDesignBaseParam
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
// ExpDesignResultAxially
ExpDesignResultAxially::ExpDesignResultAxially(
    const ExpDesignBaseParam& base_param)
    : ExpDesignResult() {
  // do something
}
//////////////////////////
// ExpDesignResultStressesAdjustable
ExpDesignResultStressesAdjustable::ExpDesignResultStressesAdjustable(
    const ExpDesignBaseParam& base_param)
    : ExpDesignResultAxially(base_param) {
  // do something
}

//////////////////////////
// ExpDesignResult2
ExpDesignResult2::ExpDesignResult2(const ExpDesignBaseParam& base_param)
    : ExpDesignResult() {
  // do something
}

//////////////////////////
// ExpDesignResultTh3pointBending
ExpDesignResultTh3pointBending::ExpDesignResultTh3pointBending(
    const ExpDesignBaseParam& base_param)
    : ExpDesignResult2(base_param) {
  // do something
}

//////////////////////////
// ExpDesignResultVibrationBending
ExpDesignResultVibrationBending::ExpDesignResultVibrationBending(
    const ExpDesignBaseParam& base_param)
    : ExpDesignResult2(base_param) {
  // do something
}

////////////////////////////// SolutionExpDesign //////////////////////////////
const int32_t kSolutionName_Axially_Symmetrical = 0;
const int32_t kSolutionName_Stresses_Adjustable = 1;
const int32_t kSolutionName_3point_Bending = 2;
const int32_t kSolutionName_Vibration_Bending = 3;

const SolutionExpDesign kSolutionExpDesign[] = {
    {kSolutionName_Axially_Symmetrical, "0.1", "2024-07-27", "2024"},
    {kSolutionName_Stresses_Adjustable, "0.1", "2024-07-27", "2024"},
    {kSolutionName_3point_Bending, "0.1", "2024-07-27", "2024"},
    {kSolutionName_Vibration_Bending, "0.1", "2024-07-27", "2024"},
};

std::unique_ptr<SolutionExpDesign>
SolutionExpDesignFactory::CreateSolutionExpDesign(int32_t solution_type) {
  if (solution_type < kSolutionName_Axially_Symmetrical ||
      solution_type > kSolutionName_Vibration_Bending) {
    return nullptr;
  }
  return std::make_unique<SolutionExpDesign>(kSolutionExpDesign[solution_type]);
}

std::unique_ptr<ExpDesignBaseParam>
SolutionExpDesignFactory::GetDefaultExpDesignBaseParam(int32_t solution_type) {
  if (solution_type < kSolutionName_Axially_Symmetrical ||
      solution_type > kSolutionName_Vibration_Bending) {
    return nullptr;
  }
  if (solution_type == kSolutionName_Axially_Symmetrical) {
    return std::make_unique<ExpDesignParamAxially>();
  } else if (solution_type == kSolutionName_Stresses_Adjustable) {
    return std::make_unique<ExpDesignParamStressesAdjustable>();
  } else if (solution_type == kSolutionName_3point_Bending) {
    return std::make_unique<ExpDesignParamTh3pointBending>();
  } else if (solution_type == kSolutionName_Vibration_Bending) {
    return std::make_unique<ExpDesignParamVibrationBending>();
  }
  return nullptr;
}

std::unique_ptr<ExpDesignResult> SolutionExpDesignFactory::GetExpDesignParam(
    int32_t solution_type,
    const ExpDesignBaseParam& base_param) {
  if (solution_type < kSolutionName_Axially_Symmetrical ||
      solution_type > kSolutionName_Vibration_Bending) {
    return nullptr;
  }

  if (solution_type == kSolutionName_Axially_Symmetrical) {
    ExpDesignResultAxially axially_result(base_param);
    return std::make_unique<ExpDesignResult>(std::move(axially_result));
  } else if (solution_type == kSolutionName_Stresses_Adjustable) {
    ExpDesignResultStressesAdjustable stresses_result(base_param);
    return std::make_unique<ExpDesignResult>(std::move(stresses_result));
  } else if (solution_type == kSolutionName_3point_Bending) {
    ExpDesignResultTh3pointBending th3point_result(base_param);
    return std::make_unique<ExpDesignResult>(std::move(th3point_result));
  } else if (solution_type == kSolutionName_Vibration_Bending) {
    ExpDesignResultVibrationBending vibration_result(base_param);
    return std::make_unique<ExpDesignResult>(std::move(vibration_result));
  }

  return nullptr;
}

}  // namespace esolution
}  // namespace anx
