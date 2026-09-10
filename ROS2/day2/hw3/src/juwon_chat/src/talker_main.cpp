#include <QApplication>
#include "../include/juwon_chat/talker_window.hpp"

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  TalkerWindow w;
  w.show();
  return a.exec();
}