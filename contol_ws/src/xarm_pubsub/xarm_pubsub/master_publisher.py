# Copyright 2016 Open Source Robotics Foundation, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import rclpy
from rclpy.node import Node

from std_msgs.msg import String
from std_msgs.msg import Float64MultiArray
import sensor_msgs.msg

import std_msgs.msg



#serial reading imports
import serial
import threading
import queue
import time
import numpy as np


data_queue_mp = queue.SimpleQueue()

ser = serial.Serial('/dev/ttyUSB0', 115200) 
stop_flag=None

original_range = [850, 525]
target_range = [0, 750] #-135 degrees to +135 degrees

#this function reads the data from the serial port
def serial_reader(data_queue):
    global stop_flag
    stop_flag = threading.Event()
    while not stop_flag.is_set():
        
        if ser.in_waiting:
            # data = ser.readline().strip().decode()
            ser_bytes = ser.readline()
            data = ser_bytes.decode("utf-8")
            data = [int(x) for x in data.split()]
            data_queue.put(data)  # Put data into the queue
            # print(data)
            # print(data_queue.get_nowait())
        else:
            time.sleep(0.1)


class MinimalPublisher(Node):

    def __init__(self):
        super().__init__('minimal_publisher')
        self.publisher_ = self.create_publisher(sensor_msgs.msg.JointState, '/joint_states', 10)
        timer_period = 0.75  # seconds
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.i = 0
        self.sync_data=6*[500]
        self.prev_sync_data = 6*[500]
        self.start_time = time.time()

    def timer_callback(self):
        msg = sensor_msgs.msg.JointState()
        
        msg.header.stamp = self.get_clock().now().to_msg()
        t0 = time.time()
        #get the serial input from the master
        data =[]
        while not data_queue_mp.empty():
            data = data_queue_mp.get()
        
        if len(data):
            self.sync_data = data
            # print(data)
            
        self.prev_sync_data = self.sync_data
        msg.position = self.sync_data
        msg.position.append(t0)
        # msg.position = self.sync_data
        self.publisher_.publish(msg)
        


def main(args=None):
    serial_thread = threading.Thread(target=serial_reader, args=(data_queue_mp,), daemon=False)
    serial_thread.start()
    rclpy.init(args=args)

    minimal_publisher = MinimalPublisher()

    rclpy.spin(minimal_publisher)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    minimal_publisher.destroy_node()
    rclpy.shutdown()
    

if __name__ == '__main__':
    main()
