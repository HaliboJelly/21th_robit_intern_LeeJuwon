import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    pkg = get_package_share_directory('road_friend')
    light_config = os.path.join(pkg, 'config', 'traffic_light.yaml')
    vehicle_config = os.path.join(pkg, 'config', 'vehicle.yaml')

    return LaunchDescription([
        Node(
            package='road_friend',
            executable='traffic_light_node',
            name='traffic_light_node',
            parameters=[light_config],
            output='screen'
        ),
        Node(
            package='road_friend',
            executable='vehicle_node',
            name='vehicle_node',
            parameters=[vehicle_config],
            output='screen'
        ),
        Node(
            package='road_friend',
            executable='road_friend_gui',
            name='road_friend_gui',
            output='screen'
        ),
        Node(package='road_friend', executable='player_node', name='player_node',
             parameters=[os.path.join(pkg, 'config', 'player.yaml')], output='screen'),
        Node(package='road_friend', executable='game_node', name='game_node', output='screen'),
       
    ])