# aloha_world

## Equipment used for this project:

__Xarm robot arm and synchronization controller__
https://www.hiwonder.com/products/xarm-1s?variant=32436121894999&_pos=1&_sid=57c2ee4b5&_ss=r

The xarm library for python is [here](https://github.com/ccourson/xArmServoController)

__webcam__
I used a logitech C922, but any decent webcam should work.

## I will now describe the directories:

### Sync
The directory contains the Arduino sketch for the synchronisation controller. This is the master that I used to control the robot. They synchronisation controller has an Arduion Nano that is used to read the joint angles and output them to the serial monitor.

I have updated the Arduino sketch from the hiwonder documentation, so that it doesn't output when there is an error and it prints to the serial monitor every 50ms. I have used the Arduino IDE to upload to the Nano. Note, to get this to work with Linux, you need to remove the brail terminal via:

```bash
sudo apt remov brltty
```

### contol_ws/src/xarm_pubsub
This contains the ros2 package that I use to record the robot actions for imitation learning.
Need to have ros2 installed. I'm using Iron Irwini.
You can build with colcon by typing from the contol_ws directory:
```bash
colcon build --symlink-install
```

### contol_ws/src/xarm_pubsub/xarm_pubsub/
This directory contains the main code for the ros2 package, which is just written with publishers and subsribers.
files are as follows:
__master_publisher.py__
This publishers the data from the synchronization controller to the ros2 topic /joint_states. It first launches a thread to read the data from the serial input. This is because the data on the serial input will buffer and we need to get the latest reading. So, it puts the data into a queue and when we want to read it for publishing, we throw away everything in the queue and just take the most recent output. 
__puppet_subscriber.py__
This subscribes to the /joint_states topic and sends the data to the robot.
__camera_publisher.py__
This publishers data from the webcam onto the /Image topic. Again, this runs a get camera on thread and then when it wants to publish the image, it throws away everything in the queue except the most recent image. This is because the images can buffer on the input, which leads to a lag.
__camera_subscriber.py__
This subscribes to the /Image topic.
__record_gui.py__
This has the gui for starting and stopping the recordings. This uses the python_qt_binding library (which I think is equivalent to PyQt5?). Note, I needed to put in the following line, otherwise there was a clash between opencv and the python_qt_binding libarary.
```python
#need this or else opencv doesn't work with PyQt5
import os
os.environ['QT_QPA_PLATFORM_PLUGIN_PATH'] = '/usr/lib/x86_64-linux-gnu/qt5/plugins'
```
To get the `QT_QPA_PLATFORM_PLUGIN_PATH`, I asked chatgpt to write me some code.








