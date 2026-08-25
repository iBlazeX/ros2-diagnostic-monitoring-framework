from launch import LaunchDescription
from launch_ros.actions import Node

from ament_index_python.packages import get_package_share_directory

import os


def generate_launch_description():

    config = os.path.join(
        get_package_share_directory("diagnostic_monitor"),
        "config",
        "expected_nodes.yaml"
    )

    return LaunchDescription([
        Node(
            package="diagnostic_monitor",
            executable="diagnostic_monitor",
            name="diagnostic_monitor",
            output="screen",
            parameters=[config]
        )
    ])
