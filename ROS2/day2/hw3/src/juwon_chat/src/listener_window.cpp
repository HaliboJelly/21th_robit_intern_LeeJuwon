#include "../include/juwon_chat/listener_window.hpp"

ListenerWindow::ListenerWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::ListenerDesign)
{
  ui->setupUi(this);
  qnode = new ListenerNode();

  connect(qnode, &ListenerNode::msgReceived, this, &ListenerWindow::updateRecvLabel);
  QObject::connect(qnode, SIGNAL(rosShutDown()), this, SLOT(close()));
}

void ListenerWindow::closeEvent(QCloseEvent* event)
{
  QMainWindow::closeEvent(event);
}

ListenerWindow::~ListenerWindow()
{
  delete ui;
}

void ListenerWindow::updateRecvLabel(QString text)
{
	ui->label_recv->setText(text);
}