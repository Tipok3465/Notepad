#include "Front.h"

Window::Window(int width, int height) : windowwidth_(width), windowheight_(height) {
  window_ = new QMainWindow();
  window_->setGeometry(200, 400, windowwidth_, windowheight_);
  window_->setFixedSize(windowwidth_, windowheight_);
  window_->setWindowTitle("Notepad++");

  basebuttons_ = new QWidget(window_);
  basebuttons_->setGeometry(0, int(windowheight_ * 0.05), int(windowwidth_ * 0.25), int(windowheight_ * 0.60));

  algobuttons_ = new QWidget(window_);
  algobuttons_->setGeometry(int(windowwidth_ * 0.81) - 5, int(windowheight_ * 0.05), int(windowwidth_ * 0.19), int(windowheight_ * 0.45));

  windowtext_ = new QTextBrowser(window_);
  windowtext_->setGeometry(int(windowwidth_ * 0.25) + 5, int(windowheight_ * 0.05) + 10, int(windowwidth_ * 0.55), int(windowheight_ * 0.60));

  inputstrings_ = new QTextEdit(window_);
  inputstrings_->setGeometry(0, int(windowheight_ * 0.8), int(windowwidth_), int(windowheight_ * 0.15));
  inputstrings_->setPlaceholderText("STRINGS");
  inputnums_ = new QTextEdit(window_);
  inputnums_->setGeometry(0, int(windowheight_ * 0.75), int(windowwidth_), int(windowheight_ * 0.03));
  inputnums_->setPlaceholderText("IDS");

  MakeButtons();
}

void Window::MakeButtons() {
  savingbutton_ = new QPushButton(window_);
  savingbutton_->setGeometry(int(windowwidth_ * 0.1), 5, int(windowwidth_ * 0.1), int(windowheight_ * 0.05));
  savingbutton_->setText("Save as..");
  connect(savingbutton_, SIGNAL (released()), this, SLOT (SavingToFile()));

  loadingbutton_ = new QPushButton(window_);
  loadingbutton_->setGeometry(0, 5, int(windowwidth_ * 0.1), int(windowheight_ * 0.05));
  loadingbutton_->setText("Load from..");
  connect(loadingbutton_, SIGNAL (released()), this, SLOT (LoadingFromFile()));

  buttons_.resize(10);
  for (int i = 0; i < 6; ++i) {
    buttons_[i] = new QPushButton(basebuttons_);
    buttons_[i]->setGeometry(0,  basebuttons_->height() * 0.10 * i + 15 * (i + 1),
     basebuttons_->width(), basebuttons_->height() * 0.10);
  }

  buttons_[0]->setText("Insert string");
  connect(buttons_[0], SIGNAL (released()), this, SLOT (InsertOneString()));

  buttons_[1]->setText("Insert some strings");
  connect(buttons_[1], SIGNAL (released()), this, SLOT (InsertManyString()));

  buttons_[2]->setText("Delete string");
  connect(buttons_[2], SIGNAL (released()), this, SLOT (DeleteString()));

  buttons_[3]->setText("Replace Char");
  connect(buttons_[3], SIGNAL (released()), this, SLOT (ReplaceChar()));

  buttons_[4]->setText("Insert substring");
  connect(buttons_[4], SIGNAL (released()), this, SLOT (InsertSubString()));

  buttons_[5]->setText("Replace substring");
  connect(buttons_[5], SIGNAL (released()), this, SLOT (DeleteSubString()));

  for (int i = 6; i < 10; ++i) {
    buttons_[i] = new QPushButton(algobuttons_);
    buttons_[i]->setGeometry(0, algobuttons_->height() * 0.25 * (i - 6) + 10 * (i - 5),
      algobuttons_->width(), algobuttons_->height() * 0.15);
  }

  buttons_[6]->setText("Delete zeros");
  connect(buttons_[6], SIGNAL (released()), this, SLOT (DeleteZero()));\
  
  buttons_[7]->setText("Delete no growing");
  connect(buttons_[7], SIGNAL (released()), this, SLOT (DeleteNoGrowingNums()));

  buttons_[8]->setText("Replace k* to k/2+");
  connect(buttons_[8], SIGNAL (released()), this, SLOT (ReplaceChars()));

  buttons_[9]->setText("Delete all in brackets");
  connect(buttons_[9], SIGNAL (released()), this, SLOT (DeleteInBrackets()));

}


void Window::Display() {
  window_->show();
}

