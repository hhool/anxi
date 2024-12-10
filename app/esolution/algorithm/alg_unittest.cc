/**
 * @file alg_unittest.cc
 * @author hhool (hhool@outlook.com)
 * @brief  Algorithm class unit test file
 * @version 0.1
 * @date 2024-09-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "gtest/gtest.h"

#include "app/esolution/algorithm/alg.h"

namespace anx {
namespace esolution {
namespace algorithm {

TEST(AlgTest, FitLine) {
  float x[] = {1, 2, 3, 4, 5};
  float y[] = {1, 2, 3, 4, 5};

  float a, b;
  LineFit(x, y, 5, &a, &b);
  EXPECT_NEAR(a, 1.0, 0.0001);
  EXPECT_NEAR(b, 0.0, 0.0001);
}

TEST(AlgTest, FitLine2) {
  float x[] = {1, 2, 3, 4, 5};
  float y[] = {2, 3, 4, 5, 6};

  float a, b;
  LineFit(x, y, 5, &a, &b);
  EXPECT_NEAR(a, 1.0, 0.0001);
  EXPECT_NEAR(b, 1.0, 0.0001);
}

TEST(AlgTest, FitLine3) {
  float x[] = {30, 38, 47, 54};
  float y[] = {20, 40, 60, 80};

  float a, b;
  LineFit(x, y, 4, &a, &b);
  EXPECT_NEAR(a, 1.0, 0.0001);
  EXPECT_NEAR(b, -20.0, 0.0001);
  float x5 = 28.34f * 2.0f;
  float y5 = a * x5 + b;
  int32_t y5_i = static_cast<int32_t>(y5);
  EXPECT_NEAR(y5, 25.0, 0.0001);
}

TEST(AlgTest, FitLine4) {
  float x[] = {37.8f, 46.8f, 55.6f, 63.f};
  float y[] = {20, 40, 60, 80};

  float a, b;
  LineFit(x, y, 4, &a, &b);
  EXPECT_NEAR(a, 1.0, 0.0001);
  EXPECT_NEAR(b, -20.0, 0.0001);
  float x5 = 28.34f * 2.0f;
  float y5 = a * x5 + b;
  int32_t y5_i = static_cast<int32_t>(y5);
  EXPECT_NEAR(y5, 25.0, 0.0001);
}

TEST(AlgTest, CalcLengthOfTh3Design) {
  double E = 116;                            // 10^9 MPa
  double h = 4;                              // height mm
  double rho = 4379;                         // rho kg/m3
  int f = 20;                                // freq 20 khz
  double W = 10;                             // width
  double t = 4;                              // height
  double cval = kConstForLenghtOfTh3Design;  // const value for length
  double result = CalcTh3Design(E, h, rho, f, W, t, cval);
  EXPECT_NEAR(result, 31.70, 0.000001);
}

TEST(AlgTest, CalcSpanOfTh3Design) {
  double E = 116;                             // 10^9 MPa
  double h = 4;                               // height mm
  double rho = 4379;                          // rho kg/m3
  int f = 20;                                 // freq 20 khz
  double W = 10;                              // width
  double t = 4;                               // height
  double cval = kConstForLength0OfTh3Design;  // const value for length
  double result = CalcTh3Design(E, h, rho, f, W, t, cval);
  EXPECT_NEAR(result, 17.23, 0.000001);
}

TEST(AlgTest, StringToDouble) {
  const wchar_t str[] = L"1.001";
  double result;
  double expect_result = 1.001f;
  /// convert string to double value with 4 decimal places
  result = std::stod(str);
  EXPECT_NEAR(result, 1.001, 0.0001);
  /// convert string to double value with 3 decimal places,
  /// double value value is 1.001
  std::wstringstream wss;
  wss << std::setprecision(3) << std::fixed << result;
  std::wstring ws = wss.str();
  result = std::stod(ws);
  EXPECT_NEAR(result, 1.001, 0.001);
  /// convert string to double value with 3 decimal places,
  /// double string to double value with 3 decimal places.
  /// not use method std::stod directly to convert string to double value
  /// with 3 decimal places directly because of the precision problem.
  /// str is value is L"1.001" and expect convert to double value is 1.001
  /// not 1.0009999999999999
  std::wstringstream wss2;
  wss2 << std::setprecision(3) << std::fixed << expect_result;
  std::wstring ws2 = wss2.str();
  result = std::stod(ws2);
  result = std::round(result * 10000) / 10000;
  EXPECT_NEAR(result, 1.001, 0.001);
  /// convert string to double value with 3 decimal places,
  /// eg: wchar_t* str = L"0.001", convert to double value is 0.001
  /// with 3 decimal places.
  /// wchar_t* str = L"0.001", convert to double value is 0.001
  /// wchar_t* str = L"0.002", convert to double value is 0.002
  /// wchar_t* str = L"0.003", convert to double value is 0.003
  /// wchar_t* str = L"0.004", convert to double value is 0.004
  /// .................
  /// wchar_t* str = L"0.999", convert to double value is 0.999
  wchar_t* str2 = L"0.001";
  result = std::stod(str2);
  result = std::round(result * 1000) / 1000;
  EXPECT_NEAR(result, 0.001, 0.001);
  wchar_t* str3 = L"0.002";
  result = std::stod(str3);
  result = std::round(result * 1000) / 1000;
  EXPECT_NEAR(result, 0.002, 0.001);
  wchar_t* str4 = L"0.003";
  result = std::stod(str4);
  result = std::round(result * 1000) / 1000;
  EXPECT_NEAR(result, 0.003, 0.001);
  wchar_t* str5 = L"0.004";
  result = std::stod(str5);
  result = std::round(result * 1000) / 1000;
  EXPECT_NEAR(result, 0.004, 0.001);
  wchar_t* str6 = L"0.999";
  result = std::stod(str6);
  result = std::round(result * 1000) / 1000;
  EXPECT_NEAR(result, 0.999, 0.001);
  wchar_t* str7 = L"1.001";
  result = std::stod(str7);
  result = std::round(result * 1000) / 1000;
  EXPECT_NEAR(result, 1.001, 0.001);
  wchar_t* str8 = L"1.01";
  result = std::stod(str8);
  result = std::round(result * 1000) / 1000;
  EXPECT_NEAR(result, 1.01, 0.001);
  wchar_t* str9 = L"1.1";
  result = std::stod(str9);
  result = std::round(result * 1000) / 1000;
  EXPECT_NEAR(result, 1.1, 0.001);
}

TEST(AlgTest, StringToDouble2) {
  /// wchar_t* array with 10000 elements
  wchar_t** str = new wchar_t*[30000];
  /// init it with L"0.001" L"0.002" L"0.003" L"0.004" L"0.005" L"0.006"
  /// L"0.007" L"0.008" L"0.009" L"0.010" until L"2.999"
  for (int i = 0; i < 30000; ++i) {
    str[i] = new wchar_t[10];
    std::swprintf(str[i], 10, L"%.3f", i / 1000.0);
  }
  /// convert wchar_t* array to double array
  double* result = new double[30000];
  for (int i = 0; i < 30000; ++i) {
    result[i] = std::stod(str[i]);
  }
  /// compare double array with expect value
  for (int i = 0; i < 30000; ++i) {
    EXPECT_NEAR(result[i], i / 1000.0, 0.001);
  }
  /// release memory
  for (int i = 0; i < 30000; ++i) {
    delete[] str[i];
  }
  delete[] str;
  delete[] result;
}

TEST(AlgTest, StringToDouble3) {
  /// wchar_t* array with 10000 elements
  wchar_t** str = new wchar_t*[30000];
  /// init it with L"0.001" L"0.002" L"0.003" L"0.004" L"0.005" L"0.006"
  /// L"0.007" L"0.008" L"0.009" L"0.010" until L"2.999"
  for (int i = 0; i < 30000; ++i) {
    str[i] = new wchar_t[10];
    std::swprintf(str[i], 10, L"%.4f", i / 10000.0);
  }
  /// convert wchar_t* array to double array
  /// expect double converted is near bigger value and less than 0.001
  double* result = new double[30000];
  for (int i = 0; i < 30000; ++i) {
    result[i] = std::stod(str[i]);
    /// split str[i] to integer and decimal part
    std::wstring ws = str[i];
    std::size_t pos = ws.find(L".");
    std::wstring int_part = ws.substr(0, pos);
    std::wstring decimal_part = ws.substr(pos + 1);
    /// get decimal part length
    int decimal_length = decimal_part.length();
    /// get decimal part value
    double decimal_value = std::stod(decimal_part);
    std::wstring str_decimal = std::to_wstring(decimal_value);
    /// get decimal part value with 3 decimal places
    double decimal_value_3 = std::round(decimal_value * 1000) / 1000;
    std::wstring str_decimal_3 = std::to_wstring(decimal_value_3);
    /// get decimal part value with 2 decimal places
    double decimal_value_2 = std::round(decimal_value * 100) / 100;
    std::wstring str_decimal_2 = std::to_wstring(decimal_value_2);
    /// get decimal part value with 1 decimal places
    double decimal_value_1 = std::round(decimal_value * 10) / 10;
    std::wstring str_decimal_1 = std::to_wstring(decimal_value_1);
    /// get string value only 3 decimal places expect value "0" "00" "000"
    EXPECT_EQ(str_decimal, str_decimal_3);
    EXPECT_EQ(str_decimal, str_decimal_2);
    EXPECT_EQ(str_decimal, str_decimal_1);
    double res = result[i] * 1000;
    ///////////////////////////////////////////////////////
    /// convert result[i] to string with 3 decimal places
    wchar_t* temp = str[i];
    std::wstringstream wss;
    wss << std::setprecision(3) << std::fixed << result[i];
    std::wstring ws2 = wss.str();
  }

  /// compare double array with expect value
  for (int i = 0; i < 30000; ++i) {
    EXPECT_NEAR(result[i], i / 1000.0, 0.001);
  }
  /// release memory
  for (int i = 0; i < 30000; ++i) {
    delete[] str[i];
  }
  delete[] str;
  delete[] result;
}
}  // namespace algorithm
}  // namespace esolution
}  // namespace anx
