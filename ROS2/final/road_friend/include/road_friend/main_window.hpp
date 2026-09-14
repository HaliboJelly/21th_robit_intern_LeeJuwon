#ifndef road_friend_MAIN_WINDOW_H
#define road_friend_MAIN_WINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVector>
#include <QPoint>
#include "QIcon"
#include "qnode.hpp"
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();
  QNode* qnode;

private Q_SLOTS:
  void setLight(QString light);
  void setVehicles(QVector<QPoint> cars);
  void setPlayer(int row, int col, bool alive, int score);

private:
  Ui::MainWindowDesign* ui;
  void closeEvent(QCloseEvent* event);
};

#endif