void Window::DisplayText() {
  QString str;
  for (std::wstring* line : lines_) {
    std::wstring s = *line;
    str.append(QString::fromWCharArray(line->c_str()));
  }
  windowtext_->setText(str);
}

void Window::InsertOneString() {
  QString data = inputstrings_->toPlainText();
  inputstrings_->clear();
  QString datanums = inputnums_->toPlainText();
  inputnums_->clear();
  int n = 0;
  if (data.isEmpty()) return;
  if (!datanums.isEmpty()) {
    QStringList s = datanums.split(QRegularExpression(" "), Qt::SkipEmptyParts);
    n = s[0].toInt();
  }
  QStringList text = data.split(QRegularExpression("\n"), Qt::SkipEmptyParts);
  std::wstring str = text[0].toStdWString();
  std::vector<std::wstring*> strs = Text::Lines(str);
  for (int j = 0; j < strs.size(); ++j) {
    lines_.insert(lines_.begin() + n + j, strs[j]);
  }

  DisplayText();
}

void Window::InsertManyString() {
  QString data = inputstrings_->toPlainText();
  inputstrings_->clear();
  QString datanums = inputnums_->toPlainText();
  inputnums_->clear();
  if (data.isEmpty()) return;
  QStringList text = data.split(QRegularExpression("\n"), Qt::SkipEmptyParts);
  int n = 0, m = 0;
  if (!datanums.isEmpty()) {
    QStringList s = datanums.split(QRegularExpression(" "), Qt::SkipEmptyParts);
    n = s[0].toInt();
    m = s[1].toInt();
  }
  --n;
  --m;
  for (int i = 0; i < m; ++i) {
    std::wstring str = text[i].toStdWString();
    std::vector<std::wstring*> strs = Text::Lines(str);
    int j;
    for (j = 0; j < strs.size(); ++j) {
      lines_.insert(lines_.begin() + n + j, strs[j]);
    }
    n += j;
  }
  DisplayText();
}

void Window::DeleteString() {
  QString data = inputstrings_->toPlainText();
  inputstrings_->clear();
  QString datanums = inputnums_->toPlainText();
  inputnums_->clear();
  QStringList text = datanums.split(QRegularExpression("\n"), Qt::SkipEmptyParts);
  int n = text[0].toInt();
  --n;
  if (n > lines_.size()) return;
  delete lines_[n];
  lines_.erase(lines_.begin() + n);
  DisplayText();
}

void Window::ReplaceChar() {
  QString data = inputstrings_->toPlainText();
  inputstrings_->clear();
  QString datanums = inputnums_->toPlainText();
  inputnums_->clear();
  if (data.isEmpty()) return;
  QStringList text = data.split(QRegularExpression("\n"), Qt::SkipEmptyParts);
  QStringList nums = datanums.split(QRegularExpression(" "), Qt::SkipEmptyParts);
  int n = 0, m = 0;
  n = nums[0].toInt();
  m = nums[1].toInt();
  --n;
  --m;
  if (n > lines_.size() || m > lines_[n]->size()) return;
  wchar_t rep = text[1].toStdWString()[0];
  int i = 0;
  for (wchar_t& c : *lines_[n]) {
    if (i == m) {
      c = rep;
      break;
    }
    ++i;
  }
  DisplayText();
}

void Window::InsertSubString() {
  QString data = inputstrings_->toPlainText();
  inputstrings_->clear();
  QString datanums = inputnums_->toPlainText();
  inputnums_->clear();
  if (data.isEmpty()) return;
  QStringList text = data.split(QRegularExpression("\n"), Qt::SkipEmptyParts);
  int n = 0, m = 0;
  QStringList s = datanums.split(QRegularExpression(" "), Qt::SkipEmptyParts);
  n = s[0].toInt();
  m = s[1].toInt();
  --n;
  --m;
  if (n > lines_.size()) return;
  std::wstring str = text[0].toStdWString();
  std::wstring target = *lines_[n];
  lines_.erase(lines_.begin() + n);
  const wchar_t* paste = str.c_str();
  target.insert(m, paste);
  std::vector<std::wstring*> strs = Text::Lines(target);
  for (int j = 0; j < strs.size(); ++j) {
    lines_.insert(lines_.begin() + n + j, strs[j]);
  }

  DisplayText();
}

