// defines pin numbers
const int EnPin = 8;
const int stepPin1 = 2;
const int dirPin1 = 5;
const int stepPin2 = 3;
const int dirPin2 = 6;
const int stepPin3 = 4;
const int dirPin3 = 7;

const int Limit1 = 9;
const int Limit2 = 10;
const int Limit3 = 11;

void setup() {
  // Sets pins as Outputs
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(stepPin3, OUTPUT);
  pinMode(dirPin3, OUTPUT);
  pinMode(EnPin, OUTPUT);
}
void loop() {
  digitalWrite(EnPin, LOW);
  digitalWrite(dirPin1, HIGH); // Enables the motors to move in a particular direction
  digitalWrite(dirPin2, HIGH);
  digitalWrite(dirPin3, HIGH);
  // Makes 200 pulses for making one full cycle rotation
  for (int x = 0; x < 5000; x++) {
    digitalWrite(stepPin1, HIGH);
    digitalWrite(stepPin2, HIGH);
    digitalWrite(stepPin3, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin1, LOW);
    digitalWrite(stepPin2, LOW);
    digitalWrite(stepPin3, LOW);
    delayMicroseconds(5);
  }
  delay(100); // One second delay

  digitalWrite(dirPin1, LOW); //Changes the rotations direction
  digitalWrite(dirPin2, LOW);
  digitalWrite(dirPin3, LOW);
  // Makes 200 pulses for making two full cycle rotation
  for (int x = 0; x < 5000; x++) {
    digitalWrite(stepPin1, HIGH);
    digitalWrite(stepPin2, HIGH);
    digitalWrite(stepPin3, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin1, LOW);
    digitalWrite(stepPin2, LOW);
    digitalWrite(stepPin3, LOW);
    delayMicroseconds(5);
  }
  delay(100);
}
