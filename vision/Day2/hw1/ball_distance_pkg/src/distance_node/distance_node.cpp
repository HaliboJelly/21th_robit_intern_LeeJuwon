#include "ball_distance_pkg/distance_node/distance_node.hpp"

DistanceNode::DistanceNode() : Node("distance_node")
{
    this->declare_parameter("ball_diameter", 0.065);
    this->declare_parameter("cam_height", 0.30);
    this->declare_parameter("tilt_deg", 0.0);	// 아래로 숙인 각이 +
    ball_d_ = this->get_parameter("ball_diameter").as_double();
    cam_h_ = this->get_parameter("cam_height").as_double();
    tilt_ = this->get_parameter("tilt_deg").as_double() * M_PI / 180.0;

    info_sub_ = this->create_subscription<sensor_msgs::msg::CameraInfo>(
        "/camera1/info", 10,
        std::bind(&DistanceNode::info_callback, this, std::placeholders::_1));
    ball_sub_ = this->create_subscription<std_msgs::msg::Float64MultiArray>(
        "/ball/info", 10,
        std::bind(&DistanceNode::ball_callback, this, std::placeholders::_1));
    dist_pub_ = this->create_publisher<std_msgs::msg::Float64MultiArray>("/ball/distance", 10);
}

void DistanceNode::info_callback(const sensor_msgs::msg::CameraInfo::SharedPtr msg)
{
    fx_ = msg->k[0];	// K = [fx 0 cx; 0 fy cy; 0 0 1]
    cx_ = msg->k[2];
    fy_ = msg->k[4];
    cy_ = msg->k[5];
    got_info_ = true;
}

void DistanceNode::ball_callback(const std_msgs::msg::Float64MultiArray::SharedPtr msg)
{
    if (!got_info_ || msg->data.size() < 3)
    {
        return;
    }
    double v = msg->data[1];
    double w = msg->data[2];

    double dist_a = -1;
    double dist_b = -1;
    if (w > 0)
    {
        dist_a = fx_ * ball_d_ / w;	// 방법 a

        double yn = (v - cy_) / fy_;	// 정규화 y좌표
        double ang = tilt_ + atan(yn);	// 수평선 기준 내려다보는 각
        if (ang > 0)
        {
            dist_b = cam_h_ / tan(ang);	// 방법 b
        }
    }

    std_msgs::msg::Float64MultiArray out;
    out.data = {dist_a, dist_b};
    dist_pub_->publish(out);
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DistanceNode>());
    rclcpp::shutdown();
    return 0;
}