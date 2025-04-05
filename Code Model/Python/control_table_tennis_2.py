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
arduino = serial.Serial(port='COM4', baudrate=1000000)  # Adjust 'COM4' to your Arduino port
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
    cv2.putText(frame, "(0, 0)", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)

# Loop through the video frames
while cap.isOpened():
    # Read a frame from the video
    success, frame = cap.read()

    if success:
        # Print the size of the input frame
        height, width, _ = frame.shape
        print(f"Frame size: Width={width}, Height={height}")

        # Calculate the center of the frame
        frame_center = (width // 2, height // 2)

        # Draw a circle at the center of the frame
        cv2.circle(frame, frame_center, 5, (255, 0, 0), -1)  # Blue color in BGR

        # Run YOLOv8 inference on the frame
        results = model(frame)
        for r in results:
            for box in r.boxes:
                coordinates = (box.xyxy).tolist()[0]
                print(coordinates)  # returns the list of bounding box coordinates

                left, top, right, bottom = coordinates[0], coordinates[1], coordinates[2], coordinates[3]
                center = (int((left + right) / 2), int((top + bottom) / 2))
                print("Center: ", center)

                # Check if the center coordinates are valid before sending to Arduino
                if center[0] is not None and center[1] is not None:
                    # Send the center coordinates to Arduino
                    data = f"{center[0]},{center[1]}\n"
                    arduino.write(data.encode('utf-8'))

                    # Draw a small circle at the center with a radius of 3
                    cv2.circle(frame, center, 3, (0, 0, 255), -1)  # Red color in BGR

                    # Display the coordinates of the center at the bottom-right corner of the frame
                    text = f"({center[0]}, {center[1]})"
                    text_size, _ = cv2.getTextSize(text, cv2.FONT_HERSHEY_SIMPLEX, 0.7, 2)
                    text_x = width - text_size[0] - 10
                    text_y = height - 10
                    cv2.putText(frame, text, (text_x, text_y), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)

        # Draw grid on the frame
        draw_grid(frame)

        # Visualize the results on the frame
        annotated_frame = results[0].plot()

        # Display the annotated frame
        cv2.imshow("YOLOv8 Inference", annotated_frame)

        # Break the loop if 'q' is pressed
        if cv2.waitKey(1) & 0xFF == ord("q"):
            break
    else:
        # Break the loop if the end of the video is reached
        break

# Release the video capture object and close the display window
cap.release()
cv2.destroyAllWindows()
arduino.close()
