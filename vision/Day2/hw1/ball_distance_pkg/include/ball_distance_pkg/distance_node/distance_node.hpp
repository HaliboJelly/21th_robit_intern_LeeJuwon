#ifndef DISTANCE_NODE_HPP
#define DISTANCE_NODE_HPP

#include <cmath>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>

class DistanceNode : public rclcpp::Node
{
public:
    DistanceNode();

private:
    void info_callback(const sensor_msgs::msg::CameraInfo::SharedPtr msg);
    void ball_callback(const std_msgs::msg::Float64MultiArray::SharedPtr msg);

    double ball_d_;	// 공 실제 지름(미터단위라 잘 생각해서 넣기)
    double cam_h_;	// 카메라 높이(이것도)
    double tilt_;	// 틸트 각(rad 라디안으로 계산)
    double fx_ = 0, fy_ = 0, cx_ = 0, cy_ = 0;
    bool got_info_ = false;	// camera_info 받았는지

    rclcpp::Subscription<sensor_msgs::msg::CameraInfo>::SharedPtr info_sub_;
    rclcpp::Subscription<std_msgs::msg::Float64MultiArray>::SharedPtr ball_sub_;
    rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr dist_pub_;	// [A, B] (미터)
};

#endif