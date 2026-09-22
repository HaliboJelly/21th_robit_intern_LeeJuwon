#include <fstream>
#include <QDir>
#include "ball_distance_pkg/ui_node/main_window.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui_node = new UiNode();
    connect(ui_node, &UiNode::img_ready, this, &MainWindow::update_img);
    connect(ui_node, &UiNode::dist_ready, this, &MainWindow::update_dist);
    connect(ui->btn_record, &QPushButton::clicked, this, &MainWindow::record_dist);
}

MainWindow::~MainWindow()
{
    delete ui_node;
    delete ui;
}

void MainWindow::update_img()
{
    QImage img = ui_node->get_img();
    if (img.isNull())	// 비어있으면 그냥 반환
    {
        return;
    }
    ui->label_img->setPixmap(QPixmap::fromImage(img).scaled(ui->label_img->size(), Qt::KeepAspectRatio));
}

void MainWindow::update_dist()
{
    double a, b;
    ui_node->get_dist(a, b);
    ui->label_a->setText(a < 0 ? "방법 A: -" : QString("방법 A: %1 m").arg(a, 0, 'f', 3));
    ui->label_b->setText(b < 0 ? "방법 B: -" : QString("방법 B: %1 m").arg(b, 0, 'f', 3));
}

void MainWindow::record_dist()
{
    double a, b;
    ui_node->get_dist(a, b);	// 누른 순간의 최신 값
    double real = ui->spin_real->value();

    std::string path = QDir::homePath().toStdString() + "/ball_distance_log.csv";
    std::ifstream check(path);
    bool is_new = !check.good();	// 파일 없으면 헤더부터
    check.close();

    std::ofstream f(path, std::ios::app);	// 이어쓰기
    if (is_new)
    {
        f << "real,dist_a,dist_b,err_a,err_b\n";
    }
    f << real << "," << a << "," << b << "," << a - real << "," << b - real << "\n";

    record_cnt++;
    ui->btn_record->setText(QString("거리 기록 (%1)").arg(record_cnt));
}