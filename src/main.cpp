#include "variables.h"
#include "movement.h"
#include "sensors.h"
#include <MPU6050.h>
#include <Arduino.h>

// MPU6050 accelgyro;

void setup() {
  Serial.begin(115200);
  Serial.println("Setting up");

  for (int i = 0; i < 3; i++) {
    pinMode(ultrasonicSensors[i][0], INPUT);  //echo
    pinMode(ultrasonicSensors[i][1], OUTPUT); //trig
  }

  //pinMode(12, OUTPUT);
  //pinMode(13, OUTPUT);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  Serial.println("Pins set");

  while(!accelgyro.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }

  accelgyro.calibrateGyro();
  Serial.println("Gyro calibrated");
  accelgyro.setThreshold(3); //Fiddle with this more
  Serial.println("Finished setup");
  delay(5000);
}

bool turnedRight = false;
float timeTurned = 0.0;

float timeSound = 0.0;
float timeGyro = 0.0;

bool obstaclesAvoided = false;
bool objectFound = false;

void loop() {
  // put your main code here, to run repeatedly:
    getYaw();
    // Serial.print("Yaw: ");
    // Serial.println(yaw);
    if (millis() - timeSound > 250) {
      readUltrasonicSensors();
      timeSound = millis();
    }
    Serial.print("Left: ");
    Serial.print(leftDist);
    Serial.print(" Front: ");
    Serial.print(frontDist);
    Serial.print(" Right: ");
    Serial.println(rightDist);
    if (!obstaclesAvoided) { //TODO Calculate actual distance on course
      // if(leftDist + rightDist > 175 && (leftDist > 100 || rightDist > 100)) {
      //   obstaclesAvoided = true;
      // }
      if (frontDist < 10 || (leftDist < 10 || rightDist < 10)) {
        reverse(frontDist + 5);
      }
      else if (frontDist <= 30) {
        aroundObstacle(); //TODO: Jittering after reaching a wall?
        //obstaclesAvoided = clearedObstacles();
      }
      else {
        forward(30.0);
      }

    }
    else {
      if (!objectFound){
        while(frontDist > 5) {
          forward(max(findObject() / 2, 5));
          readUltrasonicSensors();
        }
        objectFound = true;
      }
    }

}
