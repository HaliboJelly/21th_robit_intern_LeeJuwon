#ifndef juwon_chat_TALKER_WINDOW_H
#define juwon_chat_TALKER_WINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "talker_qnode.hpp"
#include "ui_talker.h"

class TalkerWindow : public QMainWindow
{
  Q_OBJECT

public:
  TalkerWindow(QWidget* parent = nullptr);
  ~TalkerWindow();
  TalkerNode* qnode;

private:
  Ui::TalkerDesign* ui;
  void closeEvent(QCloseEvent* event);
};

#endif