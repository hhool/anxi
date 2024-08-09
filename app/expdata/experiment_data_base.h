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
/// helper function

int32_t SaveExperimentDataToCsvWithDefaultPath(
    const std::vector<anx::expdata::ExperimentData>& exp_data,
    int64_t start_time);
}  // namespace expdata
}  // namespace anx

#endif  // APP_EXPDATA_EXPERIMENT_DATA_BASE_H_
