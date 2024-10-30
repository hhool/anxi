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

#if defined(WIN32)
extern "C" {
#include "app/expdata/strptime.h"
}
#endif

#include "app/common/module_utils.h"
#include "app/common/string_utils.h"
#include "app/expdata/LibOb_strptime.h"

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

uint64_t StringToTime(const std::string& time_str) {
  struct tm tm;
  memset(&tm, 0, sizeof(tm));
  ::strptime(time_str.c_str(), "%Y-%m-%d_%H-%M-%S", &tm);
  return mktime(&tm);
}

const char kCsvHeader[] = "id,cycle_count,KHz,MPa,um\n";
const char kCsvFormat[] = "%lu,%lu,%f,%f,%f\n";
const char kCsvDefaultPath[] = "expdata";
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
        anx::common::String2WString(module_dir.c_str()).c_str());
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

////////////////////////////////////////////////////////////////////////////////
ExperimentFileSummary::ExperimentFileSummary() {}
ExperimentFileSummary::~ExperimentFileSummary() {}

namespace {

int32_t TraverseDir(
    const std::string& dir,
    std::vector<anx::expdata::ExperimentFileSummary>* summarys) {
#if defined(_WIN32)
  WIN32_FIND_DATA find_data;
  HANDLE hFind = FindFirstFile(anx::common::String2WString(dir + "\\*").c_str(),
                               &find_data);
  if (hFind == INVALID_HANDLE_VALUE) {
    return -1;
  }
  do {
    if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      continue;
    }
    /* file_list->push_back(dir + "\\" +
                          anx::common::WString2String(find_data.cFileName));*/
    // parse file content to get the start_time, end_time.
    // file name format is start_time_stop_time.csv
    // represent as 2024-08-11_12-00-00_2024-08-11_12-00-00.csv
    std::string file_name = anx::common::WString2String(find_data.cFileName);
    std::string start_time_str = file_name.substr(0, 19);
    std::string end_time_str = file_name.substr(20, 19);
    anx::expdata::ExperimentFileSummary file_summary;
    file_summary.file_name_ = file_name;
    file_summary.start_time_ = anx::expdata::StringToTime(start_time_str);
    file_summary.end_time_ = anx::expdata::StringToTime(end_time_str);
    summarys->push_back(file_summary);
  } while (FindNextFile(hFind, &find_data) != 0);
  FindClose(hFind);
  return 0;
#else
  return -1;
#endif
}
}  // namespace

int32_t TraverseExpDataFolder(
    std::vector<anx::expdata::ExperimentFileSummary>* exp_file_list) {
  // get module path
  std::string module_dir = anx::common::GetModuleDir();
  std::string sub_dir = kCsvDefaultPath;
  module_dir = module_dir + "\\" + sub_dir;
  // check if dir exists
#if defined(_WIN32)
  DWORD dwAttr = GetFileAttributes(
      anx::common::String2WString(module_dir.c_str()).c_str());
  if (dwAttr == 0xffffffff || !(dwAttr & FILE_ATTRIBUTE_DIRECTORY)) {
#else
  if (access(module_dir.c_str(), 0) != 0) {
#endif
    return -1;
  }
  // traverse the directory
  return TraverseDir(module_dir, exp_file_list);
}
}  // namespace expdata
}  // namespace anx
