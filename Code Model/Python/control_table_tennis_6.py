import cv2
from ultralytics import YOLO
import serial
import time
import math
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from threading import Thread, Event

# Load the YOLOv8 model
model = YOLO("E:/ktdktd_project/Yolov8_baseball/runs/detect/train18/weights/best.pt")

# Open the video file
video_path = 1
cap = cv2.VideoCapture(video_path)

# Initialize serial communication with Arduino
arduino = serial.Serial(port='COM4', baudrate=921600)  # Adjust 'COM4' to your Arduino port
time.sleep(1)

# Initialize lists for plotting
time_data = []
y_data = []
sampling_rate = 100  # 100ms between samples
start_time = time.time()

# Function to draw grid on the frame
def draw_grid(frame, grid_size=50):
    height, width, _ = frame.shape
    
    # Draw vertical lines
    for x in range(0, width, grid_size):
        cv2.line(frame, (x, 0), (x, height), (255, 255, 255), 1)

    # Draw horizontal lines
    for y in range(0, height, grid_size):
        cv2.line(frame, (0, y), (width, y), (255, 255, 255), 1)
    
    # Draw the origin point (top-left corner)
    cv2.putText(frame, "(0, 0)", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)

# Initialize a list to store PWM values for smoothing
pwm_ser_values = []

# Function to handle video processing and object detection
def process_video(stop_event):
    global pwm_ser_values
    while not stop_event.is_set():
        success, frame = cap.read()
        if not success:
            break

        height, width, _ = frame.shape
        frame_center = (width // 2, height // 2)
        cv2.circle(frame, frame_center, 5, (255, 0, 0), -1)  # Blue color in BGR

        results = model(frame)
        for r in results:
            for box in r.boxes:
                coordinates = (box.xyxy).tolist()[0]
                left, top, right, bottom = coordinates[0], coordinates[1], coordinates[2], coordinates[3]
                center = (int((left + right) / 2), int((top + bottom) / 2))
                
                if center[0] is not None and center[1] is not None:
                    # Send center coordinates to Arduino
                    data = f"{center[0]},{center[1]}\n"
                    arduino.write(data.encode('utf-8'))

                    cv2.circle(frame, center, 3, (0, 0, 255), -1)
                    text = f"({center[0]}, {center[1]})"
                    text_size, _ = cv2.getTextSize(text, cv2.FONT_HERSHEY_SIMPLEX, 0.7, 2)
                    text_x = width - text_size[0] - 10
                    text_y = height - 10
                    cv2.putText(frame, text, (text_x, text_y), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)

        draw_grid(frame)
        annotated_frame = results[0].plot()
        cv2.imshow("YOLOv8 Inference", annotated_frame)

        if cv2.waitKey(1) & 0xFF == ord("q"):
            stop_event.set()
            break

    cap.release()
    cv2.destroyAllWindows()
    arduino.close()

# Function to update the plot
def update_plot(frame):
    global time_data, y_data
    if arduino.in_waiting > 0:
        line = arduino.readline().decode('utf-8').strip()
        if line:
            try:
                # Assuming the format "x,y"
                parts = line.split(',')
                if len(parts) == 2:
                    y_value = int(parts[1])
                    current_time = time.time() - start_time

                    time_data.append(current_time)
                    y_data.append(y_value)

                    ax.clear()
                    ax.plot(time_data, y_data, label='center[1] Value')
                    ax.set_xlabel('Time (s)')
                    ax.set_ylabel('center[1] Value')
                    ax.set_title('Center Y Coordinate over Time')
                    ax.legend()
            except ValueError:
                # Handle the case where the data format is unexpected
                pass

# Set up the plotting window
fig, ax = plt.subplots()
ani = animation.FuncAnimation(fig, update_plot, interval=sampling_rate)

# Create an event to signal the video processing thread to stop
stop_event = Event()

# Start video processing in a separate thread
video_thread = Thread(target=process_video, args=(stop_event,))
video_thread.start()

# Show the plot
plt.show()

# Wait for the video processing thread to finish
video_thread.join()
