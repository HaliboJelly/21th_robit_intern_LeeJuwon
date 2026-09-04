#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(button_f()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(button_b()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(button_r()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(button_l()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(button_stop()));
    connect(ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(updateSpeed(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::button_f()
{
    ui->label_3->setText("전진");
    ui->label_6->setText("이동");
}


void MainWindow::button_b()
{
    ui->label_3->setText("후진");
    ui->label_6->setText("이동");
}


void MainWindow::button_r()
{
    ui->label_3->setText("우회전");
    ui->label_6->setText("이동");
}

void MainWindow::button_l()
{
    ui->label_3->setText("좌회전");
    ui->label_6->setText("이동");
}

void MainWindow::button_stop()
{
    ui->label_6->setText("정지");
    ui->label_3->setText("정지");
}

void MainWindow::updateSpeed(int value)
{
    ui->label_4->setText(QString::number(value));
}


void MainWindow::dir_label(const QString &link)
{

}

void MainWindow::speedLabel(const QString &link)
{

}
