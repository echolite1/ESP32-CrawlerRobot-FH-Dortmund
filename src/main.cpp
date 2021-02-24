#include <Arduino.h>
#include "motorDriver.h"
#include <sensorDriver.h>

void task1(void * parameter);
static void mainLogic();

const uint8_t left = 0;
const uint8_t front = 1;
const uint8_t right = 2;
const uint8_t back = 3;

const uint8_t maxSpeed = 255;
const uint8_t minSpeed = 80;
const uint8_t optimalSpeed = 140;

const uint16_t maxDistance = 300;
const uint8_t minDistance = 55;
const uint8_t optimalDistance = 140;

static void moveForward(uint8_t set_speed);
static void moveBackward(uint8_t set_speed);
static void moveForwardT(uint8_t set_speed, int time);
static void moveBackwardT(uint8_t set_speed, int time);
static void stopMotors();
static void turnRight(uint8_t set_speed, int time);
static void turnLeft(uint8_t set_speed, int time);
static void makeCirlce(int direction, int duration, uint8_t set_speed);

static bool obstacle(int direction,int distance);
static void showProximity(int direction);
static int16_t getProximity(int direction);

void setup(){
    Serial.begin(9600);
    motorobject.SETUP();
    sensorobject.SETUP();
    delay(1500);
    xTaskCreate(    task1,          /* Task function. */
                    "",        /* String with name of task. */
                    15360,              /* Stack size in bytes. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL);            /* Task handle. */
}

void loop(){
}

void task1(void * parameter){

    for( ;; ){
        mainLogic();
    }
    vTaskDelete(NULL);
}

void mainLogic(){
    // find target by calculating Angle

    if(obstacle(front, maxDistance) && obstacle(left, maxDistance) && obstacle(right, maxDistance)){
        moveForward(maxSpeed);
        // Serial.println("nothing less than 300 in all directions");
        // vTaskDelay(30);
    }
    if(!obstacle(left, optimalDistance) && !obstacle(right, optimalDistance) && !obstacle(front, minDistance)){
        //moveBackwardT(minSpeed, 2500);
        makeCirlce(back, 2500, maxSpeed);
        //Serial.println("smth at the R + L less than 140 and F less than 35");
        //vTaskDelay(30);
    } 
    else if(!obstacle(left, maxDistance) && !obstacle(right, maxDistance)){
        moveForwardT(minSpeed, 400);
        //Serial.println("smth at the R + L less than 140");
        // showProximity(right);
        // showProximity(left);
        // vTaskDelay(30);
    } 
    else {
        if(!obstacle(right, optimalDistance)){
            turnLeft(maxSpeed, 100);
            //Serial.println("smth at the R less than 140");
            //showProximity(right);
            //vTaskDelay(30);
        } 
        if(!obstacle(left, optimalDistance)){
            turnRight(maxSpeed, 100);
            //Serial.println("smth at the L less than 140");
            //showProximity(left);
            //vTaskDelay(30);
        }
    }
}

void makeCirlce(int direction, int duration, uint8_t set_speed){
    if (direction == 3){
        motorobject.set_speed(MotorB, Backward, set_speed);
        motorobject.set_speed(MotorA, Forward, set_speed - 185);
    } else {
        motorobject.set_speed(MotorA, Backward, set_speed);
        motorobject.set_speed(MotorB, Forward, set_speed - 185);
    }
    vTaskDelay(duration);
}
// сделать плавный старт
void moveForward(uint8_t set_speed){
    motorobject.set_speed(MotorA, Backward, set_speed);
    motorobject.set_speed(MotorB, Forward, set_speed);
    vTaskDelay(30);
}
// сделать плавный старт
void moveBackward(uint8_t set_speed){
    motorobject.set_speed(MotorB, Backward, set_speed);
    motorobject.set_speed(MotorA, Forward, set_speed);
    vTaskDelay(30);
}
// сделать плавный старт
void moveForwardT(uint8_t set_speed, int time){
    motorobject.set_speed(MotorA, Backward, set_speed);
    motorobject.set_speed(MotorB, Forward, set_speed);
    vTaskDelay(30 + time);
}
// сделать плавный старт
void moveBackwardT(uint8_t set_speed, int time){
    //stopMotors(); TODO:
    motorobject.set_speed(MotorB, Backward, set_speed);
    motorobject.set_speed(MotorA, Forward, set_speed);
    vTaskDelay(30 + time);
    //stopMotors(); TODO:
}

void stopMotors(){
    motorobject.set_speed(MotorA, Forward, 0);
    motorobject.set_speed(MotorB, Forward, 0);
    vTaskDelay(30);
}
// сделать плавный старт
void turnRight(uint8_t set_speed, int time){
    motorobject.set_speed(MotorB, Backward, set_speed);
    motorobject.set_speed(MotorA, Backward, set_speed);
    vTaskDelay(30 + time);
}
// сделать плавный старт
void turnLeft(uint8_t set_speed, int time){
    motorobject.set_speed(MotorB, Forward, set_speed);
    motorobject.set_speed(MotorA, Forward, set_speed);
    vTaskDelay(30 + time);
}

void showProximity(int direction){
    int16_t *proximityArray;
    proximityArray = sensorobject.reading();
    delay(30);

    // do{
    //     proximityArray = sensorobject.reading();
    //     delay(30);
    // }while(proximityArray[direction] == int(proximityArray[direction]));

    // while(proximityArray[direction] <= 0){
    //     proximityArray[direction] = 1000;
    // }

    // while(proximityArray[direction] < 14){
    //     proximityArray[direction] = 14;
    // }

    Serial.println(proximityArray[direction]);
}

int16_t getProximity(int direction){
    int16_t* proximityArray;
    proximityArray = sensorobject.reading();
    delay(30);

    // do{
    //     proximityArray = sensorobject.reading();
    //     delay(30);
    // }while(proximityArray[direction] == int(proximityArray[direction]));

    // while(proximityArray[direction] < 1){
    //     proximityArray[direction] = 1000;
    // }

    // while(proximityArray[direction] < 14){
    //     proximityArray[direction] = 14;
    // }
    return proximityArray[direction];
}

bool obstacle(int direction,int distance){
    int16_t *proximityArray;
    bool obstacle;

    proximityArray = sensorobject.reading();
    delay(1);
    if(proximityArray[direction] > distance){
        obstacle = true;
    } else {
        obstacle = false;
    }
    return obstacle;
}
