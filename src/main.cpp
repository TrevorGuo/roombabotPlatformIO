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
  pinMode(servo, OUTPUT);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  leftServo.attach(leftServoPin);
  rightServo.attach(rightServoPin);
  leftServo.write(0);
  rightServo.write(0);
  Serial.println("Pins set");

  while(!accelgyro.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }

  accelgyro.calibrateGyro();
  //Serial.println("Gyro calibrated");
  accelgyro.setThreshold(1); //Fiddle with this more
  Serial.println("Finished setup");
  delay(5000);
}

bool turnedRight = false;
float timeTurned = 0.0;

float timeSound = 0.0;

void loop() {
  // put your main code here, to run repeatedly:
    getYaw();
    if (millis() - timeSound > 250) {
      readUltrasonicSensors();
      timeSound = millis();
    }
    //   Serial.print("Left: ");
    //   Serial.print(leftDist);
    //   Serial.print(" Front: ");
    //   Serial.print(frontDist);
    //   Serial.print(" Right: ");
    //   Serial.println(rightDist);
    // }
    // rotate(90);
    if (frontDist < 20) {
      aroundObstacle();
    }
    else {
      forward(20.0);
    }
}
