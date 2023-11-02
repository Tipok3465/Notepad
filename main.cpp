#include "Front.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  Window Notepad(1600, 900);
  Notepad.Display();
  return QApplication::exec();
}