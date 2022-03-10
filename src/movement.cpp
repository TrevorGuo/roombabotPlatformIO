#include "variables.h"
#include "sensors.h"
#include "movement.h"
#include <CircularBuffer.h>

/* const int MPU = 0x68; // MPU6050 I2C address

float yaw = 0;
float gyroZ, gyroAngleZ, gyroErrorZ;
int c = 0; */

void stop() {
    analogWrite(ENA, LOW);
    analogWrite(ENB, LOW);
}

void forward(float threshold) {  // move forward until an object is detected within a certain threshold distance
    readUltrasonicSensors();
    if (frontDist >= threshold) {   // arbitrary for now
        analogWrite(ENA, motorSpeed);
        digitalWrite(IN1, LOW); //Motor INS are reversed, fix wiring
        digitalWrite(IN2, HIGH);
        analogWrite(ENB, motorSpeed);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
    }
    while (frontDist >= threshold && leftDist >= 7 && rightDist >= 7 || (leftDist + rightDist > 150)) {
        readUltrasonicSensors();
    }
    
    stop();
}

void reverse(float threshold) {
    readUltrasonicSensors();
    if (frontDist <= threshold) {
        analogWrite(ENA, motorSpeed);
        digitalWrite(IN1, HIGH); //Motor INS are reversed, fix wiring
        digitalWrite(IN2, LOW);
        analogWrite(ENB, motorSpeed);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
    }
    while (frontDist <= threshold && leftDist >= 5 && rightDist >= 5) {
        readUltrasonicSensors();
    }
    
    stop();
}



/* equivalent degree values:
 *  +/- 180º -> +/- 155º
 *  +/- 90º -> +/- 68º
 */ 



void rotate(float degree) { //TODO: Motor stalls a bit
    float initYaw = getYaw();
    float finalYaw = initYaw + degree;
    float adjustedSpeed;
    double P = 2.5;
    if (degree > 0) {
        //Left
        while (finalYaw - getYaw() > 0.5) { // threshold 0.5º difference
            adjustedSpeed = min(abs(finalYaw - getYaw()) * P + motorSpeed, 255);

            analogWrite(ENA, adjustedSpeed);
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            analogWrite(ENB, adjustedSpeed);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);

            getYaw();
        }
    }
    else {
        //Right
        while (getYaw() - finalYaw > 0.5) { 
            adjustedSpeed = min(abs(finalYaw - getYaw()) * P + motorSpeed, 255);
            analogWrite(ENA, adjustedSpeed);
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            analogWrite(ENB, adjustedSpeed);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);

            getYaw();
        }
    }
    stop();
}

void rotateSonic(float distance, bool left) {
    Serial.print("Target: ");
    Serial.println(distance);
    int speed = 100;
    if (left) {
        Serial.print("Dist: ");
        Serial.println(frontDist);
        analogWrite(ENA, speed);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENB, speed);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        while (abs(frontDist - distance) > 5) { // threshold
            Serial.print("Dist: ");
            Serial.println(frontDist);
            getYaw();
            readUltrasonicSensors();
        }
        Serial.print("Dist: ");
        Serial.println(frontDist);
    }

    stop();
}

void rotateToZero() {
    float adjustedSpeed = 0;
    double P = 2.5;
    if (yaw < 0) {
        while (getYaw() < -0.5) { // threshold 0.5º difference
            adjustedSpeed = min(abs(getYaw()) * P + motorSpeed, 255);

            analogWrite(ENA, adjustedSpeed);
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            analogWrite(ENB, adjustedSpeed);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);

            getYaw();
        }
    }
    else {
        while (getYaw() > 0.5) { 
            adjustedSpeed = min(abs(getYaw()) * P + motorSpeed, 255);
            analogWrite(ENA, adjustedSpeed);
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            analogWrite(ENB, adjustedSpeed);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);

            getYaw();
        }
    }
    stop();
}

// call when obstacle detected
void aroundObstacle() {
   readUltrasonicSensors();
   float deg = 40;
   deg = leftDist > rightDist ? deg : -deg;

   rotate(deg);
   forward(15);
   rotate(-deg);
   readUltrasonicSensors();
}

float findObject() {
    rotateToZero();
    readUltrasonicSensors();
    float minFrontDist = frontDist;

    int speed = 70;
    analogWrite(ENA, speed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENB, speed);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    while(getYaw() > -80) {
        readUltrasonicSensors();
        Serial.print("Dist: ");
        Serial.println(frontDist);
        if (frontDist < minFrontDist) {
            minFrontDist = frontDist;
        }
    }
    stop();
    Serial.print("Target: ");
    Serial.println(minFrontDist);
    rotateSonic(minFrontDist, true);
    return minFrontDist;
}

bool clearedObstacles() {
    rotateToZero();
    float left, right;
    rotate(40);
    readUltrasonicSensors();
    left = leftDist;
    rotate(-80);
    readUltrasonicSensors();
    right = rightDist;
    rotate(40);
    return left + right > 100;
}
