#include <ament_index_cpp/get_package_share_directory.hpp>
#include "color_detect_pkg/camera_node/camera_node.hpp"

using namespace std::chrono_literals;


CameraNode::CameraNode() : Node("camera_node")
{
    this->declare_parameter("image_path", std::string(""));
    this->declare_parameter("ms(1/hz)",100);

    image_path_ = this->get_parameter("image_path").as_string();
    period_ms_ = this->get_parameter("ms(1/hz)").as_int();

    std::string share = ament_index_cpp::get_package_share_directory("color_detect_pkg");
    frame_ = cv::imread(share + "/" + image_path_);
    if (frame_.empty())                     //비어있으면 안되니까
    {
        RCLCPP_ERROR(this->get_logger(), "경로 확인: %s", image_path_.c_str());
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
    if (frame_.empty())
    {
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