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

#include "app/common/file_utils.h"
#include "app/common/logger.h"
#include "app/common/module_utils.h"
#include "app/common/num_string_convert.hpp"
#include "app/common/string_utils.h"
#include "app/expdata/LibOb_strptime.h"

#include "third_party/tinyxml2/source/tinyxml2.h"

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

const char kTimeFormat[] = "%Y-%m-%d_%H-%M-%S";

std::string TimeToString(int64_t time, const char* format = kTimeFormat) {
  struct tm* timeinfo = localtime(&time);
  char buffer[80];
  strftime(buffer, 80, format, timeinfo);
  return std::string(buffer);
}

int64_t StringToTime(const std::string& time_str) {
  struct tm tm;
  memset(&tm, 0, sizeof(tm));
  ::strptime(time_str.c_str(), kTimeFormat, &tm);
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
        "," + anx::common::to_string_with_precision(data.KHz_, 3) + "," +
        anx::common::to_string_with_precision(data.MPa_, 6) + "," +
        anx::common::to_string_with_precision(data.um_, 2) + "\n";
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
    const ExperimentReport& exp_report,
    const std::vector<anx::expdata::ExperimentData>& exp_data,
    std::string* file_pathname) {
  // format file name as start_time_stop_time.csv
  // represent as 2024-08-11_12-00-00_2024-08-11_12-00-00.csv
  std::string start_time_str = TimeToString(exp_report.start_time_);
  std::string stop_time_str = TimeToString(exp_report.end_time_);
  std::string default_csv = start_time_str + "_" + stop_time_str + ".csv";
  // get module path
  std::string app_data_dir = anx::common::GetApplicationDataPath("anxi");
  std::string sub_dir = kCsvDefaultPath;
  app_data_dir = app_data_dir + anx::common::kPathSeparator + sub_dir;

  // make sure the folder exist
  if (!anx::common::MakeSureFolderPathExist(app_data_dir)) {
    LOG_F(LG_ERROR) << "make sure folder path exist failed:" << app_data_dir;
    return -1;
  }

  default_csv = app_data_dir + anx::common::kPathSeparator + default_csv;
  if (file_pathname != nullptr) {
    *file_pathname = default_csv;
  }
  return SaveExperimentDataFile(default_csv, exp_data);
}
////////////////////////////////////////////////////////////////////////////////
ExperimentFileSummary::ExperimentFileSummary() {}
ExperimentFileSummary::~ExperimentFileSummary() {}

