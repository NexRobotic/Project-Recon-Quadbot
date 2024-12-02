#include "Recon.h"

/* 
    (pin to attach pwm)
    __________ __________ _________ ________
    |(9)_______)(2)          (8)(________(3)|
    |__|      |left BACK  right|         |__|
              |                |
              |                |
              |                |
    _________ |     FRONT      | __________
    |(5)_____)(6)_____________(4)(_______(7)|
    |__|                                 |__|

*/


Recon::Recon()
  : reverse{ 0, 0, 0, 0, 0, 0, 0, 0 }, trim{ 0, 0, 0, 0, 0, 0, 0, 0 } {
  board_pins[FRONT_RIGHT_HIP] = 2;  // front left inner
  board_pins[FRONT_LEFT_HIP] = 8;   // front right inner
  board_pins[BACK_RIGHT_HIP] = 4;   // back left inner
  board_pins[BACK_LEFT_HIP] = 6;    // back right inner
  board_pins[FRONT_RIGHT_LEG] = 3;  // front left outer
  board_pins[FRONT_LEFT_LEG] = 9;   // front right outer
  board_pins[BACK_RIGHT_LEG] = 5;   // back left outer
  board_pins[BACK_LEFT_LEG] = 7;    // back right outer
}

void Recon::init() {

  // You Can tweak some more values to account for minor deviations.
  trim[FRONT_LEFT_HIP] = 3;
  trim[FRONT_RIGHT_HIP] = 0;
  trim[BACK_LEFT_HIP] = 5;
  trim[BACK_RIGHT_HIP] = 0;

  trim[FRONT_LEFT_LEG] = 3;
  trim[FRONT_RIGHT_LEG] = -3;
  trim[BACK_LEFT_LEG] = -2;
  trim[BACK_RIGHT_LEG] = 0;

  for (int i = 0; i < 8; i++) {
    oscillator[i].start();
    servo[i].attach(board_pins[i]);
  }
  home();
}

// Some of my custom Function to play with the robot.
// Dont forget to declare, Define and Call your Custom function in Recon.h, Recon.cpp, recon.main respectively  (Basic OPPS Concept)

// Custom Functions


