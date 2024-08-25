/**
 * @file sqlite_unittest.cc
 * @author hhool (hhool@outlook.com)
 * @brief just a test file
 * @version 0.1
 * @date 2024-08-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <gtest/gtest.h>

#include <iostream>

#include <sqlite3.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

#if defined(_DEBUG) && defined(_MSC_VER) && defined(WIN32)
#include <crtdbg.h>
#endif

namespace {
int callback(void* NotUsed, int argc, char** argv, char** azColName) {
  int i;
  for (i = 0; i < argc; i++) {
    std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL")
              << std::endl;
  }
  std::cout << std::endl;
  return 0;
}
}  // namespace

TEST(SqliteTest, test) {
  sqlite3* db;
  char* zErrMsg = 0;
  int rc;

  rc = sqlite3_open("test.db", &db);
  if (rc) {
    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    return;
  } else {
    std::cout << "Opened database successfully" << std::endl;
  }

  const char* sql =
      "CREATE TABLE COMPANY("
      "ID INT PRIMARY KEY     NOT NULL,"
      "NAME           TEXT    NOT NULL,"
      "AGE            INT     NOT NULL,"
      "ADDRESS        CHAR(50),"
      "SALARY         REAL );";
  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " << zErrMsg << std::endl;
    sqlite3_free(zErrMsg);
  } else {
    std::cout << "Table created successfully" << std::endl;
  }

  sqlite3_close(db);
}

TEST(SqliteTest, insert) {
  sqlite3* db;
  char* zErrMsg = 0;
  int rc;

  rc = sqlite3_open("test.db", &db);
  if (rc) {
    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    return;
  } else {
    std::cout << "Opened database successfully" << std::endl;
  }

  const char* sql =
      "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
      "VALUES (1, 'Paul', 32, 'California', 20000.00 ); "
      "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
      "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "
      "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
      "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );"
      "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
      "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";
  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " << zErrMsg << std::endl;
    sqlite3_free(zErrMsg);
  } else {
    std::cout << "Records created successfully" << std::endl;
  }

  sqlite3_close(db);
}

TEST(SqliteTest, select) {
  sqlite3* db;
  char* zErrMsg = 0;
  int rc;
  const char* sql;
  const char* data = "Callback function called";

  rc = sqlite3_open("test.db", &db);
  if (rc) {
    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    return;
  } else {
    std::cout << "Opened database successfully" << std::endl;
  }

  sql = "SELECT * from COMPANY";

  rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " << zErrMsg << std::endl;
    sqlite3_free(zErrMsg);
  } else {
    std::cout << "Operation done successfully" << std::endl;
  }

  sqlite3_close(db);
}

TEST(SqliteTest, update) {
  sqlite3* db;
  char* zErrMsg = 0;
  int rc;
  const char* sql;

  rc = sqlite3_open("test.db", &db);
  if (rc) {
    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    return;
  } else {
    std::cout << "Opened database successfully" << std::endl;
  }

  sql =
      "UPDATE COMPANY set SALARY = 25000.00 where ID=1; "
      "SELECT * from COMPANY";

  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " << zErrMsg << std::endl;
    sqlite3_free(zErrMsg);
  } else {
    std::cout << "Operation done successfully" << std::endl;
  }

  sqlite3_close(db);
}

TEST(SqliteTest, delete) {
  sqlite3* db;
  char* zErrMsg = 0;
  int rc;
  const char* sql;

  rc = sqlite3_open("test.db", &db);
  if (rc) {
    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    return;
  } else {
    std::cout << "Opened database successfully" << std::endl;
  }

  sql =
      "DELETE from COMPANY where ID=2; "
      "SELECT * from COMPANY";

  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " << zErrMsg << std::endl;
    sqlite3_free(zErrMsg);
  } else {
    std::cout << "Operation done successfully" << std::endl;
  }

  sqlite3_close(db);
}

TEST(SqliteTest, transaction) {
  sqlite3* db;
  char* zErrMsg = 0;
  int rc;
  const char* sql;

  rc = sqlite3_open("test.db", &db);
  if (rc) {
    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    return;
  } else {
    std::cout << "Opened database successfully" << std::endl;
  }

  sql =
      "BEGIN TRANSACTION; "
      "UPDATE COMPANY set SALARY = 25000.00 where ID=1; "
      "UPDATE COMPANY set SALARY = 25000.00 where ID=2; "
      "COMMIT; "
      "SELECT * from COMPANY";

  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " << zErrMsg << std::endl;
    sqlite3_free(zErrMsg);
  } else {
    std::cout << "Operation done successfully" << std::endl;
  }

  sqlite3_close(db);
}

TEST(SqliteTest, rollback) {
  sqlite3* db;
  char* zErrMsg = 0;
  int rc;
  const char* sql;

  rc = sqlite3_open("test.db", &db);
  if (rc) {
    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    return;
  } else {
    std::cout << "Opened database successfully" << std::endl;
  }

  sql =
      "BEGIN TRANSACTION; "
      "UPDATE COMPANY set SALARY = 25000.00 where ID=1; "
      "UPDATE COMPANY set SALARY = 25000.00 where ID=2; "
      "ROLLBACK; "
      "SELECT * from COMPANY";

  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " << zErrMsg << std::endl;
    sqlite3_free(zErrMsg);
  } else {
    std::cout << "Operation done successfully" << std::endl;
  }

  sqlite3_close(db);
}

TEST(SqliteTest, query) {
  sqlite3* db;
  char* zErrMsg = 0;
  int rc;
  const char* sql;
  sqlite3_stmt* stmt;

  rc = sqlite3_open("test.db", &db);
  if (rc) {
    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    return;
  } else {
    std::cout << "Opened database successfully" << std::endl;
  }

  sql = "SELECT * from COMPANY";

  rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " << zErrMsg << std::endl;
    sqlite3_free(zErrMsg);
  } else {
    std::cout << "Operation done successfully" << std::endl;
  }

  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    int id = sqlite3_column_int(stmt, 0);
    const unsigned char* name = sqlite3_column_text(stmt, 1);
    int age = sqlite3_column_int(stmt, 2);
    const unsigned char* address = sqlite3_column_text(stmt, 3);
    double salary = sqlite3_column_double(stmt, 4);

    std::cout << "ID = " << id << std::endl;
    std::cout << "NAME = " << name << std::endl;
    std::cout << "AGE = " << age << std::endl;
    std::cout << "ADDRESS = " << address << std::endl;
    std::cout << "SALARY = " << salary << std::endl;
  }

  sqlite3_finalize(stmt);
  sqlite3_close(db);
}

class SqliteExpTest : public testing::Test {
 protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(SqliteExpTest, test) {}

int main(int argc, char** argv) {
#if defined(_DEBUG) && defined(_MSC_VER) && defined(WIN32)
  int Flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
  Flag |= _CRTDBG_LEAK_CHECK_DF;
  _CrtSetDbgFlag(Flag);
  // _CrtSetBreakAlloc(1940);
#endif
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
