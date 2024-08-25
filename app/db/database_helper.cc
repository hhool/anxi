/**
 * @file database_helper.cc
 * @author hhool (hhool@outlook.com)
 * @brief  sqlite3 database helper class
 * @version 0.1
 * @date 2024-08-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/db/database_helper.h"

#include <sqlite3.h>

namespace anx {
namespace db {
DatabaseHelper::DatabaseHelper() {}

DatabaseHelper::~DatabaseHelper() {}

int32_t DatabaseHelper::Initlize() {
  return 0;
}

int32_t DatabaseHelper::UnInitlize() {
  return 0;
}
}  // namespace common
}  // namespace anx
