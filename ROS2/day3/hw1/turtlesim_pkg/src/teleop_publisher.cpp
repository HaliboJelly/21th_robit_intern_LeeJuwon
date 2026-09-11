#include "turtlesim_pkg/teleop_publisher.hpp"
#include <termios.h>
#include <unistd.h>
#include <chrono>

using namespace std::chrono_literals;

TeleopPublisher::TeleopPublisher() : Node("teleop_publisher")
{
    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10);
    pen_client_ = this->create_client<turtlesim::srv::SetPen>("turtle1/set_pen");

    this->declare_parameter("tri_r", 255);
    this->declare_parameter("tri_g", 0);
    this->declare_parameter("tri_b", 0);
    this->declare_parameter("tri_width", 6);

    this->declare_parameter("rect_r", 0);
    this->declare_parameter("rect_g", 255);
    this->declare_parameter("rect_b", 0);
    this->declare_parameter("rect_width", 10);

    this->declare_parameter("circle_r", 0);
    this->declare_parameter("circle_g", 0);
    this->declare_parameter("circle_b", 255);
    this->declare_parameter("circle_width", 16);

    this->declare_parameter("star_r", 255);
    this->declare_parameter("star_g", 255);
    this->declare_parameter("star_b", 0);
    this->declare_parameter("star_width", 3);

    tri_r = this->get_parameter("tri_r").as_int();
    tri_g = this->get_parameter("tri_g").as_int();
    tri_b = this->get_parameter("tri_b").as_int();
    tri_width = this->get_parameter("tri_width").as_int();

    rect_r = this->get_parameter("rect_r").as_int();
    rect_g = this->get_parameter("rect_g").as_int();
    rect_b = this->get_parameter("rect_b").as_int();
    rect_width = this->get_parameter("rect_width").as_int();

    circle_r = this->get_parameter("circle_r").as_int();
    circle_g = this->get_parameter("circle_g").as_int();
    circle_b = this->get_parameter("circle_b").as_int();
    circle_width = this->get_parameter("circle_width").as_int();

    star_r = this->get_parameter("star_r").as_int();
    star_g = this->get_parameter("star_g").as_int();
    star_b = this->get_parameter("star_b").as_int();
    star_width = this->get_parameter("star_width").as_int();
}

char TeleopPublisher::get_key()
{
    struct termios old, raw;
    tcgetattr(STDIN_FILENO, &old);      // 현재 터미널 설정 백업
    raw = old;
    cfmakeraw(&raw);                    // raw 모드로 전환
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    char key = 0;
    read(STDIN_FILENO, &key, 1);        // 한 글자만 읽음

    tcsetattr(STDIN_FILENO, TCSADRAIN, &old);   // 원복
    return key;
}

void TeleopPublisher::run()
{
    printf("W: 삼각형  A: 사각형  S: 원  D: 별  Q: 종료\r\n");
    while (rclcpp::ok())
    {
        char key = get_key();
        if (key == 'w')
        {
            triangle();
        }
        else if (key == 'a')
        {
            rectangle();
        }
        else if (key == 's')
        {
            circle();
        }
        else if (key == 'd')
        {
            star();
        }
        else if (key == 'q' || key == 3)    // 3 = Ctrl+C
        {
            break;
        }
    }
}

void TeleopPublisher::set_pen(int r, int g, int b, int width)
{
    auto req = std::make_shared<turtlesim::srv::SetPen::Request>();
    req->r = r;
    req->g = g;
    req->b = b;
    req->width = width;
    req->off = 0;
    pen_client_->async_send_request(req);
    rclcpp::sleep_for(200ms);   // 적용될 시간
}

