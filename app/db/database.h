/**
 * @file database.h
 * @author hhool (hhool@outlook.com)
 * @brief sqlite3 database helper class
 * @note record exp data amp and static load to sqlite3 database.
 * @version 0.1
 * @date 2024-08-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_DB_DATABASE_H_
#define APP_DB_DATABASE_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace anx {
namespace db {

extern const char* kDefaultDatabaseName;

/// @brief sqlite3 database helper class
class DatabaseInterface {
 public:
  /// @brief Open the database
  /// @param db_name the database name
  /// @return true if open success
  virtual bool Open(const std::string& db_name) = 0;

  /// @brief Close the database
  virtual void Close() = 0;

  /// @brief Execute the sql
  /// @param sql the sql
  /// @return true if execute success
  virtual bool Execute(const std::string& sql) = 0;

  /// @brief Query the sql
  /// @param sql the sql
  /// @param result the result
  /// @return true if query success
  virtual bool Query(const std::string& sql,
                     std::vector<std::vector<std::string>>* result) = 0;
};
}  // namespace db
}  // namespace anx

#endif  // APP_DB_DATABASE_H_
