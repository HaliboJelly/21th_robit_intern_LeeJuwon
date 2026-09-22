import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node

def generate_launch_description():
    cam = IncludeLaunchDescription(PythonLaunchDescriptionSource(
        os.path.join(get_package_share_directory('insta360_usb_cam'), 'launch', 'usb_cam.launch.py')))
    config = os.path.join(get_package_share_directory('ball_distance_pkg'), 'config', 'camera_ui.yaml')
    return LaunchDescription([
        cam,
        Node(package='ball_distance_pkg', 
             executable='detect_node', 
             name='detect_node', 
             parameters=[config], 
             output='screen'),

        Node(package='ball_distance_pkg', 
             executable='ui_node', 
             name='ui_node', 
             output='screen'),

        Node(package='ball_distance_pkg', 
             executable='distance_node', 
             name='distance_node', 
             parameters=[config], 
             output='screen'),
    ])