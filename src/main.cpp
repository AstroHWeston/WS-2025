#include <Arduino.h>                // Main arduino dep.  //
#include <NewPing.h>                // Ultrasound sensor  //
#include <LiquidCrystal_I2C.h>      // LCD                //
#include <Wire.h>                   // I2C comms.         //
#include <WS2812-SOLDERED.h>        // LED strip          //
#include <APDS9960-SOLDERED.h>      // Color sensor       //
#include <Servo.h>                  // Servo              //
#include <SPI.h>
// *************************************************** //

// Ultrasound sensor //
#define MAX_DISTANCE 400
const int usF = 0;
const int usB = 1;
const int usL = 2;
const int usR = 3;
const int trigPin[] = {31, 33, 35, 37};
const int echoPin[] = {30, 32, 34, 36};

NewPing sonarF(trigPin[usF], echoPin[usF], MAX_DISTANCE);
NewPing sonarB(trigPin[usB], echoPin[usB], MAX_DISTANCE);
NewPing sonarL(trigPin[usL], echoPin[usL], MAX_DISTANCE);
NewPing sonarR(trigPin[usR], echoPin[usR], MAX_DISTANCE);

// Liquid Crystal Display (LCD) //
LiquidCrystal_I2C lcd(0x27, 2, 16);

// Color sensor //
APDS_9960 apds;

// LED strip //
#define PIN 4
#define NUMPIXELS 10
WS2812 pixels(NUMPIXELS, PIN);

// Servo motor //
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
  Forward,        // 0 //
  Backward,       // 1 //
  Left,           // 2 //
  Right,          // 3 //
  None            // 4 //
};
Direction oldDir = Forward;
Direction currentDir = Forward;
Direction newDir = None;

// *************************************************** //
void motor_stop() {
  motor_fr.write(90);
  motor_fl.write(90);
  motor_bl.write(90);
  motor_br.write(90);
}
// *************************************************** //
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
// *************************************************** //
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
// *************************************************** //
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
// *************************************************** //
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
// *************************************************** //
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
// *************************************************** //
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
// *************************************************** //
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
// *************************************************** //
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
// *************************************************** //

void setup() {
  
}