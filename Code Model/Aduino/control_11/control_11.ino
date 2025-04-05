#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Khởi tạo PCA9685 tại địa chỉ mặc định (0x40)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN  150  // Xung tối thiểu (servo về 0 độ)
#define SERVOMAX  635  // Xung tối đa (servo về 180 độ)
#define NEUTRAL   381  // Vị trí trung lập của servo
#define OFFSET0   30   // Offset cho servo 0
#define OFFSET5   30   // Offset cho servo 5

const int x0 = 320;
const int y0 = 240;
const int maxVector = 275;
const int maxServoDiff = 50;

// int lastX = -1; // Lưu trữ giá trị x trước đó
// int lastY = -1; // Lưu trữ giá trị y trước đó

void setup() {
  Serial.begin(1000000);
  Serial.println("6-Servo Test!");

  pwm.begin();
  pwm.setPWMFreq(60);  // Tần số PWM 60 Hz

  delay(10);
}

void loop() {
  // Thiết lập vị trí trung lập của các servo
  int servo0 = NEUTRAL - OFFSET0;
  int servo1 = NEUTRAL;
  int servo2 = NEUTRAL;
  int servo3 = NEUTRAL;
  int servo4 = NEUTRAL;
  int servo5 = NEUTRAL + OFFSET5;

  // Kiểm tra xem có tín hiệu x và y từ Serial hay không
  if (Serial.available() > 0) {
    int x = Serial.parseInt();
    int y = Serial.parseInt();

    // Nếu x và y không thay đổi, bỏ qua quá trình tính toán và điều khiển servo
    // if (x == lastX && y == lastY) {
    //   delay(10);  // Chờ 10ms trước khi tiếp tục lặp lại
    //   return;
    // }

    // Cập nhật giá trị x và y hiện tại cho lần lặp sau
    // lastX = x;
    // lastY = y;

    // Tính toán tọa độ cho hai vector phụ
    // int deltaX = x - x0;
    // int deltaY = y - y0;
    float devdeltaXY = sqrt(sq(y-y0))/sqrt(sq(x-x0));
    float vectorMagnitude = sqrt(sq(x-x0) + sq(y-y0));

    // Tính toán tọa độ cho hai vector phụ
    int x1 = x;
    int y1 = y0;  // y1 = 240
    int x2 = x0;  // x2 = 320
    int y2 = y;

    // Tính độ lớn của vector 1 và vector 2
    float vector1Magnitude = sqrt(sq(x - x1) + sq(y - y1));
    float vector2Magnitude = sqrt(sq(x - x2) + sq(y - y2));

    // Tính góc nghiêng dựa trên tọa độ x và y
    float theta1 = atan(devdeltaXY); // Sử dụng atan2 để xử lý các góc ngoài khoảng [-π, π]
    float thetadegree1 = theta1 * (180 / PI);
    float servoangle_y = -thetadegree1 * (180 / 15);

    // Tính giá trị điều khiển cho servo 0 và 5
    int servo0SignalUp = NEUTRAL - OFFSET0 - (servoangle_y * (SERVOMAX - SERVOMIN) / 180);
    int servo5SignalUp = NEUTRAL + OFFSET5 + (servoangle_y * (SERVOMAX - SERVOMIN) / 180);

    int servo0SignalDown = NEUTRAL - OFFSET0 + (servoangle_y * (SERVOMAX - SERVOMIN) / 180);
    int servo5SignalDown = NEUTRAL + OFFSET5 - (servoangle_y * (SERVOMAX - SERVOMIN) / 180);

    int servoDiff = map(vectorMagnitude, maxVector, 0, maxServoDiff, 0);

    // Cập nhật các giá trị servo theo điều kiện
    if (x >= 260 && x <= 390) {
      if (y < 215) {
        servo0 = servo0SignalDown;
        servo5 = servo5SignalDown;
      } else if (y > 280) {
        servo0 = servo0SignalUp;
        servo5 = servo5SignalUp;
      }
    } if (x < 260 && y <= y0) {  // Nếu x < 260 và y <= 240
      if (vector1Magnitude < vector2Magnitude) {
        servo1 = NEUTRAL + servoDiff;
        servo2 = NEUTRAL - servoDiff;
        servo3 = NEUTRAL - servoDiff;
        servo4 = NEUTRAL + servoDiff;
      } else {
        servo0 = servo0SignalDown;
        servo5 = servo5SignalDown;
        servo1 = NEUTRAL + servoDiff;
        servo2 = NEUTRAL - servoDiff;
      }
    } if (x > 390 && y <= y0) {  // Nếu x > 390 và y <= 240
      if (vector1Magnitude < vector2Magnitude) {
        servo3 = NEUTRAL + servoDiff;
        servo4 = NEUTRAL - servoDiff;
        servo1 = NEUTRAL - servoDiff;
        servo2 = NEUTRAL + servoDiff;
      } else {
        servo0 = servo0SignalDown;
        servo5 = servo5SignalDown;
        servo3 = NEUTRAL + servoDiff;
        servo4 = NEUTRAL - servoDiff;
      }
    } if (x < 260 && y > y0) {  // Nếu x < 260 và y > 240
      if (vector1Magnitude < vector2Magnitude) {
        servo1 = NEUTRAL + servoDiff;
        servo2 = NEUTRAL - servoDiff;
        servo3 = NEUTRAL - servoDiff;
        servo4 = NEUTRAL + servoDiff;
      } else {
        servo0 = servo0SignalUp;
        servo5 = servo5SignalUp;
        servo1 = NEUTRAL + servoDiff;
        servo2 = NEUTRAL - servoDiff;
      }
    } if (x > 390 && y > y0) {  
      if (vector1Magnitude < vector2Magnitude) {
        servo3 = NEUTRAL + servoDiff;
        servo4 = NEUTRAL - servoDiff;
        servo1 = NEUTRAL - servoDiff;
        servo2 = NEUTRAL + servoDiff;
      } else {
        servo0 = servo0SignalUp;
        servo5 = servo5SignalUp;
        servo3 = NEUTRAL + servoDiff;
        servo4 = NEUTRAL - servoDiff;
      }
    }
  }

  // Điều khiển các cặp servo
  pwm.setPWM(0, 0, servo0);
  pwm.setPWM(5, 0, servo5);
  pwm.setPWM(1, 0, servo1);
  pwm.setPWM(2, 0, servo2);
  pwm.setPWM(3, 0, servo3);
  pwm.setPWM(4, 0, servo4);

  delay(10);  // Chờ 10ms trước khi tiếp tục lặp lại
}
