/*
 *   Project Recon 
 *   This Project is to learn the logic behind walking and maneuver of Robots,
 *   Version: 2.1
 *   Version Date: 2024-August-30
 *
 *   Release Notes : This version contain serial command center to control the robot 
                     This Version contain RC interface to control your Robot, i am using flysky 6 channel RC , its commented by default at line 95.
 *   
 *   Author:  Karan Khaswal 
 *   Brought to you by : Nex Robotic (New Era eXploration)
 *   
 *  DEV Notes : go to Recon.CPP to understand the logic and to make your own custom functions 
*/


#include <Arduino.h>
#include "Recon.h"

void RC_Control();

const int channelPins[] = {A1, A2, A3, A4, A5, A6};
const int numChannels = 6;

unsigned long pulseWidths[numChannels];
int channelValues[numChannels];

// Threshold values for mizn and max pulse widths used for RC
const int minValue = 1000;
const int maxValue = 1800;
unsigned long TIME_INTERVAL = 35000; // Timeout in microseconds (adjust based on your RC receiver) : This is pain in the ass be carefull with this 


//Serial commands mapping for movement
#define FORWARD 'f'
#define BACKWARD 'b'

#define STAND 's'

#define RIGHT 'r'
#define LEFT 'l'

#define GO 'g'

#define RIGHT_FRONT 'c'
#define RIGHT_BACK 'e'
#define LEFT_FRONT 'd'
#define LEFT_BACK 'h'
#define FRONT_BACK 'q'

#define HELLO 'z'


#define JUMP 'j'
#define DANCE 'd'

#define UMNIWALK 'o'
#define MOON_WALK 'm'
#define UP_DOWN 'u'
#define PUSH_UP 'p'

#define BODY_LEFT 't'
#define BODY_RIGHT 'w'

#define SITWIGGLE 'i'

//i have an awsom idea for this mode wait for the release
#define WHEEL_MODE 'n'

Recon robot;

bool auto_mode = false;
bool state = true;
char cmd = STAND;
bool random_walk = false;

float Speed = 500; //you can adjust this for movement speed
int heightValue = 100;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < numChannels; i++) {
    pinMode(channelPins[i], INPUT);
  }
  randomSeed(analogRead(A7)); // getting seed for random
  robot.init();
  delay(2000);

// uncomment this function (line 90) if you want to run your custom function
// robot.myfunction(5,600); delay(5000);


 systemservices();

}

void loop() {

  //RC_Control();   //uncomment this line if you want to control with the RC : "Note" if you uncomment this and rc is not bind the robot will puckup garbage reading and move randomly.
  
  if (auto_mode) {
    if (random_walk)
      auto_gaits(0);
    else
      auto_gaits(1);
  } else {
    gaits(cmd);
  }

  // Print the current command for debugging
  Serial.println(cmd);
  delay(10);
}

void serialEvent() {
  char tmp = -1;
  boolean taken = false; 

  while (Serial.available()) {
    state = !state; 
    tmp = Serial.read();
    taken = gaits(tmp); 

    if (taken) 
      cmd = tmp;
  }
}

void systemservices(){
   delay(5000);
  //robot.hello(Speed);
  //delay(1000);
  robot.walk(1,6,Speed);
  //delay(1000);
  robot.walk(0,6,Speed);
  //delay(1000);
  robot.turnR(5, Speed);
  //delay(1000);
  robot.turnL(5, Speed);
  //delay(1000);
  Speed = 1000;
  robot.pushUp(5, Speed);
  Speed = 400;
  //delay(1000);
  robot.upDown(4, Speed);
  Speed = 500; 
  //delay(1000);
  robot.sitwiggle();
  robot.twerk();
  //delay(1000);
  robot.home();
}

// types of walks, read cmd from serial
boolean  gaits(char cmd) {
  bool taken = true;
  switch (cmd) {
    case GO:
      robot.run(1,1,Speed);
      break;
    case FORWARD:
      robot.walk(1,1,Speed);
      break;
    case BACKWARD:
      robot.walk(0,1,Speed);
      break;
    case RIGHT:
      robot.turnR(1, Speed);
      break;
    case LEFT:
      robot.turnL(1, Speed);
      break;
    case RIGHT_FRONT:
      robot.turnR(1, Speed);
      robot.walk();
      break;
    case RIGHT_BACK:
      robot.turnR(1, Speed);
      robot.walk(0);
      break;
    case LEFT_FRONT:
      robot.turnL(1, Speed);
      robot.walk(1,1,Speed);
      break;
    case LEFT_BACK:
      robot.turnL(1, Speed);
      robot.walk(0,1,Speed);
      break;
    case STAND:
      robot.home();
      break;
    case HELLO:
      robot.hello(Speed);
      break;
    case PUSH_UP:
      robot.pushUp(1, Speed);
      break;
    case MOON_WALK:
      robot.moonwalkL(1, Speed);
      break;
    case JUMP:
      robot.jump();
      break;
    case UP_DOWN:
      robot.upDown(4, Speed);
      break;
    case UMNIWALK:
      robot.omniWalk();
      break;
    case FRONT_BACK:
      robot.frontBack(1,Speed);
      break;

    case BODY_RIGHT:
      robot.bodyRight(1,Speed);
      break;
    case BODY_LEFT:
      robot.bodyLeft(1,Speed);
      break;
    case WHEEL_MODE:
    robot.wheelmode();
    break;
    case SITWIGGLE:
    robot.sitwiggle();
    break;
    default:
      taken = false;
  }
  return taken;
}

