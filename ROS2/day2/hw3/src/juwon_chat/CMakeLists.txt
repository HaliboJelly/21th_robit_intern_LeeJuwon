cmake_minimum_required(VERSION 3.5)
project(juwon_chat)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(Boost 1.71.0 REQUIRED COMPONENTS thread)
find_package(Qt5 COMPONENTS Widgets Core Gui Network REQUIRED)
find_package(std_msgs REQUIRED)

include_directories(
	include/
	${CMAKE_CURRENT_BINARY_DIR}
)

qt5_wrap_ui(TALKER_UI ui/talker.ui)
qt5_wrap_cpp(TALKER_MOC
  include/${PROJECT_NAME}/talker_window.hpp
  include/${PROJECT_NAME}/talker_qnode.hpp
)

add_executable(talker
  src/talker_main.cpp
  src/talker_window.cpp
  src/talker_qnode.cpp
  ${TALKER_MOC}
  ${TALKER_UI}
)
ament_target_dependencies(talker rclcpp Boost std_msgs)
target_link_libraries(talker Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Network)

qt5_wrap_ui(LISTENER_UI ui/listener.ui)
qt5_wrap_cpp(LISTENER_MOC
  include/${PROJECT_NAME}/listener_window.hpp
  include/${PROJECT_NAME}/listener_qnode.hpp
)

add_executable(listener
  src/listener_main.cpp
  src/listener_window.cpp
  src/listener_qnode.cpp
  ${LISTENER_MOC}
  ${LISTENER_UI}
)
ament_target_dependencies(listener rclcpp Boost std_msgs)
target_link_libraries(listener Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Network)

install(TARGETS talker listener DESTINATION lib/${PROJECT_NAME})

install(TARGETS talker DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()