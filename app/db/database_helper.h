/**
 * @file database_helper.h
 * @author hhool (hhool@outlook.com)
 * @brief  database helper class for exp data amp and static load to sqlite3
 * database.
 * @version 0.1
 * @date 2024-08-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_DB_DATABASE_HELPER_H_
#define APP_DB_DATABASE_HELPER_H_

#include <map>
#include <string>
#include <vector>

#include "app/db/database.h"
#include "app/db/database_factory.h"
#include "app/db/database_impl.h"

namespace anx {
namespace db {
namespace helper {

extern const char* kDefaultDatabasePathname;
extern const char* kTableExpDataGraph;
extern const char* kTableExpDataList;
extern const char* kTableSendData;
extern const char* kTableNotification;
extern const char* kTableSendNotify;

namespace sql {
extern const char* kCreateTableExpDataGraphSqlFormat;
extern const char* kInsertTableExpDataGraphSqlFormat;
extern const char* kQueryTableExpDataGraphSqlByIdFormat;
extern const char* kQueryTableExpDataGraphSqlByTimeFormat;

extern const char* kCreateTableExpDataListSqlFormat;
extern const char* kInsertTableExpDataListSqlFormat;
extern const char* kQueryTableExpDataListSqlByIdFormat;
extern const char* kQueryTableExpDataListSqlByTimeFormat;

extern const char* kCreateTableSendDataSqlFormat;
extern const char* kInsertTableSendDataSqlFormat;
extern const char* kQueryTableSendDataSqlByIdFormat;
extern const char* kQueryTableSendDataSqlByTimeFormat;

extern const char* kCreateTableNotificationSqlFormat;
extern const char* kInsertTableNotificationSqlFormat;
extern const char* kQueryTableNotificationSqlByIdFormat;
extern const char* kQueryTableNotificationSqlByTimeFormat;

extern const char* kCreateTableSendNotifySqlFormat;
extern const char* kInsertTableSendNotifySqlFormat;
extern const char* kQueryTableSendNotifySqlByIdFormat;
extern const char* kQueryTableSendNotifySqlByTimeFormat;

}  // namespace sql

/// @brief Get the default database pathname
/// @param db_pathname the database path name
/// @return 0 if success, -1 if failed
int32_t DefaultDatabasePathname(std::string* db_pathname);

/// @brief Clear the database file and remove it
/// @param db_name the database name
void ClearDatabaseFile(const std::string& db_name);

/// @brief Initialize the database
/// @param db_pathname the database path name, relative path of module path
/// @param sql the sql
/// @return true if success
bool InitializeDataBase(const std::string& db_pathname,
                        const std::vector<std::string>& sql);

/// @brief Query the database
/// @param db_name the database name
/// @param sql the sql
/// @param result the result
/// @return true if success
bool QueryDataBase(const std::string& db_name,
                   const std::string& table,
                   const std::string& sql,
                   std::vector<std::map<std::string, std::string>>* result);

/// @brief Execute the database
/// @param db_name the database name
/// @param sql the sql
/// @return true if success
bool ExecuteDataBase(const std::string& db_name, const std::string& sql);

/// @brief Insert the database table
/// @param table the table name in the database
/// @param sql the sql
/// @return true if success
bool InsertDataTable(const std::string& db_name,
                     const std::string& table,
                     const std::string& sql);

/// @brief Drop the database table
/// @param table the table name in the database
/// @return true if success
bool DropDataTable(const std::string& db_name, const std::string& table);

/// @brief Close the database
/// @param db_name the database name
void CloseDataBase(const std::string& db_name);

}  // namespace helper
}  // namespace db
}  // namespace anx

#endif  // APP_DB_DATABASE_HELPER_H_
