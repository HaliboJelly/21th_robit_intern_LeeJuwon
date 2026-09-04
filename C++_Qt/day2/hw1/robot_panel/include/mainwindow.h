#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void button_f();

    void button_b();

    void button_r();

    void button_l();

    void dir_label(const QString &link);

    void updateSpeed(int value);

    void speedLabel(const QString &link);

    void button_stop();

private:
    Ui::MainWindow *ui;
};

#endif
