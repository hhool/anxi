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
    "kHz REAL, MPa REAL, um REAL, date REAL)";
static const char* insert_sql_format =
    "INSERT INTO amp (cycle, kHz, MPa, um, date) VALUES (%d, %f, %f, %f, %f)";
static const char* query_sql_format_by_id_range =
    "SELECT * FROM amp WHERE id >= %d AND id <= %d";
static const char* query_sql_format_by_date_range =
    "SELECT * FROM amp WHERE date >= %f AND date <= %f";

/// @brief create table test with id auto increment and  cycle type uint64_t and
/// kHz type float and MPa type float and um type float and var date double type
/// @param
/// @param
TEST_F(DatabaseTest, TestExecute) {
  /// delelte the test.db if exist
  remove("test.db");
  Database db;
  EXPECT_TRUE(db.Open("test.db"));
  EXPECT_TRUE(db.Execute(create_table_sql));
  db.Close();
}

/// @brief insert test with id auto increment and  cycle type uint64_t and kHz
/// type float and MPa type float and um type float and time type milliseconds
TEST_F(DatabaseTest, TestInsert) {
  /// delelte the test.db if exist
  remove("test.db");
  Database db;
  EXPECT_TRUE(db.Open("test.db"));
  EXPECT_TRUE(db.Execute(create_table_sql));
  for (int i = 0; i < 100000; ++i) {
    std::string insert_sql =
        "INSERT INTO amp (cycle, kHz, MPa, um, date) VALUES (1, 1.0, 1.0, 1.0, "
        "1.0)";
    EXPECT_TRUE(db.Execute(insert_sql));
  }
  db.Close();
}

/// @brief query test
TEST_F(DatabaseTest, TestQuery) {
  /// delelte the test.db if exist
  Database db;
  EXPECT_TRUE(db.Open("test.db"));
  EXPECT_TRUE(db.Execute(create_table_sql));
  std::vector<std::vector<std::string>> result;
  EXPECT_TRUE(db.Query("SELECT * FROM amp", &result));
  db.Close();
}

/// @brief query test order by id from 0 to 100 and limit 10
TEST_F(DatabaseTest, TestQueryOrderBy) {
  Database db;
  EXPECT_TRUE(db.Open("test.db"));
  std::vector<std::vector<std::string>> result;
  EXPECT_TRUE(db.Query("SELECT * FROM amp ORDER BY id LIMIT 10", &result));
  db.Close();
}

}  // namespace common
}  // namespace anx
