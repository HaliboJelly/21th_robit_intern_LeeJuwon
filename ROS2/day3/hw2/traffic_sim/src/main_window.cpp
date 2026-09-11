#include "../include/traffic_sim/main_window.hpp"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindowDesign)
{
  ui->setupUi(this);

  QIcon icon("://ros-icon.png");
  this->setWindowIcon(icon);

  qnode = new QNode();

  QObject::connect(qnode, SIGNAL(rosShutDown()), this, SLOT(close()));
  connect(qnode, &QNode::lightUpdated, this, &MainWindow::setLight);
  connect(qnode, &QNode::vehiclesUpdated, this, &MainWindow::setVehicles);

  setLight("RED");
}

void MainWindow::setLight(QString light)
{
  ui->redLight->setStyleSheet("background-color: rgb(70,70,70); border-radius: 20px;");
  ui->yellowLight->setStyleSheet("background-color: rgb(70,70,70); border-radius: 20px;");
  ui->greenLight->setStyleSheet("background-color: rgb(70,70,70); border-radius: 20px;");

  if (light == "RED")
  {
    ui->redLight->setStyleSheet("background-color: rgb(255,50,50); border-radius: 20px;");
  }
  else if (light == "YELLOW")
  {
    ui->yellowLight->setStyleSheet("background-color: rgb(255,220,50); border-radius: 20px;");
  }
  else if (light == "GREEN")
  {
    ui->greenLight->setStyleSheet("background-color: rgb(50,220,100); border-radius: 20px;");
  }
}

void MainWindow::setVehicles(QVector<float> vehicles)
{
  QLabel* cars[5] = { ui->car1, ui->car2, ui->car3, ui->car4, ui->car5 };

  for (int i = 0; i < 5; i++)
  {
    if (i < vehicles.size())
    {
      cars[i]->move((int)vehicles[i], 215);
      cars[i]->show();
    }
    else
    {
      cars[i]->hide();
    }
  }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
  QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
  delete ui;
}