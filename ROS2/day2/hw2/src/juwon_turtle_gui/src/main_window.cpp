/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date August 2024
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include "../include/juwon_turtle_gui/main_window.hpp"
#include <QApplication>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindowDesign)
{
	ui->setupUi(this);
	qnode = new QNode();

	connect(ui->btn_w, &QPushButton::clicked, this, [this]() { qnode->triangle(); });
	connect(ui->btn_s, &QPushButton::clicked, this, [this]() { qnode->rectangle(); });
	connect(ui->btn_a, &QPushButton::clicked, this, [this]() { qnode->circle(); });
	connect(ui->btn_d, &QPushButton::clicked, this, [this]() { qnode->star(); });
	connect(qnode, &QNode::cmdVelUpdated, this, &MainWindow::updateCmdVelLabel);

	QIcon icon("://ros-icon.png");
	this->setWindowIcon(icon);

	QObject::connect(qnode, SIGNAL(rosShutDown()), this, SLOT(close()));
}

void MainWindow::closeEvent(QCloseEvent* event)
{
  QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::updateCmdVelLabel(double linear, double angular)
{
	ui->label_cmd_vel->setText(QString("linear.x: %1 / angular.z: %2").arg(linear).arg(angular));
	QApplication::processEvents();
}
