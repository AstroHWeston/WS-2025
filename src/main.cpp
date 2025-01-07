#include <Arduino.h>
#include <Servo.h>

#define Servo_bl 9
#define Servo_br 10
#define Servo_fl 11
#define Servo_fr 12

Servo motor_bl;
Servo motor_br;
Servo motor_fl;
Servo motor_fr;
int maxP = 120;
int maxN = 180 - maxP;

enum Direction {
  Forward,      // 0
  Backward,     // 1
  Left,         // 2
  Right,        // 3
  None          // 4
};
Direction oldDir = Forward;
Direction currentDir = Forward;
Direction newDir = None;

void motor_stop() {
  motor_fr.write(90);
  motor_fl.write(90);
  motor_bl.write(90);
  motor_br.write(90);
}
//***************************************************
void move_fw(int d = 0) { // Kretanje naprijed
  currentDir = Forward;
  for (int i = 90; i >= maxN; i--) {
    motor_fr.write(i);
    motor_fl.write(180 - i);
    motor_bl.write(180 - i);
    motor_br.write(i);
  }
  if(d > 0) {
    delay(d);
    motor_stop();
  }
}
//***************************************************
void move_back(int d = 0) { // Kretanje natrag
  currentDir = Backward;
  for (int i = 90; i >= maxN; i--) {
    motor_fr.write(180 - i);
    motor_fl.write(i);
    motor_bl.write(i);
    motor_br.write(180 - i);
  }
  if(d > 0) {
    delay(d);
    motor_stop();
  }
}
//***************************************************
void move_right(int d = 0) { // Kretanje desno
  currentDir = Right;
  for (int i = 90; i >= maxN; i--) {
    motor_fr.write(180 - i);
    motor_fl.write(180 - i);
    motor_bl.write(i);
    motor_br.write(i);
  }
  if(d > 0) {
    delay(d);
    motor_stop();
  }
}
//***************************************************
void move_left(int d = 0) { // Kretanje lijevo
  currentDir = Left;
  for (int i = 90; i >= maxN; i--) {
    motor_fr.write(i);
    motor_fl.write(i);
    motor_bl.write(180 - i);
    motor_br.write(180 - i);
  }
  if(d > 0) {
    delay(d);
    motor_stop();
  }
}
//***************************************************
void rotate_right(int d = 0) { // Rotacija desno
  for (int i = 90; i >= maxN; i--) {
    motor_fr.write(180 - i);
    motor_fl.write(180 - i);
    motor_bl.write(90);
    motor_br.write(90);
  }
  if(d > 0) {
    delay(d);
    motor_stop();
  }
}
//***************************************************
void rotate_left(int d = 0) { // Rotacija lijevo
  for (int i = 90; i >= maxN; i--) {
    motor_fr.write(i);
    motor_fl.write(i);
    motor_bl.write(90);
    motor_br.write(90);
  }
  if(d > 0) {
    delay(d);
    motor_stop();
  }
}
//***************************************************
void okret_90() {
  for (int i = 90; i >= 0; i--) {
    motor_fr.write(180 - i);
    motor_fl.write(180 - i);
    motor_bl.write(180 - i);
    motor_br.write(180 - i);
  }
  delay(1550);
  motor_stop();
}
//***************************************************
void okret_180() {
  for (int i = 90; i <= 180; i++) { //prije maxp je bilo 180
    motor_fr.write(i);
    motor_fl.write(i);
    motor_bl.write(i);
    motor_br.write(i);
  }
  delay(3300);
  motor_stop();
}
//***************************************************

void setup() {
  
}