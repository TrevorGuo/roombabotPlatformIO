#include "variables.h"
#include "sensors.h"
#include "movement.h"

/* const int MPU = 0x68; // MPU6050 I2C address

float yaw = 0;
float gyroZ, gyroAngleZ, gyroErrorZ;
int c = 0; */

void forward() {  // move forward until an object is detected within a certain threshold distance
    readUltrasonicSensors();
    if (frontDist >= 25 && leftDist >= 10 && rightDist >= 10) {   // arbitrary for now
        analogWrite(ENA, motorSpeed);
        digitalWrite(IN1, LOW); //Motor INS are reversed, fix wiring
        digitalWrite(IN2, HIGH);
        analogWrite(ENB, motorSpeed);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
    }
    while (frontDist >= 25 && leftDist >= 10 && rightDist >= 10) {
         readUltrasonicSensors();
    }
    
    analogWrite(ENA, LOW);
    analogWrite(ENB, LOW);
}

/* equivalent degree values:
 *  +/- 180º -> +/- 155º
 *  +/- 90º -> +/- 68º
 */ 
void rotate(float degree) {
    float initYaw = getYaw();
    float finalYaw = initYaw + degree / 1.05882;
    if (degree > 0) {
        while (finalYaw - getYaw() > 2) { // threshold 0.5º difference
            analogWrite(ENA, motorSpeed);
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            analogWrite(ENB, motorSpeed);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);

            getYaw();

            Serial.print("Yaw: ");
            Serial.println(yaw);
        }
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        delay(5);
        analogWrite(ENA, LOW);
        analogWrite(ENB, LOW);
    }
    else {
        while (getYaw() - finalYaw > 2) { 
            analogWrite(ENA, motorSpeed);
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            analogWrite(ENB, motorSpeed);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);

            getYaw();
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
            Serial.print("Yaw: ");
            Serial.println(yaw);
        }

        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        delay(5);
        analogWrite(ENA, LOW);
        analogWrite(ENB, LOW);
    }
}

// call when obstacle detected, assume leftDist != rightDist?
void aroundObstacle() {
    Serial.println("AVOIDING");
    readUltrasonicSensors();
    float deg = 0.0;
    bool left;
    if (leftDist > rightDist) {
        deg = 45; // change later
        left = true;
    }
    else {
        deg = -45;
        left = false;
    }
    Serial.print("Rotation: ");
    Serial.println(deg);

    rotate(deg);


    analogWrite(ENA, motorSpeed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENB, motorSpeed);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    while(left ? leftDist : rightDist > 10) {
        readUltrasonicSensors();
    }

    analogWrite(ENA, LOW);
    analogWrite(ENB, LOW);
    Serial.println("Rotating back");


    rotate(-deg);
}
