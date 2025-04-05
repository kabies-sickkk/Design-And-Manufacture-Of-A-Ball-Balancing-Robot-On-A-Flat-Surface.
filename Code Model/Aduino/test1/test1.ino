#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Khởi tạo PCA9685 tại địa chỉ mặc định (0x40)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN  150  // Xung tối thiểu (servo về 0 độ)
#define SERVOMAX  635  // Xung tối đa (servo về 180 độ)

void setup() {
  Serial.begin(9600);
  Serial.println("6-Servo Test!");

  pwm.begin();
  pwm.setPWMFreq(60);  // Tần số PWM 60 Hz

  delay(10);
}

void loop() {
  int servo0 = 150-30;
  int servo1 = 635;
  int servo2 = 150;
  int servo3 = 635;
  int servo4 = 150;
  int servo5 = 635+30;

  // Điều khiển các cặp servo
   pwm.setPWM(0, 0, servo0);
   pwm.setPWM(5, 0, servo5);

  pwm.setPWM(1, 0, servo1);
   pwm.setPWM(2, 0, servo2);

   pwm.setPWM(3, 0, servo3);
   pwm.setPWM(4, 0, servo4);



  //delay(1000);  // Chờ 1 giây trước khi tiếp tục lặp lại
}
