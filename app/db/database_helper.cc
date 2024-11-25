/**
 * @file database_helper.cc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-08-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/db/database_helper.h"

#include <cassert>
#include <string>

#include "app/common/file_utils.h"
#include "app/common/logger.h"
#include "app/common/module_utils.h"
#include "app/db/database_factory.h"

namespace anx {
namespace db {

namespace helper {

#if defined(_WIN32) || defined(_WIN64)
const char* kDefaultDatabasePathname = "db\\anxi.db";
#else
const char* kDefaultDatabasePathname = "db/anxi.db";
#endif
const char* kTableExpDataGraph = "exp_data_graph";
const char* kTableExpDataList = "exp_data_list";
const char* kTableSendData = "send_data";
const char* kTableNotification = "notification";
const char* kTableSendNotify = "send_notify";

namespace sql {
///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Create table exp_data_graph sql format string for exp data record
const char* kCreateTableExpDataGraphSqlFormat =
    "CREATE TABLE exp_data_graph (id INTEGER PRIMARY KEY AUTOINCREMENT, cycle "
    "INTEGER, "
    "kHz REAL, MPa REAL, um REAL, state INTEGER, date REAL)";

const char* kInsertTableExpDataGraphSqlFormat =
    "INSERT INTO exp_data_graph (cycle, kHz, MPa, um, state, date) VALUES (%d, "
    "%f, "
    "%f, %f, "
    "%d, "
    "%f)";

const char* kQueryTableExpDataGraphSqlByIdFormat =
    "SELECT * FROM exp_data_graph WHERE id >= %d AND id <= %d";

const char* kQueryTableExpDataGraphSqlByTimeFormat =
    "SELECT * FROM exp_data_graph WHERE date >= %f AND date <= %f";

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Create table exp_data_list sql format string for exp data record
const char* kCreateTableExpDataListSqlFormat =
    "CREATE TABLE exp_data_list (id INTEGER PRIMARY KEY AUTOINCREMENT, cycle "
    "INTEGER, "
    "kHz REAL, MPa REAL, um REAL, date REAL)";

const char* kInsertTableExpDataListSqlFormat =
    "INSERT INTO exp_data_list (cycle, kHz, MPa, um, date) VALUES (%d, %f, "
    "%f, %f, "
    "%f)";

const char* kQueryTableExpDataListSqlByIdFormat =
    "SELECT * FROM exp_data_list WHERE id >= %d AND id <= %d";

const char* kQueryTableExpDataListSqlByTimeFormat =
    "SELECT * FROM exp_data_list WHERE date >= %f AND date <= %f";

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Create table static load sql format string for exp data record
const char* kCreateTableSendDataSqlFormat =
    "CREATE TABLE send_data (id INTEGER PRIMARY KEY AUTOINCREMENT, content "
    "TEXT, date REAL)";

const char* kInsertTableSendDataSqlFormat =
    "INSERT INTO send_data (content, date) VALUES ('%s', %f)";

const char* kQueryTableSendDataSqlByIdFormat =
    "SELECT * FROM send_data WHERE id >= %d AND id <= %d";

const char* kQueryTableSendDataSqlByTimeFormat =
    "SELECT * FROM send_data WHERE date >= %f AND date <= %f";

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Create table notification sql format string for exp data record
const char* kCreateTableNotificationSqlFormat =
    "CREATE TABLE notification (id INTEGER PRIMARY KEY AUTOINCREMENT, content "
    "TEXT, date REAL)";

const char* kInsertTableNotificationSqlFormat =
    "INSERT INTO notification (content, date) VALUES ('%s', %f)";

const char* kQueryTableNotificationSqlByIdFormat =
    "SELECT * FROM notification WHERE id >= %d AND id <= %d";

const char* kQueryTableNotificationSqlByTimeFormat =
    "SELECT * FROM notification WHERE date >= %f AND date <= %f";

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Create table send_notify sql format string for exp data record
const char* kCreateTableSendNotifySqlFormat =
    "CREATE TABLE send_notify (id INTEGER PRIMARY KEY AUTOINCREMENT, content "
    "TEXT, date REAL)";

const char* kInsertTableSendNotifySqlFormat =
    "INSERT INTO send_notify (content, date) VALUES ('%s', %f)";

const char* kQueryTableSendNotifySqlByIdFormat =
    "SELECT * FROM send_notify WHERE id >= %d AND id <= %d";

const char* kQueryTableSendNotifySqlByTimeFormat =
    "SELECT * FROM send_notify WHERE date >= %f AND date <= %f";
}  // namespace sql

int32_t DefaultDatabasePathname(std::string* db_filepathname) {
  assert(db_filepathname);
  if (db_filepathname) {
    std::string app_data_dir = anx::common::GetApplicationDataPath();
    if (app_data_dir.empty()) {
      LOG_F(LG_ERROR) << "Failed to get application data path";
      return -1;
    }
#if defined(_WIN32) || defined(_WIN64)
    *db_filepathname = app_data_dir + "\\anxi\\" + kDefaultDatabasePathname;
#else
    *db_filepathname = app_data_dir + "/anxi/" + kDefaultDatabasePathname;
#endif
    return 0;
  }
  return -1;
}

void ClearDatabaseFile(const std::string& db_name) {
  std::string app_data_dir = anx::common::GetApplicationDataPath();
  if (app_data_dir.empty()) {
    return;
  }
#if defined(_WIN32) || defined(_WIN64)
  std::string db_filepathname = app_data_dir + "\\anxi\\" + db_name;
#else
  std::string db_filepathname = app_data_dir + "/anxi/" + db_name;
#endif
  if (anx::common::FileExists(db_filepathname)) {
    if (!anx::common::RemoveFile(db_filepathname)) {
      LOG_F(LG_ERROR) << "Failed to remove file: " << db_filepathname;
    }
  }
}

void InitializeDefaultDataBase() {
  std::vector<std::string> sql;
  sql.push_back(sql::kCreateTableExpDataGraphSqlFormat);
  InitializeDataBase(kDefaultDatabasePathname, sql);
}

bool InitializeDataBase(const std::string& db_pathname,
                        const std::vector<std::string>& sql) {
  if (db_pathname.empty() || sql.empty()) {
    LOG_F(LG_ERROR) << "db_pathname or sql is empty";
    return false;
  }
  std::string db_filepathname;
  DefaultDatabasePathname(&db_filepathname);
  if (!anx::common::MakeSureFolderPathExist(db_filepathname)) {
    LOG_F(LG_ERROR) << "Failed to make sure folder path exist: "
                    << db_filepathname;
    return false;
  }
  auto db = DatabaseFactory::Instance()->CreateOrGetDatabase(db_filepathname);
  if (db) {
    for (auto& s : sql) {
      if (!db->Execute(s)) {
        LOG_F(LG_ERROR) << "Failed to execute sql: " << s;
        return false;
      }
    }
  }
  return true;
}

bool QueryDataBase(const std::string& db_name,
                   const std::string& table,
                   const std::string& sql,
                   std::vector<std::map<std::string, std::string>>* result) {
  std::string db_filepathname;
  DefaultDatabasePathname(&db_filepathname);
  auto db = DatabaseFactory::Instance()->CreateOrGetDatabase(db_filepathname);
  if (db) {
    return db->Query(sql, result);
  }
  return false;
}

bool ExecuteDataBase(const std::string& db_name, const std::string& sql) {
  auto db = DatabaseFactory::Instance()->CreateOrGetDatabase(db_name);
  if (db) {
    return db->Execute(sql);
  }
  return false;
}

bool InsertDataTable(const std::string& db_name,
                     const std::string& table,
                     const std::string& sql) {
  std::string db_filepathname;
  DefaultDatabasePathname(&db_filepathname);
  auto db = DatabaseFactory::Instance()->CreateOrGetDatabase(db_filepathname);
  if (db) {
    return db->Execute(sql);
  }
  return false;
}

bool DropDataTable(const std::string& db_name, const std::string& table) {
  std::string db_filepathname;
  DefaultDatabasePathname(&db_filepathname);
  auto db = DatabaseFactory::Instance()->CreateOrGetDatabase(db_filepathname);
  if (db) {
    std::string sql = "DROP TABLE " + table;
    return db->Execute(sql);
  }
  return false;
}

void CloseDataBase(const std::string& db_name) {
  DatabaseFactory::Instance()->CloseDatabase(db_name);
}

}  // namespace helper
}  // namespace db
}  // namespace anx
