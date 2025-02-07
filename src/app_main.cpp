#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define Servo_sl 9
#define Servo_sd 10
#define Servo_pl 11
#define Servo_pd 12

Servo motor_sl;
Servo motor_sd;
Servo motor_pl;
Servo motor_pd;

char smjer;

LiquidCrystal_I2C lcd(0x27, 2, 16);

void reset_display() {
  lcd.clear();
  lcd.setCursor(0, 0);
}

void motor_stop() {
  motor_pd.write(90);
  motor_pl.write(90);
  motor_sl.write(90);
  motor_sd.write(90);
}

void move_fw() { // Kretanje naprijed
  for (int i = 90; i >= 0; i--) {
    motor_pd.write(i);
    motor_pl.write(180 - i);
    motor_sl.write(180 - i);
    motor_sd.write(i);
  }
}

void move_back() { // Kretanje natrag
  for (int i = 90; i >= 0; i--) {
    motor_pd.write(180 - i);
    motor_pl.write(i);
    motor_sl.write(i);
    motor_sd.write(180 - i);
  }
}

void move_right() { // Kretanje desno
  for (int i = 90; i >= 0; i--) {
    motor_pd.write(180 - i);
    motor_pl.write(180 - i);
    motor_sl.write(i);
    motor_sd.write(i);
  }
}

void move_left() { // Kretanje lijevo
  for (int i = 90; i >= 0; i--) {
    motor_pd.write(i);
    motor_pl.write(i);
    motor_sl.write(180 - i);
    motor_sd.write(180 - i);
  }
}

void rotate_right() { // Rotacija desno
  for (int i = 90; i >= 0; i--) {
    motor_pd.write(180 - i);
    motor_pl.write(180 - i);
    motor_sl.write(90);
    motor_sd.write(90);
  }
}

void rotate_left() { // Rotacija lijevo
  for (int i = 90; i >= 0; i--) {
    motor_pd.write(i);
    motor_pl.write(i);
    motor_sl.write(90);
    motor_sd.write(90);
  }
}

void slide_fwRight() {
  for (int i = 90; i >= 0; i--) {
    motor_pd.write(90);
    motor_pl.write(180 - i);
    motor_sl.write(90);
    motor_sd.write(i);
  }
}

void slide_fwLeft() {
  for (int i = 90; i >= 0; i--) {
    motor_pd.write(i);
    motor_pl.write(90);
    motor_sl.write(180 - i);
    motor_sd.write(90);
  }
}

void slide_bcLeft() {
  for (int i = 90; i >= 0; i--) {
    motor_pd.write(90);
    motor_pl.write(i);
    motor_sl.write(90);
    motor_sd.write(180 - i);
  }
}

void slide_bcRight() {
  for (int i = 90; i >= 0; i--) {
    motor_pd.write(180 - i);
    motor_pl.write(90);
    motor_sl.write(i);
    motor_sd.write(90);
  }
}

void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);
  lcd.init();
  lcd.backlight();
  reset_display();
  motor_sl.attach(Servo_sl);
  motor_sd.attach(Servo_sd);
  motor_pl.attach(Servo_pl);
  motor_pd.attach(Servo_pd);
  lcd.print("Setup done.");
}

void loop() {
  if(Serial3.available() > 0) {
    smjer = Serial3.read();
    switch (smjer) {
      case 'F':
        move_fw();
        break;
      case 'B':
        move_back();
        break;
      case 'R':
        move_right();
        break;
      case 'L':
        move_left();
        break;
      case 'P':
        rotate_right();
        break;
      case 'O':
        rotate_left();
        break;
      case 'Q':
        slide_fwLeft();
        break;
      case 'Y':
        slide_fwRight();
        break;
      case 'X':
      slide_bcLeft();
      break;
      case 'Z':
      slide_bcRight();
      break;
      case 'S':
        motor_stop();
        break;
      default:
        break;
    }
  }
}
