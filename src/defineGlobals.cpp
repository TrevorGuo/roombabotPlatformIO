#include "variables.h"

float yaw = 0.0;
float leftDist = 0.0;
float frontDist = 0.0;
float rightDist = 0.0;

float elapsedTime = 0.0;
float currTime = 0.0;
float previousTime = 0.0;

float gyroZ = 0.0;
float gyroAngleZ = 0.0;
float gyroErrorZ = 0.0;

const float turnDegree = 0.0;
const int motorSpeed = 150;
const float captureDist = 0.0;

const int ENA = 10;
const int IN1 = 8;
const int IN2 = 9;
const int ENB = 11;
const int IN3 = 12;
const int IN4 = 13;

const int ultrasonicSensors[3][2] = {{2,3}, {4,5}, {6,7}};
const int leftServo = 12;
const int rightServo = 13;

MPU6050 accelgyro;
