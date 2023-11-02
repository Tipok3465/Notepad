#pragma once
#include "header.h"


class Text {
 public:
  static std::vector<std::wstring*> Lines(std::wstring& str);
  static std::vector<std::wstring*> Replace(std::wstring& str, std::wstring& from, std::wstring& to);
  static void DeleteZero(std::wstring& str);
  static void DeleteInBrackets(std::wstring& str);
  static void DeleteNoGrowingNums(std::wstring& str);
  static void ReplaceChars(std::wstring& str);
 private:
  inline static bool isNumber(wchar_t c) {
    return c >= '0' && c <= '9';
  }

  static std::wstring CPlus(int n) {
    std::wstring s;
    while (n--) {
      s += '+';
    }
    return s;
  }
};