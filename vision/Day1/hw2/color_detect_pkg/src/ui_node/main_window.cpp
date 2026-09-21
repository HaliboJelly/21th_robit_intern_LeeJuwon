#include "color_detect_pkg/ui_node/main_window.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui_node = new UiNode();
    connect(ui_node, &UiNode::img_ready, this, &MainWindow::update_img);
}

MainWindow::~MainWindow()
{
    delete ui_node;
    delete ui;
}

void MainWindow::update_img()
{
    QImage img = ui_node->get_img();
    if (img.isNull())   //비어있으면 그대로 반환 때려버리기
    {
        return;
    }
    ui->label_img->setPixmap(QPixmap::fromImage(img).scaled(ui->label_img->size(), Qt::KeepAspectRatio));
}