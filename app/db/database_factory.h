/**
 * @file database_factory.h
 * @author hhool (hhool@outlook.com)
 * @brief  sqlite3 database helper class
 * @version 0.1
 * @date 2024-08-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_DB_DATABASE_FACTORY_H_
#define APP_DB_DATABASE_FACTORY_H_

#include <map>
#include <memory>
#include <string>

#include "app/db/database.h"

namespace anx {
namespace db {
/// @brief sqlite3 database helper class

class DatabaseFactory {
 public:
  /// @brief Constructor
  DatabaseFactory();

  /// @brief Destructor
  ~DatabaseFactory();

 public:
  /// @brief Create the database
  /// @param db_name the database name
  /// @return the database
  std::shared_ptr<DatabaseInterface> CreateOrGetDatabase(
      const std::string& db_name);

 private:
  /// @brief the databases
  std::map<std::string, std::shared_ptr<DatabaseInterface>> databases_;
};


}  // namespace db
}  // namespace anx

#endif  // APP_DB_DATABASE_FACTORY_H_
