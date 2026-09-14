#include "road_friend/player_node.hpp"

PlayerNode::PlayerNode() : Node("player_node")
{
    this->declare_parameter("col_count", 13);
    this->declare_parameter("col_width", 60.0);
    this->declare_parameter("player_width", 30.0);
    this->declare_parameter("car_width", 66.0);

    col_count_ = this->get_parameter("col_count").as_int();
    col_width_ = this->get_parameter("col_width").as_double();
    player_width_ = this->get_parameter("player_width").as_double();
    car_width_ = this->get_parameter("car_width").as_double();

    row_ = 0;
    col_ = 6;
    alive_ = true;
    score_ = 0;
    deaths_ = 0;

    move_sub_ = this->create_subscription<road_friend::msg::Move>(
        "player_cmd", 10,
        [this](const road_friend::msg::Move::SharedPtr msg) { move_callback(msg); });

    car_sub_ = this->create_subscription<road_friend::msg::CarArray>(
        "vehicle_pos", 10,
        [this](const road_friend::msg::CarArray::SharedPtr msg) { car_callback(msg); });

    publisher_ = this->create_publisher<road_friend::msg::PlayerState>("player_state", 10);
    reset_client_ = this->create_client<road_friend::srv::Reset>("reset_game");

    publish_state();
}

void PlayerNode::move_callback(const road_friend::msg::Move::SharedPtr msg)
{
    if (!alive_)
    {
        return;     // 죽어있는 동안은 조작 불가
    }

    int next_row = row_ + msg->dy;
    int next_col = col_ + msg->dx;

    if (next_row < 0 || next_row > 6)
    {
        return;
    }
    if (next_col < 0 || next_col >= col_count_)
    {
        return;
    }

    row_ = next_row;
    col_ = next_col;

    // 도착
    if (row_ == 6)
    {
        score_++;
        row_ = 0;
        RCLCPP_INFO(this->get_logger(), "clear! score: %d", score_);
    }

    publish_state();
}

void PlayerNode::car_callback(const road_friend::msg::CarArray::SharedPtr msg)
{
    if (!alive_)
    {
        return;
    }
    if (row_ < 1 || row_ > 5)
    {
        return;     // 안전지대
    }

    int lane = 5 - row_;    // row 1 = 아래 차선 = lane 2
    double px = col_ * col_width_;

    for (size_t i = 0; i < msg->cars.size(); i++)
    {
        if (msg->cars[i].lane != lane)
        {
            continue;
        }

        double cx = msg->cars[i].x;
        if (px + player_width_ > cx && px < cx + car_width_)
        {
            alive_ = false;
            deaths_++;
            publish_state();
            reset_timer_ = this->create_wall_timer(
                std::chrono::seconds(1),
                [this]()
                {
                    reset_timer_->cancel();
                    request_reset();
                });
            return;
        }
    }
}

void PlayerNode::request_reset()
{
    if (!reset_client_->wait_for_service(std::chrono::milliseconds(100)))
    {
        RCLCPP_WARN(this->get_logger(), "reset service not available");
        return;
    }

    auto req = std::make_shared<road_friend::srv::Reset::Request>();
    req->deaths = deaths_;

    reset_client_->async_send_request(req,
        [this](rclcpp::Client<road_friend::srv::Reset>::SharedFuture future)
        {
            auto res = future.get();
            row_ = res->start_row;
            col_ = res->start_col;
            score_ = 0;
            alive_ = true;
            publish_state();
        });
}

void PlayerNode::publish_state()
{
    road_friend::msg::PlayerState msg;
    msg.row = row_;
    msg.col = col_;
    msg.alive = alive_;
    msg.score = score_;
    publisher_->publish(msg);
}

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<PlayerNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}