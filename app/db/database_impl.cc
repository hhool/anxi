/**
 * @file database_impl.cc
 * @author hhool (hhool@outlook.com)
 * @brief sqlite3 database helper class
 * @note record exp data amp and static load to sqlite3 database.
 * @version 0.1
 * @date 2024-08-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/db/database_impl.h"

#include <sqlite3.h>

namespace anx {
namespace db {

Database::Database() : db_(nullptr) {}

Database::~Database() {
  Close();
}

bool Database::Open(const std::string& db_name) {
  if (db_ != nullptr) {
    return false;
  }
  int ret = sqlite3_open(db_name.c_str(), reinterpret_cast<sqlite3**>(&db_));
  if (ret != SQLITE_OK) {
    return false;
  }
  return true;
}

void Database::Close() {
  if (db_ != nullptr) {
    sqlite3_close(reinterpret_cast<sqlite3*>(db_));
    db_ = nullptr;
  }
}

bool Database::Execute(const std::string& sql) {
  if (db_ == nullptr) {
    return false;
  }
  char* err_msg = nullptr;
  int ret = sqlite3_exec(reinterpret_cast<sqlite3*>(db_), sql.c_str(), nullptr,
                         nullptr, &err_msg);
  if (ret != SQLITE_OK) {
    sqlite3_free(err_msg);
    return false;
  }
  return true;
}

bool Database::Query(const std::string& sql,
                     std::vector<std::map<std::string, std::string>>* result) {
  if (db_ == nullptr) {
    return false;
  }
  char* err_msg = nullptr;
  int ret = sqlite3_exec(
      reinterpret_cast<sqlite3*>(db_), sql.c_str(),
      [](void* data, int col_count, char** col_values, char** col_names) {
        auto result =
            reinterpret_cast<std::vector<std::map<std::string, std::string>>*>(
                data);
        std::map<std::string, std::string> row;
        for (int i = 0; i < col_count; ++i) {
          row[col_names[i]] = col_values[i];
        }
        result->push_back(row);
        return 0;
      },
      result, &err_msg);
  if (ret != SQLITE_OK) {
    sqlite3_free(err_msg);
    return false;
  }
  return true;
}

}  // namespace db
}  // namespace anx
