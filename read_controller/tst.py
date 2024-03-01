import serial
import time

ser = serial.Serial(port='/dev/ttyUSB0', baudrate=115200)  # Adjust port and baud rate as needed


line = ser.readline().decode("utf-8")
print(line)

# while True:
#     # ser = serial.Serial('/dev/ttyUSB0', 115200) 
#     data = ser.read().strip().decode()
#     integers = [int(x) for x in line.split()]
    
    
#     # ser.flushInput()
#     print(data)
    

ser.close()