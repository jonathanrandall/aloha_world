# aloha_world

## Equipment used for this project:

__Xarm robot arm and synchronization controller__
https://www.hiwonder.com/products/xarm-1s?variant=32436121894999&_pos=1&_sid=57c2ee4b5&_ss=r

__webcam__
I used a logitech C922, but any decent webcam should work.

## I will now describe the directories:

### Sync
The directory contains the Arduino sketch for the synchronisation controller. This is the master that I used to control the robot. They synchronisation controller has an Arduion Nano that is used to read the joint angles and output them to the serial monitor.

I have updated the Arduino sketch from the hiwonder documentation, so that it doesn't output when there is an error and it prints to the serial monitor every 50ms. I have used the Arduino IDE to upload to the Nano. Note, to get this to work with Linux, you need to remove the brail terminal via:


```bash
sudo apt remov brltty
```






