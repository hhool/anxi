/**
 * @file experiment_data_base.h
 * @author hhool (hhool@outlook.com)
 * @brief experimental data record related
 * @version 0.1
 * @date 2024-08-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_EXPDATA_EXPERIMENT_DATA_BASE_H_
#define APP_EXPDATA_EXPERIMENT_DATA_BASE_H_

#include <memory>
#include <string>
#include <vector>

namespace anx {
namespace expdata {
class ExperimentData {
 public:
  ExperimentData();
  virtual ~ExperimentData();

 public:
  uint64_t id_;
  uint64_t cycle_count_;
  double KHz_;
  double MPa_;
  double um_;
};
////////////////////////////////////////////////////////////////////////////////
/// helper function
/// @brief Load the experiment data from the csv file
/// @param file_path the file path of the csv file
/// @return the experiment data vector
int32_t SaveExperimentDataToCsvWithDefaultPath(
    const std::vector<anx::expdata::ExperimentData>& exp_data,
    int64_t start_time,
    std::string* file_pathname = nullptr);

class ExperimentFileSummary {
 public:
  ExperimentFileSummary();
  virtual ~ExperimentFileSummary();

 public:
  std::string file_name_;
  uint64_t start_time_;
  uint64_t end_time_;
};

/// @brief Traverse the directory expdata folder and get all the csv files
/// @param file_list the file list
/// @return int32_t 0 if success, -1 if failed
int32_t TraverseExpDataFolder(
    std::vector<anx::expdata::ExperimentFileSummary>* exp_file_list);

class ExperimentReport {
 public:
  ExperimentReport();
  virtual ~ExperimentReport();

 public:
  /// @brief generate xml file with experiment final result and experiment
  /// parameters.
  /// @return std::string
  /// example:
  ///
  /// <ExperimentReport>
  ///     <StartTime>2022-01-01 12:00:00</StartTime>
  ///     <EndTime>2022-01-01 13:00:00</EndTime>
  ///     <ExperimentName>Sample ExperimentReport</ExperimentName>
  ///     <ElasticModulus>100 GPa</ElasticModulus>
  ///     <Density>2.7 g/cm^3</Density>
  ///     <MaxStress>200 MPa</MaxStress>
  ///     <RatioOfStress>0.5</RatioOfStress>
  ///     <CycleCount>1000</CycleCount>
  ///     <BottomAmplitude>10 mm</BottomAmplitude>
  /// </ExperimentReport>
  std::string ToXml() const;

 public:
  /// @brief experiment start time
  uint64_t start_time_ = 0;
  /// @brief experiment end time
  uint64_t end_time_ = 0;
  /// @brief experiment name
  std::string experiment_name_ = "";
  /// @brief unit: GPa
  double elastic_modulus_ = 0;
  /// @brief unit: g/cm^3
  double density_ = 0;
  /// @brief unit: MPa
  double max_stress_ = 0;
  /// @brief unit: none
  double ratio_stress_ = 0;
  /// @brief unit: none
  int64_t cycle_count_ = 0;
  /// @brief unit: mm
  double amplitude_ = 0;
  /// @brief IntermittentExp 1 or ContinuousExp 0
  int32_t exp_type_ = 0;
  /// @brief excitation time in ms valid when exp_type_ is IntermittentExp
  int64_t excitation_time_ = 0;
  /// @brief interval time in ms valid when exp_type_ is IntermittentExp
  int64_t interval_time_ = 0;
  /// @brief exp_mode_ 0 - linear, 1 - exponent
  int32_t exp_mode_ = 0;
};

/// @brief Save the report to the docx file
int32_t SaveReportToDocxWithDefaultPath(const ExperimentReport& exp_report,
                                        const std::string& cvs_file_pathname,
                                        const std::string& file_name);

}  // namespace expdata
}  // namespace anx

#endif  // APP_EXPDATA_EXPERIMENT_DATA_BASE_H_
