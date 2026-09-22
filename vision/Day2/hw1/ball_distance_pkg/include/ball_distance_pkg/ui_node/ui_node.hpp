#ifndef UI_NODE_HPP
#define UI_NODE_HPP

#include <mutex>
#include <QThread>
#include <QImage>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>
#include <cv_bridge/cv_bridge.hpp>
#include <opencv2/opencv.hpp>

class UiNode : public QThread
{
    Q_OBJECT
public:
    UiNode();
    ~UiNode();
    QImage get_img();
    void get_dist(double &a, double &b);

protected:
    void run() override;

signals:
    void img_ready();
    void dist_ready();

private:
    void image_callback(const sensor_msgs::msg::Image::SharedPtr msg);
    void dist_callback(const std_msgs::msg::Float64MultiArray::SharedPtr msg);

    rclcpp::Node::SharedPtr node_;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_;
    rclcpp::Subscription<std_msgs::msg::Float64MultiArray>::SharedPtr dist_sub_;

    QImage img_;	// 공유 이미지
    double dist_a_ = -1;	// 공유 거리값
    double dist_b_ = -1;
    std::mutex mtx_;	// img_, dist_ 보호
};

#endif