void Recon::myfunction(float steps, float T) {


  int offset[] = {
    90, // Front left shoulder servo 2
    90,  // Front right shoulder servo 8

    90,  // Front left leg servo 3
    90,  // Front right leg servo 9

    90,  // Rear left Shoulder servo 4
    90,  // Rear right Shoulder servo 6

    90,  // Rear left leg servo 5
    90   // Rear right leg servo 7 
  }; 

  float period[] = { T, T, T, T, T, T, T, T };

  int amplitude[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  
  
  int phase[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  execute(steps, period, amplitude, offset, phase);
}


void Recon::pushUp(float steps, float T = 1000) {
  int z_amp = 40;
  int x_amp = 25;
  int hi = 30;
  int ap = 80;

  float period[] = { T, T, T, T, T, T, T, T };

  int amplitude[] = { 0, 0, z_amp, z_amp, 0, 0, 0, 0 };

  int offset[] = {
    90 + x_amp,
    90 - x_amp,
    90 - hi,
    90 + hi,
    90 - ap,
    90 + ap,
    90 + hi,
    90 - hi
  };

  int phase[] = { 0, 90, 180, 0, 0, 0, 0, 0 };

  execute(steps, period, amplitude, offset, phase);
}

void Recon::bodyRight(float steps, float T = 600) {
  int x_amp = 35;
  int z_amp = 15;
  int ap = 15;
  int hi = 50;

  float period[] = { T, T, T, T, T, T, T, T };

  int amplitude[] = { 0, 0, 0, 0, x_amp, x_amp, 0, 0 };

  int offset[] = { 90 + ap, 90 - ap, 90 - hi, 90 + hi, 90 - ap, 90 + ap, 90 + hi, 90 - hi };

  int phase[] = { 0, 0, 90, 270, 0, 0, 270, 90 };

  execute(steps, period, amplitude, offset, phase);
}

void Recon::bodyLeft(float steps, float T = 600) {
  int x_amp = 35;
  int z_amp = 15;
  int ap = 15;
  int hi = 50;

  float period[] = { T, T, T, T, T, T, T, T };

  int amplitude[] = { 0, 0, 0, 0, x_amp, x_amp, 0, 0 };

  int offset[] = { 90 + ap, 90 - ap, 90 - hi, 90 + hi, 90 - ap, 90 + ap, 90 + hi, 90 - hi };

  int phase[] = { 0, 0, 90, 270, 0, 0, 270, 90 };

  execute(steps, period, amplitude, offset, phase);
}

void Recon::adjustHeight(int heightPercentage) {

  heightPercentage = constrain(heightPercentage, 0, 100);
  int ap = 20;
  int hi = 0;


  int minOffset = 30;
  int maxOffset = 100;
  int servoOffset = map(heightPercentage, 0, 100, minOffset, maxOffset);
  int servopos = 180;

  // Define the parameters for the height adjustment
  int amplitude[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  int offset[] = {
    90 + ap,            // Front left shoulder servo 2
    90 - ap,            // Front right shoulder servo 8
    servoOffset - 0,    // Front left leg servo 3  S-0
    180 - servoOffset,  // Front right leg servo 9

    90 - ap,            // Rear left Shoulder servo 4
    90 + ap,            // Rear right Shoulder servo 6 S 90
    180 - servoOffset,  // Rear left leg servo 5 S 180
    servoOffset - 0     // Rear right leg servo 7
  };
  int phase[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  float period[] = { 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000 };

  // Set up the oscillators for height adjustment
  for (int i = 0; i < 8; i++) {
    oscillator[i].reset();
    oscillator[i].setPeriod(period[i]);
    oscillator[i].setAmplitude(amplitude[i]);
    oscillator[i].setPhase(phase[i]);
    oscillator[i].setOffset(offset[i]);
  }

  // Refresh the servos to adjust the height
  for (int i = 0; i < 8; i++) {
    setServo(i, oscillator[i].refresh());
  }
}

void Recon::jump() {
  int z_amp = 40;
  int x_amp = 95;
  int ap = 20;
  int ip = 40;
  int hi = 0;
  int T = 1000;
  float period[] = { T, T, T, T, T, T, T, T };
  int amplitude[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  int offset[] = {
    90 + ap,  // Front left shoulder servo 2
    90 - ap,  // Front right shoulder servo 8
    90 - hi,  // Front left leg servo 3  S-0
    90 + hi,  // Front right leg servo 9

    90 - x_amp,  // Rear left Shoulder servo 4
    90 + x_amp,  // Rear right Shoulder servo 6 S 90
    90 + ip,     // Rear left leg servo 5 S 180
    90 - ip      // Rear right leg servo 7
  };

  int phase[] = { 0, 0, 180, 0, 0, 0, 180, 0 };

  execute(1, period, amplitude, offset, phase);

  delay(200);
  home();
  delay(900);
}

void Recon::sit() {
  int ap = 20;
  int hi = 50;
  int position[] = { 90 + ap, 90 - ap, 90 - hi, 90 + hi, 90 - ap, 90 + ap, 90 + hi, 90 - hi };
  for (int i = 0; i < 8; i++) {
    if (position[i] + trim[i] <= 180 && position[i] + trim[i] > 0) {
      setServo(i, position[i] + trim[i]);
    }
  }
}
void Recon::wheelmode() {

  int ap = 90;
  int T = 1000;
  float period[] = { T, T, T, T, T, T, T, T };
  int amplitude[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  int offset[] = {
    90 + ap,
    90 - ap,
    90 - ap,
    90 + ap,

    90 - ap,
    90 + ap,

    90 + ap,
    90 - ap
  };

  int phase[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  execute(1, period, amplitude, offset, phase);
}

void Recon::sitwiggle() {
  int ap = 90;
  int hi = 90;
  int ti = -20;
  int T = 400;
  float period[] = { T, T, T, T, T, T, T, T };
  int amplitude[] = { 0, 0, 0, 0, 30, 30, 0, 0 };
  //int amplitude[] = { 0, 0, 20, 20, 0, 0, 0, 0 };

  int offset[] = {
    90 + ap,
    90 - ap,
    90 - ap,
    90 + ap,

    90 - hi,
    90 + hi,
    90 + ti,
    90 - ti
  };
  int phase[] = { 0, 0, 90, 180, 0, 0, 0, 0 };

  execute(5, period, amplitude, offset, phase);

  int amplitude2[] = { 0, 0, 50, 50, 0, 0, 0, 0 };
  execute(5, period, amplitude2, offset, phase);
}

void Recon::crabstand() {
  int ap = 90;
  int hi = 0;
  int T = 400;
  float period[] = { T, T, T, T, T, T, T, T };
  int amplitude[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  int offset[] = {
    90 + ap,
    90 - ap,
    90 - ap,
    90 + ap,

    90 + hi,
    90 - hi,
    90 + ap,
    90 - ap
  };

  int phase[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  execute(1, period, amplitude, offset, phase);
}

void Recon::twerk() {
  //Twerk
  float steps = 13;
  float T = 350;
  int x_amp = 0;
  int z_amp = 35;
  int ap = 20;
  //int hi = 25;
  int hi = 50;
  int front_x = 0;
  float period[] = { T, T, T, T, T, T, T, T };
  int amplitude[] = { x_amp, x_amp, x_amp, x_amp, x_amp, x_amp, z_amp, z_amp };
  int offset[] = { 90,
                   90,
                   90 - hi,
                   90 + hi,
                   90 - 90,
                   90 + 90,
                   90 + hi,
                   90 - hi };
  // int phase[] = { 0, 0, 90, 270, 90, 270, 90, 90 };  //Can be used for running
  int phase[] = { 0, 0, 90, 270, 90, 270, 270, 90 };

  execute(steps, period, amplitude, offset, phase);
}

//Custom Functions


void Recon::turnL(float steps, float T = 600) {
  int x_amp = 15;
  int z_amp = 15;
  int ap = 15;
  int hi = 23;
  float period[] = { T, T, T, T, T, T, T, T };
  int amplitude[] = { x_amp, x_amp, z_amp, z_amp, x_amp, x_amp, z_amp, z_amp };
  int offset[] = { 90 + ap, 90 - ap, 90 - hi, 90 + hi, 90 - ap, 90 + ap, 90 + hi, 90 - hi };
  int phase[] = { 0, 180, 90, 90, 180, 0, 90, 90 };

  execute(steps, period, amplitude, offset, phase);
}


void Recon::turnR(float steps, float T = 600) {
  int x_amp = 15;
  int z_amp = 15;
  int ap = 15;
  int hi = 23;
  float period[] = { T, T, T, T, T, T, T, T };
  int amplitude[] = { x_amp, x_amp, z_amp, z_amp, x_amp, x_amp, z_amp, z_amp };
  int offset[] = { 90 + ap, 90 - ap, 90 - hi, 90 + hi, 90 - ap, 90 + ap, 90 + hi, 90 - hi };
  int phase[] = { 180, 0, 90, 90, 0, 180, 90, 90 };

  execute(steps, period, amplitude, offset, phase);
}

void Recon::dance(float steps, float T = 600) {
  int x_amp = 0;
  int z_amp = 40;
  int ap = 30;
  int hi = 0;
  float period[] = { T, T, T, T, T, T, T, T };
  int amplitude[] = { x_amp, x_amp, z_amp, z_amp, x_amp, x_amp, z_amp, z_amp };
  int offset[] = { 90 + ap, 90 - ap, 90 - hi, 90 + hi, 90 - ap, 90 + ap, 90 + hi, 90 - hi };
  int phase[] = { 0, 0, 0, 270, 0, 0, 90, 180 };

  execute(steps, period, amplitude, offset, phase);
}

void Recon::frontBack(float steps, float T = 600) {
  int x_amp = 30;
  int z_amp = 25;
  int ap = 20;
  int hi = 0;
  float period[] = { T, T, T, T, T, T, T, T };
  int amplitude[] = { x_amp, x_amp, z_amp, z_amp, x_amp, x_amp, z_amp, z_amp };
  int offset[] = { 90 + ap, 90 - ap, 90 - hi, 90 + hi, 90 - ap, 90 + ap, 90 + hi, 90 - hi };
  int phase[] = { 0, 180, 270, 90, 0, 180, 90, 270 };

  execute(steps, period, amplitude, offset, phase);
}


void Recon::run(int dir, float steps, float T) {
  int x_amp = 15;
  int z_amp = 15;
  int ap = 15;
  int hi = 0;
  int front_x = 0;
  float period[] = { T, T, T, T, T, T, T, T };
  int amplitude[] = { x_amp, x_amp, z_amp, z_amp, x_amp, x_amp, z_amp, z_amp };
  int offset[] = { 90 + ap - front_x,
                   90 - ap + front_x,
                   90 - hi,
                   90 + hi,
                   90 - ap - front_x,
                   90 + ap + front_x,
                   90 + hi,
                   90 - hi };
  int phase[] = { 0, 0, 90, 90, 180, 180, 90, 90 };
  if (dir == 1) {
    phase[0] = phase[1] = 180;
    phase[4] = phase[5] = 0;
  }
  execute(steps, period, amplitude, offset, phase);
}


void Recon::omniWalk(float steps, float T, bool side, float turn_factor) {
  int x_amp = 15;
  int z_amp = 15;
  int ap = 15;
  int hi = 23;
  int front_x = 6 * (1 - pow(turn_factor, 2));
  float period[] = { T, T, T, T, T, T, T, T };
  int amplitude[] = { x_amp, x_amp, z_amp, z_amp, x_amp, x_amp, z_amp, z_amp };
  int offset[] = { 90 + ap - front_x,
                   90 - ap + front_x,
                   90 - hi,
                   90 + hi,
                   90 - ap - front_x,
                   90 + ap + front_x,
                   90 + hi,
                   90 - hi };

  int phase[8];
  if (side) {
    int phase1[] = { 0, 0, 90, 90, 180, 180, 90, 90 };
    int phase2R[] = { 0, 180, 90, 90, 180, 0, 90, 90 };
    for (int i = 0; i < 8; i++)
      phase[i] = phase1[i] * (1 - turn_factor) + phase2R[i] * turn_factor;
  } else {
    int phase1[] = { 0, 0, 90, 90, 180, 180, 90, 90 };
    int phase2L[] = { 180, 0, 90, 90, 0, 180, 90, 90 };
    for (int i = 0; i < 8; i++)
      phase[i] = phase1[i] * (1 - turn_factor) + phase2L[i] * turn_factor + oscillator[i].getPhaseProgress();
  }

  execute(steps, period, amplitude, offset, phase);
}

void Recon::moonwalkL(float steps, float T = 5000) {
  int z_amp = 45;
  float period[] = { T, T, T, T, T, T, T, T };
  int amplitude[] = { 0, 0, z_amp, z_amp, 0, 0, z_amp, z_amp };
  int offset[] = { 90, 90, 90, 90, 90, 90, 90, 90 };
  int phase[] = { 0, 0, 0, 120, 0, 0, 180, 290 };

  execute(steps, period, amplitude, offset, phase);
}


void Recon::walk(int dir, float steps, float T) {
  int x_amp = 15;
  int z_amp = 15;
  int ap = 15;
  int hi = 25;
  int front_x = 5;
  float period[] = { T, T, T / 2, T / 2, T, T, T / 2, T / 2 };
  int amplitude[] = { x_amp, x_amp, z_amp, z_amp, x_amp, x_amp, z_amp, z_amp };
  int offset[] = { 90 + ap - front_x,
                   90 - ap + front_x,
                   90 - hi,
                   90 + hi,
                   90 - ap - front_x,
                   90 + ap + front_x,
                   90 + hi,
                   90 - hi };
  int phase[] = { 270, 270, 90, 270, 90, 90, 270, 90 };
  if (dir == 0) {  //backward
    phase[0] = phase[1] = 90;
    phase[4] = phase[5] = 270;
    offset[0] = 90 + ap + front_x;
    offset[1] = 90 - ap - front_x;
    offset[4] = 90 - ap + front_x;
    offset[5] = 90 + ap - front_x;
  }
  for (int i = 0; i < 8; i++) {
    oscillator[i].reset();
    oscillator[i].setPeriod(period[i]);
    oscillator[i].setAmplitude(amplitude[i]);
    oscillator[i].setPhase(phase[i]);
    oscillator[i].setOffset(offset[i]);
  }

  _final_time = millis() + period[0] * steps;
  _init_time = millis();
  bool side;
  while (millis() < _final_time) {
    side = (int)((millis() - _init_time) / (period[0] / 2)) % 2;
    setServo(0, oscillator[0].refresh());
    setServo(1, oscillator[1].refresh());
    setServo(4, oscillator[4].refresh());
    setServo(5, oscillator[5].refresh());

    if (side == 0) {
      setServo(3, oscillator[3].refresh());
      setServo(6, oscillator[6].refresh());
    } else {
      setServo(2, oscillator[2].refresh());
      setServo(7, oscillator[7].refresh());
    }
    delay(10);
  }
}

void Recon::upDown(float steps, float T = 3000) {
  int x_amp = 0;
  int z_amp = 35;
  int ap = 20;
  //int hi = 25;
  int hi = 25;
  int front_x = 0;
  float period[] = { T, T, T, T, T, T, T, T };
  int amplitude[] = { x_amp, x_amp, z_amp, z_amp, x_amp, x_amp, z_amp, z_amp };
  int offset[] = { 90 + ap - front_x,
                   90 - ap + front_x,
                   90 - hi,
                   90 + hi,
                   90 - ap - front_x,
                   90 + ap + front_x,
                   90 + hi,
                   90 - hi };
  int phase[] = { 0, 0, 90, 270, 180, 180, 270, 90 };

  execute(steps, period, amplitude, offset, phase);
}




void Recon::hello(int T) {
  // float sentado[]={90+15,90-15,90-65,90+65,90+20,90-20,90+10,90-10};
  //   moveServos(150, sentado);
  //   delay(200);

  int z_amp = 40;
  int x_amp = 60;
  int ap = 10;
  int hi = 70;
  float period[] = { T, T, T, T, T, T, T, T };
  //int amplitude[] = { 0, 0, 0, 0, 0, 0, 0,0 };
  int amplitude[] = { 0, 50, 30, 0, 0, 0, 0, 0 };

  int offset[] = {
    90 + 15,
    40,

    90 - 90,
    90,

    90 + ap,
    90 - ap,

    90 + hi,
    90 - hi
  };
  int phase[] = { 0, 180, 90, 0, 0, 0, 0, 0 };

  execute(4, period, amplitude, offset, phase);

  // float goingUp[]={160,20,90,90,90-20,90+20,90+10,90-10};
  // moveServos(500, goingUp);
  // delay(200);
}


void Recon::home() {
  int ap = 20;
  int hi = 0;
  int position[] = { 90 + ap, 90 - ap, 90 - hi, 90 + hi, 90 - ap, 90 + ap, 90 + hi, 90 - hi };
  for (int i = 0; i < 8; i++) {
    if (position[i] + trim[i] <= 180 && position[i] + trim[i] > 0) {
      setServo(i, position[i] + trim[i]);
    }
  }
}



void Recon::reverseServo(int id) {
  if (reverse[id])
    reverse[id] = 0;
  else
    reverse[id] = 1;
}

void Recon::setServo(int id, float target) {
  if (!reverse[id])
    servo[id].writeMicroseconds(angToUsec(target + trim[id]));
  else
    servo[id].writeMicroseconds(angToUsec(180 - (target + trim[id])));
  _servo_position[id] = target + trim[id];
}

float Recon::getServo(int id) {
  return _servo_position[id];
}

void Recon::moveServos(int time, float target[8]) {
  if (time > 10) {
    for (int i = 0; i < 8; i++) _increment[i] = (target[i] - _servo_position[i]) / (time / 10.0);
    _final_time = millis() + time;

    while (millis() < _final_time) {
      _partial_time = millis() + 10;
      for (int i = 0; i < 8; i++) setServo(i, _servo_position[i] + _increment[i]);
      while (millis() < _partial_time)
        ;  //pause
    }
  } else {
    for (int i = 0; i < 8; i++) setServo(i, target[i]);
  }
  for (int i = 0; i < 8; i++) _servo_position[i] = target[i];
}

void Recon::execute(float steps, float period[8], int amplitude[8], int offset[8], int phase[8]) {
  for (int i = 0; i < 8; i++) {
    oscillator[i].setPeriod(period[i]);
    oscillator[i].setAmplitude(amplitude[i]);
    oscillator[i].setPhase(phase[i]);
    oscillator[i].setOffset(offset[i]);
  }

  unsigned long global_time = millis();

  for (int i = 0; i < 8; i++) oscillator[i].setTime(global_time);

  _final_time = millis() + period[0] * steps;
  while (millis() < _final_time) {
    for (int i = 0; i < 8; i++) {
      setServo(i, oscillator[i].refresh());
    }
    yield();
  }
}
int Recon::angToUsec(float value) {
  return value / 180 * (MAX_PULSE_WIDTH - MIN_PULSE_WIDTH) + MIN_PULSE_WIDTH;
};

void Recon::setTrim(int index, int value) {
  trim[index] = value;
}
