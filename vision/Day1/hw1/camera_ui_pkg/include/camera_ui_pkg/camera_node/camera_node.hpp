#ifndef CAMERA_NODE_HPP
#define CAMERA_NODE_HPP

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


class CameraNode : public rclcpp::Node
{
public:
    CameraNode();

private:
    void timer_callback();
    void publish_frame();

    int device_num_;	// 카메라 장치 번호
    int period_ms_;	// ms
    size_t count_ = 0;

    cv::Mat frame_;
    cv::VideoCapture cap_;                                           // OpenCV 비디오 캡처 객체
    rclcpp::TimerBase::SharedPtr timer_;                           
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;  
    
};

#endif