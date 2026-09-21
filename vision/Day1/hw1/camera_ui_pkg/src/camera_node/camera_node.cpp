#include "camera_ui_pkg/camera_node/camera_node.hpp"

using namespace std::chrono_literals;


CameraNode::CameraNode() : Node("camera_node")
{
    this->declare_parameter("deviceNum",0);
    this->declare_parameter("ms(1/hz)",50);

    device_num_ = this->get_parameter("deviceNum").as_int();
    period_ms_ = this->get_parameter("ms(1/hz)").as_int();

    cap_.open(device_num_);
    if (!cap_.isOpened())
    {
        RCLCPP_ERROR(this->get_logger(), "camera open failed");
    }

    publisher_ = this->create_publisher<sensor_msgs::msg::Image>("camera/image", 10);
    timer_ = this->create_wall_timer(
            std::chrono::milliseconds(period_ms_),   //20hz 기본
            std::bind(&CameraNode::timer_callback, this)
        );

    RCLCPP_INFO(this->get_logger(), "Webcam Publisher Node started.");

}


void CameraNode::timer_callback()
{
    cap_ >> frame_;  

    if (frame_.empty()) {
        RCLCPP_WARN(this->get_logger(), "Warning: Captured empty frame.");
            return;
    }
    publish_frame();
}


void CameraNode::publish_frame()
{
    std_msgs::msg::Header header;
    header.stamp = this->now();  
    header.frame_id = "camera_frame";  

    auto msg = cv_bridge::CvImage(header, "bgr8", frame_).toImageMsg();

      
    publisher_->publish(*msg); 

    RCLCPP_INFO(this->get_logger(), "Image frame %zu published.", count_++);
}

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<CameraNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}