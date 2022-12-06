
//created by Ishan_Sachintha https://www.fiverr.com/share/bog1XW

#include "AccelStepper.h"
// Library created by Mike McCauley at http://www.airspayce.com/mikem/arduino/AccelStepper/

// AccelStepper Setup
const int EnPin = 8;
AccelStepper stepper_Left(1, 2, 5);
AccelStepper stepper_Middle(1, 3, 6);
AccelStepper stepper_Right(1, 4, 7);

// Define the Limit Pins used
#define home_Left 9
#define home_Middle 10
#define home_Right 11

// Stepper Travel Variables
long TravelX;  // Used to store the X value entered in the Serial Monitor
int move_finished = 1; // Used to check if move is completed
long initial_homing = -1; // Used to Home Stepper at startup


void setup() {
  Serial.begin(9600);  // Start the Serial monitor with speed of 9600 Bauds

  pinMode(home_Left, INPUT_PULLUP);
  pinMode(home_Middle, INPUT_PULLUP);
  pinMode(home_Right, INPUT_PULLUP);
  

  delay(5);
  //  Set Max Speed and Acceleration of each Steppers at startup for homing
  stepper_Left.setMaxSpeed(100.0);      // Set Max Speed of Stepper (Faster for regular movements)
  stepper_Left.setAcceleration(100.0);  // Set Acceleration of Stepper
  stepper_Middle.setMaxSpeed(100.0);
  stepper_Middle.setAcceleration(100.0);
  stepper_Right.setMaxSpeed(100.0);
  stepper_Right.setAcceleration(100.0);


  // Start Homing procedure of Stepper Motor at startup

  Serial.print("Stepper is Homing . . . . . . . . . . . ");

  while (digitalRead(home_Left) && digitalRead(home_Middle) && digitalRead(home_Right)) {  // Make the Steppers move CCW until the switch is activated

    if(digitalRead(home_Left)==0){
      stepper_Left.moveTo(initial_homing);
      stepper_Left.run();
    }

    if(digitalRead(home_Middle)==0){
      stepper_Middle.moveTo(initial_homing);
      stepper_Middle.run();
    }

    if(digitalRead(home_Right)==0){
      stepper_Right.moveTo(initial_homing);
      stepper_Right.run();
    }

    initial_homing--;
    delay(5);
  }


  stepper_Left.setCurrentPosition(0);
  stepper_Left.setMaxSpeed(100.0);
  stepper_Left.setAcceleration(100.0);

  stepper_Middle.setCurrentPosition(0);
  stepper_Middle.setMaxSpeed(100.0);
  stepper_Middle.setAcceleration(100.0);

  stepper_Right.setCurrentPosition(0);
  stepper_Right.setMaxSpeed(100.0);
  stepper_Right.setAcceleration(100.0);
  initial_homing = 1;

  while (!digitalRead(home_Left) && !digitalRead(home_Middle) && !digitalRead(home_Right)) { // Make the Stepper move CW until the switch is deactivated
    stepper_Left.moveTo(initial_homing);
    stepper_Middle.moveTo(initial_homing);
    stepper_Right.moveTo(initial_homing);
    stepper_Left.run();
    stepper_Middle.run();
    stepper_Right.run();
    initial_homing++;
    delay(5);
  }

  stepper_Left.setCurrentPosition(0);
  stepper_Middle.setCurrentPosition(0);
  stepper_Right.setCurrentPosition(0);
  Serial.println("Homing Completed");
  Serial.println("");
  stepper_Left.setMaxSpeed(1000.0);      // Set Max Speed of Stepper (Faster for regular movements)
  stepper_Left.setAcceleration(1000.0);  // Set Acceleration of Stepper
  stepper_Middle.setMaxSpeed(1000.0);
  stepper_Middle.setAcceleration(1000.0);
  stepper_Right.setMaxSpeed(1000.0);
  stepper_Right.setAcceleration(1000.0);

  // Print out Instructions on the Serial Monitor at Start
  Serial.println("Enter Travel distance (Positive for CW / Negative for CCW and Zero for back to Home): ");
}

void loop() {

  while (Serial.available() > 0)  { // Check if values are available in the Serial Buffer

    move_finished = 0; // Set variable for checking move of the Stepper

    TravelX = Serial.parseInt(); // Put numeric value from buffer in TravelX variable
    if (TravelX < 0 || TravelX > 1350) {  // Make sure the position entered is not beyond the HOME or MAX position
      Serial.println("");
      Serial.println("Please enter a value greater than zero and smaller or equal to 1350.....");
      Serial.println("");
    } else {
      Serial.print("Moving stepper into position: ");
      Serial.println(TravelX);

      stepper_Left.moveTo(TravelX);  // Set new moveto position of Stepper
      stepper_Middle.moveTo(TravelX);  // Set new moveto position of Stepper
      stepper_Right.moveTo(TravelX);  // Set new moveto position of Stepper

      delay(1000);  // Wait 1 seconds before moving the Stepper
    }
  }

  if (TravelX >= 0 && TravelX <= 1350) {

    // Check if the Stepper has reached desired position
    if ((stepper_Left.distanceToGo() != 0)) {

      stepper_Left.run();  // Move Stepper into position
      stepper_Middle.run();  // Move Stepper into position
      stepper_Right.run();  // Move Stepper into position

    }

    // If move is completed display message on Serial Monitor
    if ((move_finished == 0) && (stepper_Left.distanceToGo() == 0)) {
      Serial.println("COMPLETED!");
      Serial.println("");
      Serial.println("Enter Travel distance (Positive for CW / Negative for CCW and Zero for back to Home): ");
      move_finished = 1; // Reset move variable
    }
  }
}
