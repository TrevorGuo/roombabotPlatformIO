#ifndef MOVEMENT_H
#define MOVEMENT_H

void forward(float threshold);
void reverse(float threshold);
void rotate(float degree);
void rotateSonic(float distance, bool left);
void rotateToZero();
void aroundObstacle();
float findObject();
bool clearedObstacles();

#endif // MOVEMENT_H