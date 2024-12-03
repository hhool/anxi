/**
 * @file database_unittest.cc
 * @author hhool (hhool@outlook.com)
 * @brief sqlite3 database helper class unit test
 * @version 0.1
 * @date 2024-08-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <gtest/gtest.h>

#include "app/db/database.h"

namespace anx {
namespace db {
class DatabaseTest : public ::testing::Test {
 protected:
  void SetUp() override {}
  void TearDown() override {}
};

static const char* create_table_sql =
    "CREATE TABLE amp (id INTEGER PRIMARY KEY AUTOINCREMENT, cycle INTEGER, "
    "kHz REAL, MPa REAL, μm REAL, date REAL)";
static const char* insert_sql_format =
    "INSERT INTO amp (cycle, kHz, MPa, μm, date) VALUES (%d, %f, %f, %f, %f)";
static const char* query_sql_format_by_id_range =
    "SELECT * FROM amp WHERE id >= %d AND id <= %d";
static const char* query_sql_format_by_date_range =
    "SELECT * FROM amp WHERE date >= %f AND date <= %f";


}  // namespace common
}  // namespace anx
