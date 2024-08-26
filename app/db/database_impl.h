/**
 * @file database_impl.h
 * @author hhool (hhool@outlook.com)
 * @brief sqlite3 database helper class
 * @note record exp data amp and static load to sqlite3 database.
 * @version 0.1
 * @date 2024-08-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_DB_DATABASE_IMPL_H_
#define APP_DB_DATABASE_IMPL_H_

#include <map>
#include <string>
#include <vector>

#include "app/db/database.h"

namespace anx {
namespace db {

namespace amp_sql {
extern const char* kCreateTableAmpSqlFormat;
extern const char* kInsertTableAmpSqlFormat;
extern const char* kQueryTableAmpSqlByIdFormat;
extern const char* kQueryTableAmpSqlByTimeFormat;
extern const char* kCreateTableStaticLoadSqlFormat;
}  // namespace amp_sql

/// @brief sqlite3 database helper class
class Database : public DatabaseInterface {
 public:
  /// @brief Constructor
  Database();

  /// @brief Destructor
  virtual ~Database();

  /// @brief Open the database
  /// @param db_name the database name
  /// @return true if open success
  bool Open(const std::string& db_name) override;

  /// @brief Close the database
  void Close() override;

  /// @brief Execute the sql
  /// @param sql the sql
  /// @return true if execute success
  bool Execute(const std::string& sql) override;

  /// @brief Query the sql
  /// @param sql the sql
  /// @param result the result
  /// @return true if query success
  bool Query(const std::string& sql,
             std::vector<std::vector<std::string>>* result) override;

 private:
  /// @brief the sqlite3 database
  void* db_;
};
}  // namespace db
}  // namespace anx

#endif  // APP_DB_DATABASE_IMPL_H_
