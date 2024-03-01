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

import h5py
import os

import numpy as np
import time

import rclpy
from rclpy.node import Node

from std_msgs.msg import String
import sensor_msgs.msg
import tkinter as tk
import xarm
import threading
from threading import Lock

# from .record_gui import myGUI, stp_ep, strt_ep

# strt_ep = 0
# stp_ep = 0

original_range = [150, 500]
target_range = [0, 750] #-135 degrees to +135 degrees

data_dict = {
        '/observations/qpos': [],
        # '/observations/qvel': [],
        # '/observations/effort': [],
        '/action': [],
    }

actual_dt_history = []

max_timesteps=0
dataset_path = '~/projects/aloha_world/episodes/'#'~/tmp/my_episode'


def get_auto_index(dataset_dir, dataset_name_prefix = '', data_suffix = 'hdf5'):
    max_idx = 1000
    if not os.path.isdir(dataset_dir):
        os.makedirs(dataset_dir)
    for i in range(max_idx+1):
        if not os.path.isfile(os.path.join(dataset_dir, f'{dataset_name_prefix}episode_{i}.{data_suffix}')):
            return i
    raise Exception(f"Error getting auto index, or more than {max_idx} episodes")


def record_episode():
    global dataset_path
    file_path = os.path.expanduser(dataset_path)
    i = get_auto_index(file_path)
    file_path = os.path.join(file_path, f'episode_{i}.hdf5')
    with h5py.File(file_path, 'w', rdcc_nbytes=1024**2*2) as root:
        root.attrs['sim'] = False
        obs = root.create_group('observations')
        _ = obs.create_dataset('qpos', (max_timesteps, 6))
        # _ = obs.create_dataset('qvel', (max_timesteps, 14))
        # _ = obs.create_dataset('effort', (max_timesteps, 14))
        _ = root.create_dataset('action', (max_timesteps, 6))

        for name, array in data_dict.items():
            root[name][...] = array
    # print(f'Saving: {time.time() - t0:.1f} secs')

    return True

class MinimalSubscriber(Node):

    def __init__(self):
        super().__init__('minimal_subscriber')
        self.declare_parameter('topic_name', '/joint_states')
        topic_name = self.get_parameter('topic_name').value
        self.subscription = self.create_subscription(
            sensor_msgs.msg.JointState,
            topic_name, #'joint_states',
            self.listener_callback,
            10)
        self.subscription  # prevent unused variable warning
        self.start_time = time.time()
        self.pos_prev = 6*[500]
        self.mutex = Lock()
        self.robot = self.connect_to_robot()
        
        if self.robot is None:
            print("Exiting program")
            exit()
        for i in range(0,6):
            self.robot.setPosition(i+1, self.pos_prev[i], wait=False)
        
        self.record_params = {"stop": False, "start":False}
        


    

    def listener_callback(self, msg):
        # self.mutex.acquire()
        global max_timesteps#=max_timesteps+1
        global strt_ep
        global stp_ep
        
        positions = msg.position
        # Convert the list of positions to a string
        positions_str = ', '.join(str(pos) for pos in positions)
        positions_str=positions_str+', '+str(max_timesteps)
        positions_int = [int(x) for x in positions]
        #I hve to adjust the gripper val because the synchronisatino controller is wired in the opposit direction
        #as the robot arm. 
        gripper_val = np.interp(1000-positions_int[0],original_range,target_range)
        positions_int[0] = int(gripper_val)
        # self.get_logger().info("Joint positions: %s" % positions_str)
        
        t1 = time.time()
        t0 = float(positions[6])
        for i in range(0,6):
            self.pos_prev[i] = int(self.robot.getPosition(i+1))
            time.sleep(0.02)
            pos_now = int(self.pos_prev[i])
            # self.robot.setPosition(i+1, positions_int[i], wait=False)
            # pos_now = int(self.robot.getPosition(i+1))
            pos_next = positions_int[i]
            if (abs(pos_now-pos_next)>20): #this will stop vibrations on the robot puppet arm
                self.robot.setPosition(i+1, positions_int[i], wait=False)
                self.pos_prev[i] = pos_next
                # self.get_logger().info("setting positions")
        t2 = time.time()
        if self.record_params["start"]:
            max_timesteps=max_timesteps+1
            actual_dt_history.append([t0, t1, t2])
            data_dict['/observations/qpos'].append(self.pos_prev)
            data_dict['/action'].append(positions_int[:6])
        if self.record_params["stop"]: #(t2-self.start_time > 20):
            # stp_ep = False
            # strt_ep = False
            self.record_params["stop"]=False
            self.record_params["start"]=False
            self.get_logger().info("episode recoreded")
            record_episode()
            # self.mutex.release()
            time.sleep(1)
        # self.mutex.release()


    def connect_to_robot(self):
        try:
            arm = xarm.Controller("USB")
            return arm
        except:
            print("Problem connecting to robot. Check connections")
            return None
        

def main(args=None):
    # gui = myGUI()
    
    # gui.thread_gui.start()
    rclpy.init(args=args)

    minimal_subscriber = MinimalSubscriber()
    

    rclpy.spin(minimal_subscriber)
    # gui.root.mainloop()

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    minimal_subscriber.destroy_node()
    rclpy.shutdown()

    # thread_node.join()

    
    
    # thread_gui = threading.Thread(target=gui.root.mainloop, daemon=False).start()
    


if __name__ == '__main__':
    main()
