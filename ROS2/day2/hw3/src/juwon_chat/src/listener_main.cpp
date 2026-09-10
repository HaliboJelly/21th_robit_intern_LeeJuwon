#include <QApplication>
#include "../include/juwon_chat/listener_window.hpp"

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  ListenerWindow w;
  w.show();
  return a.exec();
}