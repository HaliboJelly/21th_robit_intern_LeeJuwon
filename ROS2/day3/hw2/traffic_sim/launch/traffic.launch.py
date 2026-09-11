import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    pkg = get_package_share_directory('traffic_sim')
    light_config = os.path.join(pkg, 'config', 'traffic_light.yaml')
    vehicle_config = os.path.join(pkg, 'config', 'vehicle.yaml')

    return LaunchDescription([
        Node(
            package='traffic_sim',
            executable='traffic_light_node',
            name='traffic_light_node',
            parameters=[light_config],
            output='screen'
        ),
        Node(
            package='traffic_sim',
            executable='vehicle_node',
            name='vehicle_node',
            parameters=[vehicle_config],
            output='screen'
        ),
        Node(
            package='traffic_sim',
            executable='traffic_sim',
            name='traffic_sim_gui',
            output='screen'
        ),
    ])