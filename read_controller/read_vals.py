import time

import serial

# Open the serial port
ser = serial.Serial('/dev/ttyUSB0', 115200)  # You need to specify your serial port and baud rate
c = ''
value = ''
# ser.close()
try:
    while True:
        while c != '\r':
            c = ser.read(1).decode("utf-8")
            if (c == ''):
                print("Error: Serial timeout on command: ")
                break
            value += c

        print(value)
        time.sleep(1)
except KeyboardInterrupt:
        ser.close()


while True and False:
# Read a line from the serial port
    
    line = ser.readline().decode("utf-8")  # Read the line and decode bytes to string

    # Split the line into individual integers
    integers = [int(x) for x in line.split()]

    # Print out the integers
    print("Received integers:", integers)
    #ser.flushInput()
    #ser.flushOutput()

    time.sleep(1)

# Close the serial port
ser.close()