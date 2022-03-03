#include "variables.h"
#include "sensors.h"
#include "movement.h"

/* const int MPU = 0x68; // MPU6050 I2C address

float yaw = 0;
float gyroZ, gyroAngleZ, gyroErrorZ;
int c = 0; */

void forward(float threshold) {  // move forward until an object is detected within a certain threshold distance
    float currYaw = yaw;
    readUltrasonicSensors();
    int left, right = 0;
    if (frontDist >= threshold) {   // arbitrary for now
        analogWrite(ENA, motorLeft + left);
        digitalWrite(IN1, LOW); //Motor INS are reversed, fix wiring
        digitalWrite(IN2, HIGH);
        analogWrite(ENB, motorRight + right);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
    }
    while (frontDist >= threshold && leftDist >= 5 && rightDist >= 5) {
        readUltrasonicSensors();
        // getYaw();
        // if (currYaw - yaw > 1) {
        //     left++;
        //     analogWrite(ENA, motorLeft);
        // }
        // else if (yaw - currYaw > 1) {
        //     right++;
        //     analogWrite(ENA, motorRight);
        // }
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
    float finalYaw = initYaw + degree;
    if (degree > 0) {
        while (finalYaw - getYaw() > 0) { // threshold 0.5º difference
            analogWrite(ENA, motorLeft);
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            analogWrite(ENB, motorRight);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);

            getYaw();

            Serial.println(yaw);
        }
        // digitalWrite(IN1, LOW);
        // digitalWrite(IN2, HIGH);
        // digitalWrite(IN3, HIGH);
        // digitalWrite(IN4, LOW);
        // delay(5);
    }
    else {
        while (getYaw() - finalYaw > 0) { 
            analogWrite(ENA, motorLeft);
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            analogWrite(ENB, motorRight);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);

            getYaw();

            Serial.println(yaw);
        }

        // digitalWrite(IN1, HIGH);
        // digitalWrite(IN2, LOW);
        // digitalWrite(IN3, LOW);
        // digitalWrite(IN4, HIGH);
        // delay(5);
    }
    analogWrite(ENA, LOW);
    analogWrite(ENB, LOW);
}

// call when obstacle detected, assume leftDist != rightDist?
void aroundObstacle() {
    Serial.println("AVOIDING");
    readUltrasonicSensors();
    float deg = 0.0;
    float farDist = max(leftDist, rightDist);
    if (leftDist > rightDist) {
        deg = 80; // change later
    }
    else {
        deg = -80;
    }

    Serial.print("Rotation: ");
    Serial.println(deg);

    rotate(deg);
    forward(15.0);
    Serial.println("Rotating back");
    //Move past block
    rotate(-deg);
    forward(30.0);

    //Back to center
    rotate(-deg);
    forward((leftDist + rightDist) / 2);
    rotate(deg);
}
