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

// @brief 试验设计头部
// @brief The ExpDesignHeader struct
// 试验设计头部 exp design header
struct ExpDesignHeader {
  // @brief 试验设计ID @see kSolutionName_Axially_Symmetrical ...etc
  // @brief The exp design ID @see kSolutionName_Axially_Symmetrical ...etc
  int32_t solution_id_;
  // @brief 试验设计版本
  // @brief The exp design version
  int32_t version;
  // @brief 试验设计日期 NTP时间戳
  // @brief The exp design date NTP timestamp
  int64_t date;
  // @brief 试验设计版权
  // @brief The exp design copy right
  uint8_t copy_right[256];
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
  // @breif 弹性模量 GPa 1GPa=1000MPa
  // @brief The elastic modulus GPa 1GPa=1000MPa
  float f_elastic_modulus_GPa_;
  // @brief 密度 kg/m^3
  // @brief The density kg/m^3
  float f_density_kg_m3_;
  // @brief 应力最大值 MPa
  // @brief The max stress MPa
  float f_max_stress_MPa_;
  // @brief 应力比
  // @brief The stress ratio
  float f_stress_ratio_;
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

struct ExpDesignResult {};

// @brief 试验设计参数
// 试验设计参数
// @brief The ExpDesignParam struct
// exp design param
struct ExpDesignResultAxially : public ExpDesignResult {
  explicit ExpDesignResultAxially(const ExpDesignBaseParam& base_param);
  // @brief Amplitude A um
  // @brief 振幅A  um
  float f_eamplitude_;
  // @brief 试验中应力与位移的关系
  // 应力位移系数 MPa/um
  // @brief The stress displacement coefficient
  // stress displacement coefficient MPa/um
  float f_stress_displacement_coefficient_;
  // @brief 试验段半径 R2 mm
  // @brief The radius of the exp section R2 mm
  float f_exp_section_radius_R2_;
  // @brief 平行段半径 R1 mm
  // @brief The radius of the parallel section R1 mm
  float f_parallel_section_radius_R1_;
  // @brief 过渡段半径 R0 mm
  // @brief The radius of the transition section R0 mm
  float f_transition_section_radius_R0_;
  // @brief 过渡段长度 L1 mm
  // @brief The length of the transition section L1 mm
  float f_transition_section_length_L1_;
  // @brief 平行段长度 L0 mm
  // @brief The length of the parallel section L0 mm
  float f_parallel_section_length_L0_;
  // @brief 试验段长度 L2 mm
  // @brief The length of the exp section L2 mm
  float f_exp_section_length_L2_;
};

struct ExpDesignResultStressesAdjustable : public ExpDesignResultAxially {
  explicit ExpDesignResultStressesAdjustable(
      const ExpDesignBaseParam& base_param);
  // @brief 静载 MPa
  // @brief Static load MPa
  float f_static_load_MPa_;
};

struct ExpDesignResult2 : public ExpDesignResult {
  explicit ExpDesignResult2(const ExpDesignBaseParam& base_param);
  // @brief Amplitude A um
  // @brief 振幅A  um
  float f_eamplitude_um_;
  // @brief 试验中应力与位移的关系
  // 应力位移系数 MPa/um
  // @brief The stress displacement coefficient
  // stress displacement coefficient MPa/um
  float f_stress_displacement_coefficient_;
  // @brief 试验段宽度 W mm
  // @brief The width of the exp section W mm
  float f_exp_section_width_W_mm_;
  // @brief 试验段厚度 H mm
  // @brief The thickness of the exp section H mm
  float f_exp_section_thickness_H_mm_;
};

struct ExpDesignResultTh3pointBending : public ExpDesignResult2 {
  explicit ExpDesignResultTh3pointBending(const ExpDesignBaseParam& base_param);
  // @brief 静载 MPa
  // @brief Static load MPa
  float f_static_load_MPa_;
  // @brief 试件宽度 B mm
  // @brief The width of the specimen B mm
  float f_specimen_width_B_;
  // @brief 试件厚度 h mm
  // @brief The thickness of the specimen h mm
  float f_specimen_thickness_h_;
  // @brief 试件长度 L mm
  // @brief The length of the specimen L mm
  float f_specimen_length_L_;
  // @brief 支撑距离 L0 mm
  // @brief The support distance L0 mm
  float f_support_distance_L0_;
};

struct ExpDesignResultVibrationBending : public ExpDesignResult2 {
  explicit ExpDesignResultVibrationBending(
      const ExpDesignBaseParam& base_param);
  // @brief 静载 MPa
  // @brief Static load MPa
  float f_static_load_MPa_;
  // @brief 试验平行段长度 L1 mm
  // @brief The length of the parallel section L1 mm
  float f_specimen_parallel_length_L1_;
  // @brief 试验圆弧段半径 R1 mm
  // @brief The radius of the arc section R1 mm
  float f_specimen_arc_radius_R1_;
  // @brief 试验过渡段半径 R2 mm
  // @brief The radius of the transition section R2 mm
  float f_specimen_transition_radius_R2_;
  // @brief 夹持段厚度 d1 mm
  // @brief The thickness of the clamping section d1 mm
  float f_clamping_thickness_d1_;
  // @brief 试验段厚度 L0 d2 mm
  // @brief The thickness of the test section L0 d2 mm
  float f_test_thickness_L0_d2_;
};

// @brief 试验设计接口
class ExpDesign {
 public:
  ExpDesign() = default;
  virtual ~ExpDesign() = default;

 public:
  virtual const ExpDesignHeader& GetExpDesignHeader() = 0;
  virtual void SetExpDesignHeader(const ExpDesignHeader& header) = 0;
  virtual const ExpDesignBaseParam& GetExpDesignBaseParam() = 0;
  virtual void SetExpDesignBaseParam(const ExpDesignBaseParam& base_param) = 0;
};

struct SolutionExpDesign {
  int32_t solution_id_;
  std::string version;
  std::string date;
  std::string copy_right;
};

extern const SolutionExpDesign kSolutionExpDesign[];

struct SolutionExpDesignFactory {
  SolutionExpDesignFactory() = default;
  virtual ~SolutionExpDesignFactory() = default;

  // @brief 创建试验设计 @see kSolutionName_Axially_Symmetrical ...etc
  // 返回试验设计指针。对象含有默认值
  // @brief Create the exp design @see kSolutionName_Axially_Symmetrical ...etc
  // Return the exp design pointer. The object has default values.
  static std::unique_ptr<SolutionExpDesign> CreateSolutionExpDesign(
      int32_t solution_type);

  // @brief 获取ExpDesignBaseParam @see kSolutionName_Axially_Symmetrical ...etc
  // 返回ExpDesignBaseParam指针。对象含有默认值
  // @brief Get ExpDesignBaseParam @see kSolutionName_Axially_Symmetrical ...etc
  // Return the ExpDesignBaseParam pointer. The object has default values.
  static std::unique_ptr<ExpDesignBaseParam> GetDefaultExpDesignBaseParam(
      int32_t solution_type);

  // @brief 获取ExpDesignParam @see kSolutionName_Axially_Symmetrical ...etc
  // 返回ExpDesignParam指针。对象含有默认值
  // @brief Get ExpDesignParam @see kSolutionName_Axially_Symmetrical ...etc
  // Return the ExpDesignParam pointer. The object has default values.
  static std::unique_ptr<ExpDesignResult> GetExpDesignParam(
      int32_t solution_type,
      const ExpDesignBaseParam& base_param);
};

}  // namespace esolution
}  // namespace anx

#endif  // APP_ESOLUTION_SOLUTION_DESIGN_H_
