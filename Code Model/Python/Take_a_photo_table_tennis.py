import cv2
import tkinter as tk
from tkinter import ttk
from PIL import Image, ImageTk
import os

class CameraApp:
    def __init__(self, window, window_title, save_path):
        self.window = window
        self.window.title(window_title)
        self.save_path = save_path
        self.vid = cv2.VideoCapture(1)
        self.canvas = tk.Canvas(window, width=self.vid.get(cv2.CAP_PROP_FRAME_WIDTH), height=self.vid.get(cv2.CAP_PROP_FRAME_HEIGHT))
        self.canvas.pack()

        self.btn_snapshot = ttk.Button(window, text="Chụp ảnh", command=self.snapshot)
        self.btn_snapshot.pack(anchor=tk.CENTER, expand=True)

        self.photo_count = 0

        self.delay = 15
        self.update()
        self.window.mainloop()

    def snapshot(self):
        ret, frame = self.vid.read()
        if ret:
            # Resize frame to 640x360
            resized_frame = cv2.resize(frame, (640, 360))
            filename = os.path.join(self.save_path, f"Image_table_tennis_{self.photo_count}.jpg")
            cv2.imwrite(filename, resized_frame)  # Save image without color conversion
            print(f"Saved {filename}")
            self.photo_count += 1

    def update(self):
        ret, frame = self.vid.read()
        if ret:
            self.photo = ImageTk.PhotoImage(image=Image.fromarray(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)))
            self.canvas.create_image(0, 0, image=self.photo, anchor=tk.NW)
        self.window.after(self.delay, self.update)

    def __del__(self):
        if self.vid.isOpened():
            self.vid.release()

save_path = "C:/Users/Long/OneDrive/Pictures/table_tennis_img"  # Đường dẫn lưu ảnh
if not os.path.exists(save_path):
    os.makedirs(save_path)

root = tk.Tk()
app = CameraApp(root, "Camera App", save_path)
