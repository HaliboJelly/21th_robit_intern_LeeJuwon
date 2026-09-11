#include "traffic_sim/vehicle_node.hpp"

using namespace std::chrono_literals;

VehicleNode::VehicleNode() : Node("vehicle_node")
{
    this->declare_parameter("speed", 60.0);
    this->declare_parameter("stop_line", 300.0);
    this->declare_parameter("spawn_period", 2.0);
    this->declare_parameter("update_hz", 30.0);
    this->declare_parameter("road_end", 900.0);
    road_end_ = this->get_parameter("road_end").as_double();
    speed_ = this->get_parameter("speed").as_double();
    stop_line_ = this->get_parameter("stop_line").as_double();
    spawn_period_ = this->get_parameter("spawn_period").as_double();
    update_hz_ = this->get_parameter("update_hz").as_double();

    light_ = "RED";
    spawn_elapsed_ = 0.0;

    subscriber_ = this->create_subscription<std_msgs::msg::String>(
        "traffic_light", 10, [this](const std_msgs::msg::String::SharedPtr msg) { light_callback(msg); }
    );

    publisher_ = this->create_publisher<std_msgs::msg::Float32MultiArray>("vehicle_pos", 10);

    auto period = std::chrono::milliseconds((int)(1000.0 / update_hz_));
    timer_ = this->create_wall_timer(period, [this]() { timer_callback(); });
}

void VehicleNode::light_callback(const std_msgs::msg::String::SharedPtr msg)
{
    light_ = msg->data; //받아온 값을 light_에 저장
}

void VehicleNode::timer_callback()
{
    double dt = 1.0 / update_hz_;

    // 스폰
    spawn_elapsed_ += dt;
    if (spawn_elapsed_ >= spawn_period_)
    {
        vehicles_.push_back(0.0);
        spawn_elapsed_ = 0.0;
    }

    // 위치 갱신
    for (size_t i = 0; i < vehicles_.size(); i++)
    {
        double v = speed_;

        if (light_ == "YELLOW")
        {
            v = speed_ * 0.5;   // 감속
        }

        double next = vehicles_[i] + v * dt;

        // 빨강이고 아직 정지선 앞이면 정지선까지만
        if (light_ == "RED" && vehicles_[i] <= stop_line_)
        {
            if (next > stop_line_)
            {
                next = stop_line_;
            }
        }

        // 앞차와 최소 간격 유지 (40px)
        if (i > 0 && next > vehicles_[i - 1] - 40.0)
        {
            next = vehicles_[i - 1] - 40.0;
        }

        if (next < vehicles_[i])
        {
            next = vehicles_[i];    // 뒤로는 안 감
        }
        
        vehicles_[i] = next;
    }

    // 화면 밖으로 나간 차 제거
    std::vector<float> alive;
    for (size_t i = 0; i < vehicles_.size(); i++)
    {
        if (vehicles_[i] < road_end_)
        {
            alive.push_back(vehicles_[i]);
        }
    }
    vehicles_ = alive;

    std_msgs::msg::Float32MultiArray msg;
    msg.data = vehicles_;
    publisher_->publish(msg);
}

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<VehicleNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}