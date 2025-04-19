import cv2
import torch
import serial
import time
import sys  # For termination

# Initialize YOLOv5 model
model = torch.hub.load('ultralytics/yolov5', 'yolov5s')  # Load YOLOv5s model
model.classes = [0]  # Filter for "person" class (COCO ID 0)qq

# Initialize serial communication
arduino = serial.Serial(port='COM4', baudrate=9600, timeout=1)  # Replace 'COM3' with your Arduino's port
time.sleep(2)  # Wait for Arduino to initialize

# Initialize webcam
cap = cv2.VideoCapture(0)

# Key for termination
TERMINATION_KEY = 'q'
print(f"Press '{TERMINATION_KEY}' to terminate the process.")

try:
    while True:
        ret, frame = cap.read()
        if not ret:
            print("Error: Unable to read from the webcam.")
            break

        # Perform object detection
        results = model(frame)

        # Check if a person is detected
        detected = any(results.xyxy[0][:, -1] == 0)  # Check if class '0' (person) is in the detections

        # Send signal to Arduino
        signal = '1' if detected else '0'
        arduino.write(signal.encode())
        
        # Display results
        cv2.imshow("YOLOv5 Detection", results.render()[0])
        
        # Break on termination key
        if cv2.waitKey(1) & 0xFF == ord(TERMINATION_KEY):
            print("Termination key pressed. Exiting...")
            break

except Exception as e:
    print(f"An error occurred: {e}")

finally:
    # Release resources
    print("Releasing resources...")
    cap.release()
    cv2.destroyAllWindows()
    arduino.write('0'.encode())  # Ensure LED is turned OFF
    arduino.close()
    print("Process terminated.")
