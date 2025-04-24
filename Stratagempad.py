import serial
import time
from pynput.keyboard import Key, Controller

# Set up serial communication
ser = serial.Serial('COM4', 9600, timeout=1)

# Initialize keyboard controller
keyboard = Controller()

print("Waiting for data...")

while True:
    # Read a line of input from the serial port
    data = ser.readline().decode('utf-8', errors='ignore').strip()
    
    if data:
        # Simulate keyboard input based on the received data
        if data == "UP_ARROW":
            print("Up arrow pressed")
            keyboard.press("w")
            time.sleep(0.1)  # Small delay for proper input recognition
            keyboard.release("w")
        elif data == "DOWN_ARROW":
            print("Down arrow pressed")
            keyboard.press("s")
            time.sleep(0.1)
            keyboard.release("s")
        elif data == "LEFT_ARROW":
            print("Left arrow pressed")
            keyboard.press("a")
            time.sleep(0.1)
            keyboard.release("a")
        elif data == "RIGHT_ARROW":
            print("Right arrow pressed")
            keyboard.press("d")
            time.sleep(0.1)
            keyboard.release("d")
        elif data == "SHIFT":
            print("TOGGLE STRATAGEM")
            keyboard.press(Key.shift)
            time.sleep(0.1)
            keyboard.release(Key.shift)