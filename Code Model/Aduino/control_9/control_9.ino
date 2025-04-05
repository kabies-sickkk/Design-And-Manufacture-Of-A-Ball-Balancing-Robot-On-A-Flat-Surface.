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

int lastX = -1; // Lưu trữ giá trị x trước đó
int lastY = -1; // Lưu trữ giá trị y trước đó

void setup() {
  Serial.begin(2000000);
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
    if (x == lastX && y == lastY) {
      delay(10);  // Chờ 10ms trước khi tiếp tục lặp lại
      return;
    }

    // Cập nhật giá trị x và y hiện tại cho lần lặp sau
    lastX = x;
    lastY = y;

    // Tính toán tọa độ cho hai vector phụ
    int deltaX = x - x0;
    int deltaY = y - y0;
    float vectorMagnitude = sqrt(sq(deltaX) + sq(deltaY));
    float theta = atan2(deltaY, deltaX); // Sử dụng atan2 để xử lý các góc ngoài khoảng [-π, π]
    float thetadegree = theta * (180 / PI);
    float servoangle_y = -(thetadegree * 180) / 15;

      // Tính giá trị điều khiển cho servo 0 và 5
    int servo5Signal = NEUTRAL + OFFSET5 + (servoangle_y * 635 / 180);
    int servo0Signal = NEUTRAL - OFFSET0 - (servoangle_y * 150 / 180);


    // Điều kiện giữ các servo ở vị trí trung lập khi tọa độ x và y nằm trong khoảng nhất định
    if (x >= 260 && x <= 380 && y >= 215 && y <= 280) {
      vectorMagnitude = 0;
    } else if(x >= 260 && x <= 380){
      if (y < 215 || y > 280){
        servo0 = servo0Signal;
        servo5 = servo5Signal;
        // Các servo 1, 2, 3, 4 sẽ ở vị trí trung lập
        servo1 = NEUTRAL - (servoangle_y * 635 / 180);
        servo2 = NEUTRAL + (servoangle_y * 150 / 180);
        servo3 = NEUTRAL - (servoangle_y * 635 / 180);
        servo4 = NEUTRAL + (servoangle_y * 150 / 180);
        Serial.print("S5S:");
        Serial.print(servo5Signal);
        Serial.print(",");
        Serial.print("S0S:");
        Serial.println(servo0Signal);
      }
      // Tính góc nghiêng cần đáp ứng của mặt phẳng
      
      // Điều chỉnh servo theo vị trí của x
      //if (x >= 260 && x <= 380) {  // Nếu x nằm trong khoảng từ 260 đến 380
        
      } else {  // Nếu x nằm ngoài khoảng từ 260 đến 380
        // Tính độ chênh lệch servo dựa trên độ lớn của vector
        int servoDiff = map(vectorMagnitude, maxVector, 0, maxServoDiff, 0);

        // Cập nhật giá trị servo dựa trên độ chênh lệch tính được
        Serial.println("Non_servoSignal");
        if (vectorMagnitude > 0 && (x < 260 || x > 380)) {
          if (x < 260 && y < y0) {  // Nếu x < 360 và y < 240
            servo0 = NEUTRAL - OFFSET0 + servoDiff;
            servo5 = NEUTRAL + OFFSET5 - servoDiff;
            servo1 = NEUTRAL + servoDiff;
            servo2 = NEUTRAL - servoDiff;
            servo3 = NEUTRAL + servoDiff;
            servo4 = NEUTRAL - servoDiff;
          } else if (x > 380 && y < y0) {  // Nếu x > 360 và y < 240
            servo0 = NEUTRAL - OFFSET0 + servoDiff;
            servo5 = NEUTRAL + OFFSET5 - servoDiff;
            servo1 = NEUTRAL + servoDiff;
            servo2 = NEUTRAL - servoDiff;
            servo3 = NEUTRAL + servoDiff;
            servo4 = NEUTRAL - servoDiff;
          } else if (x < 260 && y > y0) {  // Nếu x < 360 và y > 240
            servo0 = NEUTRAL - OFFSET0 - servoDiff;
            servo5 = NEUTRAL + OFFSET5 + servoDiff;
            servo1 = NEUTRAL + servoDiff;
            servo2 = NEUTRAL - servoDiff;
            servo3 = NEUTRAL - servoDiff;
            servo4 = NEUTRAL + servoDiff;
          } else if (x > 380 && y > y0) {  // Nếu x > 360 và y > 240
            servo0 = NEUTRAL - OFFSET0 - servoDiff;
            servo5 = NEUTRAL + OFFSET5 + servoDiff;
            servo1 = NEUTRAL - servoDiff;
            servo2 = NEUTRAL + servoDiff;
            servo3 = NEUTRAL + servoDiff;
            servo4 = NEUTRAL - servoDiff;
          }
        }
      //}
    }

    // Điều khiển các cặp servo
    pwm.setPWM(0, 0, servo0);
    pwm.setPWM(5, 0, servo5);
    pwm.setPWM(1, 0, servo1);
    pwm.setPWM(2, 0, servo2);
    pwm.setPWM(3, 0, servo3);
    pwm.setPWM(4, 0, servo4);
  } else {
    // Nếu không có tín hiệu từ Serial, đưa tất cả servo về vị trí trung lập
    pwm.setPWM(0, 0, NEUTRAL - OFFSET0);
    pwm.setPWM(5, 0, NEUTRAL + OFFSET5);
    pwm.setPWM(1, 0, NEUTRAL);
    pwm.setPWM(2, 0, NEUTRAL);
    pwm.setPWM(3, 0, NEUTRAL);
    pwm.setPWM(4, 0, NEUTRAL);
  }

  delay(10);  // Chờ 10ms trước khi tiếp tục lặp lại
}
