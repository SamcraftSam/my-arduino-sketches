#include <Servo.h>
Servo servo;
void setup() {
  // put your setup code here, to run once:
  servo.attach(12);
  delay(500);
  servo.write(0);
  delay(1000);
  servo.write(80);
  delay(1000);
  servo.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:

}
