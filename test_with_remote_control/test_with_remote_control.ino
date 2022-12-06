//This code is developed by Ishan_Sachintha

#include <AccelStepper.h>
#include <RH_ASK.h>
#include <SPI.h>

#define homeX 9
#define homeY 10
#define homeZ 11

RH_ASK driver(2000, 12, 12, 0);

long initial_homing = -1;

long Xsteps = 0;
long Xspeed = 0;
long Xacceleration = 100;
long Ysteps = 0;
long Yspeed = 0;
long Yacceleration = 100;
long Zsteps = 0;
long Zspeed = 0;
long Zacceleration = 100;

int anime = 0;

int directionMultiplierX = 1;
int directionMultiplierY = 1;
int directionMultiplierZ = 1;
bool newData, runallowed = false;
AccelStepper stepperX(1, 2, 5);
AccelStepper stepperY(1, 3, 6);
AccelStepper stepperZ(1, 4, 7);


void setup() {
  Serial.begin(9600);

  if (!driver.init())
    Serial.println("init failed");

  stepperX.setEnablePin(8);
  stepperX.setPinsInverted(false, false, true);

  pinMode(homeX, INPUT_PULLUP);
  pinMode(homeY, INPUT_PULLUP);
  pinMode(homeZ, INPUT_PULLUP);

  Serial.println("Use remort control to send command");

  stepperX.disableOutputs();
}
void loop() {

  checkRadio();
  RunTheMotor();
  animation1();
  //  if(anime > 1){
  //    if(anime == 2){
  //      Serial.println("Looping Animation 2");
  //      animation2();
  //    }
  //    else if(anime == 3){
  //      Serial.println("Looping Animation 3");
  //      animation3();
  //    }
  //  }

}

void RunTheMotor()
{
  if (runallowed == true)
  {
    stepperX.enableOutputs();
    wait_for_stepper_completion();
  }
  else
  {
    stepperX.disableOutputs();
    return;
  }
}