// gaits to be taken if auto is enabled and random walks
void auto_gaits(int pattern) {
  char movements[] = {FORWARD, LEFT, STAND, RIGHT, BACKWARD, GO, DANCE, MOON_WALK, PUSH_UP, BODY_LEFT, BODY_RIGHT, UP_DOWN, UMNIWALK};
  char movements2[] = {FORWARD, LEFT, FORWARD,  RIGHT, BACKWARD, DANCE, MOON_WALK, STAND, PUSH_UP,BODY_LEFT,BODY_RIGHT,UP_DOWN,UMNIWALK};
  static unsigned long cur_time = 0, prev_time = 0;
  static char cmd = FORWARD, prev_cmd = -1;
  static int c = 0;

  cur_time = millis();
  if (cur_time - prev_time >= TIME_INTERVAL) {
    prev_time += TIME_INTERVAL; // fixed prev_time = cur_time that can result in cummulative errors
    do {
      switch (pattern) {
        case 0: // if random_walk is 0
          c = (int)random(0, sizeof(movements));
          cmd = movements[c];
          break;
        case 1: // if random_walk is 1 
          c = c % sizeof(movements2);
          cmd = movements2[c++];
          break;
        default:
          pattern = 0;
      }
    } while (cmd == prev_cmd); // as long as no new commands is sent from serial
    prev_cmd = cmd; 
  }

  
  switch (cmd) {
    case GO:
      robot.run(1,1,Speed);
      break;
    case FORWARD:
      robot.walk(1,1,Speed);
      break;
    case BACKWARD:
      robot.walk(0,1,Speed);
      break;
    case RIGHT:
      robot.turnR(1, Speed);
      break;
    case LEFT:
      robot.turnL(1, Speed);
      break;
    case RIGHT_FRONT:
      robot.turnR(1, Speed);
      robot.walk();
      break;
    case RIGHT_BACK:
      robot.turnR(1, Speed);
      robot.walk(0);
      break;
    case LEFT_FRONT:
      robot.turnL(1, Speed);
      robot.walk(1,1,Speed);
      break;
    case LEFT_BACK:
      robot.turnL(1, Speed);
      robot.walk(0,1,Speed);
      break;
    case STAND:
      robot.home();
      break;
    case HELLO:
      robot.hello(Speed);
      break;
    case PUSH_UP:
      robot.pushUp(1, Speed);
      break;
    case MOON_WALK:
      robot.moonwalkL(1, Speed);
      break;
    case JUMP:
      robot.jump();
      break;
    case UP_DOWN:
      robot.upDown(4, Speed);
      break;
    case UMNIWALK:
      robot.omniWalk();
      break;
    case FRONT_BACK:
      robot.frontBack(1,Speed);
      break;

    case BODY_RIGHT:
      robot.bodyRight(1,Speed);
      break;
    case BODY_LEFT:
      robot.bodyLeft(1,Speed);
      break;
    case WHEEL_MODE:
    robot.wheelmode();
    break;

    default:
      robot.home();
  }
}

// This is the code to take input from RC Channels and use them to control the robot
// note use the RC analyse code to analyse the values of your rc and make logic around it here  i am using F,B,L,R and speed control from my RC
void RC_Control(){
  Serial.println("Reading channel values...");
  //Take the channels value
  pulseWidths[0] = pulseIn(channelPins[0], HIGH, TIME_INTERVAL);
  pulseWidths[1] = pulseIn(channelPins[1], HIGH, TIME_INTERVAL);
  pulseWidths[2] = pulseIn(channelPins[2], HIGH, TIME_INTERVAL);
  pulseWidths[3] = pulseIn(channelPins[3], HIGH, TIME_INTERVAL);
  pulseWidths[4] = pulseIn(channelPins[4], HIGH, TIME_INTERVAL);
  // pulseWidths[5] = pulseIn(channelPins[5], HIGH, TIME_INTERVAL);


  channelValues[0] = (pulseWidths[0] > 1500) ? 1 : (pulseWidths[0] < 1100) ? -1 : 0;  //for left and right
  channelValues[1] = (pulseWidths[1] > 1500) ? 1 : (pulseWidths[1] < 1100) ? -1 : 0;  //for forward and backward
  // channelValues[2] = (pulseWidths[2] > 1500) ? 1 : (pulseWidths[2] < 1100) ? -1 : 0;
  // channelValues[3] = (pulseWidths[3] > 1500) ? 1 : (pulseWidths[3] < 1100) ? -1 : 0;
  channelValues[4] = (pulseWidths[4] > 1500) ? 1 : (pulseWidths[4] < 1100) ? -1 : 0;  //to control movement speed
  // channelValues[5] = (pulseWidths[5] > 1500) ? 1 : (pulseWidths[5] < 1100) ? -1 : 0;

  boolean taken2 = false; 
  char tmp2 = -1;

  // Determine command based on channel values
  if (channelValues[0] != 0) {
    if (channelValues[0] == -1) {
      tmp2 = LEFT;
    } else if (channelValues[0] == 1) {
      tmp2 = RIGHT;
    }
  } else if (channelValues[1] != 0) {
    if (channelValues[1] == -1) {
      tmp2 = BACKWARD;
    } else if (channelValues[1] == 1) {
      tmp2 =  FORWARD;
    }
  } else {
    tmp2 = STAND;  // Default to STAND if no valid channel values
  }


  Speed = map(pulseWidths[4], 1000, 2000, 300, 1500);  // Assuming 1000 to 2000 as pulse width range
  Speed = constrain(Speed, 300, 1500);  //this will map the speed with in suitable range

  taken2 = gaits(tmp2); 
  
    if (taken2) {
      cmd = tmp2;
    }
}
