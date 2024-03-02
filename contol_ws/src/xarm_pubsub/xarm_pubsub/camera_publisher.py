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
import cv2


from sensor_msgs.msg import Image
from cv_bridge import CvBridge


data_queue = queue.SimpleQueue()


stop_flag_cam=None



#this function reads the data from the serial port
def get_image(data_queue):
    global stop_flag_cam
    stop_flag_cam = threading.Event()
    cap = cv2.VideoCapture(2)

# Check if the webcam is opened successfully
    if not cap.isOpened():
        print("Error: Cannot open webcam")
        exit()

    width = 640
    height = 480

    # Set the resolution
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, width)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, height)

    while not stop_flag_cam.is_set():
        ret, frame = cap.read()
        if not ret:
            print('failed to grab image')
            break
        data_queue.put(frame)
        cv2.imshow("RGB", frame)
        cv2.waitKey(1)
        


class CameraPublisher(Node):

    def __init__(self):
        super().__init__('camera_publisher')
        self.intel_publisher_rgb = self.create_publisher(Image, "rgb_frame", 10)
        timer_period = 0.75  # seconds
        self.br_rgb = CvBridge()
        self.timer = self.create_timer(timer_period, self.timer_callback)
        
        self.start_time = time.time()

    def timer_callback(self):
        
        data = None
        while not data_queue.empty():
            data = data_queue.get()
        
        if data is not None:
            self.intel_publisher_rgb.publish(self.br_rgb.cv2_to_imgmsg(data))
            
        

        
        
        


def main(args=None):
    serial_thread = threading.Thread(target=get_image, args=(data_queue,), daemon=False)
    serial_thread.start()
    rclpy.init(args=args)

    minimal_publisher = CameraPublisher()

    rclpy.spin(minimal_publisher)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    minimal_publisher.destroy_node()
    rclpy.shutdown()
    

if __name__ == '__main__':
    main()
