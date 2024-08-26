/**
 * @file database_factory.cc
 * @author hhool (hhool@outlook.com)
 * @brief  sqlite3 database helper class
 * @version 0.1
 * @date 2024-08-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "app/db/database_factory.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "app/db/database.h"
#include "app/db/database_impl.h"

namespace anx {
namespace db {
DatabaseFactory::DatabaseFactory() {}

DatabaseFactory::~DatabaseFactory() {}

std::shared_ptr<DatabaseInterface> DatabaseFactory::CreateOrGetDatabase(
    const std::string& db_name) {
  auto iter = databases_.find(db_name);
  if (iter != databases_.end()) {
    return iter->second;
  }

  auto db = std::make_shared<Database>();
  if (db->Open(db_name)) {
    databases_.insert(std::make_pair(db_name, db));
    return db;
  }

  return nullptr;
}

}  // namespace db
}  // namespace anx
