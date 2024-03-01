ros2 run robot_state_publisher robot_state_publisher --ros-args -p robot_description:="$( xacro example_robot.urdf.xacro)"
ros2 run joint_stat_publisher_gui joint_state_publisher_gui
