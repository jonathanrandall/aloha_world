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



import time
import numpy as np
import cv2


from sensor_msgs.msg import Image
import sensor_msgs.msg
from cv_bridge import CvBridge

import threading
import queue

image_queue = queue.SimpleQueue()

#I don't use this
# def disp_image(image_queue):
#     # global stop_flag_cam
#     stop_flag_cam = threading.Event()
    

#     width = 640
#     height = 480
#     data = None
#     while not stop_flag_cam.is_set():
#         data = None
#         while not image_queue.empty():
#             data = image_queue.get()
#         # print(data.shape)
#         if data is not None:
#             cv2.imshow("RGB", data)
#             cv2.waitKey(1)



class CameraSubscriber(Node):

    def __init__(self, arg1='camera_subscriber'):
        super().__init__(arg1)
        self.subscription = self.create_subscription(
            sensor_msgs.msg.Image,
            '/rgb_frame', #'joint_states',
            self.listener_callback, 
            10)
        self.subscription  # prevent unused variable warning
        self.start_time = time.time()
        
        self.br_rgb = CvBridge()
        
        self.data_dict = {
            '/observations/images/top': [],
            # '/observations/qvel': [],
            # '/observations/effort': [],
            # '/action': [],
            }
        
        self.record_params = {"stop": False, "start":False}
        self.current_frame = np.zeros((480,640, 3), dtype=np.uint8)
        # self.current_frame = {"frame":cf}
        time.sleep(1)
        print(self.current_frame.shape)

        # serial_thread = threading.Thread(target=disp_image, args=(image_queue,), daemon=False)
        # serial_thread.start()


    def listener_callback(self, data):
        
        # self.get_logger().warning(f'Receiving RGB frame {self.current_frame.shape}')
        self.current_frame = self.br_rgb.imgmsg_to_cv2(data)
        # image_queue.put(self.current_frame)
        # self.current_frame = np.array(self.current_frame).astype(np.uint8)
        # cv2.imshow("RGB", self.current_frame)
        if self.record_params["start"]:
            self.data_dict['/observations/images/top'].append(self.current_frame)
        # cv2.waitKey(1)
        

        
        
        


def main(args=None):
    rclpy.init(args=args)

    camera_subscriber = CameraSubscriber()

    rclpy.spin(camera_subscriber)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    camera_subscriber.destroy_node()
    rclpy.shutdown()
    

if __name__ == '__main__':
    main()