void TeleopPublisher::star()
{
    set_pen(star_r, star_g, star_b, star_width);
    for (int i = 0; i < 5; i++)
    {
        // 직진
        geometry_msgs::msg::Twist msg;
        msg.linear.x = 2.0;
        msg.angular.z = 0.0;
        publisher_->publish(msg);
        RCLCPP_INFO(this->get_logger(), "Publishing: linear.x=%.4f, angular.z=%.4f", msg.linear.x, msg.angular.z);
        rclcpp::sleep_for(1000ms);

        // 정지
        msg.linear.x = 0.0;
        publisher_->publish(msg);
        RCLCPP_INFO(this->get_logger(), "Publishing: linear.x=%.4f, angular.z=%.4f", msg.linear.x, msg.angular.z);
        rclcpp::sleep_for(100ms);

        // 144도 회전
        msg.angular.z = 2.5133;
        publisher_->publish(msg);
        RCLCPP_INFO(this->get_logger(), "Publishing: linear.x=%.4f, angular.z=%.4f", msg.linear.x, msg.angular.z);
        rclcpp::sleep_for(1000ms);

        // 정지
        msg.angular.z = 0.0;
        publisher_->publish(msg);
        RCLCPP_INFO(this->get_logger(), "Publishing: linear.x=%.4f, angular.z=%.4f", msg.linear.x, msg.angular.z);
        rclcpp::sleep_for(100ms);
    }
}

void TeleopPublisher::triangle()
{
    set_pen(tri_r, tri_g, tri_b, tri_width);
    for (int i = 0; i < 3; i++)
    {
        // 직진
        geometry_msgs::msg::Twist msg;
        msg.linear.x = 2.0;
        msg.angular.z = 0.0;
        publisher_->publish(msg);
        RCLCPP_INFO(this->get_logger(), "Publishing: linear.x=%.4f, angular.z=%.4f", msg.linear.x, msg.angular.z);
        rclcpp::sleep_for(1000ms);

        // 정지
        msg.linear.x = 0.0;
        publisher_->publish(msg);
        RCLCPP_INFO(this->get_logger(), "Publishing: linear.x=%.4f, angular.z=%.4f", msg.linear.x, msg.angular.z);
        rclcpp::sleep_for(100ms);

        // 120도 회전
        msg.angular.z = 2.0944;
        publisher_->publish(msg);
        RCLCPP_INFO(this->get_logger(), "Publishing: linear.x=%.4f, angular.z=%.4f", msg.linear.x, msg.angular.z);
        rclcpp::sleep_for(1000ms);

        // 정지
        msg.angular.z = 0.0;
        publisher_->publish(msg);
        RCLCPP_INFO(this->get_logger(), "Publishing: linear.x=%.4f, angular.z=%.4f", msg.linear.x, msg.angular.z);
        rclcpp::sleep_for(100ms);
    }
}

void TeleopPublisher::rectangle()
{
    set_pen(rect_r, rect_g, rect_b, rect_width);
    for (int i = 0; i < 4; i++)
    {
        // 직진
        geometry_msgs::msg::Twist msg;
        msg.linear.x = 2.0;
        msg.angular.z = 0.0;
        publisher_->publish(msg);
        RCLCPP_INFO(this->get_logger(), "Publishing: linear.x=%.4f, angular.z=%.4f", msg.linear.x, msg.angular.z);
        rclcpp::sleep_for(1000ms);

        // 정지
        msg.linear.x = 0.0;
        publisher_->publish(msg);
        RCLCPP_INFO(this->get_logger(), "Publishing: linear.x=%.4f, angular.z=%.4f", msg.linear.x, msg.angular.z);
        rclcpp::sleep_for(100ms);

        // 90도 회전
        msg.angular.z = 1.5708;
        publisher_->publish(msg);
        RCLCPP_INFO(this->get_logger(), "Publishing: linear.x=%.4f, angular.z=%.4f", msg.linear.x, msg.angular.z);
        rclcpp::sleep_for(1000ms);

        // 정지
        msg.angular.z = 0.0;
        publisher_->publish(msg);
        RCLCPP_INFO(this->get_logger(), "Publishing: linear.x=%.4f, angular.z=%.4f", msg.linear.x, msg.angular.z);
        rclcpp::sleep_for(100ms);
    }
}

void TeleopPublisher::circle()
{
    set_pen(circle_r, circle_g, circle_b, circle_width);
    geometry_msgs::msg::Twist msg;
    msg.linear.x = 10.0;    // 전진 속도 (m/s)
    msg.angular.z = 7.0;    // 회전 속도 (rad/s)
    publisher_->publish(msg);
    RCLCPP_INFO(this->get_logger(), "Publishing: linear.x=%.4f, angular.z=%.4f", msg.linear.x, msg.angular.z);
    rclcpp::sleep_for(1000ms);  // 한 바퀴 도는 시간
}

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TeleopPublisher>();
    node->run();
    rclcpp::shutdown();
    return 0;
}