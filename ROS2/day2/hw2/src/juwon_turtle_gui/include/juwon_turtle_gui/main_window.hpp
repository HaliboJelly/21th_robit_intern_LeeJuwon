/**
 * @file /include/juwon_turtle_gui/main_window.hpp
 *
 * @brief Qt based gui for %(package)s.
 *
 * @date August 2024
 **/

#ifndef juwon_turtle_gui_MAIN_WINDOW_H
#define juwon_turtle_gui_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QMainWindow>
#include <QPushButton>
#include "QIcon"
#include "qnode.hpp"
#include "ui_mainwindow.h"

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();
  QNode* qnode;

private Q_SLOTS:
  void updateCmdVelLabel(double linear, double angular);
  
private:
  Ui::MainWindowDesign* ui;
  void closeEvent(QCloseEvent* event);
};

#endif  // juwon_turtle_gui_MAIN_WINDOW_H
