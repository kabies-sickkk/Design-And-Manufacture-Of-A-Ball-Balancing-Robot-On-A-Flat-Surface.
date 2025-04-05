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

void setup() {
  Serial.begin(921600);  // Khởi tạo Serial với tốc độ baud rate cao
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

  if (Serial.available() > 0) {
    int x = Serial.parseInt();
    int y = Serial.parseInt();
    int pwm_ser = Serial.parseInt();

    // Tính toán tọa độ cho hai vector phụ
    int x1 = x;
    int y1 = y0;
    int x2 = x0;
    int y2 = y;

    // Tính độ lớn của vector 1 và vector 2
    float vector1Magnitude = sqrt(sq(x - x1) + sq(y - y1));
    float vector2Magnitude = sqrt(sq(x - x2) + sq(y - y2));

    // Tính giá trị điều khiển cho servo 0 và 5
    int servo0Signal = NEUTRAL - OFFSET0 - pwm_ser;
    int servo5Signal = NEUTRAL + OFFSET5 + pwm_ser;
    int servo0SignalD = NEUTRAL - OFFSET0 + pwm_ser;
    int servo5SignalD = NEUTRAL + OFFSET5 - pwm_ser;

    // Tính giá trị servoDiff
    float vectorMagnitude = sqrt(sq(x - x0) + sq(y - y0));
    int servoDiff = map(vectorMagnitude, 0, maxVector, 0, maxServoDiff);

    // Cập nhật giá trị servo theo điều kiện
    if (x >= 260 && x <= 390) {
      if (y <= 240) {
        servo0 = servo0SignalD;
        servo5 = servo5SignalD;
        servo1 = NEUTRAL + pwm_ser;
        servo2 = NEUTRAL - pwm_ser;
        servo3 = NEUTRAL + pwm_ser;
        servo4 = NEUTRAL - pwm_ser;
      } else {
        servo0 = servo0Signal;
        servo5 = servo5Signal;
        servo1 = NEUTRAL - pwm_ser;
        servo2 = NEUTRAL + pwm_ser;
        servo3 = NEUTRAL - pwm_ser;
        servo4 = NEUTRAL + pwm_ser;
      }
    } else if (x < 260 && y <= 240) {
      if (vector1Magnitude < vector2Magnitude) {
        servo0 = NEUTRAL - OFFSET0;
        servo5 = NEUTRAL + OFFSET5;
        servo1 = NEUTRAL + servoDiff;
        servo2 = NEUTRAL - servoDiff;
        servo3 = NEUTRAL - servoDiff;
        servo4 = NEUTRAL + servoDiff;
      } else {
        servo0 = servo0SignalD;
        servo5 = servo5SignalD;
        servo1 = NEUTRAL + pwm_ser;
        servo2 = NEUTRAL - pwm_ser;
        servo3 = NEUTRAL + pwm_ser;
        servo4 = NEUTRAL - pwm_ser;
      }
    } else if (x > 390 && y <= 240) {
      if (vector1Magnitude < vector2Magnitude) {
        servo0 = NEUTRAL - OFFSET0;
        servo5 = NEUTRAL + OFFSET5;
        servo3 = NEUTRAL + servoDiff;
        servo4 = NEUTRAL - servoDiff;
        servo1 = NEUTRAL - servoDiff;
        servo2 = NEUTRAL + servoDiff;
      } else {
        servo0 = servo0SignalD;
        servo5 = servo5SignalD;
        servo1 = NEUTRAL + pwm_ser;
        servo2 = NEUTRAL - pwm_ser;
        servo3 = NEUTRAL + pwm_ser;
        servo4 = NEUTRAL - pwm_ser;
      }
    } else if (x < 260 && y > 240) {
      if (vector1Magnitude < vector2Magnitude) {
        servo0 = NEUTRAL - OFFSET0;
        servo5 = NEUTRAL + OFFSET5;
        servo1 = NEUTRAL + servoDiff;
        servo2 = NEUTRAL - servoDiff;
        servo3 = NEUTRAL - servoDiff;
        servo4 = NEUTRAL + servoDiff;
      } else {
        servo0 = servo0Signal;
        servo5 = servo5Signal;
        servo1 = NEUTRAL - pwm_ser;
        servo2 = NEUTRAL + pwm_ser;
        servo3 = NEUTRAL - pwm_ser;
        servo4 = NEUTRAL + pwm_ser;
      }
    } else if (x > 390 && y > 240) {
      if (vector1Magnitude < vector2Magnitude) {
        servo0 = NEUTRAL - OFFSET0;
        servo5 = NEUTRAL + OFFSET5;
        servo3 = NEUTRAL + servoDiff;
        servo4 = NEUTRAL - servoDiff;
        servo1 = NEUTRAL - servoDiff;
        servo2 = NEUTRAL + servoDiff;
      } else {
        servo0 = servo0Signal;
        servo5 = servo5Signal;
        servo1 = NEUTRAL - pwm_ser;
        servo2 = NEUTRAL + pwm_ser;
        servo3 = NEUTRAL - pwm_ser;
        servo4 = NEUTRAL + pwm_ser;
      }
    }
  } else {
    // Nếu không có tín hiệu từ Serial, đưa tất cả servo về vị trí trung lập
    servo0 = NEUTRAL - OFFSET0;
    servo1 = NEUTRAL;
    servo2 = NEUTRAL;
    servo3 = NEUTRAL;
    servo4 = NEUTRAL;
    servo5 = NEUTRAL + OFFSET5;
  }

  // Điều khiển các cặp servo
  pwm.setPWM(0, 0, servo0);
  pwm.setPWM(1, 0, servo1);
  pwm.setPWM(2, 0, servo2);
  pwm.setPWM(3, 0, servo3);
  pwm.setPWM(4, 0, servo4);
  pwm.setPWM(5, 0, servo5);

  delay(10);  // Chờ 10ms trước khi tiếp tục lặp lại
}
