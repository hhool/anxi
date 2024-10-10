/**
 * @file sloution_design.h
 * @author hhool (hhool@outlook.com)
 * @brief sloution exp design struct for the solution
 * @version 0.1
 * @date 2024-07-27
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_ESOLUTION_SOLUTION_DESIGN_H_
#define APP_ESOLUTION_SOLUTION_DESIGN_H_

#include <memory>
#include <string>

namespace anx {
namespace esolution {

extern const int32_t kSolutionName_Axially_Symmetrical;
extern const int32_t kSolutionName_Stresses_Adjustable;
extern const int32_t kSolutionName_Th3point_Bending;
extern const int32_t kSolutionName_Vibration_Bending;

extern std::string ToSolutionName(int32_t solution_type);
extern std::string ToTailPrefixName(int32_t solution_type);

// @brief 试验设计头部
// @brief The ExpDesignHeader struct
// 试验设计头部 exp design header
struct ExpDesignHeader {
 public:
  ExpDesignHeader();

 public:
  virtual std::string ToXml(bool close_tag = true) const;
  // @brief 试验设计类型 @see kSolutionName_Axially_Symmetrical ...etc
  // @brief The exp design type @see kSolutionName_Axially_Symmetrical ...etc
  int32_t solution_type_;
  // @brief 试验设计版本
  // @brief The exp design version
  int32_t version_;
  // @brief 试验设计名称
  // @brief The exp design name
  uint8_t name_[256];
  // @brief 试验设计日期 NTP时间戳
  // @brief The exp design date NTP timestamp
  int64_t date_;
  // @brief 试验设计版权
  // @brief The exp design copy right
  uint8_t copy_right_[256];
};

// @brief 试验设计基础参数
// 试验设计基础参数
// 弹性模量、密度、应力最大值、应力比
// 作为试验输入参数经过计算后得到设计参数结果
// @brief The ExpDesignBaseParam struct
// exp design base param
// elastic modulus, density, max stress, stress ratio
// as the input parameter of the experiment, the design
// parameter result is obtained after calculation
struct ExpDesignBaseParam {
 public:
  ExpDesignBaseParam();

 public:
  virtual std::string ToXml(bool close_tag = true) const;

 public:
  // @brief 材料名称
  // @brief The material name
  uint8_t material_name_[256];
  // @breif 弹性模量 GPa 1GPa=1000MPa
  // @brief The elastic modulus GPa 1GPa=1000MPa
  double f_elastic_modulus_GPa_;
  // @brief 密度 kg/m^3
  // @brief The density kg/m^3
  double f_density_kg_m3_;
  // @brief 应力最大值 MPa
  // @brief The max stress MPa
  double f_max_stress_MPa_;
  // @brief 应力比
  // @brief The stress ratio
  double f_stress_ratio_;
};

// @brief 试验设计参数 作为试验设计的输入参数
// @see kSolutionName_Axially_Symmetrical ...etc
// @brief The ExpDesignParamAxially struct as the
// input parameter of the experiment design
// @see kSolutionName_Axially_Symmetrical ...etc
struct ExpDesignParamAxially : public ExpDesignBaseParam {
  ExpDesignParamAxially();
};

// @brief 试验设计参数 作为试验设计的输入参数
// @see kSolutionName_Stresses_Adjustable ...etc
// @brief The ExpDesignParamStressesAdjustable struct as the
// input parameter of the experiment design
// @see kSolutionName_Stresses_Adjustable ...etc
struct ExpDesignParamStressesAdjustable : public ExpDesignBaseParam {
  ExpDesignParamStressesAdjustable();
};

// @brief 试验设计参数 作为试验设计的输入参数
// @see kSolutionName_Th3point_Bending ...etc
// @brief The ExpDesignParamTh3pointBending struct as the
// input parameter of the experiment design
// @see kSolutionName_Th3point_Bending ...etc
struct ExpDesignParamTh3pointBending : public ExpDesignBaseParam {
  ExpDesignParamTh3pointBending();
};

// @brief 试验设计参数 作为试验设计的输入参数
// @see kSolutionName_Vibration_Bending ...etc
// @brief The ExpDesignParamVibrationBending struct as the
// input parameter of the experiment design
// @see kSolutionName_Vibration_Bending ...etc
struct ExpDesignParamVibrationBending : public ExpDesignBaseParam {
  ExpDesignParamVibrationBending();
};

// @brief 试验设计结果, 作为试验设计的输出参数
// 基础结构体，用于扩展。
// @brief The ExpDesignResult struct as the output
// parameter of the experiment design. The basic structure
// is used for extension.

struct ExpDesignResult {
 public:
  explicit ExpDesignResult(int32_t solution_type);

 public:
  virtual std::string ToXml(bool close_tag = true) const;

  // @brief 试验设计类型 @see kSolutionName_Axially_Symmetrical ...etc
  // @brief The exp design type @see kSolutionName_Axially_Symmetrical ...etc
  int32_t solution_type_;
};

struct ExpDesignResult0 : public ExpDesignResult {
 public:
  explicit ExpDesignResult0(int32_t solution_type);

 public:
  virtual std::string ToXml(bool close_tag = true) const;

  // @brief Amplitude A um
  // @brief 振幅A  um
  double f_eamplitude_;
  // @brief 试验中应力与位移的关系
  // 应力位移系数 MPa/um
  // @brief The stress displacement coefficient
  // stress displacement coefficient MPa/um
  double f_dc_stress_MPa_;
};

struct ExpDesignResult1 : public ExpDesignResult0 {
 public:
  explicit ExpDesignResult1(int32_t solution_type);

 public:
  virtual std::string ToXml(bool close_tag = true) const;

  // @brief 试验段半径 R2 mm
  // @brief The radius of the exp section R2 mm
  double f_exp_section_radius_R2_;
  // @brief 平行段半径 R1 mm
  // @brief The radius of the parallel section R1 mm
  double f_parallel_section_radius_R1_;
  // @brief 过渡段半径 R0 mm
  // @brief The radius of the transition section R0 mm
  double f_transition_section_radius_R0_;
  // @brief 过渡段长度 L1 mm
  // @brief The length of the transition section L1 mm
  double f_transition_section_length_L1_;
  // @brief 平行段长度 L0 mm
  // @brief The length of the parallel section L0 mm
  double f_parallel_section_length_L0_;
  // @brief 试验段长度 L2 mm
  // @brief The length of the exp section L2 mm
  double f_exp_section_length_L2_;
};

// @brief 试验设计参数
// 试验设计参数
// @brief The ExpDesignParam struct
// exp design param
struct ExpDesignResultAxially : public ExpDesignResult1 {
 public:
  ExpDesignResultAxially();

  explicit ExpDesignResultAxially(const ExpDesignResultAxially& result) =
      default;

 public:
  virtual std::string ToXml(bool close_tag = true) const;
};

struct ExpDesignResultStressesAdjustable : public ExpDesignResult1 {
 public:
  ExpDesignResultStressesAdjustable();

  explicit ExpDesignResultStressesAdjustable(
      const ExpDesignResultStressesAdjustable& result) = default;

 public:
  virtual std::string ToXml(bool close_tag = true) const;
  // @brief 静载 MPa
  // @brief Static load MPa
  double f_static_load_MPa_;
};

struct ExpDesignResultTh3pointBending : public ExpDesignResult0 {
 public:
  ExpDesignResultTh3pointBending();

  explicit ExpDesignResultTh3pointBending(
      const ExpDesignResultTh3pointBending& result) = default;

 public:
  virtual std::string ToXml(bool close_tag = true) const;
  // @brief 理论振幅A' um
  // @brief Theoretical amplitude A' um
  double f_theory_eamplitude_;
  // @brief 理论应力 Cs' MPa
  // @brief Theoretical stress Cs' MPa
  double f_theory_dc_stress_MPa_;
  // @brief 静载 MPa
  // @brief Static load MPa
  double f_static_load_MPa_;
  // @brief 试件宽度 B mm
  // @brief The width of the specimen B mm
  double f_specimen_width_B_;
  // @brief 试件厚度 h mm
  // @brief The thickness of the specimen h mm
  double f_specimen_thickness_h_;
  // @brief 试件长度 L mm
  // @brief The length of the specimen L mm
  double f_specimen_length_L_;
  // @brief 支撑距离 L0 mm
  // @brief The support distance L0 mm
  double f_support_distance_L0_;
};

struct ExpDesignResultVibrationBending : public ExpDesignResult0 {
  ExpDesignResultVibrationBending();

  explicit ExpDesignResultVibrationBending(
      const ExpDesignResultVibrationBending& result) = default;

 public:
  virtual std::string ToXml(bool close_tag = true) const;
  // @brief 试验平行段长度 L1 mm
  // @brief The length of the parallel section L1 mm
  double f_specimen_length_parallel_section_L1_;
  // @brief 试验圆弧段半径 R1 mm
  // @brief The radius of the arc section R1 mm
  double f_specimen_radius_arc_R1_;
  // @brief 试验过渡段半径 R2 mm
  // @brief The radius of the transition section R2 mm
  double f_specimen_radius_transition_R2_;
  // @brief 夹持段厚度 d1 mm
  // @brief The thickness of the clamping section d1 mm
  double f_thickness_clamping_d1_;
  // @brief 试验段厚度 L0 d2 mm
  // @brief The thickness of the test section L0 d2 mm
  double f_thickness_exp_section_L0_d2_;
};

class SolutionDesign {
 public:
  SolutionDesign();
  explicit SolutionDesign(const SolutionDesign& design);

 public:
  std::unique_ptr<ExpDesignHeader> header_;
  std::unique_ptr<ExpDesignBaseParam> base_param_;
  std::unique_ptr<ExpDesignResult> result_;
  std::string ToXml() const;

 public:
  static int32_t FromXml(const std::string& xml, SolutionDesign* design);
};

}  // namespace esolution
}  // namespace anx

#endif  // APP_ESOLUTION_SOLUTION_DESIGN_H_