namespace {

/// @brief  Traverse the directory and get all the csv files with xml file
/// @param dir  the directory of files stored
/// @param summarys  the file summary list
/// @return int32_t 0 if success, -1 if failed
/// @note the file name format is start_time_stop_time.csv and the xml file
/// name is start_time_stop_time.xml represent as
/// 2024-08-11_12-00-00_2024-08-11_12-00-00.csv and
/// 2024-08-11_12-00-00_2024-08-11_12-00-00.xml
/// TODO(hhool): add the xml file check to make sure the csv file is valid
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
    /* file_list->push_back(dir + anx::common::kPathSeparator +
                          anx::common::WString2String(find_data.cFileName));*/
    // parse file content to get the start_time, end_time.
    // file name format is start_time_stop_time.csv
    // represent as 2024-08-11_12-00-00_2024-08-11_12-00-00.csv
    std::string file_full_name =
        anx::common::WString2String(find_data.cFileName);
    std::string file_ext = file_full_name.substr(file_full_name.size() - 4);
    if (file_ext != ".csv") {
      continue;
    }
    if (file_full_name.size() < 40) {
      continue;
    }
    if (file_ext == ".csv") {
      // get the file name without extension
      std::string file_name =
          file_full_name.substr(0, file_full_name.size() - 4);
      // check if the file name with .xml extension is exsited
      std::string xml_file_name = file_name + ".xml";
      std::string xml_file_full_name =
          dir + anx::common::kPathSeparator + xml_file_name;
      // if the xml file is not exsited then delete the csv file
      if (anx::common::FileExists(xml_file_full_name) == false) {
        std::string csv_file_full_name =
            dir + anx::common::kPathSeparator + file_full_name;
        remove(csv_file_full_name.c_str());
        continue;
      }
    }
    std::string start_time_str = file_full_name.substr(0, 19);
    std::string end_time_str = file_full_name.substr(20, 19);
    anx::expdata::ExperimentFileSummary file_summary;
    file_summary.file_name_ = file_full_name;
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
  std::string app_data_dir = anx::common::GetApplicationDataPath("anxi");
  std::string sub_dir = kCsvDefaultPath;
  app_data_dir = app_data_dir + anx::common::kPathSeparator + sub_dir;
  // check if dir exists
#if defined(_WIN32)
  DWORD dwAttr = GetFileAttributes(
      anx::common::String2WString(app_data_dir.c_str()).c_str());
  if (dwAttr == 0xffffffff || !(dwAttr & FILE_ATTRIBUTE_DIRECTORY)) {
#else
  if (access(module_dir.c_str(), 0) != 0) {
#endif
    return -1;
  }
  // traverse the directory
  return TraverseDir(app_data_dir, exp_file_list);
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

void MakeFolderPathByDate(const std::string& root,
                          struct tm tm,
                          std::string* folder_pathname) {
  std::string folders[4] = {
      root,
      root + anx::common::kPathSeparator + std::to_string(tm.tm_year + 1900),
      root + anx::common::kPathSeparator + std::to_string(tm.tm_year + 1900) +
          anx::common::kPathSeparator + std::to_string(tm.tm_mon + 1),
      root + anx::common::kPathSeparator + std::to_string(tm.tm_year + 1900) +
          anx::common::kPathSeparator + std::to_string(tm.tm_mon + 1) +
          anx::common::kPathSeparator + std::to_string(tm.tm_mday)};
  for (int i = 0; i < 4; i++) {
    if (anx::common::DirectoryExists(folders[i])) {
      continue;
    }
    if (anx::common::MakeSureFolderPathExist(folders[i])) {
      continue;
    }
  }
  *folder_pathname = folders[3];
}
}  // namespace

ExperimentReport::ExperimentReport() {}
ExperimentReport::~ExperimentReport() {}

std::string ExperimentReport::ToXml() const {
  std::stringstream ss;
  ss << "<ExperimentReport>\r\n";
  /// format the time to string format e.g. 2024-08-11_12-00-00
  std::string start_time_str = TimeToString(start_time_);
  std::string end_time_str = TimeToString(end_time_);
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
  ss << "<ExcitationTime>" << excitation_time_ << "</ExcitationTime>\r\n";
  ss << "<IntervalTime>" << interval_time_ << "</IntervalTime>\r\n";
  ss << "<ExpMode>" << exp_mode_ << "</ExpMode>\r\n";
  ss << "</ExperimentReport>\r\n";
  return ss.str();
}

std::unique_ptr<ExperimentReport> ExperimentReport::FromXml(
    const std::string& xml_content) {
  tinyxml2::XMLDocument doc;
  if (doc.Parse(xml_content.c_str()) != tinyxml2::XML_SUCCESS) {
    return nullptr;
  }
  tinyxml2::XMLElement* root = doc.RootElement();
  if (root == nullptr) {
    return nullptr;
  }
  tinyxml2::XMLElement* ele_start_time = root->FirstChildElement("StartTime");
  if (ele_start_time == nullptr) {
    return nullptr;
  }
  tinyxml2::XMLElement* ele_end_time = root->FirstChildElement("EndTime");
  if (ele_end_time == nullptr) {
    return nullptr;
  }
  tinyxml2::XMLElement* ele_experiment_name =
      root->FirstChildElement("ExperimentName");
  if (ele_experiment_name == nullptr) {
    return nullptr;
  }
  tinyxml2::XMLElement* ele_elastic_modulus =
      root->FirstChildElement("ElasticModulus");
  if (ele_elastic_modulus == nullptr) {
    return nullptr;
  }
  tinyxml2::XMLElement* ele_density = root->FirstChildElement("Density");
  if (ele_density == nullptr) {
    return nullptr;
  }
  tinyxml2::XMLElement* ele_max_stress = root->FirstChildElement("MaxStress");
  if (ele_max_stress == nullptr) {
    return nullptr;
  }
  tinyxml2::XMLElement* ele_ratio_stress =
      root->FirstChildElement("RatioOfStress");
  if (ele_ratio_stress == nullptr) {
    return nullptr;
  }
  tinyxml2::XMLElement* ele_cycle_count = root->FirstChildElement("CycleCount");
  if (ele_cycle_count == nullptr) {
    return nullptr;
  }
  tinyxml2::XMLElement* ele_amplitude =
      root->FirstChildElement("BottomAmplitude");
  if (ele_amplitude == nullptr) {
    return nullptr;
  }
  tinyxml2::XMLElement* ele_exp_type =
      root->FirstChildElement("IntermittentExp");
  if (ele_exp_type == nullptr) {
    return nullptr;
  }
  tinyxml2::XMLElement* ele_excitation_time =
      root->FirstChildElement("ExcitationTime");
  if (ele_excitation_time == nullptr) {
    return nullptr;
  }
  tinyxml2::XMLElement* ele_interval_time =
      root->FirstChildElement("IntervalTime");
  if (ele_interval_time == nullptr) {
    return nullptr;
  }
  tinyxml2::XMLElement* ele_exp_mode = root->FirstChildElement("ExpMode");
  if (ele_exp_mode == nullptr) {
    return nullptr;
  }
  std::unique_ptr<ExperimentReport> exp_report =
      std::make_unique<ExperimentReport>();
  exp_report->start_time_ =
      anx::expdata::StringToTime(ele_start_time->GetText());
  exp_report->end_time_ = anx::expdata::StringToTime(ele_end_time->GetText());
  exp_report->experiment_name_ = ele_experiment_name->GetText();
  exp_report->elastic_modulus_ = std::stod(ele_elastic_modulus->GetText());
  exp_report->density_ = std::stod(ele_density->GetText());
  exp_report->max_stress_ = std::stod(ele_max_stress->GetText());
  exp_report->ratio_stress_ = std::stod(ele_ratio_stress->GetText());
  exp_report->cycle_count_ = std::stol(ele_cycle_count->GetText());
  exp_report->amplitude_ = std::stod(ele_amplitude->GetText());
  exp_report->exp_type_ = std::stoi(ele_exp_type->GetText());
  exp_report->excitation_time_ = std::stol(ele_excitation_time->GetText());
  exp_report->interval_time_ = std::stol(ele_interval_time->GetText());
  exp_report->exp_mode_ = std::stoi(ele_exp_mode->GetText());
  return exp_report;
}

int32_t LoadExperimentReportWithFilePath(const std::string& file_path,
                                         ExperimentReport* exp_report) {
  std::string xml_content;
  if (!anx::common::ReadFile(file_path, &xml_content, true)) {
    LOG_F(LG_ERROR) << "read file failed:" << file_path;
    return -1;
  }
  std::unique_ptr<ExperimentReport> report =
      ExperimentReport::FromXml(xml_content);
  if (report == nullptr) {
    LOG_F(LG_ERROR) << "parse xml failed:" << file_path;
    return -2;
  }
  *exp_report = *report;
  return 0;
}

int32_t SaveExperimentReportToXmlWithDefaultPath(
    const ExperimentReport& exp_report,
    std::string* file_pathname) {
  // get app data path
  std::string app_data_dir = anx::common::GetApplicationDataPath("anxi");
  // generate xml file, save to the app data dir under the anxi/expdata dir
  std::string sub_dir = kCsvDefaultPath;
  app_data_dir = app_data_dir + anx::common::kPathSeparator + sub_dir;
  // make sure the folder exist
  if (!anx::common::MakeSureFolderPathExist(app_data_dir)) {
    LOG_F(LG_ERROR) << "make sure folder path exist failed:" << app_data_dir;
    return -1;
  }
  // format file name as start_time_stop_time.xml
  // represent as 2024-08-11_12-00-00_2024-08-11_12-00-00.xml
  std::string start_time_str = TimeToString(exp_report.start_time_);
  std::string stop_time_str = TimeToString(exp_report.end_time_);
  std::string default_xml = start_time_str + "_" + stop_time_str + ".xml";
  default_xml = app_data_dir + anx::common::kPathSeparator + default_xml;
  if (file_pathname != nullptr) {
    *file_pathname = default_xml;
  }
  // write to the file
  std::string xml_content = exp_report.ToXml();
  if (!anx::common::WriteFile(default_xml, xml_content, true)) {
    LOG_F(LG_ERROR) << "write file failed:" << default_xml;
    return -2;
  }
  return 0;
}

int32_t SaveReportToDocxWithDefaultPath(const ExperimentReport& exp_report,
                                        const std::string& cvs_file_pathname,
                                        std::string* file_pathname) {
  // 1. generate xml file with experiment final result and experiment
  // parameters.
  // 2. generate csv file with the experiment data.
  // 3. call program `todocx` with 1 and 2 to generate the final report docx
  // file.
  // 4. remove the temp xml and csv file.
  // get module path
  std::string app_data_dir = anx::common::GetApplicationDataPath("anxi");
  // generate xml file, save to the module dir
  //// copy file to the expreport dir under the module dir
  std::string exp_report_dir =
      app_data_dir + anx::common::kPathSeparator + "expreport";
  if (!anx::common::MakeSureFolderPathExist(exp_report_dir)) {
    LOG_F(LG_ERROR) << "make sure folder path exist failed:" << exp_report_dir;
    return -1;
  }
  std::string xml_file =
      exp_report_dir + anx::common::kPathSeparator + "summary.xml";
  std::string xml_content = exp_report.ToXml();
  if (!anx::common::WriteFile(xml_file, xml_content, true)) {
    LOG_F(LG_ERROR) << "write file failed:" << xml_file;
    return -2;
  }
  //// template file is in the app data dir
  std::string template_file_src =
      anx::common::GetModuleDir() + anx::common::kPathSeparator + "template" +
      anx::common::kPathSeparator + "3th_report_template.docx";
  struct tm tm;
  /// @note convert start time to tm struct
  time_t start_time = exp_report.start_time_;
#ifdef _WIN32
  if (localtime_s(&tm, &start_time) != 0)
#else
  if (!localtime_r(&start_time, &tm))
#endif
  {
    LOG_F(LG_ERROR) << "localtime failed";
    return -4;
  }
  std::string root_folder_path;
  MakeFolderPathByDate(exp_report_dir, tm, &root_folder_path);
  if (root_folder_path.empty()) {
    LOG_F(LG_ERROR) << "make folder path by date failed";
    return -5;
  }
  //// to file path name
  /// format file name as start_time H-M-S+experiment_name.docx
  std::string to_file_name = TimeToString(exp_report.start_time_, "%H-%M-%S") +
                             "+" + exp_report.experiment_name_ + ".docx";
  std::string template_file_to =
      root_folder_path + anx::common::kPathSeparator + to_file_name;

  //// copy the template file to the expreport dir force overwrite
  std::string src = template_file_src;
  std::string to =
      anx::common::WString2String(anx::common::UTF8ToUnicode(template_file_to));
  if (!anx::common::CCopyFile(src.c_str(), to.c_str())) {
    LOG_F(LG_ERROR) << "copy file failed:" << template_file_to;
    return -6;
  }

  /// @note start todocx program with args
  ///       todocx.exe -j summary.xml -i input.csv -t template.docx
  std::string todocx =
      anx::common::GetModuleDir() + anx::common::kPathSeparator + "todocx.exe";
  std::string args =
      "-s " + xml_file + " -i " + cvs_file_pathname + " -t " + to;
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
    return -7;  // NOLINT
  }
  // remove the temp xml and csv file
  remove(xml_file.c_str());
  if (file_pathname != nullptr) {
    *file_pathname = to;
  }
  return 0;
}

}  // namespace expdata
}  // namespace anx
