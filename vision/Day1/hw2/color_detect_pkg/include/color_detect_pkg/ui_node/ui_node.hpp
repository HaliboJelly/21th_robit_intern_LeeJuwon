#ifndef UI_NODE_HPP
#define UI_NODE_HPP

#include <QThread>
#include <QImage>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.hpp>
#include <opencv2/opencv.hpp>
#include <mutex>

class UiNode : public QThread
{
    Q_OBJECT
public:
    UiNode();
    ~UiNode();
    QImage get_img();
    
protected:
    void run() override;

signals:
    void img_received(QImage img);
    void img_ready();

private:
    void image_callback(const sensor_msgs::msg::Image::SharedPtr msg);

    rclcpp::Node::SharedPtr node_;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_;
    QImage img_;
    std::mutex mtx_;
};

#endif