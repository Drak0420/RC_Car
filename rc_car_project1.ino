// two pairs of motor pins
int motorPins[][2] = {{3, 5}, {6, 9}};

// state variables
int velocity = 0;
int torque = 0;

void setup()
{
  initPins();
  
  initSerialMonitor();
}

void loop()
{
  // prompt user for a command
  String command = getInput("Type a command: ");
  
  if (command == "accelerate") {
    // prompt user for velocity (-100 to 100)
    velocity = getIntInput("Percent Velocity: ");
  }
  else if (command == "turn") {
    // prompt user for torque (-100 to 100)
    torque = getIntInput("Percent Torque: ");
  }
  
  if (command == "brake") {
    // set both inputs to high
    setMotorInputs(0, 100, 100);
    setMotorInputs(1, 100, 100);
  }
  else if (command == "coast") {
    // set both motor inputs to low
    setMotorInputs(0, 0, 0);
    setMotorInputs(1, 0, 0);
  }
  else {
    // keep driving if not braking or coasting
  	drive();
  }
}

void drive() {
  // while turning, one motor velocity stays the same while the
  // other changes to create a torque
  int velocity1 = velocity;
  int velocity2 = velocity * (1 - abs(torque) / 100. * 2);
  
  // choose motor to adjust depending on turn direction
  if (torque > 0) {
    setMotorVelocity(0, velocity2);
    setMotorVelocity(1, velocity1);
  }
  else {
    setMotorVelocity(0, velocity1);
    setMotorVelocity(1, velocity2);
  }
}
void setMotorInput(int motorIndex, int inputIndex, int percent) {
  // write a value to a single motor pin
  
  analogWrite(motorPins[motorIndex][inputIndex], 
              255 * percent / 100.);
}
void setMotorInputs(int motorIndex, int input1, int input2) {
  // write to both pins of a motor simultaneously
  
  setMotorInput(motorIndex, 0, input1);
  setMotorInput(motorIndex, 1, input2);
}

void setMotorVelocity(int motorIndex, int percentVelocity) {
  // write velocity to the motor pin corresponding to the
  // direction
  
  if (percentVelocity > 0) {
    setMotorInputs(motorIndex, percentVelocity, 0);
  }
  else {
    setMotorInputs(motorIndex, 0, abs(percentVelocity));
  }
}

String getInput(String prompt) {
  // prompt the user for a string input and return it
  
  Serial.print(prompt);
  while (Serial.available() == 0) {}
  String input = Serial.readString();
  Serial.println(input);
  
  return input;
}

int getIntInput(String prompt) {
  // prompt the user for an int input and return it
  
  Serial.print(prompt);
  while (Serial.available() == 0) {}
  int input = Serial.parseInt();
  Serial.println(input);
  
  return input;
}

void initPins() {
  // iterate over motors
  for (int i = 0; i < 2; i++) {
    // iterate over motor pins
    for (int motorPin = 0; motorPin < 2; motorPin++) {
      pinMode(motorPins[i][motorPin], OUTPUT);
    }
  }
}

void initSerialMonitor() {
  Serial.begin(9600);
  
  // print command menu
  Serial.println("Commands: ");
  Serial.println("- accelerate");
  Serial.println("- turn");
  Serial.println("- brake");
  Serial.println("- coast");
  Serial.println("");
}