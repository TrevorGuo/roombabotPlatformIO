#include "variables.h"
#include "sensors.h"
#include <MPU6050.h>

//Might want to add a for loop to get average distances for accuracy
void getDistance(int dir) {     //0 = left, 1 = front, 2 = right {

    float dist = 0;

    for (int i = 0; i < 5; i++) {
        digitalWrite(ultrasonicSensors[dir][1], LOW);
        delayMicroseconds(2);
        digitalWrite(ultrasonicSensors[dir][1], HIGH);
        delayMicroseconds(10);
        digitalWrite(ultrasonicSensors[dir][1], LOW);
        
        float duration = pulseIn(ultrasonicSensors[dir][0], HIGH);
        Serial.print(dir);
        Serial.print(": ");
        Serial.println(duration);
        dist += duration / 2 * 0.034;
        delayMicroseconds(50);
    }

    float maxDist = 150;

    switch(dir) {
        case 0:
            leftDist = min(dist / 5.0, maxDist);
        case 1:
            frontDist = min(dist / 5.0, maxDist);
        case 2:
            rightDist = min(dist / 5.0, maxDist);
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
