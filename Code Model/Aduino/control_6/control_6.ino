#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Khởi tạo PCA9685 tại địa chỉ mặc định (0x40)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN  150  // Xung tối thiểu (servo về 0 độ)
#define SERVOMAX  635  // Xung tối đa (servo về 180 độ)
#define NEUTRAL   381  // Vị trí trung lập của servo
#define OFFSET0   30   // Offset cho servo 0
#define OFFSET5   30   // Offset cho servo 5

const int x0 = 360;
const int y0 = 240;
const int maxVector = 275;
const int maxServoDiff =29;

void setup() {
  Serial.begin(9600);
  Serial.println("6-Servo Test!");

  pwm.begin();
  pwm.setPWMFreq(60);  // Tần số PWM 60 Hz

  delay(10);
}

void loop() {
  // Thiết lập vị trí ban đầu của các servo
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

    // Tính toán tọa độ cho hai vector phụ
    int x1 = x;
    int y1 = y0;  // y1 = 240
    int x2 = x0;  // x2 = 360
    int y2 = y;

    // Tính độ lớn của vector 1 và vector 2
    float vector1Magnitude = sqrt(sq(x - x1) + sq(y - y1));
    float vector2Magnitude = sqrt(sq(x - x2) + sq(y - y2));

    // Tính độ lớn của vector từ tọa độ hiện tại
    int deltaX = x - x0;
    int deltaY = y - y0;
    float vectorMagnitude = sqrt(sq(deltaX) + sq(deltaY));

    // Điều kiện giữ các servo ở vị trí trung lập khi tọa độ x và y nằm trong khoảng nhất định
    if (x >= 280 && x <= 380 && y >= 215 && y <= 280) {
      vectorMagnitude = 0;
    }

    // Tính độ chênh lệch servo dựa trên độ lớn của vector
    //int servoDiff = map(vectorMagnitude, 0, maxVector, 0, maxServoDiff);
    //int servoDiff = map(vectorMagnitude, 0, maxVector, maxServoDiff,0);
    int servoDiff = map(vectorMagnitude, maxVector, 0, maxServoDiff,0);

    // Cập nhật giá trị servo dựa trên độ chênh lệch tính được
    if (vectorMagnitude > 0) {
      if (x < x0 && y < y0) {  // Nếu x < 360 và y < 240
        if (vector1Magnitude < vector2Magnitude) {
          servo0 = NEUTRAL - OFFSET0 + servoDiff;
          servo5 = NEUTRAL + OFFSET5 - servoDiff;
          servo1 = NEUTRAL + servoDiff;
          servo2 = NEUTRAL - servoDiff;
          servo3 = NEUTRAL + servoDiff;
          servo4 = NEUTRAL - servoDiff;
        } else {
          servo0 = NEUTRAL - OFFSET0 + servoDiff;
          servo5 = NEUTRAL + OFFSET5 - servoDiff;
          servo1 = NEUTRAL + servoDiff;
          servo2 = NEUTRAL - servoDiff;
          servo3 = NEUTRAL + servoDiff;
          servo4 = NEUTRAL - servoDiff;
        }
      } else if (x > x0 && y < y0) {  // Nếu x > 360 và y < 240
        if (vector1Magnitude < vector2Magnitude) {
          servo0 = NEUTRAL - OFFSET0 + servoDiff;
          servo5 = NEUTRAL + OFFSET5 - servoDiff;
          servo1 = NEUTRAL + servoDiff;
          servo2 = NEUTRAL - servoDiff;
          servo3 = NEUTRAL + servoDiff;
          servo4 = NEUTRAL - servoDiff;
        } else {
          servo0 = NEUTRAL - OFFSET0 + servoDiff;
          servo5 = NEUTRAL + OFFSET5 - servoDiff;
          servo1 = NEUTRAL + servoDiff;
          servo2 = NEUTRAL - servoDiff;
          servo3 = NEUTRAL + servoDiff;
          servo4 = NEUTRAL - servoDiff;
        }
      } else if (x < x0 && y > y0) {  // Nếu x < 360 và y > 240
        if (vector1Magnitude < vector2Magnitude) {
          servo0 = NEUTRAL - OFFSET0 - servoDiff;
          servo5 = NEUTRAL + OFFSET5 + servoDiff;
          servo1 = NEUTRAL + servoDiff;
          servo2 = NEUTRAL - servoDiff;
          servo3 = NEUTRAL - servoDiff;
          servo4 = NEUTRAL + servoDiff;
        } else {
          servo0 = NEUTRAL - OFFSET0 - servoDiff;
          servo5 = NEUTRAL + OFFSET5 + servoDiff;
          servo1 = NEUTRAL + servoDiff;
          servo2 = NEUTRAL - servoDiff;
          servo3 = NEUTRAL - servoDiff;
          servo4 = NEUTRAL + servoDiff;
        }
      } else if (x > x0 && y > y0) {  // Nếu x > 360 và y > 240
        if (vector1Magnitude < vector2Magnitude) {
          servo0 = NEUTRAL - OFFSET0 - servoDiff;
          servo5 = NEUTRAL + OFFSET5 + servoDiff;
          servo1 = NEUTRAL - servoDiff;
          servo2 = NEUTRAL + servoDiff;
          servo3 = NEUTRAL + servoDiff;
          servo4 = NEUTRAL - servoDiff;
        } else {
          servo0 = NEUTRAL - OFFSET0 - servoDiff;
          servo5 = NEUTRAL + OFFSET5 + servoDiff;
          servo1 = NEUTRAL - servoDiff;
          servo2 = NEUTRAL + servoDiff;
          servo3 = NEUTRAL - servoDiff;
          servo4 = NEUTRAL + servoDiff;
        }
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
