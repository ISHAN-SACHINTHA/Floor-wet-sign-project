//This code is develop by Ishan_Sachintha

#include <AccelStepper.h>

#define homeX 9
#define homeY 10
#define homeZ 11


long initial_homing = -1;

long receivedSteps = 0;
long receivedSpeed = 0;
long receivedAcceleration = 0;
char receivedCommand;

int directionMultiplier = 1;
bool newData, runallowed = false;
AccelStepper stepperX(1, 2, 5);
AccelStepper stepperY(1, 3, 6);
AccelStepper stepperZ(1, 4, 7);


void setup() {
  stepperX.setEnablePin(8);
  stepperX.setPinsInverted(false, false, true);

  Serial.begin(9600);

  pinMode(homeX, INPUT_PULLUP);
  pinMode(homeY, INPUT_PULLUP);
  pinMode(homeZ, INPUT_PULLUP);

  Serial.println("Demonstration of AccelStepper Library");
  Serial.println("Send 'C' for printing the commands.");

  stepperX.disableOutputs();
}
void loop() {

  checkSerial();
  RunTheMotor();

}

void RunTheMotor()
{
  if (runallowed == true)
  {
    stepperX.enableOutputs();
    stepperX.run();
    stepperY.run();
    stepperZ.run();
  }
  else
  {
    stepperX.disableOutputs();
    return;
  }
}

void checkSerial()
{
  if (Serial.available() > 0)
  {
    receivedCommand = Serial.read();
    newData = true;

    if (newData == true)
    {
      switch (receivedCommand)
      {

        case 'P':

          receivedSteps = Serial.parseFloat();
          receivedSpeed = Serial.parseFloat();
          directionMultiplier = 1;
          Serial.println("Positive direction.");
          RotateRelative();

          //example: P2000 400 - 2000 steps (5 revolution with 400 step/rev microstepping) and 400 steps/s speed
          //In theory, this movement should take 5 seconds
          break;

        case 'N': //N uses the move() function of the AccelStepper library, which means that it moves relatively to the current position.

          receivedSteps = Serial.parseFloat(); //value for the steps
          receivedSpeed = Serial.parseFloat(); //value for the speed
          directionMultiplier = -1; //We define the direction
          Serial.println("Negative direction."); //print action
          RotateRelative(); //Run the function

          //example: N2000 400 - 2000 steps (5 revolution with 400 step/rev microstepping) and 500 steps/s speed; will rotate in the other direction
          //In theory, this movement should take 5 seconds
          break;

        case 'R': //R uses the moveTo() function of the AccelStepper library, which means that it moves absolutely to the current position.

          receivedSteps = Serial.parseFloat(); //value for the steps
          receivedSpeed = Serial.parseFloat(); //value for the speed
          directionMultiplier = 1; //We define the direction
          Serial.println("Absolute position (+)."); //print the action
          RotateAbsoluteX(); //Run the function
          RotateAbsoluteY(); //Run the function
          RotateAbsoluteZ(); //Run the function

          //example: R800 400 - It moves to the position which is located at +800 steps away from 0.
          break;

        case 'r': //r uses the moveTo() function of the AccelStepper library, which means that it moves absolutely to the current position.

          receivedSteps = Serial.parseFloat(); //value for the steps
          receivedSpeed = Serial.parseFloat(); //value for the speed
          directionMultiplier = -1; //We define the direction
          Serial.println("Absolute position (-)."); //print the action
          RotateAbsoluteX(); //Run the function

          //example: r800 400 - It moves to the position which is located at -800 steps away from 0.
          break;

        case 'S': // Stops the motor

          stepperX.stop(); //stop motor
          stepperY.stop();
          stepperZ.stop();
          stepperX.disableOutputs(); //disable power
          Serial.println("Stopped."); //print action
          runallowed = false; //disable running
          break;

        case 'A': // Updates acceleration

          runallowed = false; //we still keep running disabled, since we just update a variable
          stepperX.disableOutputs(); //disable power
          receivedAcceleration = Serial.parseFloat(); //receive the acceleration from serial
          stepperX.setAcceleration(receivedAcceleration); //update the value of the variable
          stepperY.setAcceleration(receivedAcceleration);
          stepperZ.setAcceleration(receivedAcceleration);
          Serial.print("New acceleration value: "); //confirm update by message
          Serial.println(receivedAcceleration); //confirm update by message
          break;

        case 'L': //L: Location

          runallowed = false; //we still keep running disabled
          stepperX.disableOutputs(); //disable power
          Serial.print("Current location of the motor: ");//Print the message
          Serial.println(stepperX.currentPosition()); //Printing the current position in steps.
          break;

        case 'H': //H: Homing

          runallowed = true;
          //Serial.println("Homing"); //Print the message
          Home();// Run the function
          stepperX.stop(); //stop motor
          stepperY.stop();
          stepperZ.stop();
          stepperX.disableOutputs(); //disable power
          runallowed = false; //disable running
          break;

        case 'U':

          runallowed = false; //we still keep running disabled
          stepperX.disableOutputs(); //disable power
          stepperX.setCurrentPosition(0); //Reset current position. "new home"
          stepperY.setCurrentPosition(0);
          stepperZ.setCurrentPosition(0);
          Serial.print("The current position is updated to: "); //Print message
          Serial.println(stepperX.currentPosition()); //Check position after reset.
          break;

        case 'C':

          PrintCommands(); //Print the commands for controlling the motor
          break;

        case 'x':
          receivedSteps = Serial.parseFloat(); //value for the steps
          receivedSpeed = Serial.parseFloat(); //value for the speed
          directionMultiplier = 1; //We define the direction
          Serial.println("Absolute position (+)."); //print the action
          RotateAbsoluteX(); //Run the function

          break;

        default:

          break;
      }
    }
    //after we went through the above tasks, newData is set to false again, so we are ready to receive new commands again.
    newData = false;
  }
}

