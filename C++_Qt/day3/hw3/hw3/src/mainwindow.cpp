#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>

static const char *keys[10] = {"0", ".,?!", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    last_key = -1;
    tap = 0;
    shift = 0;
    cur_upper = false;

    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &MainWindow::commit);

    QPushButton *pad[10] = {ui->btn_0, ui->btn_1, ui->btn_2, ui->btn_3, ui->btn_4,
                            ui->btn_5, ui->btn_6, ui->btn_7, ui->btn_8, ui->btn_9};
    for (int k = 0; k < 10; k++)
    {
        connect(pad[k], &QPushButton::clicked, this, [=]{ key_press(k); });
    }

    connect(ui->btn_back, &QPushButton::clicked, this, &MainWindow::backspace);
    connect(ui->btn_enter, &QPushButton::clicked, this, &MainWindow::enter);
    connect(ui->btn_space, &QPushButton::clicked, this, &MainWindow::space);
    connect(ui->btn_shift, &QPushButton::clicked, this, &MainWindow::toggle_shift);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::key_press(int k)
{
    QString set = keys[k];
    if (k == last_key && timer->isActive())	// 같은 키 연속 -> 다음 글자로 교체
    {
        tap = (tap + 1) % set.length();
        text.chop(1);
    }
    else
    {
        commit();
        tap = 0;
        last_key = k;
        cur_upper = (shift > 0);	// 조합 시작 시점에 대소문자 고정
    }

    QChar c = set[tap];
    if (cur_upper)
    {
        c = c.toUpper();
    }
    text += c;

    if (shift == 1)		// 한 글자만 대문자면 바로 해제
    {
        shift = 0;
        ui->btn_shift->setText("↑ abc");
    }

    timer->start(700);
    update_display();
}

void MainWindow::commit()	// 조합 종료, 다음 입력은 새 글자
{
    timer->stop();
    last_key = -1;
    tap = 0;
}

void MainWindow::backspace()
{
    commit();
    text.chop(1);
    update_display();
}

void MainWindow::space()
{
    commit();
    text += ' ';
    update_display();
}

void MainWindow::enter()
{
    commit();
    if (text.isEmpty())
    {
        return;
    }

    QFile f("output.txt");
    if (f.open(QIODevice::Append | QIODevice::Text))	// Append 라서 마지막 줄에 추가됨
    {
        QTextStream out(&f);
        out << text << "\n";
        f.close();
    }

    text.clear();
    update_display();
}

void MainWindow::toggle_shift()
{
    shift = (shift + 1) % 3;
    if (shift == 0)
    {
        ui->btn_shift->setText("↑ abc");
    }
    else if (shift == 1)
    {
        ui->btn_shift->setText("↑ Abc");
    }
    else
    {
        ui->btn_shift->setText("↑ ABC");
    }
}

void MainWindow::update_display()
{
    ui->lineEdit->setText(text);
}

void MainWindow::on_MainWindow_iconSizeChanged(const QSize &iconSize)
{

}

