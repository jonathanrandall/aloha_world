# this is the modified file for the robot environment for inference.

import xarm
import threading
import queue
import time
import numpy as np
import cv2
import subprocess
from collections import deque
import collections
import dm_env

from xarm_pubsub.camera_publisher import find_webcam_index

data_queue = deque(maxlen=30)
class ModifiedEnv:
    
    def __init__(self) -> None:
        self.data_queue = deque(maxlen=30)
        self.robot_state=6*[500]
        
        
        self.robot = self.connect_to_robot()
        
        serial_thread = threading.Thread(target=self.read_camera, args=(data_queue,), daemon=False)
        serial_thread.start()
        
    def get_reward(self):
        return 0

    def connect_to_robot(self):
        try:
            arm = xarm.Controller("USB")
            return arm
        except:
            print("Problem connecting to robot. Check connections")
            return None   
    
    def plonkal(self):
        for i in range(20):
            print('jonny is the best')
    
    def get_robot_state(self):
        for i in range(0,6):
            self.robot_state[i] = int(self.robot.getPosition(i+1))
            time.sleep(0.02)
        return self.robot_state
    
    def set_robot_state(self, rs=None):
        wa = False
        for i in range(0,6):
            # wa = True if i == 5 else False
            if rs is not None:
                self.robot.setPosition(i+1, rs[i], wait=wa)
            else:
                self.robot.setPosition(i+1, self.robot_state[i], wait=wa)
            
    def step(self, action):
        self.robot_state = action
        self.set_robot_state()
        time.sleep(0.02)
        return dm_env.TimeStep(
            step_type=dm_env.StepType.MID,
            reward=self.get_reward(),
            discount=None,
            observation=self.get_observation())
    
    def reset(self, fake=False):
        return self.step(6*[500])


    def get_observation(self):
        obs = collections.OrderedDict()
        obs['qpos'] = self.get_robot_state()
        obs['images'] = self.get_images()
        return obs

    def get_images(self):
        image_dict = dict()
        image_dict['top'] = data_queue.pop()
        return image_dict

    def read_camera(self, dq):
        global stop_flag_cam
        stop_flag_cam = threading.Event()

        webcam_name = "C922 Pro Stream Webcam (usb-0000:00:14.0-4):"

    # Find the index of the webcam
        webcam_index = int(find_webcam_index(webcam_name))

        cap = cv2.VideoCapture(webcam_index)

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
            dq.append(frame)
            cv2.imshow("RGB", frame)
            cv2.waitKey(1)
        


def main(args=None):
    t1 = ModifiedEnv()
    time.sleep(1)
    t1.plonkal()
    # while True:
    #     img_d=t1.get_images()
    #     print(img_d['top'].size)
    time.sleep(2)
    ts = t1.reset()
    # print(ts.observation)



if __name__ == '__main__':
    main()
