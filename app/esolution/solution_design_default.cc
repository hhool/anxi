/**
 * @file solution_design_default.cc
 * @author hhool (hhool@outlook.com)
 * @brief  solution design default xml content
 * @version 0.1
 * @date 2024-07-31
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/esolution/solution_design_default.h"

#include "app/esolution/solution_design.h"

#include <cassert>

namespace anx {
namespace esolution {

const char kSolutionDesign_Axially_Symmetrical_DefaultXml[] = R"(
<?xml version="1.0" encoding="UTF-8"?>
<header>
  <solution_type>0</solution_type>
  <version>0</version>
  <name>名称</name>
  <date>0</date>
  <copy_right>axially symmetrical copy right</copy_right>
</header>
<base_param>
  <material_name>其他材料</material_name>
  <elastic_modulus>0</elastic_modulus>
  <density>0</density>
  <max_stress>0</max_stress>
  <stress_ratio>0</stress_ratio>
</base_param>
<result type="axially">
    <f_eamplitude>15.000000</f_eamplitude>
    <f_dc_stress_MPa>13.570000</f_dc_stress_MPa>
    <f_exp_section_radius_R2>5.000000</f_exp_section_radius_R2>
    <f_parallel_section_radius_R1>1.500000</f_parallel_section_radius_R1>
    <f_transition_section_radius_R0>22.300000</f_transition_section_radius_R0>
    <f_transition_section_length_L1>12.000000</f_transition_section_length_L1>
    <f_parallel_section_length_L0>2.000000</f_parallel_section_length_L0>
    <f_exp_section_length_L2>15.000000</f_exp_section_length_L2>
</result>
)";

const char kSolutionDesign_Stresses_Adjustable_DefaultXml[] = R"(
<?xml version="1.0" encoding="UTF-8"?>
<header>
  <solution_type>1</solution_type>
  <version>0</version>
  <name>名称</name>
  <date>0</date>
  <copy_right>stresses adjustable copy right</copy_right>
</header>
<base_param>
  <material_name>其他材料</material_name>
  <elastic_modulus>0</elastic_modulus>
  <density>0</density>
  <max_stress>0</max_stress>
  <stress_ratio>0</stress_ratio>
</base_param>
<result type="stresses">
    <f_eamplitude>20.800000</f_eamplitude>
    <f_dc_stress_MPa>14.060000</f_dc_stress_MPa>
    <f_exp_section_radius_R2>5.000000</f_exp_section_radius_R2>
    <f_parallel_section_radius_R1>1.500000</f_parallel_section_radius_R1>
    <f_transition_section_radius_R0>22.000000</f_transition_section_radius_R0>
    <f_transition_section_length_L1>11.900000</f_transition_section_length_L1>
    <f_parallel_section_length_L0>1.700000</f_parallel_section_length_L0>
    <f_exp_section_length_L2>15.700000</f_exp_section_length_L2>
    <f_static_load_MPa>607.500000</f_static_load_MPa>
</result>
)";

const char kSolutionDesign_Th3point_Bending_DefaultXml[] = R"(
<?xml version="1.0" encoding="UTF-8"?>
<header>
  <solution_type>2</solution_type>
  <version>0</version>
  <name>名称</name>
  <date>0</date>
  <copy_right>th3point bending copy right</copy_right>
</header>
<base_param>
  <material_name>其他材料</material_name>
  <elastic_modulus>0</elastic_modulus>
  <density>0</density>
  <max_stress>0</max_stress>
  <stress_ratio>0</stress_ratio>
</base_param>
<result type="th3point">
    <f_eamplitude>29.000000</f_eamplitude>
    <f_dc_stress_MPa>6.200000</f_dc_stress_MPa>
    <f_static_load_MPa>547.500000</f_static_load_MPa>
    <f_specimen_width_B>10.000000</f_specimen_width_B>
    <f_specimen_thickness_h>4.000000</f_specimen_thickness_h>
    <f_specimen_length_L>31.600000</f_specimen_length_L>
    <f_support_distance_L0>17.200000</f_support_distance_L0>
</result>
)";

const char kSolutionDesign_Vibration_Bending_DefaultXml[] = R"(
<?xml version="1.0" encoding="UTF-8"?>
<header>
  <solution_type>3</solution_type>
  <version>0</version>
  <name>名称</name>
  <date>0</date>
  <copy_right>vibration bending copy right</copy_right>
</header>
<base_param>
  <material_name>其他材料</material_name>
  <elastic_modulus>0</elastic_modulus>
  <density>0</density>
  <max_stress>0</max_stress>
  <stress_ratio>0</stress_ratio>
</base_param>
<result type="vibration">
    <f_eamplitude>8.500000</f_eamplitude>
    <f_dc_stress_MPa>24.500000</f_dc_stress_MPa>
    <f_specimen_length_parallel_section_L1>3.700000</f_specimen_length_parallel_section_L1>
    <f_specimen_radius_arc_R1>3.000000</f_specimen_radius_arc_R1>
    <f_specimen_radius_transition_R2>1.000000</f_specimen_radius_transition_R2>
    <f_thickness_clamping_d1>5.200000</f_thickness_clamping_d1>
    <f_thickness_exp_section_L0_d2>3.200000</f_thickness_exp_section_L0_d2>
</result>
)";

std::unique_ptr<SolutionDesign> LoadSolutionDesignDefaultWithType(
    int32_t solution_type) {
  std::unique_ptr<SolutionDesign> design = std::make_unique<SolutionDesign>();
  int32_t ret = 0;
  switch (solution_type) {
    case 0:
      ret = SolutionDesign::FromXml(
          kSolutionDesign_Axially_Symmetrical_DefaultXml, design.get());
      break;
    case 1:
      ret = SolutionDesign::FromXml(
          kSolutionDesign_Stresses_Adjustable_DefaultXml, design.get());
      break;
    case 2:
      ret = SolutionDesign::FromXml(kSolutionDesign_Th3point_Bending_DefaultXml,
                                    design.get());
      break;
    case 3:
      ret = SolutionDesign::FromXml(
          kSolutionDesign_Vibration_Bending_DefaultXml, design.get());
      break;
    default:
      ret = -1;
      break;
  }
  assert(ret == 0);
  return design;
}

}  // namespace esolution
}  // namespace anx
