#include "mainwindow.h"
#include "./ui_mainwindow.h"

#define port1 50000

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    text_socket = new QUdpSocket(this);
    if(text_socket->bind(QHostAddress::AnyIPv4,port1, QUdpSocket::ShareAddress))
    {
        connect(text_socket, & QUdpSocket::readyRead,
                this, &MainWindow::udp_read);
    }
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::udp_send);


    QPushButton *num[10] = {ui->btn_0, ui->btn_1, ui->btn_2, ui->btn_3, ui->btn_4,
                            ui->btn_5, ui->btn_6, ui->btn_7, ui->btn_8, ui->btn_9};
    pad = new keypad(ui->lineEdit, ui->btn_shift, this);
    pad->set_buttons(num, ui->btn_back, ui->btn_space, ui->btn_enter);
    connect(pad, &keypad::enter_pressed, this, &MainWindow::udp_send);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::udp_send()
{
    QString msg = ui->lineEdit->text();
    text_socket->writeDatagram(msg.toUtf8(),
                               QHostAddress("172.100.2.99"), port1);
    ui->textBrowser->append("나 : " + msg);
    ui->lineEdit->clear();
}

void MainWindow::udp_read()
{
    while(text_socket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = text_socket->receiveDatagram();
        QString msg = QString::fromUtf8(datagram.data());
        ui->textBrowser->append(datagram.senderAddress().toString() + " : " + msg);
    }
}

