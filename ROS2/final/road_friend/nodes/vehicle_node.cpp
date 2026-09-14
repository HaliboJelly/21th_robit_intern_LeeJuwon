#include "road_friend/vehicle_node.hpp"

using namespace std::chrono_literals;

VehicleNode::VehicleNode() : Node("vehicle_node")
{
    this->declare_parameter("lane_count", 3);
    this->declare_parameter("lane_speeds", std::vector<double>{ 80.0, 120.0, 60.0 });
    this->declare_parameter("stop_line", 300.0);
    this->declare_parameter("spawn_period", 1.5);
    this->declare_parameter("update_hz", 30.0);
    this->declare_parameter("road_end", 900.0);
    this->declare_parameter("speed_step", 0.2);

    lane_count_ = this->get_parameter("lane_count").as_int();
    lane_speeds_ = this->get_parameter("lane_speeds").as_double_array();
    stop_line_ = this->get_parameter("stop_line").as_double();
    spawn_period_ = this->get_parameter("spawn_period").as_double();
    update_hz_ = this->get_parameter("update_hz").as_double();
    road_end_ = this->get_parameter("road_end").as_double();
    speed_step_ = this->get_parameter("speed_step").as_double();

    light_ = "RED";
    spawn_elapsed_ = 0.0;
    speed_scale_ = 1.0;

    subscriber_ = this->create_subscription<std_msgs::msg::String>(
        "traffic_light", 10,
        [this](const std_msgs::msg::String::SharedPtr msg) { light_callback(msg); });

    player_sub_ = this->create_subscription<road_friend::msg::PlayerState>(
        "player_state", 10,
        [this](const road_friend::msg::PlayerState::SharedPtr msg)
        {
            speed_scale_ = 1.0 + msg->score * speed_step_;
        });

    publisher_ = this->create_publisher<road_friend::msg::CarArray>("vehicle_pos", 10);

    auto period = std::chrono::milliseconds((int)(1000.0 / update_hz_));
    timer_ = this->create_wall_timer(period, [this]() { timer_callback(); });
}

void VehicleNode::light_callback(const std_msgs::msg::String::SharedPtr msg)
{
    light_ = msg->data;
}

void VehicleNode::timer_callback()
{
    double dt = 1.0 / update_hz_;

    // 스폰 - 차선은 무작위
    spawn_elapsed_ += dt;
    if (spawn_elapsed_ >= spawn_period_)
    {
        Car c;
        c.lane = rand() % lane_count_;
        c.x = 0.0;
        vehicles_.push_back(c);
        spawn_elapsed_ = 0.0;
    }

    // 차선별 앞차 위치 (처음엔 앞차 없음)
    std::vector<float> front(lane_count_, 999999.0);

    // 위치 갱신
    for (size_t i = 0; i < vehicles_.size(); i++)
    {
        int lane = vehicles_[i].lane;
        double v = lane_speeds_[lane] * speed_scale_;

        if (light_ == "YELLOW")
        {
            v = v * 0.5;    // 감속
        }

        double next = vehicles_[i].x + v * dt;

        // 빨강이고 아직 정지선 앞이면 정지선까지만
        if (light_ == "RED" && vehicles_[i].x <= stop_line_)
        {
            if (next > stop_line_)
            {
                next = stop_line_;
            }
        }

        // 같은 차선 앞차와 최소 간격 유지
        if (next > front[lane] - 40.0)
        {
            next = front[lane] - 40.0;
        }

        if (next < vehicles_[i].x)
        {
            next = vehicles_[i].x;      // 뒤로는 안 감
        }

        vehicles_[i].x = next;
        front[lane] = next;
    }

    // 도로 끝을 지난 차 제거
    std::vector<Car> alive;
    for (size_t i = 0; i < vehicles_.size(); i++)
    {
        if (vehicles_[i].x < road_end_)
        {
            alive.push_back(vehicles_[i]);
        }
    }
    vehicles_ = alive;

    // 발행
    road_friend::msg::CarArray msg;
    for (size_t i = 0; i < vehicles_.size(); i++)
    {
        road_friend::msg::Car c;
        c.lane = vehicles_[i].lane;
        c.x = vehicles_[i].x;
        msg.cars.push_back(c);
    }
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