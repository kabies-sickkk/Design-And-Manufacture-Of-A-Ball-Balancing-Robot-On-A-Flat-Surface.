
#include <Servo.h>
#include <Wire.h>
#include <TFLI2C.h>

TFLI2C tfLI2C;
int16_t tfDistance;
int16_t tfAddr = TFL_DEF_ADR;

Servo servo;
int ServoPin = 9;

const double Kp = 4.1 , Ki = 0.12, Kd = 15.7;
double setpoint = 25.0, sp, output = 0.0, error = 0.0, previousError = 0.0, integral = 0.0, derivative = 0.0;
byte byteCome[3];

void setup() {
  Serial.begin(115200);
  servo.attach(ServoPin);
  servo.write(0);
  delay(200);
  Wire.begin();
}

void loop() {
  if (Serial.available() >= 4) {
    for (int i = 0; i < 4; i++) {byteCome[i] = Serial.read();}

    int32_t sp = (int32_t)byteCome[0] | ((int32_t)byteCome[1] << 8) | ((int32_t)byteCome[2] << 16) | ((int32_t)byteCome[3] << 24);
    // Serial.print(int(sp));
    // Serial.print(",");
    setpoint = sp;
    }

  if (tfLI2C.getData(tfDistance, tfAddr)) {
    if (tfDistance >= 50) { tfDistance = 50; };}

  error = tfDistance - setpoint;
  integral += error ;
  derivative = (error - previousError);
  output = Kp * error +  Ki * integral + Kd * derivative;

  if (output >= 90) {
    output = 90;
  } else if (output < -90) {
    output = -90;
  }
  servo.write(90 + output);
  delay(100);
  Serial.print(int(setpoint));
  Serial.print(",");
  Serial.print(tfDistance);
  Serial.print(",");
  Serial.print(Kp);
  Serial.print(",");
  Serial.print(Ki);
  Serial.print(",");
  Serial.println(Kd);
  previousError = error;
}
