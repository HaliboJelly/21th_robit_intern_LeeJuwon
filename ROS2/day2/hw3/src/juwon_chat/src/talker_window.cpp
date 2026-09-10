#include "../include/juwon_chat/talker_window.hpp"

TalkerWindow::TalkerWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::TalkerDesign)
{
  ui->setupUi(this);
  qnode = new TalkerNode();

  connect(ui->btn_send, &QPushButton::clicked, this, [this]() {
    qnode->publish_msg(ui->line_input->text().toStdString());
    ui->line_input->clear();
  });

  QObject::connect(qnode, SIGNAL(rosShutDown()), this, SLOT(close()));
}

void TalkerWindow::closeEvent(QCloseEvent* event)
{
  QMainWindow::closeEvent(event);
}

TalkerWindow::~TalkerWindow()
{
  delete ui;
}