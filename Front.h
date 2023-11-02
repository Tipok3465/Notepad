#pragma once
#include "Back.h"

class Window : public QMainWindow {
  Q_OBJECT
 public:
  Window(int width, int height);
  void Display();
 private slots:
  void InsertOneString();
  void InsertManyString();
  void DeleteString();
  void ReplaceChar();
  void InsertSubString();
  void DeleteSubString();
  void DeleteZero();
  void DeleteNoGrowingNums();
  void ReplaceChars();
  void DeleteInBrackets();
  void SavingToFile();
  void LoadingFromFile();
 private:
  int windowwidth_, windowheight_;
  QWidget* window_;
  QTextBrowser* windowtext_;
  QTextEdit* inputstrings_;
  QTextEdit* inputnums_;
  std::vector<std::wstring*> lines_;
  QWidget* basebuttons_;
  QWidget* algobuttons_;
  std::vector<QPushButton*> buttons_;
  QPushButton* savingbutton_;
  QPushButton* loadingbutton_;
  void MakeButtons();
  void DisplayText();
};