#ifndef VARIABLES_H
#define VARIABLES_H
#include <Arduino.h>
#include <MPU6050.h>
#include <Servo.h>

extern float yaw, leftDist, frontDist, rightDist;
extern float elapsedTime, currTime, previousTime;
extern float gyroZ, gyroAngleZ, gyroErrorZ;

//constants
extern const float turnDegree;
extern const int motorLeft, motorRight;
extern const float captureDist;

//motors
extern const int ENA;
extern const int IN1;
extern const int IN2;
extern const int ENB;
extern const int IN3;
extern const int IN4;

//servos
const int leftServoPin = 12;
const int rightServoPin = 13;
Servo leftServo;
Servo rightServo;

//left, middle, right
extern const int ultrasonicSensors[3][2];
extern const int servo;

extern MPU6050 accelgyro;

#endif // VARIABLES_H
