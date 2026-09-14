#include "road_friend/traffic_light_node.hpp"

using namespace std::chrono_literals;

TrafficLightNode::TrafficLightNode() : Node("traffic_light_node")
{
   
    this->declare_parameter("r_time",5.0);
    this->declare_parameter("g_time",5.0);
    this->declare_parameter("y_time",5.0);
  
    r_time_ = this->get_parameter("r_time").as_double();
    g_time_ = this->get_parameter("g_time").as_double();
    y_time_ = this->get_parameter("y_time").as_double();

    // 상태 초기화
    state_ = "RED";
    elapsed_ = 0.0;

    publisher_ = this->create_publisher<std_msgs::msg::String>("traffic_light", 10);
    timer_ = this->create_wall_timer(100ms, [this]() { timer_callback(); });
}

void TrafficLightNode::timer_callback()
{
    // 경과 시간 누적
    elapsed_ += 0.1;
    // 제한 시간 넘으면 다음 상태로
    if(state_ == "RED" && elapsed_ >= r_time_ )
    {
        state_ = "GREEN";
        elapsed_ = 0;
        RCLCPP_INFO(this->get_logger(), "Current Light: %s", state_.c_str());
    }
    else if(state_ == "GREEN" && elapsed_ >= g_time_ )
    {
        state_ = "YELLOW";
        elapsed_ = 0;
        RCLCPP_INFO(this->get_logger(), "Current Light: %s", state_.c_str());
    }
    else if(state_ == "YELLOW" && elapsed_ >= y_time_ )
    {
        state_ = "RED";
        elapsed_ = 0;
        RCLCPP_INFO(this->get_logger(), "Current Light: %s", state_.c_str());
    }

    // 현재 상태 발행
    std_msgs::msg::String msg;
    msg.data = state_;
    publisher_->publish(msg);
}

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TrafficLightNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}