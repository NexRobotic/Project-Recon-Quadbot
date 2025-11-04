
/*
Project Recon : Servo Calibration Code

This code sets all servos to 90 degrees, establishing the initial position of the robot.
Why 90 degrees? It represents the middle of our movement range (0 to 180 degrees).
By starting at the midpoint, the servos have room to move both forward and backward,
giving the robot balanced flexibility for movement in both directions.
This is important for the quadbot to ensure smooth and consistent initial movement.

once all the servo are at 90 degree attach the servo to the body so that initial position of the bot look like this

Front view

     --------[ ]---------
     |                  |
     |                  |


Top View 

    ---------  ----------
             []
    ---------  ----------

*/
#include <Servo.h>

// Define servo objects
Servo servo0;
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;
Servo servo7;

// Define servo pin numbers
int servoPins[] = {2, 3, 4, 5, 6, 7, 8, 9};

// Create an array of Servo objects
Servo servos[] = {servo0, servo1, servo2, servo3, servo4, servo5, servo6, servo7};

void setup() {
  // Attach each servo to the corresponding pin
  for (int i = 0; i < 8; i++) {
    servos[i].attach(servoPins[i]);
  }

  // Set all servos to the 90-degree position
  for (int i = 0; i < 8; i++) {
    servos[i].write(90);
  }

  // Optionally, add a delay to allow servos to reach the 90-degree position
  delay(1000);
}

void loop() {
  // Your main code here
}
