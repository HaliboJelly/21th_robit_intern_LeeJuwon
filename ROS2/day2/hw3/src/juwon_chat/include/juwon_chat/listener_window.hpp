#ifndef juwon_chat_LISTENER_WINDOW_H
#define juwon_chat_LISTENER_WINDOW_H

#include <QMainWindow>
#include "listener_qnode.hpp"
#include "ui_listener.h"

class ListenerWindow : public QMainWindow
{
  Q_OBJECT

public:
  ListenerWindow(QWidget* parent = nullptr);
  ~ListenerWindow();
  ListenerNode* qnode;

private Q_SLOTS:
  void updateRecvLabel(QString text);

private:
  Ui::ListenerDesign* ui;
  void closeEvent(QCloseEvent* event);
};

#endif