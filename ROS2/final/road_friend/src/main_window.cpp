#include "../include/road_friend/main_window.hpp"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindowDesign)
{
  ui->setupUi(this);

  QIcon icon("://ros-icon.png");
  this->setWindowIcon(icon);

  qnode = new QNode();

  QObject::connect(qnode, SIGNAL(rosShutDown()), this, SLOT(close()));
  connect(qnode, &QNode::lightUpdated, this, &MainWindow::setLight);
  connect(qnode, &QNode::vehiclesUpdated, this, &MainWindow::setVehicles);

  connect(ui->btnUp, &QPushButton::clicked, this, [this]() { qnode->sendMove(0, +1); });
  connect(ui->btnDown, &QPushButton::clicked, this, [this]() { qnode->sendMove(0, -1); });
  connect(ui->btnLeft, &QPushButton::clicked, this, [this]() { qnode->sendMove(-1, 0); });
  connect(ui->btnRight, &QPushButton::clicked, this, [this]() { qnode->sendMove(+1, 0); });

  connect(qnode, &QNode::playerUpdated, this, &MainWindow::setPlayer);

  setLight("RED");
}

void MainWindow::setPlayer(int row, int col, bool alive, int score)
{
  int player_y[7] = { 520, 440, 380, 320, 260, 200, 140 };
  // 0=출발, 1~5=차선(아래→위), 6=도착 
  ui->player->move(col * 60, player_y[row]);

  ui->scoreLabel->setText(QString("SCORE  %1").arg(score));

  if (alive)
  {
    ui->player->setStyleSheet("background-color: rgb(50,220,100);");
    ui->statusLabel->setText("GO!");
    ui->statusLabel->setStyleSheet("color: rgb(50,220,100); font-size: 20px; font-weight: bold;");
  }
  else
  {
    ui->player->setStyleSheet("background-color: rgb(120,120,120);");
    ui->statusLabel->setText("CRASHED");
    ui->statusLabel->setStyleSheet("color: rgb(255,60,60); font-size: 20px; font-weight: bold;");
  }
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

void MainWindow::setVehicles(QVector<QPoint> cars)
{
  int lane_y[5] = { 200, 260, 320, 380, 440 };
  QLabel* labels[12] = { ui->car1, ui->car2, ui->car3, ui->car4, ui->car5, ui->car6,
                         ui->car7, ui->car8, ui->car9, ui->car10, ui->car11, ui->car12 };

  for (int i = 0; i < 12; i++)
  {
    if (i < cars.size())
    {
      labels[i]->move(cars[i].x(), lane_y[cars[i].y()]);
      labels[i]->show();
    }
    else
    {
      labels[i]->hide();
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