import serial
import threading

def serial_reader(ser):
    while True:    
        data = ser.readline().strip().decode()
        print("Received:", data)

def main():
    ser = serial.Serial('/dev/ttyUSB0', 115200)  # Adjust port and baud rate as needed

    # Create a thread for serial reading
    serial_thread = threading.Thread(target=serial_reader, args=(ser,), daemon=True)
    serial_thread.start()

    try:
        while True:
            # Your main program logic goes here
            pass
    except KeyboardInterrupt:
        print("\nCtrl+C pressed. Exiting gracefully.")
    finally:
        ser.close()

if __name__ == "__main__":
    main()
