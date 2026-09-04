#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->add_btn, &QPushButton::clicked, this, &MainWindow::add_task);
    connect(ui->process_btn, &QPushButton::clicked, this, &MainWindow::process_task);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::add_task()
{
    QString text = ui->task_input->text();
    if (text.isEmpty())//예외처리 부분 만약 텍스트 입력창이 비어있으면 그냥 return
    {
        return;
    }
    task_queue.push_back(text);	// 뒤에 삽입(벡터 쪽 문법)
    ui->task_input->clear();//입력창 비우기
    refresh_view();
}

void MainWindow::process_task()
{
    if (task_queue.empty())//아까와 비슷한 예외처리 없으면 그냥 반환
    {
        return;
    }
    task_queue.erase(task_queue.begin());	// 앞에서 제거
    refresh_view();
}

void MainWindow::refresh_view()//입력이 들어오면 list에 있는거 다 지우고 다시 다 적는 구조.
{
    ui->queue_list->clear();
    for (int i = 0; i < (int)task_queue.size(); i++)
    {
        ui->queue_list->addItem(QString::number(i + 1) + ". " + task_queue[i]);
    }                       // 바로 윗 줄에서 addItem은 무조건 Qstring으로 받기 때문에 i+1을 문자로 바꿔주는게 필요함.
}
