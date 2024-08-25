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

#include <string>
#include <vector>

namespace anx {
namespace db {
/// @brief sqlite3 database helper class
class Database {
 public:
  /// @brief Constructor
  Database();

  /// @brief Destructor
  ~Database();

  /// @brief Open the database
  /// @param db_name the database name
  /// @return true if open success
  bool Open(const std::string& db_name);

  /// @brief Close the database
  void Close();

  /// @brief Execute the sql
  /// @param sql the sql
  /// @return true if execute success
  bool Execute(const std::string& sql);

  /// @brief Query the sql
  /// @param sql the sql
  /// @param result the result
  /// @return true if query success
  bool Query(const std::string& sql,
             std::vector<std::vector<std::string>>* result);

 private:
  /// @brief the sqlite3 database
  void* db_;
};
}  // namespace db
}  // namespace anx

#endif  // APP_DB_DATABASE_H_
