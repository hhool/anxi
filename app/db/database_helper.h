/**
 * @file database_helper.h
 * @author hhool (hhool@outlook.com)
 * @brief  sqlite3 database helper class
 * @version 0.1
 * @date 2024-08-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_DB_DATABASE_HELPER_H_
#define APP_DB_DATABASE_HELPER_H_

#include "app/db/database.h"

#include <string>
#include <vector>

namespace anx {
namespace db {
/// @brief sqlite3 database helper class

class DatabaseHelper {
 public:
  /// @brief Constructor
  DatabaseHelper();

  /// @brief Destructor
  ~DatabaseHelper();

 public:
  static int32_t Initlize();
  static int32_t UnInitlize();

};
}  // namespace db
}  // namespace anx

#endif  // APP_DB_DATABASE_HELPER_H_
