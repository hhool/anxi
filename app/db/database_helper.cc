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
const char* kTableExpData = "exp_data";
const char* kTableSendData = "send_data";
const char* kTableNotification = "notification";
const char* kTableSendNotify = "send_notify";

namespace sql {
///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Create table amp sql format string for exp data record
const char* kCreateTableAmpSqlFormat =
    "CREATE TABLE exp_data (id INTEGER PRIMARY KEY AUTOINCREMENT, cycle "
    "INTEGER, "
    "kHz REAL, MPa REAL, um REAL, date REAL)";

const char* kInsertTableAmpSqlFormat =
    "INSERT INTO exp_data (cycle, kHz, MPa, um, date) VALUES (%d, %f, %f, %f, "
    "%f)";

const char* kQueryTableAmpSqlByIdFormat =
    "SELECT * FROM exp_data WHERE id >= %d AND id <= %d";

const char* kQueryTableAmpSqlByTimeFormat =
    "SELECT * FROM exp_data WHERE date >= %f AND date <= %f";

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

void DefaultDatabasePathname(std::string* db_filepathname) {
  if (db_filepathname) {
    std::string module_dir = anx::common::GetModuleDir();
    if (module_dir.empty()) {
      return;
    }
#if defined(_WIN32) || defined(_WIN64)
    *db_filepathname = module_dir + "\\" + kDefaultDatabasePathname;
#else
    *db_filepathname = module_dir + "/" + kDefaultDatabasePathname;
#endif
  }
}

void ClearDatabaseFile(const std::string& db_name) {
  std::string module_dir = anx::common::GetModuleDir();
  if (module_dir.empty()) {
    return;
  }
#if defined(_WIN32) || defined(_WIN64)
  std::string db_filepathname = module_dir + "\\" + db_name;
#else
  std::string db_filepathname = module_dir + "/" + db_name;
#endif
  if (anx::common::FileExists(db_filepathname)) {
    if (!anx::common::RemoveFile(db_filepathname)) {
      LOG_F(LG_ERROR) << "Failed to remove file: " << db_filepathname;
    }
  }
}

void InitializeDefaultDataBase() {
  std::vector<std::string> sql;
  sql.push_back(sql::kCreateTableAmpSqlFormat);
  InitializeDataBase(kDefaultDatabasePathname, sql);
}

bool InitializeDataBase(const std::string& db_pathname,
                        const std::vector<std::string>& sql) {
  if (db_pathname.empty() || sql.empty()) {
    return false;
  }
  std::string db_filepathname;
  DefaultDatabasePathname(&db_filepathname);
  if (!anx::common::MakeSureFolderPathExist(db_filepathname)) {
    return false;
  }
  auto db = DatabaseFactory::Instance()->CreateOrGetDatabase(db_filepathname);
  if (db) {
    for (auto& s : sql) {
      if (!db->Execute(s)) {
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
