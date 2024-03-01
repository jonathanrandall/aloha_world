import serial
import threading
import queue
import time

## good one
data_queue = queue.SimpleQueue()

ser = serial.Serial('/dev/ttyUSB0', 115200) 
stop_flag=None
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

def do_stuff():
    while True:
        time.sleep(0.5)
        if not data_queue.empty():
            print(data_queue.qsize())
            while not data_queue.empty():
                data=data_queue.get()
            # print(data_queue.qsize())
            print(data)

def main():
    #ser = serial.Serial('/dev/ttyUSB0', 115200)  # Adjust port and baud rate as needed
      # Create a queue for storing serial data

    # Create a thread for serial reading
    serial_thread = threading.Thread(target=serial_reader, args=(data_queue,), daemon=False)
    serial_thread.start()

    serial_thread2 = threading.Thread(target=do_stuff, daemon=False)
    serial_thread2.start()
    print("reader started")

    # try:
        
    #     # while True:
    #     #     # Dequeue data from the queue
    #     #     if not data_queue.empty():
    #     #         data = data_queue.get_nowait()
    #     #         #print("Received:", data)

    #     #     # Your main program logic goes here

    # except KeyboardInterrupt:
    #     print("\nCtrl+C pressed. Exiting gracefully.")
    # finally:
    #     ser.close()

if __name__ == "__main__":
    main()
