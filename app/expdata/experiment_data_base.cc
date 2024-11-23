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
#include <sstream>
#include <string>
#include <vector>

#if defined(WIN32)
extern "C" {
#include "app/expdata/strptime.h"
}
#endif

#include "app/common/logger.h"
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

std::string TimeToString(int64_t time, const std::string& format) {
  struct tm* timeinfo = localtime(&time);
  char buffer[80];
  strftime(buffer, 80, format.c_str(), timeinfo);
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
    int64_t start_time,
    std::string* file_pathname) {
  // format file name as start_time_stop_time.csv
  // represent as 2024-08-11_12-00-00_2024-08-11_12-00-00.csv
  std::string start_time_str = TimeToString(start_time);
  std::string stop_time_str = TimeToString(time(nullptr));
  std::string default_csv = start_time_str + "_" + stop_time_str + ".csv";
  // get module path
  std::string app_data_dir = anx::common::GetApplicationDataPath();
  std::string sub_dir = kCsvDefaultPath;
  app_data_dir = app_data_dir + "\\anxi\\" + sub_dir;
  // make sure the folder exist
  if (!anx::common::MakeSureFolderPathExist(app_data_dir)) {
    LOG_F(LG_ERROR) << "make sure folder path exist failed:" << app_data_dir;
    return -1;
  }

  default_csv = app_data_dir + "\\" + default_csv;
  if (file_pathname != nullptr) {
    *file_pathname = default_csv;
  }
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

////////////////////////////////////////////////////////////////////////////////
namespace {
int32_t win_system(const std::string& cmd, const std::string& par) {
  DWORD ReturnValue = -1;
  SHELLEXECUTEINFOA ShExecInfo = {0};
  ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
  ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
  ShExecInfo.hwnd = NULL;
  ShExecInfo.lpVerb = NULL;
  ShExecInfo.lpFile = cmd.c_str();
  ShExecInfo.lpParameters = par.c_str();
  ShExecInfo.lpDirectory = NULL;
  ShExecInfo.nShow = SW_HIDE;
  ShExecInfo.hInstApp = NULL;
  if (!ShellExecuteExA(&ShExecInfo)) {
    return -1;
  }
  WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
  if (!GetExitCodeProcess(ShExecInfo.hProcess, &ReturnValue)) {
    return -2;
  }
  return ReturnValue;
}
}  // namespace

ExperimentReport::ExperimentReport() {}
ExperimentReport::~ExperimentReport() {}

std::string ExperimentReport::ToXml() const {
  std::stringstream ss;
  ss << "<ExperimentReport>\r\n";
  /// format the time to string format e.g. 2024-08-11_12-00-00
  std::string start_time_str = TimeToString(start_time_, "%m/%d %H-%M-%S");
  std::string end_time_str = TimeToString(end_time_, "%m/%d %H-%M-%S");
  ss << "<StartTime>" << start_time_str << "</StartTime>\r\n";
  ss << "<EndTime>" << end_time_str << "</EndTime>\r\n";
  ss << "<ExperimentName>"
     << ((experiment_name_.length() == 0) ? "default_experiment_name"
                                          : experiment_name_)
     << "</ExperimentName>\r\n";
  ss << "<ElasticModulus>" << elastic_modulus_ << "</ElasticModulus>\r\n";
  ss << "<Density>" << density_ << "</Density>\r\n";
  ss << "<MaxStress>" << max_stress_ << "</MaxStress>\r\n";
  ss << "<RatioOfStress>" << ratio_stress_ << "</RatioOfStress>\r\n";
  ss << "<CycleCount>" << cycle_count_ << "</CycleCount>\r\n";
  ss << "<BottomAmplitude>" << amplitude_ << "</BottomAmplitude>\r\n";
  ss << "<IntermittentExp>" << exp_type_ << "</IntermittentExp>\r\n";
  ss << "<ExcitationTime>" << excitation_time_ * 100 << "</ExcitationTime>\r\n";
  ss << "<IntervalTime>" << interval_time_ * 100 << "</IntervalTime>\r\n";
  ss << "<ExpMode>" << exp_mode_ << "</ExpMode>\r\n";
  ss << "</ExperimentReport>\r\n";
  return ss.str();
}

int32_t SaveReportToDocxWithDefaultPath(const ExperimentReport& exp_report,
                                        const std::string& cvs_file_pathname,
                                        const std::string& file_name) {
  // 1. generate xml file with experiment final result and experiment
  // parameters.
  // 2. generate csv file with the experiment data.
  // 3. call program `todocx` with 1 and 2 to generate the final report docx
  // file.
  // 4. remove the temp xml and csv file.
  // get module path
  std::string app_data_dir = anx::common::GetApplicationDataPath();
  // generate xml file, save to the module dir
  //// copy file to the expreport dir under the module dir
  std::string exp_report_dir = app_data_dir + "\\anxi\\expreport";
  if (!anx::common::MakeSureFolderPathExist(exp_report_dir)) {
    LOG_F(LG_ERROR) << "make sure folder path exist failed:" << exp_report_dir;
    return -1;
  }
  std::string xml_file = exp_report_dir + "\\summary.xml";
  std::string xml_content = exp_report.ToXml();
  FILE* file = fopen(xml_file.c_str(), "wb");
  if (file == nullptr) {
    LOG_F(LG_ERROR) << "open file failed:" << xml_file;
    return -2;
  }
  size_t size = xml_content.size();
  size_t written = fwrite(xml_content.c_str(), 1, size, file);
  if (written != size) {
    fclose(file);
    LOG_F(LG_ERROR) << "write file failed:" << xml_file;
    return -3;
  }
  fclose(file);
  //// template file is in the app data dir
  std::string template_file_src =
      anx::common::GetModuleDir() + "\\template\\3th_report_template.docx";
  std::string template_file_to = exp_report_dir + "\\3th_report_template.docx";
  //// copy the template file to the expreport dir force overwrite
  if (!CopyFileA(template_file_src.c_str(), template_file_to.c_str(), FALSE)) {
    LOG_F(LG_ERROR) << "copy file failed:" << template_file_to;
    return -5;
  }
  // start todocx program with args
  //  todocx.exe -j summary.xml -i input.csv -t template.docx
  std::string todocx = anx::common::GetModuleDir() + "\\todocx.exe";
  std::string args =
      "-s " + xml_file + " -i " + cvs_file_pathname + " -t " + template_file_to;
  std::string cmd = todocx + " " + args;
  /// @note call the todocx program that is console program, hide the console
  /// window wait for the program to finish, get result from the program and
  /// remove the temp xml and csv file.
  int ret;
#if defined(_WIN32)
  ret = win_system(todocx, args);
#else
  ret = system(cmd.c_str());
#endif
  if (ret != 0) {
    LOG_F(LG_ERROR) << "call todocx failed:" << ret << " " << cmd;
    return -6;
  }
  // remove the temp xml and csv file
  remove(xml_file.c_str());
  return 0;
}

}  // namespace expdata
}  // namespace anx
