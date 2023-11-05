// Replace 12345 with the correct team number and then uncomment the line below.
#define TEAM_NUMBER 2

#ifndef TEAM_NUMBER
#error "Define your team number with `#define TEAM_NUMBER 12345` at the top of the file."
#elif TEAM_NUMBER < 1 || 20 < TEAM_NUMBER
#error "Team number must be within 1 and 20"
#endif

// servo 1 states - set servo for each level of cargo bay
#define HIGH 180 // top of the delivery zone
#define MID 90   // middle of the delivery zone
#define LOW 0    // bottom of the delivery zone 

// servo 2 states - set servo swipe
#define SWIPE 180 // top of the delivery zone
#define NOSWIPE 0   // middle of the delivery zone

// scale the joystick axis - these will be in the range [-1.0, 1.0]
float scale(float val) {
  return (val * val * (val < 0.0 ? -1.0 : 1.0));
}

// combine driving for left and right motors
void RR_setLeft(float speed) { // left side 
  RR_setMotor1(speed);
  RR_setMotor2(speed);
}
void RR_setRight(float speed) { // right side
  RR_setMotor3(speed);
  RR_setMotor4(speed);
}

void setup() {
  Serial.begin(115200);

  RR_setServo1(HIGH);
  RR_setServo2(HIGH);
}

// state = 0 AUTONOMOUS
// state = 1 TELEOP
int state = 0; // state begins in idle
int offset = 0; // used in case servo 1 states need modification

void loop() {

  if (RR_buttonStart()) { 
    state ++;
    if (state > 2){
      state = 2;
    }
  }

  if (state == 1){ // AUTONOMOUS
    // follow the line

    // place the cargo - cargo should already be at top level

    // go back to the cargo pickup (maybe?)
    RR_setServo2(SWIPE);
    delay(50);
    RR_setServo2(NOSWIPE);

    // move onto teleop
  }
  
  if (state == 2){ // TELEOP

    ////////////////////////////////////////////////////////
    //                     CONTROLLER                     //
    ////////////////////////////////////////////////////////

    // Read the four joystick axes
    // These will be in the range [-1.0, 1.0]
    float rightX = scale(RR_axisRX());
    float leftY  = scale(RR_axisLY());
    float rightY = RR_axisRY(); // current unused
    float leftX  = RR_axisLX(); // current unused

    // Get the button states
    bool btnA = RR_buttonA();
    bool btnB = RR_buttonB();
    bool btnX = RR_buttonX();
    bool btnY = RR_buttonY();
    bool btnRB = RR_buttonRB();
    bool btnLB = RR_buttonLB();

    ////////////////////////////////////////////////////////
    //                       DRIVING                      //
    ////////////////////////////////////////////////////////

    // Arcade-drive scheme
    // Left Y-axis = throttle
    // Right X-axis = steering
    RR_setLeft(leftY + rightX);
    RR_setRight(leftY - rightX);

    ////////////////////////////////////////////////////////
    //                        CARGO                       //
    ////////////////////////////////////////////////////////

    // in case servo position gets altered, add an offset the driver can change
    // 6 = left, 2 = right, 0 = up, 4 = down, 8 = center
    if (RR_dpad() == 0) { // increase cargo level by few degrees
      offset += 3;
    }
    if (RR_dpad() == 4) { // decrease cargo level by few degrees
      offset -= 3;
    }

    // 3 different heights
    if (btnA) {
      RR_setServo1(HIGH + offset);
    }
    if (btnX){
      RR_setServo1(MID + offset);
    }
    if (btnB){
      RR_setServo1(LOW + offset);
    }

    // knock cargo off
    if (RR_buttonRB) {
      RR_setServo2(SWIPE);
      delay(50);
      RR_setServo2(NOSWIPE);
    }

  }
  // This is important - it sleeps for 0.02 seconds (= 50 times / second)
  // Running the code too fast will overwhelm the microcontroller and peripherals
  delay(20);
}

// vim: tabstop=2 shiftwidth=2 expandtab
