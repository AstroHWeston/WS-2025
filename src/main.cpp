#include <Arduino.h>                // Main arduino dep.  //
#include <NewPing.h>                // Ultrasound sensor  //
#include <LiquidCrystal_I2C.h>      // LCD                //
#include <Wire.h>                   // I2C comms.         //
#include <WS2812-SOLDERED.h>        // LED strip          //
#include <APDS9960-SOLDERED.h>      // Color sensor       //
#include <Servo.h>                  // Servo              //
#include <SPI.h>                    // dawg idek          //
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

// Line follower sensor //
const int linApin[] = { A0, A1, A2, A3, A4, A5, A6, A7 };
const int linDpin[] = { 41, 43, 45, 47, 49 };
int calibration_values[5] = { 200, 200, 200, 200, 150 };

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

enum Line_follower_status {
  AllWhite,               // 0 //
  AllBlack,               // 1 //
  LeftBlack,              // 2 //
  RightBlack,             // 3 //
  MiddleBlack,            // 4 //
  RightSlightDeviation,   // 5 //
  LeftSlightDeviation,    // 6 //
  RightExtremeDeviation,  // 7 //
  LeftExtremeDeviation,   // 8 //
};
Line_follower_status old_line_status = AllWhite;

// *************************************************** //
// The following function takes in an array of 5 bits (from the line follow sensor readings) and converts it to a decimal number. //
int binaryToDecimal(int arr[], int size = 5) {
  int decimal = 0;

  for (int i = 0; i < size; i++) {
    decimal += arr[i] * (1 << (size - 1 - i));
  }

  return decimal;
}
// *************************************************** //
// The function below clears the LCD and sets the cursor to the top left corner. //
void reset_display () {
  lcd.clear();
  lcd.setCursor(0, 0);
}
// *************************************************** //
// The function below is used to display the red color on the LED strip. //
void red () {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(150, 0, 0));
    pixels.show();
    delay(20);
  }
}
// *************************************************** //
// The function below is used to display the green color on the LED strip. //
void green () {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    pixels.show();
    delay(20);
  }  
}
// *************************************************** //
// The function below is used to display the blue color on the LED strip. //
void blue () {
    for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 150));
    pixels.show();
    delay(20);
  }
}
// *************************************************** //
// The function below is used to display the yellow color on the LED strip. //
void yellow () {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(150, 150, 0));
    pixels.show();
    delay(20);
  }  
}
// *************************************************** //
// The function below is used to stop the servo motor. //
void motor_stop () {
  motor_fr.write(90);
  motor_fl.write(90);
  motor_bl.write(90);
  motor_br.write(90);
}
// *************************************************** //
// The functions below determine different directions for the servo motor. //
void move_fw (int d = 0) {
  currentDir = Forward;
  for (int i = 90; i >= maxN; i--) {
    motor_fr.write(i);
    motor_fl.write(180 - i);
    motor_bl.write(180 - i);
    motor_br.write(i);
  }
  if (d > 0) {
    delay(d);
    motor_stop();
  }
}
// *************************************************** //
void move_back (int d = 0) {
  currentDir = Backward;
  for (int i = 90; i >= maxN; i--) {
    motor_fr.write(180 - i);
    motor_fl.write(i);
    motor_bl.write(i);
    motor_br.write(180 - i);
  }
  if (d > 0) {
    delay(d);
    motor_stop();
  }
}
// *************************************************** //
void move_right (int d = 0) {
=======
  currentDir = Right;
  for (int i = 90; i >= maxN; i--) {
    motor_fr.write(180 - i);
    motor_fl.write(180 - i);
    motor_bl.write(i);
    motor_br.write(i);
  }
  if (d > 0) {
    delay(d);
    motor_stop();
  }
}
// *************************************************** //
void move_left (int d = 0) {
  currentDir = Left;
  for (int i = 90; i >= maxN; i--) {
    motor_fr.write(i);
    motor_fl.write(i);
    motor_bl.write(180 - i);
    motor_br.write(180 - i);
  }
  if (d > 0) {
    delay(d);
    motor_stop();
  }
}
// *************************************************** //
void rotate_right (int d = 0) {
  int current_motor_status_bl = motor_bl.read();
  int current_motor_status_br = motor_br.read();
  for (int i = 90; i >= maxN; i--) {
    motor_fr.write(180 - i);
    motor_fl.write(180 - i);
    motor_bl.write(current_motor_status_bl);
    motor_br.write(current_motor_status_br);
  }
  if (d > 0) {
    delay(d);
    motor_stop();
  }
}
// *************************************************** //
void rotate_left (int d = 0) {
  int current_motor_status_bl = motor_bl.read();
  int current_motor_status_br = motor_br.read();
  for (int i = 90; i >= maxN; i--) {
    motor_fr.write(i);
    motor_fl.write(i);
    motor_bl.write(current_motor_status_bl);
    motor_br.write(current_motor_status_br);
  }
  if (d > 0) {
    delay(d);
    motor_stop();
  }
}
// *************************************************** //
void okret_90 () {
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
void okret_180 () {
  for (int i = 90; i <= 180; i++) {
    motor_fr.write(i);
    motor_fl.write(i);
    motor_bl.write(i);
    motor_br.write(i);
  }
  delay(3300);
  motor_stop();
}
// *************************************************** //
Line_follower_status line_sensor_status (bool debug = false, bool show_on_display = false) {
  int result[5];

  for (int i = 0; i < 5; i++) {
    digitalWrite(linDpin[i], HIGH);
  }
  delay(200);

  for (int i = 0; i < 5; i++) {
    if (debug) {
      result[i] = analogRead(linApin[i]);
    } else {
      result[i] = analogRead(linApin[i]) > calibration_values[i] ? 1 : 0;
    }
    digitalWrite(linDpin[i], LOW);
  }

  int decimalStatus = binaryToDecimal(result, 5);

  if (show_on_display) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(result[0]);
    lcd.setCursor(6,0);
    lcd.print(result[2]);
    lcd.setCursor(12,0);
    lcd.print(result[4]);
    lcd.setCursor(3,1);
    lcd.print(result[1]);
    lcd.setCursor(9,1);
    lcd.print(result[3]);
    delay(100);
  }

  switch (decimalStatus) {
    case 0:
      return AllWhite;
    case 1:
      return RightExtremeDeviation;
    case 2:
      return RightSlightDeviation;
    case 3:
      return RightBlack;
    case 4:
      return MiddleBlack;
    case 6:
      return MiddleBlack;
    case 7:
      return RightBlack;
    case 8:
      return LeftSlightDeviation;
    case 12:
      return MiddleBlack;
    case 16:
      return LeftExtremeDeviation;
    case 24:
      return LeftBlack;
    case 28:
      return LeftBlack;
    case 31:
      return AllBlack;
  }
}
// *************************************************** //
void line_following_straight () {
  int line_status = line_sensor_status();
  Serial.println(line_status);

  //if (line_status == old_line_status) return;
  if (line_status == AllBlack) {
    motor_stop();
    delay(1000);
    move_fw(1000);
  } else if (line_status == LeftBlack) {
    rotate_left();
  } else if (line_status == RightBlack) {
    rotate_right();
  } else if (line_status == MiddleBlack) {
    move_fw();
  } else if (line_status == AllWhite) {
    motor_stop();
  } else if (line_status == RightSlightDeviation) {
    rotate_right();
  } else if (line_status == LeftSlightDeviation) {
    rotate_left();
  } else if (line_status == RightExtremeDeviation) {
    rotate_right();
  } else if (line_status == LeftExtremeDeviation) {
    rotate_left();
  }
  delay(250);
}
// *************************************************** //
void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("Welcome to Stjepan! Let's begin.");

  // LCD //
  Serial.println("Initializing the LCD...");
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Booting up...");

  // Line follower sensors initialization //
  Serial.println("Initializing the line follower sensors...");

  for (int i = 0; i < 5; i++) {
    pinMode(linApin[i], INPUT);
    pinMode(linDpin[i], OUTPUT);
  }

  // Color sensors initialization //
  Serial.println("Initializing the color sensor...");
  if (!apds.begin()) {
    Serial.println("Error initializing the color sensor, bailing out!");
    exit(1);
  } else {
    lcd.setCursor(0, 1);
  }
  delay(1000);

  // LED strip initialization //
  Serial.println("Initializing the LED strip...");

  pixels.begin();
  pixels.clear();
  Serial.println("LED strip initialized, running tests...");

  Serial.println("Testing red color...");
  red();
  Serial.println("Testing green color...");
  green();
  Serial.println("Testing blue color...");
  blue();
  Serial.println("Testing yellow color...");
  yellow();

  // Servo motors initialization //
  Serial.println("Initializing the servo motors...");

  motor_bl.attach(Servo_bl);
  motor_br.attach(Servo_br);
  motor_fl.attach(Servo_fl);
  motor_fr.attach(Servo_fr);
  Serial.println("Servo motors initialized.");

  // Cleaning up before starting main loop //
  Serial.println("Cleaning up before starting the main loop...");
  pixels.clear();
  reset_display();
  Serial.println("Done! Stjepan is online and ready!");
}

void loop() {
  //int status = line_sensor_status(true, true);
  line_following_straight();  
  //Serial.println(status);
}