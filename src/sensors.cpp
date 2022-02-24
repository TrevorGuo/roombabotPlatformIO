#include "variables.h"
#include "sensors.h"
#include <MPU6050.h>

//Might want to add a for loop to get average distances for accuracy
void getDistance(int dir) {     //0 = left, 1 = front, 2 = right {
    digitalWrite(ultrasonicSensors[dir][1], LOW);
    delayMicroseconds(2);
    digitalWrite(ultrasonicSensors[dir][1], HIGH);
    delayMicroseconds(5);
    digitalWrite(ultrasonicSensors[dir][1], LOW);
    
    long duration = pulseIn(ultrasonicSensors[dir][0], HIGH);
    long cm = duration / 2 / 29.1;

    switch(dir) {
        case 0:
            leftDist = cm;
        case 1:
            frontDist = cm;
        case 2:
            rightDist = cm;
    }
}

void readUltrasonicSensors() {
    for (int i = 0; i < 3; i++) {
        getDistance(i);
    }
}

float getYaw() {
    Vector norm = accelgyro.readNormalizeGyro();
    currTime = millis();
    elapsedTime = currTime - previousTime; 
    yaw += norm.ZAxis * elapsedTime/1000;    // hello what is gyroAngleZ the angular displacement or velocity?? im tired now bye
    previousTime = currTime;
    return yaw;
}
