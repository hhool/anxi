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

DatabaseFactory* DatabaseFactory::instance_ = nullptr;

DatabaseFactory* DatabaseFactory::Instance() {
  if (instance_ == nullptr) {
    instance_ = new DatabaseFactory();
  }
  return instance_;
}

void DatabaseFactory::ReleaseInstance() {
  if (instance_) {
    delete instance_;
    instance_ = nullptr;
  }
}
///////////////////////////////////////////////
/// @brief Constructor
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

void DatabaseFactory::CloseDatabase(const std::string& db_name) {
  auto iter = databases_.find(db_name);
  if (iter != databases_.end()) {
    iter->second->Close();
    databases_.erase(iter);
  }
}

void DatabaseFactory::CloseAllDatabase() {
  for (auto& iter : databases_) {
    iter.second->Close();
  }
  databases_.clear();
}

namespace Helper {}  // namespace Helper
}  // namespace db
}  // namespace anx
