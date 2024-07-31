/**
 * @file solution_design_default_unittest.cc
 * @author hhool (hhool@outlook.com)
 * @brief solution design default unittest
 * @version 0.1
 * @date 2024-07-31
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/esolution/solution_design_default.h"

#include "app/esolution/solution_design.h"

#include <gtest/gtest.h>

namespace anx {
namespace esolution {

TEST(SolutionDesignDefaultTest, LoadSolutionDesignDefaultWithType) {
  auto solution_design =
      LoadSolutionDesignDefaultWithType(kSolutionName_Axially_Symmetrical);
  ASSERT_TRUE(solution_design != nullptr);

  solution_design =
      LoadSolutionDesignDefaultWithType(kSolutionName_Stresses_Adjustable);
  ASSERT_TRUE(solution_design != nullptr);

  solution_design =
      LoadSolutionDesignDefaultWithType(kSolutionName_Th3point_Bending);
  ASSERT_TRUE(solution_design != nullptr);

  solution_design =
      LoadSolutionDesignDefaultWithType(kSolutionName_Vibration_Bending);
  ASSERT_TRUE(solution_design != nullptr);
}

}  // namespace esolution
}  // namespace anx

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
