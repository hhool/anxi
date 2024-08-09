/**
 * @file experiment_data_base.cc
 * @author hhool (hhool@outlook.com)
 * @brief experimental data record related
 * @version 0.1
 * @date 2024-08-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/expdata/experiment_data_base.h"

#include <time.h>
#include <memory>
#include <string>
#include <vector>

#include "app/common/module_utils.h"
#include "app/common/string_utils.h"

#if defined(_WIN32)
#include <direct.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif
namespace anx {
namespace expdata {
ExperimentData::ExperimentData() {}
ExperimentData::~ExperimentData() {}

////////////////////////////////////////////////////////////////////////////////
namespace {

std::string TimeToString(int64_t time) {
  struct tm* timeinfo = localtime(&time);
  char buffer[80];
  strftime(buffer, 80, "%Y-%m-%d_%H-%M-%S", timeinfo);
  return std::string(buffer);
}

const std::string kCsvHeader = "id,cycle_count,KHz,MPa,um\n";
const std::string kCsvFormat = "%lu,%lu,%f,%f,%f\n";
const std::string kCsvDefaultPath = "expdata";
}  // namespace

/// helper function
int32_t SaveExperimentDataFile(
    const std::string& file_path,
    const std::vector<anx::expdata::ExperimentData>& exp_data) {
  FILE* file = fopen(file_path.c_str(), "wb");
  if (file == nullptr) {
    return -1;
  }
  // write as csv format
  // write header id, cycle_count, KHz, MPa, um
  std::string header = "id,cycle_count,KHz,MPa,um\n";
  size_t header_size = header.size();
  size_t header_written = fwrite(header.c_str(), 1, header_size, file);
  if (header_written != header_size) {
    fclose(file);
    return -1;
  }
  // write data
  for (const auto& data : exp_data) {
    std::string line =
        std::to_string(data.id_) + "," + std::to_string(data.cycle_count_) +
        "," + std::to_string(data.KHz_) + "," + std::to_string(data.MPa_) +
        "," + std::to_string(data.um_) + "\n";
    size_t size = line.size();
    size_t written = fwrite(line.c_str(), 1, size, file);
    if (written != size) {
      fclose(file);
      return -2;
    }
  }
  fclose(file);
  return 0;
}

int32_t SaveExperimentDataToCsvWithDefaultPath(
    const std::vector<anx::expdata::ExperimentData>& exp_data,
    int64_t start_time) {
  // format file name as start_time_stop_time.csv
  // represent as 2024-08-11_12-00-00_2024-08-11_12-00-00.csv
  std::string start_time_str = TimeToString(start_time);
  std::string stop_time_str = TimeToString(time(nullptr));
  std::string default_csv = start_time_str + "_" + stop_time_str + ".csv";
  // get module path
  std::string module_dir = anx::common::GetModuleDir();
  std::string sub_dir = kCsvDefaultPath;
  module_dir = module_dir + "\\" + sub_dir;
  // create sub dir
#if defined(_WIN32)
  int status = _mkdir(module_dir.c_str());
#else
  int status = mkdir(module_dir.c_str(), 0777);
#endif
  // check status
  if (status != 0) {
// check if dir exists
#if defined(_WIN32)
    DWORD dwAttr = GetFileAttributes(
        anx::common::string2wstring(module_dir.c_str()).c_str());
    if (dwAttr == 0xffffffff || !(dwAttr & FILE_ATTRIBUTE_DIRECTORY)) {
#else
    if (access(module_dir.c_str(), 0) != 0) {
#endif
      return -1;
    }
  }

  default_csv = module_dir + "\\" + default_csv;
  return SaveExperimentDataFile(default_csv, exp_data);
}
}  // namespace expdata
}  // namespace anx
