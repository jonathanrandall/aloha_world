import threading
import time
from queue import SimpleQueue
import tkinter as tk
import serial

# create the serial instance
ser = serial.Serial('/dev/ttyUSB0', 115200) # provide other parameters as well
# create the queue for holding serial data
queue = SimpleQueue()
# flag use to start/stop thread tasks
stop_flag = None

# root = tk.Tk()

def readSerial(queue):
    global stop_flag
    if stop_flag:
        print("Reading task is already running")
    else:
        print("started")
        stop_flag = threading.Event()
        while not stop_flag.is_set():
            if ser.in_waiting:
                ser_bytes = ser.readline()
                data = ser_bytes.decode("utf-8")
                queue.put(data)
                
                print(data)
                
            else:
                time.sleep(0.1)
        print("stopped")
        stop_flag = None

def start_reader():
    threading.Thread(target=readSerial, args=(queue,)).start()

def stop_reader():
    if stop_flag:
        stop_flag.set()
    else:
        print("Reading task is not running")

# function to monitor whether data is in the queue
# if there is data, get it and insert into the text box
def data_monitor(queue):
    if not queue.empty():
        text.insert("end", f"{queue.get()}\n")
    root.after(100, data_monitor, queue)

def main():
    start_reader()
    

    # text = tk.Text(root, width=80, height=20)
    # text.pack()

    # frame = tk.Frame(root)
    # tk.Button(frame, text="Start", command=start_reader).pack(side="left")
    # tk.Button(frame, text="Stop", command=stop_reader).pack(side="left")
    # frame.pack()

    # # start data monitor task
    # data_monitor(queue)

    # root.mainloop()

if __name__ == "__main__":
    main()
