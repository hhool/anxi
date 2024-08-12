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
  float KHz_;
  float MPa_;
  float um_;
};
////////////////////////////////////////////////////////////////////////////////
/// helper function
/// @brief Load the experiment data from the csv file
/// @param file_path the file path of the csv file
/// @return the experiment data vector
int32_t SaveExperimentDataToCsvWithDefaultPath(
    const std::vector<anx::expdata::ExperimentData>& exp_data,
    int64_t start_time);

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

}  // namespace expdata
}  // namespace anx

#endif  // APP_EXPDATA_EXPERIMENT_DATA_BASE_H_
