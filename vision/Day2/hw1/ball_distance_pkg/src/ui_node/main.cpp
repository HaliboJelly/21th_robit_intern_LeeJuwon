#include <QApplication>
#include "ball_distance_pkg/ui_node/main_window.hpp"

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}