void checkRadio()
{

  uint8_t buf[1];
  uint8_t buflen = sizeof(buf);

  if (driver.recv(buf, &buflen)) // Non-blocking
  {

    //
    Serial.println((char*)buf);
    //    newData = true;
    //
    //    if (newData == true)
    //    {
    switch (buf[0])
    {



      case 's': // Stops the motor

        stepperX.stop(); //stop motor
        stepperY.stop();
        stepperZ.stop();
        stepperX.disableOutputs(); //disable power
        Serial.println("Stopped."); //print action
        runallowed = false; //disable running
        break;


      case 'h': //H: Homing

        runallowed = true;
        //Serial.println("Homing"); //Print the message
        Home();// Run the function
        stepperX.stop(); //stop motor
        stepperY.stop();
        stepperZ.stop();
        stepperX.disableOutputs(); //disable power
        runallowed = false; //disable running
        break;

      case '0':
        anime = 0;
        Serial.print("Animation = ");
        Serial.println(anime);
        Xsteps = 0;
        Ysteps = 0;
        Zsteps = 0;
        Xspeed = 1000;
        Yspeed = 1000;
        Zspeed = 1000;
        stepperX.setAcceleration(Xacceleration);
        stepperY.setAcceleration(Yacceleration);
        stepperZ.setAcceleration(Zacceleration);
        directionMultiplierX = -1;
        directionMultiplierY = -1;
        directionMultiplierZ = -1;
        Serial.println("0 position");
        RotateAbsolute();
        RunTheMotor();
        break;

      case '1':
        anime = 1;
        Serial.print("Animation = ");
        Serial.println(anime);
        Xsteps = 1000;
        Ysteps = 1000;
        Zsteps = 900;
        Xspeed = 1000;
        Yspeed = 1000;
        Zspeed = 1000;
        stepperX.setAcceleration(Xacceleration);
        stepperY.setAcceleration(Yacceleration);
        stepperZ.setAcceleration(Zacceleration);
        directionMultiplierX = 1;
        directionMultiplierY = 1;
        directionMultiplierZ = 1;
        Serial.println("Floor position");
        RotateAbsolute();
        RunTheMotor();
        break;

      case '2':
        anime = 2;
        Serial.print("Animation = ");
        Serial.println(anime);
        animation1();

        break;

      case '3':
        anime = 3;
        Serial.print("Animation = ");
        Serial.println(anime);
        animation2();

        break;

      case '4':
        anime = 4;
        Serial.print("Animation = ");
        Serial.println(anime);
        animation3();

        break;

      case '5':
        anime = 5;
        Serial.print("Animation = ");
        Serial.println(anime);
        animation4();

        break;

      default:

        break;
    }
  }
  //after we went through the above tasks, newData is set to false again, so we are ready to receive new commands again.
  newData = false;
  // }
}
void animation1() {

  runallowed = true;
  RunTheMotor();

  Xspeed = 1000;
  Yspeed = 1000;
  Zspeed = 1000;
  stepperX.setAcceleration(Xacceleration);
  stepperY.setAcceleration(Yacceleration);
  stepperZ.setAcceleration(Zacceleration);
  //2
  directionMultiplierX = -1;
  directionMultiplierZ = -1;
  directionMultiplierY = -1;
  Xsteps = 0;
  Zsteps = 8000;
  Ysteps = 0;
  RotateAbsolute();
  RunTheMotor();
  //3
  //directionMultiplierX = -1;
  directionMultiplierZ = 1;
  directionMultiplierY = -1;
  //Xsteps = 0;  
  Zsteps = 5000;
  Ysteps = 4000;
  RotateAbsolute();
  RunTheMotor();
  //4
  //directionMultiplierX = 1;
  //directionMultiplierZ = 1;
  directionMultiplierY = 1;
  //Xsteps = 0;
  //Zsteps = 0;
  Ysteps = 0;
  RotateAbsolute();
  RunTheMotor();
  //5
  //directionMultiplierX = -1;
  //directionMultiplierZ = -1;
  directionMultiplierY = -1;
  //Xsteps = 1000;
  //Zsteps = 5000;
  Ysteps = 4000;
  RotateAbsolute();
  RunTheMotor();
  //6
  //directionMultiplierX = 1;
  directionMultiplierZ = 1;
  //directionMultiplierY = 1;
  //Xsteps = 1000;
  Zsteps = 0;
  //Ysteps = 0;
  RotateAbsolute();
  RunTheMotor();
  //7
  //directionMultiplierX = 1;
  //directionMultiplierZ = -1;
  directionMultiplierY = -1;
  //Xsteps = 1000;
  //Zsteps = 8000;
  Ysteps = 8000;
  RotateAbsolute();
  RunTheMotor();
  //
  //  for (int x = 0, x < 100, x++) {
  //    directionMultiplierX = 1;
  //    directionMultiplierY = 1;
  //    directionMultiplierZ = 1;
  //    Xsteps = 1000;
  //    Ysteps = 1500;
  //    Zsteps = 800;
  //    RotateAbsolute();
  //    RunTheMotor();
  //
  //    directionMultiplierX = 1;
  //    directionMultiplierY = 1;
  //    directionMultiplierZ = 1;
  //    Xsteps = 1000;
  //    Ysteps = 1500;
  //    Zsteps = 800;
  //    RotateAbsolute();
  //    RunTheMotor();
  // }
  //8
  //directionMultiplierX = 1;
  //directionMultiplierZ = 1;
  directionMultiplierY = 1;
  //Xsteps = 1000;
  //Zsteps = 0;
  Ysteps = 0;
  RotateAbsolute();
  RunTheMotor();
  //9
  directionMultiplierX = -1;
  directionMultiplierZ = -1;
  //directionMultiplierY = 1;
  Xsteps = 7000;
  Zsteps = 10000;
  //ysteps = 8000;
  RotateAbsolute();
  RunTheMotor();
  //10
  directionMultiplierX = 1;
  directionMultiplierZ = 1;
  //directionMultiplierY = -1;
  Xsteps = 4000;
  Zsteps = 0;
  //Ysteps = 8000;
  RotateAbsolute();
  RunTheMotor();
  //11
  directionMultiplierX = 1;
  directionMultiplierZ = -1;
  //directionMultiplierY = -1;
  Xsteps = 0;
  Zsteps = 6000;
  //Ysteps = 0;
  RotateAbsolute();
  RunTheMotor();
  //12
  //directionMultiplierX = 1;
  directionMultiplierZ = 1;
  //directionMultiplierY = -1;
  //Xsteps = 4000;
  Zsteps = 0;
  //Ysteps = 8000;
  RotateAbsolute();
  RunTheMotor();
  //13
  //directionMultiplierX = 1;
  directionMultiplierZ = -1;
  //directionMultiplierY = -1;
  //Xsteps = 4000;
  Zsteps = 10000;
  //Ysteps = 8000;
  RotateAbsolute();
  RunTheMotor();
  //14
  directionMultiplierX = -1;
  //directionMultiplierZ = 1;
  directionMultiplierY = -1;
  Xsteps = 6000;
  //Zsteps = 0;
  Ysteps = 6000;
  RotateAbsolute();
  RunTheMotor();
  //15
  //directionMultiplierX = 1;
  directionMultiplierZ = 1;
  //directionMultiplierY = -1;
  //Xsteps = 4000;
  Zsteps = 0;
  //Ysteps = 8000;
  RotateAbsolute();
  RunTheMotor();
  //16
  directionMultiplierX = 1;
  //directionMultiplierZ = 1;
  directionMultiplierY = 1;
  Xsteps = 0;
  //Zsteps = 0;
  Ysteps = 0;
  RotateAbsolute();
  RunTheMotor();
  //17
  directionMultiplierX = -1;
  directionMultiplierZ = -1;
  //directionMultiplierY = -1;
  Xsteps = 20000;
  Zsteps = 3000;
  //Ysteps = 8000;
  RotateAbsolute();
  RunTheMotor();
  //18
  directionMultiplierX = 1;
  directionMultiplierZ = 1;
  //directionMultiplierY = -1;
  Xsteps = 0;
  Zsteps = 0;
  //Ysteps = 8000;
  RotateAbsolute();
  RunTheMotor();
  //19
  //directionMultiplierX = 1;
  directionMultiplierZ = -1;
  //directionMultiplierY = -1;
  //Xsteps = 4000;
  Zsteps = 8000;
  //Ysteps = 8000;
  RotateAbsolute();
  RunTheMotor();
  //20
  //directionMultiplierX = 1;
  directionMultiplierZ = 1;
  //directionMultiplierY = -1;
  //Xsteps = 4000;
  Zsteps = 3000;
  //Ysteps = 8000;
  RotateAbsolute();
  RunTheMotor();
  //21
  //directionMultiplierX = 1;
  directionMultiplierZ = -1;
  //directionMultiplierY = -1;
  //Xsteps = 4000;
  Zsteps = 6000;
  //Ysteps = 8000;
  RotateAbsolute();
  RunTheMotor();
  //22
  //directionMultiplierX = 1;
  directionMultiplierZ = 1;
  //directionMultiplierY = -1;
  //Xsteps = 4000;
  Zsteps = 0;
  //Ysteps = 8000;
  RotateAbsolute();
  RunTheMotor();
  //23
  directionMultiplierX = -1;
  //directionMultiplierZ = 1;
  directionMultiplierY = -1;
  Xsteps = 6000;
  //Zsteps = 3000;
  Ysteps = 6000;
  RotateAbsolute();
  RunTheMotor();
  //24
  //directionMultiplierX = 1;
  directionMultiplierZ = -1;
  //directionMultiplierY = -1;
  //Xsteps = 4000;
  Zsteps = 6000;
  //Ysteps = 8000;
  RotateAbsolute();
  RunTheMotor();
  //25
  directionMultiplierX = 1;
  directionMultiplierZ = -1;
  directionMultiplierY = -1;
  Xsteps = 0;
  Zsteps = 8000;
  Ysteps = 12000;
  RotateAbsolute();
  RunTheMotor();
  //26
  directionMultiplierX = -1;
  directionMultiplierZ = 1;
  directionMultiplierY = 1;
  Xsteps = 6000;
  Zsteps = 0;
  Ysteps = 4000;
  RotateAbsolute();
  RunTheMotor();
  //27
  directionMultiplierX = 1;
  directionMultiplierZ = -1;
  directionMultiplierY = 1;
  Xsteps = 0;
  Zsteps = 9000;
  Ysteps = 0;
  RotateAbsolute();
  RunTheMotor();
  //28
  //directionMultiplierX = 1;
  directionMultiplierZ = 1;
  directionMultiplierY = -1;
  //Xsteps = 4000;
  Zsteps = 0;
  Ysteps = 3000;
  RotateAbsolute();
  RunTheMotor();
  //29
  //directionMultiplierX = 1;
  directionMultiplierZ = -1;
  directionMultiplierY = -1;
  //Xsteps = 4000;
  Zsteps = 3000;
  Ysteps = 10000;
  RotateAbsolute();
  RunTheMotor();
  //30
  directionMultiplierX = -1;
  //directionMultiplierZ = 1;
  //directionMultiplierY = -1;
  Xsteps = 15000;
  //Zsteps = 3000;
  //Ysteps = 8000;
  RotateAbsolute();
  RunTheMotor();
  //31
  directionMultiplierX = 1;
  directionMultiplierZ = 1;
  directionMultiplierY = 1;
  Xsteps = 0;
  Zsteps = 0;
  Ysteps = 0;
  RotateAbsolute();
  RunTheMotor();

  runallowed = false;
  RunTheMotor();
  
  delay(60000);
}

