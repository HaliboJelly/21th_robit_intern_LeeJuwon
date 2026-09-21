#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QPixmap>
#include "color_detect_pkg/ui_node/ui_node.hpp"

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

private:
    Ui::MainWindow *ui;
    UiNode *ui_node;
};

#endif