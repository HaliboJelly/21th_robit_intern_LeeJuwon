#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_MainWindow_iconSizeChanged(const QSize &iconSize);

private:
    void key_press(int k);
    void commit();
    void backspace();
    void space();
    void enter();
    void toggle_shift();
    void update_display();

    Ui::MainWindow *ui;
    QTimer *timer;
    QString text;
    int last_key;	// 직전에 누른 키, 없으면 -1
    int tap;		// 같은 키를 몇 번째 눌렀는지
    int shift;		// 0:소문자 1:한글자만 2:고정
    bool cur_upper;	// 조합 중인 글자의 대소문자
};

#endif
