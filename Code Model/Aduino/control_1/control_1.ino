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
    if (!(x >= 295 && x <= 365 && y >= 200 && y <= 290)) {
      // Điều khiển các servo dựa trên tọa độ x và y
      if (y > 290) {  // Nếu y lớn hơn 290
        servo0 = map(y, 290, 480, NEUTRAL - OFFSET0, SERVOMIN);  // Dao động từ 381-30 đến min
        servo5 = map(y, 290, 480, NEUTRAL + OFFSET5, SERVOMAX);  // Dao động từ 381+30 đến max
      } else if (y < 200) {  // Nếu y nhỏ hơn 200
        servo0 = map(y, 0, 200, SERVOMIN, NEUTRAL - OFFSET0);  // Dao động từ min đến 381-30
        servo5 = map(y, 0, 200, SERVOMAX, NEUTRAL + OFFSET5);  // Dao động từ max đến 381+30
      }

      if (x < 295) {  // Nếu x nhỏ hơn 295
        servo1 = map(x, 0, 295, NEUTRAL, SERVOMAX);  // Dao động từ 381 đến max
        servo2 = map(x, 0, 295, NEUTRAL, SERVOMIN);  // Dao động từ 381 đến min
        servo3 = map(x, 0, 295, NEUTRAL, SERVOMAX);  // Dao động từ 381 đến max
        servo4 = map(x, 0, 295, NEUTRAL, SERVOMIN);  // Dao động từ 381 đến min
      } else if (x > 365) {  // Nếu x lớn hơn 365
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

  delay(10);  // Chờ 1 giây trước khi tiếp tục lặp lại
}
