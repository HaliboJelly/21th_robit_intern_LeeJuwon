#ifndef DETECT_NODE_HPP
#define DETECT_NODE_HPP

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.hpp>
#include <opencv2/opencv.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>

class DetectNode : public rclcpp::Node
{
public:
    DetectNode();

private:
    void image_callback(const sensor_msgs::msg::Image::SharedPtr msg);

    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr mask_pub_;	
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr img_sub_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr img_pub_;
    rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr info_pub_;	// u, v, w  px
};

#endif