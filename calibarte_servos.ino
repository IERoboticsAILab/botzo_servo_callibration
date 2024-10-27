#include <Servo.h>

Servo servo;
int servo_pin = 9;

/*
y = mx + q
PWM = 7.4 * desire_angle + 500
-------- calibration
y = a * x^2 + b * x + c
PWM = a * desire_angle^2 + b * desire_angle + c
for servo 1:
PWM = 0.0011 * angle^2 + 7.2778 * angle + 533.5024
*/

void setup()
{
  servo.attach(servo_pin);

  //desire angles: 0, 45, 90, 135, 180, 270
  // ------------before calibration:
  //servo.writeMicroseconds(500.0);
  //servo.writeMicroseconds(833.0);
  //servo.writeMicroseconds(1167.0);
  //servo.writeMicroseconds(1500.0);
  //servo.writeMicroseconds(1833.0);
  //servo.writeMicroseconds(2500.0);

  //desire angles: 0, 45, 90, 135, 180, 270
  // ------------after calibration:
  //servo.writeMicroseconds(534.0);
  //servo.writeMicroseconds(863.0);
  servo.writeMicroseconds(1197.0);
  //servo.writeMicroseconds(1535.0);
  //servo.writeMicroseconds(1878.0);
  //servo.writeMicroseconds(2576.0);
}

void loop() {}