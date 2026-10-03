#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Servo.h>


Adafruit_TCS34725 tcs =
Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);


Servo myServo;
Servo myServo2;


const int servoPin  = 3;
const int servoPin2 = 5;
const int buzzerPin = 6; 

//corresponds to location for green, yellow, purple, red, orange
int positions[5] = {20, 60, 100, 140, 180};
//corresponds to the 3 physical locations of the candies (pick up candy, detect, and route to bin)
int servo2Positions[3] = {0, 30, 113};


String lastColor = "";
int emptyCounter = 0; 


void playBeep(int frequency, int duration) {
  tone(buzzerPin, frequency, duration);
  delay(duration);
  noTone(buzzerPin);
}

// Color thresholds were experimentally calibrated by recording
// TCS34725 readings from each candy color under the project’s lighting 
// and sensor configuration.


String detectColor(float R, float G, float B) { 
  if (R > 70 && G > 70 && B < 55 && G >= R)          return "YELLOW";
  if (R > G && R > B && B < 52)                      return "ORANGE";
  if (R > G && R > B)                                 return "RED";
  if (R > 65 && B > 58)                              return "PURPLE";
  if (G > R && (G - R) < 25 && B < R) {
    if (B < 52)                                       return "YELLOW";
    return "RED";
  }

  if (G > R && G > B)                                return "GREEN";
//blue tape underneath location where candy color is detected, so blue detection = empty
  if (B > R && B > G)                                return "EMPTY"; 

  return "UNKNOWN";
}

String readColor() {
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);


  if (c == 0) return "UNKNOWN";


  float R = (float)r / c * 255;
  float G = (float)g / c * 255;
  float B = (float)b / c * 255;


  String color = detectColor(R, G, B);


  Serial.print("R:"); Serial.print(R);
  Serial.print(" G:"); Serial.print(G);
  Serial.print(" B:"); Serial.print(B);
  Serial.print(" -> "); Serial.println(color);


  return color;
}


void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);


  // play buzzer on start
  playBeep(1000, 150);
  delay(50);
  playBeep(1200, 300);


  bool sensorFound = false;
  for (int i = 0; i < 5; i++) {
    if (tcs.begin()) {
      sensorFound = true;
      break;
    }
    delay(200);
  }


  if (!sensorFound) {
    while (1);
  }


  myServo.attach(servoPin);
  myServo2.attach(servoPin2);
  myServo2.write(servo2Positions[0]);
  delay(500);
}


void loop() {
  int servo2Step = 0;


  while (true) {
    myServo2.write(servo2Positions[servo2Step]);


    if (servo2Step == 1) {
      delay(800);


      String color = readColor();


     
      if (color == "EMPTY") { //Blue tape under location where candies are typically “sensed”
        emptyCounter++; // if blue is detected it means there is no candy present or EMPTY
      } else {
        emptyCounter = 0;
      }


      // Trigger buzzer if blue/empty is seen for 3 cycles and end
      if (emptyCounter >= 3) {
        Serial.println("Empty.");
        for (int i = 0; i < 3; i++) {
          playBeep(600, 200);
          delay(100);
        }
        playBeep(400, 1000); 
       
        
        while(true) {
        // shutdown code
        }
      }


      if (color != lastColor && color != "EMPTY") {
        int index = -1;


        if      (color == "GREEN")  index = 0;
        else if (color == "YELLOW") index = 1;
        else if (color == "PURPLE") index = 2;
        else if (color == "RED")    index = 3;
        else if (color == "ORANGE") index = 4;


        if (index != -1) {
          myServo.write(positions[index]);
          delay(800);
          lastColor = color;
        }
      }


      delay(400);
    } else {
      delay(600);
    }


    servo2Step = (servo2Step + 1) % 3;
  }
}

