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
const int motorSpeed = 125;
const float captureDist = 0.0;

const int ENA = 44;
const int IN1 = 8;
const int IN2 = 9;
const int ENB = 46;
const int IN3 = 10;
const int IN4 = 11;

const int ultrasonicSensors[3][2] = {{2,3}, {4,5}, {6,7}};
const int servo = 9;

MPU6050 accelgyro;