void animation2() {

  Xspeed = 1000;
  Yspeed = 1000;
  Zspeed = 1000;
  stepperX.setAcceleration(Xacceleration);
  stepperY.setAcceleration(Yacceleration);
  stepperZ.setAcceleration(Zacceleration);

  directionMultiplierX = 1;
  directionMultiplierY = 1;
  directionMultiplierZ = 1;
  Xsteps = 1500;
  Ysteps = 1500;
  Zsteps =  1000;
  RotateAbsolute();
  RunTheMotor();

  directionMultiplierX = 1;
  directionMultiplierY = 1;
  directionMultiplierZ = -1;
  Xsteps = 2000;
  Ysteps = 2000;
  Zsteps = 500;
  RotateAbsolute();
  RunTheMotor();

  directionMultiplierX = -1;
  directionMultiplierY = -1;
  directionMultiplierZ = 1;
  Xsteps = 500;
  Ysteps = 500;
  Zsteps = 2000;
  RotateAbsolute();
  RunTheMotor();

}

void animation3() {

}

void animation4() {

}

void wait_for_stepper_completion()
{
  while (stepperX.distanceToGo() != 0 ||
         stepperY.distanceToGo() != 0 ||
         stepperZ.distanceToGo() != 0
        )
  {
    stepperX.run();
    stepperY.run();
    stepperZ.run();
  }
}

