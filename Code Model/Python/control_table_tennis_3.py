import cv2
from ultralytics import YOLO
import serial
import time

# Load the YOLOv8 model
model = YOLO("E:/ktdktd_project/Yolov8_baseball/runs/detect/train18/weights/best.pt")

# Open the video file
video_path = 1
cap = cv2.VideoCapture(video_path)

# Initialize serial communication with Arduino
arduino = serial.Serial(port='COM4', baudrate=2000000, timeout=0.5)  # Adjust 'COM4' to your Arduino port
time.sleep(1)

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
    cv2.putText(frame, "(0, 0)", (10, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1, cv2.LINE_AA)

# Function to draw the X and Y axes on the frame
def draw_axes(frame, origin_x, origin_y):
    height, width, _ = frame.shape
    
    # Draw X axis
    cv2.line(frame, (0, origin_y), (width, origin_y), (0, 0, 255), 2)
    cv2.putText(frame, "X", (width - 20, origin_y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2, cv2.LINE_AA)

    # Draw Y axis
    cv2.line(frame, (origin_x, 0), (origin_x, height), (0, 0, 255), 2)
    cv2.putText(frame, "Y", (origin_x + 10, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2, cv2.LINE_AA)

# Function to draw the coordinate text on the frame
def draw_coordinates_text(frame, x, y, grid_size=50):
    coord_text = f"({x}, {y})"
    cv2.putText(frame, coord_text, (x + 10, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 1, cv2.LINE_AA)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    results = model(frame)

    for result in results:
        boxes = result.boxes
        for box in boxes:
            x1, y1, x2, y2 = box.xyxy[0]
            x_center = int((x1 + x2) / 2)
            y_center = int((y1 + y2) / 2)

            # Đọc giá trị servo từ Arduino
            arduino.write(f"{x_center},{y_center}\n".encode())
            time.sleep(0.05)  # Chờ Arduino xử lý

            # Đọc giá trị servo từ Arduino (servo0Signal, servo5Signal)
            if arduino.in_waiting > 0:
                response = arduino.readline().decode().strip()
                if "servo0Signal" in response and "servo5Signal" in response:
                    # Phân tích giá trị servo0Signal và servo5Signal
                    values = response.split(',')
                    servo0Signal = int(values[0].split(':')[1])
                    servo5Signal = int(values[1].split(':')[1])
                    print(f"Servo0: {servo0Signal}, Servo5: {servo5Signal}")

            draw_axes(frame, 320, 240)  # Assuming 640x480 resolution
            draw_grid(frame)
            draw_coordinates_text(frame, x_center, y_center)

            # Draw center point of the box
            cv2.circle(frame, (x_center, y_center), 5, (0, 255, 0), -1)

            # Draw the bounding box
            cv2.rectangle(frame, (int(x1), int(y1)), (int(x2), int(y2)), (0, 255, 0), 2)

    # Display the frame
    cv2.imshow("YOLOv8 Object Detection", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