void RotateRelative()
{
  //We move X steps from the current position of the stepper motor in a given direction.
  //The direction is determined by the multiplier (+1 or -1)

  runallowed = true; //allow running - this allows entering the RunTheMotor() function.
  stepperX.setMaxSpeed(receivedSpeed); //set speed
  stepperY.setMaxSpeed(receivedSpeed);
  stepperZ.setMaxSpeed(receivedSpeed);
  stepperX.move(directionMultiplier * receivedSteps); //set relative distance and direction
  stepperY.move(directionMultiplier * receivedSteps);
  stepperZ.move(directionMultiplier * receivedSteps);
}

void RotateAbsoluteX()
{
  //We move to an absolute position.
  //The AccelStepper library keeps track of the position.
  //The direction is determined by the multiplier (+1 or -1)
  //Why do we need negative numbers? - If you drive a threaded rod and the zero position is in the middle of the rod...

  runallowed = true; //allow running - this allows entering the RunTheMotor() function.
  stepperX.setMaxSpeed(receivedSpeed); //set speed
  stepperX.moveTo(directionMultiplier * receivedSteps); //set relative distance
}

void RotateAbsoluteY()
{
  //We move to an absolute position.
  //The AccelStepper library keeps track of the position.
  //The direction is determined by the multiplier (+1 or -1)
  //Why do we need negative numbers? - If you drive a threaded rod and the zero position is in the middle of the rod...

  runallowed = true; //allow running - this allows entering the RunTheMotor() function.
  stepperY.setMaxSpeed(receivedSpeed);
  stepperY.moveTo(directionMultiplier * receivedSteps);
}

void RotateAbsoluteZ()
{
  //We move to an absolute position.
  //The AccelStepper library keeps track of the position.
  //The direction is determined by the multiplier (+1 or -1)
  //Why do we need negative numbers? - If you drive a threaded rod and the zero position is in the middle of the rod...

  runallowed = true; //allow running - this allows entering the RunTheMotor() function.
  stepperZ.setMaxSpeed(receivedSpeed);
  stepperZ.moveTo(directionMultiplier * receivedSteps);
}

void Home() {
  stepperX.setMaxSpeed(10000);
  stepperX.setAcceleration(800);
  stepperY.setMaxSpeed(10000);
  stepperY.setAcceleration(800);
  stepperZ.setMaxSpeed(10000);
  stepperZ.setAcceleration(800);
  stepperX.enableOutputs();
  Serial.print("Stepper is Homing . . . . . . . . . . . ");
  while (digitalRead(homeX) || digitalRead(homeY) || digitalRead(homeZ)) {
    if (digitalRead(homeX) == 1) {
      stepperX.moveTo(initial_homing);
      stepperX.run();
    }
//    if (digitalRead(homeY) == 1) {
//      stepperY.moveTo(initial_homing);
//      stepperY.run();
//    }
//    if (digitalRead(homeZ) == 1) {
//      stepperZ.moveTo(initial_homing);
//      stepperZ.run();
//    }
    initial_homing--;
  }

  stepperX.setCurrentPosition(0);  // Set the current position as zero for now
  stepperY.setCurrentPosition(0);
  stepperZ.setCurrentPosition(0);
  stepperX.setMaxSpeed(1000);      // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperY.setMaxSpeed(1000);
  stepperZ.setMaxSpeed(1000);
  stepperX.setAcceleration(100);  // Set Acceleration of Stepper
  stepperY.setAcceleration(100);
  stepperZ.setAcceleration(100);
  initial_homing = 1;

  while (!digitalRead(homeX) || !digitalRead(homeY) || !digitalRead(homeZ)) {
    if (digitalRead(homeX) == 0) {
      stepperX.moveTo(initial_homing);
      stepperX.run();
    }
    if (digitalRead(homeY) == 0) {
      stepperY.moveTo(initial_homing);
      stepperY.run();
    }
    if (digitalRead(homeZ) == 0) {
      stepperZ.moveTo(initial_homing);
      stepperZ.run();
    }
    initial_homing++;
  }

  stepperX.setCurrentPosition(0);  // Set the current position as zero for now
  stepperY.setCurrentPosition(0);
  stepperZ.setCurrentPosition(0);
  Serial.println("Homing Completed");
  Serial.println("");
  stepperX.stop(); //stop motor
  stepperY.stop();
  stepperZ.stop();
  stepperX.disableOutputs(); //disable power
  runallowed = false; //disable running
}

void PrintCommands()
{
  //Printing the commands
  Serial.println(" 'C' : Prints all the commands and their functions.");
  Serial.println(" 'P' : Rotates the motor in positive (CW) direction, relative.");
  Serial.println(" 'N' : Rotates the motor in negative (CCW) direction, relative.");
  Serial.println(" 'R' : Rotates the motor to an absolute positive position (+).");
  Serial.println(" 'r' : Rotates the motor to an absolute negative position (-).");
  Serial.println(" 'S' : Stops the motor immediately.");
  Serial.println(" 'A' : Sets an acceleration value.");
  Serial.println(" 'L' : Prints the current position/location of the motor.");
  Serial.println(" 'H' : Goes back to 0 position from the current position (homing).");
  Serial.println(" 'U' : Updates the position current position and makes it as the new 0 position. ");
}
