import cv2
from ultralytics import YOLO

# Load the YOLOv8 model
model = YOLO("E:/ktdktd_project/Yolov8_baseball/runs/detect/train18/weights/best.pt")

# Open the video file
video_path = 1
cap = cv2.VideoCapture(video_path)

# Loop through the video frames
while cap.isOpened():
    # Read a frame from the video
    success, frame = cap.read()

    if success:
        # Print the size of the input frame
        height, width, _ = frame.shape
        print(f"Frame size: Width={width}, Height={height}")

        # Run YOLOv8 inference on the frame
        results = model(frame)
        for r in results:
            for box in r.boxes:
                coordinates = (box.xyxy).tolist()[0]
                print(coordinates)  # returns the list of bounding box coordinates

                left, top, right, bottom = coordinates[0], coordinates[1], coordinates[2], coordinates[3]
                center = (int(left+right)/2, int(top+bottom)/2)
                print("Center: ", center)

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
