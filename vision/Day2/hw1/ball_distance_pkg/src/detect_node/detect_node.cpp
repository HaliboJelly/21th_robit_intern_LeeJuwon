#include "ball_distance_pkg/detect_node/detect_node.hpp"

DetectNode::DetectNode() : Node("detect_node")
{
    img_sub_ = this->create_subscription<sensor_msgs::msg::Image>(
        "/camera1/camera/compressed_image", 10,
        std::bind(&DetectNode::image_callback, this, std::placeholders::_1));
    img_pub_ = this->create_publisher<sensor_msgs::msg::Image>("/ball/image", 10);
    mask_pub_ = this->create_publisher<sensor_msgs::msg::Image>("/ball/mask", 10);
    info_pub_ = this->create_publisher<std_msgs::msg::Float64MultiArray>("/ball/info", 10);
}

void DetectNode::image_callback(const sensor_msgs::msg::Image::SharedPtr msg)
{
    cv::Mat frame;
    try
    {
        frame = cv_bridge::toCvCopy(msg, "bgr8")->image;
    }
    catch (cv_bridge::Exception &e)
    {
        RCLCPP_ERROR(this->get_logger(), "cv_bridge: %s", e.what());
        return; 
    }

    cv::Mat hsv, mask;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, cv::Scalar(20, 100, 100), cv::Scalar(35, 255, 255), mask);	// 노랑

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));	// 5x5 원형
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    int best = -1;	// 가장 큰 덩어리가 공
    double best_area = 500;	// 이거 보다 작으면 무시
    for (int i = 0; i < (int)contours.size(); i++)
    {
        double area = cv::contourArea(contours[i]);
        if (area > best_area)
        {
            best_area = area;
            best = i;
        }
    }

    double u = -1;
    double v = -1;
    double w = -1;
    if (best >= 0)
    {
        cv::Rect box = cv::boundingRect(contours[best]);
        u = box.x + box.width / 2.0;        //u,v,w 계산 하는거
        v = box.y + box.height;
        w = box.width;
        cv::rectangle(frame, box, cv::Scalar(0, 255, 255), 2);	// BGR 노랑
        cv::circle(frame, cv::Point(u, v), 4, cv::Scalar(0, 0, 255), -1);	// 바닥 접점
    }

    std_msgs::msg::Float64MultiArray info;
    info.data = {u, v, w};
    info_pub_->publish(info);

    mask_pub_->publish(*cv_bridge::CvImage(msg->header, "mono8", mask).toImageMsg());	

    img_pub_->publish(*cv_bridge::CvImage(msg->header, "bgr8", frame).toImageMsg());	// 박스 그린 BGR 그대로 발행
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DetectNode>());
    rclcpp::shutdown();
    return 0;
}