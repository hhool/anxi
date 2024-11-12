/**
 * @file ui_num_string_convert.hpp
 * @author hhool (hhool@outlook.com)
 * @brief number and string convert implementation
 * @version 0.1
 * @date 2024-11-13
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef APP_UI_UI_NUM_STRING_CONVERT_HPP__
#define APP_UI_UI_NUM_STRING_CONVERT_HPP__

#include <iomanip>
#include <string>

#include "third_party\duilib\source\DuiLib\UIlib.h"

namespace anx {
namespace ui {

template <typename R>
R get_value_from_control(DuiLib::CControlUI* edit) {
  if (edit == nullptr) {
    return static_cast<R>(0);
  }
  DuiLib::CDuiString value = edit->GetText();
  if (value.IsEmpty()) {
    return static_cast<R>(0);
  }
  // check R type
  if (std::is_same<R, int>::value) {
    return static_cast<R>(_ttoi(value.GetData()));
  } else if (std::is_same<R, int64_t>::value) {
    return static_cast<R>(_ttoi64(value.GetData()));
  } else if (std::is_same<R, double>::value) {
    return static_cast<R>(_ttof(value.GetData()));
  } else {
    return static_cast<R>(_ttof(value.GetData()));
  }
}

template <typename T>
bool get_value_from_control(DuiLib::CControlUI* control, T* value) {
  if (control == nullptr || value == nullptr) {
    return false;
  }
  CDuiString text = control->GetText();
  if (text.IsEmpty()) {
    return false;
  }
  /// check T is not int and int64_t and double type
  if (!std::is_same<T, int>::value && !std::is_same<T, int64_t>::value &&
      !std::is_same<T, double>::value) {
    return false;
  }
  *value = get_value_from_control<T>(control);
  return true;
}

template <typename T>
bool set_value_to_control(DuiLib::CControlUI* control, T value) {
  if (control == nullptr) {
    return false;
  }
  /// check T is int or int64_t
  if (std::is_same<T, int>::value) {
    CDuiString text;
    text.Format(_T("%d"), value);
    control->SetText(text);
    return true;
  } else if (std::is_same<T, int64_t>::value) {
    CDuiString text;
    text.Format(_T("%lld"), value);
    control->SetText(text);
    return true;
  } else {
    return false;
  }
  return false;
}

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 2) {
  int nn = n;
  std::ostringstream out;
  out << std::fixed << std::setprecision(nn) << a_value;
  return out.str();
}

inline void set_value_to_edit(DuiLib::CControlUI* edit,
                              double value,
                              int32_t keep_count = 2) {
  DuiLib::CDuiString str;
  /// get integer part and decimal part, decimal part with keep_count
  /// decimal places, eg: 3.1415 -> 3.14 3.1414 -> 3.14 3.1416 -> 3.14
  /// 3.0001 -> 3.00 3.0009 -> 3.001 3.0000 -> 3 3.000 -> 3 3.001 -> 3.001
  /// 3.009 -> 3.009 3.01 -> 3.01 3.1 -> 3.1
  /// TODO(hhool): add test case
  /// get integer part and decimal part
  str.Format(_T("%.*f"), keep_count, value);
  edit->SetText(str);
}

inline void set_value_to_edit(DuiLib::CControlUI* edit, int value) {
  DuiLib::CDuiString str;
  str.Format(_T("%d"), value);
  edit->SetText(str);
}

inline void set_value_to_edit(DuiLib::CControlUI* edit, int64_t value) {
  DuiLib::CDuiString str;
  str.Format(_T("%lld"), value);
  edit->SetText(str);
}

}  // namespace ui
}  // namespace anx

#endif  // APP_UI_UI_NUM_STRING_CONVERT_HPP__
