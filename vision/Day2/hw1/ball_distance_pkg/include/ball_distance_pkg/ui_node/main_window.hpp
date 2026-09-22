#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QPixmap>
#include "ball_distance_pkg/ui_node/ui_node.hpp"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void update_img();
    void update_dist();
    void record_dist();

private:
    Ui::MainWindow *ui;
    UiNode *ui_node;
    int record_cnt = 0;
};

#endif