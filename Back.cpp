#include "Back.h"

std::vector<std::wstring*> Text::Lines(std::wstring &str) {
  std::vector<std::wstring*> lines;
  int size = int(str.size()) - (str[str.size() - 1] == '\n'), n = 0;
  while (size > 0) {
    std::wstring s = std::wstring(str.begin() + 70 * n, str.begin() + 70 * n + std::min(70, size)) + wchar_t('\n');
    lines.push_back(new std::wstring(s));
    ++n;
    size -= 70;
  }
  return lines;
}

std::vector<std::wstring*> Text::Replace(std::wstring& str, std::wstring& from, std::wstring& to) {
  int start = 0, idx = 0;
  for (int i = 0; i < str.size(); ++i) {
    if (str[i] == from[idx]) {
      if (idx == 0) start = i;
      ++idx;
      if (idx == from.size()) {
        str.erase(start, from.size());
        str.insert(start, to);
        i = start + int(to.size()) - 2;
      }
    } else {
      idx = 0;
    }
  }
  std::vector<std::wstring*> strs = Lines(str);
  return strs;
}

void Text::DeleteZero(std::wstring& str) {
  bool remove = true;
  for (int i = 0; i < str.size();) {
    if (str[i] == '0') {
      if (i == str.size() - 1 || !isNumber(str[i + 1]) || !remove) {
        ++i;
        continue;
      }
      str.erase(str.begin() + i);
      continue;
    }

    if ('0' < str[i] && str[i] <= '9') {
      remove = false;
    } else {
      remove = true;
    }

    ++i;
  }
}

void Text::DeleteInBrackets(std::wstring &str) {
  int start = 0;
  for (int i = 0; i < str.size(); ++i) {
    if (str[i] == '{') {
      start = i;
    }
    if (str[i] == '}') {
      int len = i - start;
      i -= len;
      str.erase(start, len + 1);
    }
  }
}

void Text::DeleteNoGrowingNums(std::wstring &str) {
  int start = !isNumber(str[0]), len = isNumber(str[0]);
  bool remove = false;

  for (int i = 1; i < str.size(); ++i) {
    if (isNumber(str[i])) {
      if (len == 0) {
        start = i;
      }
      if (isNumber(str[i - 1]) && str[i - 1] >= str[i]) {
        remove = true;
      }
      ++len;
    } else {
      if (remove) {
        i -= len;
        str.erase(start, len);
        remove = false;
      }
      len = 0;
    }
  }
}

void Text::ReplaceChars(std::wstring &str) {
  int n = 0, start = 0;
  for (int i = 0; i < str.size();) {
    if (str[i] == '*') {
      if (n == 0) start = i;
      ++n;
    } else {
      if (n < 2) {
        n = 0;
        ++i;
        continue;
      }
      i -= n;
      str.erase(start, n);
      str.insert(i, CPlus(n / 2));
      i += n / 2;
      n = 0;
    }
    ++i;
  }
}