void Window::DeleteSubString() {
  QString data = inputstrings_->toPlainText();
  inputstrings_->clear();
  QString datanums = inputnums_->toPlainText();
  inputnums_->clear();
  if (data.isEmpty()) return;
  QStringList text = data.split(QRegularExpression("\n"), Qt::SkipEmptyParts);

  int i = 0, n = 0, m = int(lines_.size());
  QStringList s = datanums.split(QRegularExpression(" "), Qt::SkipEmptyParts);
  n = s[0].toInt();
  m = s[1].toInt();
  --n;
  m = std::min(int(lines_.size()), m);
  QStringList text1 = text[0].split(QRegularExpression(" "), Qt::SkipEmptyParts);
  std::wstring from = text1[0].toStdWString(), to = text1[1].toStdWString();
  for (int j = n; j < m;) {
    std::vector<std::wstring*> strs = Text::Replace(*lines_[j], from, to);
    delete lines_[j];
    lines_.erase(lines_.begin() + j);
    m += int(strs.size()) - 1;
    for (int i = j; i < strs.size() + j; ++i) {
      lines_.insert(lines_.begin() + i, strs[i - j]);
    }
    j += int(strs.size());
  }

  DisplayText();
}

void Window::DeleteZero() {
  QString data = inputstrings_->toPlainText();
  inputstrings_->clear();
  QString datanums = inputnums_->toPlainText();
  inputnums_->clear();
  int n = 0, m = int(lines_.size());
  QStringList s = datanums.split(QRegularExpression(" "), Qt::SkipEmptyParts);
  n = s[0].toInt();
  m = s[1].toInt();
  --n;
  m = std::min(int(lines_.size()), m);
  for (int i = n; i < m; ++i) {
    Text::DeleteZero(*lines_[i]);
  }

  DisplayText();
}

void Window::DeleteNoGrowingNums() {
  QString data = inputstrings_->toPlainText();
  inputstrings_->clear();
  QString datanums = inputnums_->toPlainText();
  inputnums_->clear();
  int n = 0, m = int(lines_.size());
  QStringList s = datanums.split(QRegularExpression(" "), Qt::SkipEmptyParts);
  n = s[0].toInt();
  m = s[1].toInt();
  --n;
  m = std::min(int(lines_.size()), m);

  for (int i = n; i < m; ++i) {
    Text::DeleteNoGrowingNums(*lines_[i]);
  }

  DisplayText();
}

void Window::ReplaceChars() {
  QString data = inputstrings_->toPlainText();
  inputstrings_->clear();
  QString datanums = inputnums_->toPlainText();
  inputnums_->clear();
  int n = 0, m = int(lines_.size());
  QStringList s = datanums.split(QRegularExpression(" "), Qt::SkipEmptyParts);
  n = s[0].toInt();
  m = s[1].toInt();
  --n;
  m = std::min(int(lines_.size()), m);

  for (int i = n; i < m; ++i) {
    Text::ReplaceChars(*lines_[i]);
  }

  DisplayText();
}

void Window::DeleteInBrackets() {
  QString data = inputstrings_->toPlainText();
  inputstrings_->clear();
  QString datanums = inputnums_->toPlainText();
  inputnums_->clear();
  int n = 0, m = int(lines_.size());
  QStringList s = datanums.split(QRegularExpression(" "), Qt::SkipEmptyParts);
  n = s[0].toInt();
  m = s[1].toInt();
  --n;
  m = std::min(int(lines_.size()), m);

  for (int i = n; i < m; ++i) {
    Text::DeleteInBrackets(*lines_[i]);
  }

  DisplayText();
}

void Window::SavingToFile() {
  QString file = QFileDialog::getSaveFileName(nullptr, "Сохранить", "", "*.txt");
  QFile outFile(file);
  if (!outFile.open(QIODevice::WriteOnly)) return;
  QTextStream ostream(&outFile);

  for (std::wstring* line : lines_) {
    ostream << QString::fromStdWString(*line);
  }
  outFile.close();
}

void Window::LoadingFromFile() {
  for (auto& line : lines_) {
    delete line;
  }
  lines_.resize(0);

  QString file = QFileDialog::getOpenFileName(nullptr, "Выбрать файл", "", "*.txt");
  QFile in(file);
  if (!in.open(QIODevice::ReadOnly)) return;
  QString data = in.readAll();
  in.close();

  if (data.isEmpty()) return;
  QStringList text = data.split(QRegularExpression("[\n]"), Qt::SkipEmptyParts);
  for (const auto& i : text) {
    std::wstring line = i.toStdWString();
    std::vector<std::wstring*> lines = Text::Lines(line);
    for (auto s : lines) {
      lines_.push_back(s);
    }
  }

  DisplayText();
}
