#include "road_friend/game_node.hpp"

GameNode::GameNode() : Node("game_node")
{
    this->declare_parameter("start_row", 0);
    this->declare_parameter("start_col", 6);

    start_row_ = this->get_parameter("start_row").as_int();
    start_col_ = this->get_parameter("start_col").as_int();

    service_ = this->create_service<road_friend::srv::Reset>(
        "reset_game",
        [this](const std::shared_ptr<road_friend::srv::Reset::Request> req,
               std::shared_ptr<road_friend::srv::Reset::Response> res)
        {
            reset_callback(req, res);
        });

    RCLCPP_INFO(this->get_logger(), "game_node ready");
}

void GameNode::reset_callback(const std::shared_ptr<road_friend::srv::Reset::Request> req,
                              std::shared_ptr<road_friend::srv::Reset::Response> res)
{
    RCLCPP_INFO(this->get_logger(), "reset requested (deaths: %d)", req->deaths);
    res->start_row = start_row_;
    res->start_col = start_col_;
}

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<GameNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}