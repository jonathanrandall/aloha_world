import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.substitutions import LaunchConfiguration
from launch.actions import DeclareLaunchArgument
from launch_ros.actions import Node

import xacro


def generate_launch_description():

    
    node_robot_state_publisher = Node(
        package='xarm_pubsub',
        executable='master',
        name='right_master',#'master',
        remappings=[('/joint_states', '/right/joint_states')]
    )

    node_robot_subscriber = Node(
        package='xarm_pubsub',
        executable='puppet',
        name='right_puppet',
        remappings=[('/joint_states', '/right/joint_states')]
        # parameters=[{'topic_name': '/right/joint_states'}]
    )

    node_robot_subscriber_gui = Node(
        package='xarm_pubsub',
        executable='record_gui',
        # name='right_puppet_gui',
        # arguments=['--arg1', 'value1', '--arg2', 'value2'],
        remappings=[('/joint_states', '/right/joint_states')]
        # parameters=[{'topic_name': '/right/joint_states'}]
    )

    node_robot_image_pub = Node(
        package='xarm_pubsub',
        executable='image_pub',
        name='image_pub',
        # remappings=[('/joint_states', '/right/joint_states')]
        # parameters=[{'topic_name': '/right/joint_states'}]
    )

    node_robot_image_sub = Node(
        package='xarm_pubsub',
        executable='image_sub',
        name='image_sub',
        # remappings=[('/joint_states', '/right/joint_states')]
        # parameters=[{'topic_name': '/right/joint_states'}]
    )

    


    # Launch!
    return LaunchDescription([

        node_robot_state_publisher,
        node_robot_image_pub,
        # node_robot_image_sub,
        node_robot_subscriber_gui,
        # node_robot_image_pub
        
    ])
