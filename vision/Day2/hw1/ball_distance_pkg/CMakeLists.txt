cmake_minimum_required(VERSION 3.8)
project(ball_distance_pkg)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(std_msgs REQUIRED)

include_directories(include)

add_executable(distance_node
  src/distance_node/distance_node.cpp
)
ament_target_dependencies(distance_node rclcpp sensor_msgs std_msgs)

add_executable(detect_node
  src/detect_node/detect_node.cpp
)
ament_target_dependencies(detect_node rclcpp sensor_msgs cv_bridge std_msgs)
target_link_libraries(detect_node ${OpenCV_LIBS})

add_executable(ui_node
  src/ui_node/main.cpp
  src/ui_node/main_window.cpp
  src/ui_node/ui_node.cpp
  include/ball_distance_pkg/ui_node/main_window.hpp
  include/ball_distance_pkg/ui_node/ui_node.hpp
  ui/mainwindow.ui
)
ament_target_dependencies(ui_node rclcpp sensor_msgs cv_bridge std_msgs)
target_link_libraries(ui_node ${OpenCV_LIBS} Qt5::Widgets)

install(TARGETS detect_node ui_node
  DESTINATION lib/${PROJECT_NAME}
)
install(DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME}
)
install(TARGETS detect_node distance_node ui_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