void RotateRelative()
{
  //We move X steps from the current position of the stepper motor in a given direction.
  //The direction is determined by the multiplier (+1 or -1)

  runallowed = true; //allow running - this allows entering the RunTheMotor() function.
  stepperX.setMaxSpeed(Xspeed); //set speed
  stepperY.setMaxSpeed(Yspeed);
  stepperZ.setMaxSpeed(Zspeed);
  stepperX.move(directionMultiplierX * Xsteps); //set relative distance and direction
  stepperY.move(directionMultiplierY * Ysteps);
  stepperZ.move(directionMultiplierZ * Zsteps);
}

void RotateAbsolute()
{
  //We move to an absolute position.
  //The AccelStepper library keeps track of the position.
  //The direction is determined by the multiplier (+1 or -1)
  //Why do we need negative numbers? - If you drive a threaded rod and the zero position is in the middle of the rod...

  runallowed = true; //allow running - this allows entering the RunTheMotor() function.
  stepperX.setMaxSpeed(Xspeed); //set speed
  stepperY.setMaxSpeed(Yspeed);
  stepperZ.setMaxSpeed(Zspeed);
  stepperX.moveTo(directionMultiplierX * Xsteps); //set relative distance
  stepperY.moveTo(directionMultiplierY * Ysteps);
  stepperZ.moveTo(directionMultiplierZ * Zsteps);
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
    checkRadio();
    if (digitalRead(homeX) == 1) {
      stepperX.moveTo(initial_homing);
      stepperX.run();
    }
    if (digitalRead(homeY) == 1) {
      stepperY.moveTo(initial_homing);
      stepperY.run();
    }
    if (digitalRead(homeZ) == 1) {
      stepperZ.moveTo(initial_homing);
      stepperZ.run();
    }
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
    checkRadio();
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
