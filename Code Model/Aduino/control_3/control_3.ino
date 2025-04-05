#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Khởi tạo PCA9685 tại địa chỉ mặc định (0x40)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN  150  // Xung tối thiểu (servo về 0 độ)
#define SERVOMAX  635  // Xung tối đa (servo về 180 độ)
#define NEUTRAL   381  // Vị trí trung lập của servo
#define OFFSET0   30   // Offset cho servo 0
#define OFFSET5   30   // Offset cho servo 5

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

    // Điều kiện giữ các servo ở vị trí trung lập khi tọa độ x và y nằm trong khoảng nhất định
    if (!(x >= 380 && x <= 280 && y >= 280 && y <= 215)) {
      // Điều khiển các servo dựa trên tọa độ y
      if (y > 280) {  // Nếu y lớn hơn 280
        servo1 = map(y, 280, 480, NEUTRAL, SERVOMAX);  // Dao động từ 381 đến max
        servo3 = map(y, 280, 480, NEUTRAL, SERVOMAX);  // Dao động từ 381 đến max
        servo5 = map(y, 280, 480, NEUTRAL + OFFSET5, SERVOMAX);  // Dao động từ 381+30 đến max
        servo0 = map(y, 280, 480, NEUTRAL - OFFSET0, SERVOMIN);  // Dao động từ 381-30 đến min
        servo2 = map(y, 280, 480, NEUTRAL, SERVOMIN);  // Dao động từ 381 đến min
        servo4 = map(y, 280, 480, NEUTRAL, SERVOMIN);  // Dao động từ 381 đến min
      } else if (y <215) {  // Nếu y nhỏ hơn 200
        servo1 = map(y, 0, 200, SERVOMAX, NEUTRAL);  // Dao động từ max đến 381
        servo3 = map(y, 0, 200, SERVOMAX, NEUTRAL);  // Dao động từ max đến 381
        servo5 = map(y, 0, 200, SERVOMAX, NEUTRAL + OFFSET5);  // Dao động từ max đến 381+30
        servo0 = map(y, 0, 200, SERVOMIN, NEUTRAL - OFFSET0);  // Dao động từ min đến 381-30
        servo2 = map(y, 0, 200, SERVOMIN, NEUTRAL);  // Dao động từ min đến 381
        servo4 = map(y, 0, 200, SERVOMIN, NEUTRAL);  // Dao động từ min đến 381
      } else {  // Khi y trong khoảng 200 đến 280
        servo0 = NEUTRAL - OFFSET0;  // Servo 0 ở vị trí 381 - 30
        servo5 = NEUTRAL + OFFSET5;  // Servo 5 ở vị trí 381 + 30
        servo1 = NEUTRAL;  // Servo 1 ở vị trí 381
        servo2 = NEUTRAL;  // Servo 2 ở vị trí 381
        servo3 = NEUTRAL;  // Servo 3 ở vị trí 381
        servo4 = NEUTRAL;  // Servo 4 ở vị trí 381
      }

      // Điều khiển các servo dựa trên tọa độ x
      if (x < 280) {  // Nếu x nhỏ hơn 295
        servo1 = map(x, 0, 295, NEUTRAL, SERVOMAX);  // Dao động từ 381 đến max
        servo2 = map(x, 0, 295, NEUTRAL, SERVOMIN);  // Dao động từ 381 đến min
        servo3 = map(x, 0, 295, NEUTRAL, SERVOMAX);  // Dao động từ 381 đến max
        servo4 = map(x, 0, 295, NEUTRAL, SERVOMIN);  // Dao động từ 381 đến min
      } else if (x > 380) {  // Nếu x lớn hơn 365
        servo1 = map(x, 365, 640, NEUTRAL, SERVOMAX);  // Dao động từ 381 đến max
        servo2 = map(x, 365, 640, NEUTRAL, SERVOMIN);  // Dao động từ 381 đến min
        servo3 = map(x, 365, 640, NEUTRAL, SERVOMAX);  // Dao động từ 381 đến max
        servo4 = map(x, 365, 640, NEUTRAL, SERVOMIN);  // Dao động từ 381 đến min
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

  //delay(10);  // Chờ 10ms trước khi tiếp tục lặp lại
}
