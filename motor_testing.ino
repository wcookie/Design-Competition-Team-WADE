  //include<Servo.h> //including the servo library
  //////////////////////////////////////
 // Servo laser_servo; //initializing the servo for the laser
  //////////////////////////////////////

  //////////////////////////////////////
  //some constants here
  #define EDGE_THRESHOLD 75
  #define STANDARD_SPEED 150
  #define SLOW_SPEED 75
  int testPin = 11;
  //////////////////////////////////////
  
  //////////////////////////////////////
  //these 4 pins relate to the h bridges for the drive motors
  //left is the upper H bridge
  int analogLeft = 9; //PWM out
  int digitalLeft = 4; // Digital out
  int analogRight = 3; //PWM out (recently switched due to defection with arduino 3 pin
  int digitalRight = 2; //Digital out
  //////////////////////////////////////
  
  //////////////////////////////////////
  //servo stuff
  int servo_pos = 0; // the servo position for the lasers
  int servo_pin = 6; //PWM out
  //////////////////////////////////////

  //////////////////////////////////////
  //Pins for edge detection
  int frontLeftLine = A3; //Analog in
  int frontRightLine = A2; //Analog in
  int backLine = A1; //Analog in
  //////////////////////////////////////

  //////////////////////////////////////
  //laser values
  //left
  int leftLaserPin = A4 ; //Analog in for the left phototransistor
  int leftLaserValue = 0; //Analog value of the left phototransistor
  //right
  int rightLaserPin = A5; //Analog in for the right phototransistor
  int rightLaserValue = 0; //Analog value of the right phototransistor
  //////////////////////////////////////

  //////////////////////////////////////
  //logic values
  short detectionSide = 1; //detectionSide can be either 0, 1, or 2.  
  //If it is 0, then we have last seen the cup on the left.  
  //if it is 1 we have last seen it in the middle
  //If it is 2 then we have last seen it on the right
  bool isDetected = 0; //isDetected sees if we are currently looking at the cup
  short edgeSide = -1; //edgeSide can be either 0, 1, 2, or 3
  //If it is 0, then it is on the back
  //If it is 1, then it is on the left
  //If it is 2, then it is on the right
  //If it is 3, then it is on both front ones
  bool edgeDetected =0; //edgeDetected purely is whether or not we think we're on an edge.
  int edgeBase =0; //edgeBase is our base threshold for the line detection we find in setup
  ///////////////////////////////////////
void setup() {
  // put your setup code here, to run once:
  //initializing pin modes
  pinMode(testPin, OUTPUT);
  pinMode(analogLeft, OUTPUT);
  pinMode(digitalLeft, OUTPUT);
  pinMode(analogRight, OUTPUT);
  pinMode(digitalRight, OUTPUT);
  pinMode(frontLeftLine, INPUT);
  pinMode(frontRightLine, INPUT);
  pinMode(backLine, INPUT);
  pinMode(leftLaserPin, INPUT);
  pinMode(rightLaserPin, INPUT);
  delay(5);
  edgeBase = (analogRead(frontLeftLine)+analogRead(frontRightLine)+analogRead(backLine))/3;

  //laser_servo.attach(servo_pin); //attaching the servo
  Serial.begin(9600); //allowing serial communication 
}

void loop() {
  //check if we are near the edge
  nearTheEdge();
 /*
  if (edgeDetected == false){
    analogWrite(analogLeft, 175); //128 out of 255 (0-255 bc 8 bits 2 to the 8 eecs213 lol)
    digitalWrite(digitalLeft, HIGH);
    //right is the lower h bridge
    //right LOW is for forward
    analogWrite(analogRight, 175);
    digitalWrite(digitalRight, HIGH);
  }
  else{
    analogWrite(analogRight, 50);
    analogWrite(analogLeft, 50);
    digitalWrite(digitalRight, LOW);
    digitalWrite(digitalLeft, LOW);
  }
  */
  if (edgeDetected){
    
  }
  //moving the servo
  servo_pos ++;
 // laser_servo.write(servo_pos); //telling the servo to move to 100 degrees
  delay(10);
 
}
//setMotors() sets the motors to the corresponding values and directions.
//leftSpeed, leftDirection, rightSpeed, rightDirection is pretty self explanatory
//speed values go from 0-255
//direction values are 1 for forward -1 for backward (HIGH, LOW)
void setMotors(int leftSpeed, int leftDirection, int rightSpeed, int rightDirection){
  //sets associated motors with parameters
  analogWrite(analogLeft, leftSpeed);
  analogWrite(analogRight, rightSpeed);
  if(leftDirection==1){
    digitalWrite(digitalLeft, HIGH);
  }
  else{
    digitalWrite(digitalLeft, LOW);
  }
  if (rightDirection==1){
    digitalWrite(digitalRight, HIGH);
  }
  else{
    digitalWrite(digitalRight, LOW);
  }
}


//nearTheEdge() checks to see if our robot is near the edge of the table
void nearTheEdge(){
  bool not_detected=true;

  if ((analogRead(frontLeftLine)-edgeBase>EDGE_THRESHOLD)&&(analogRead(frontRightLine)-edgeBase>EDGE_THRESHOLD)){
    edgeSide=3;
    edgeDetected=true;
    Serial.println("IN THE FRONT");
  }
  //if the front left sees black set edgeSide to 1
  else if (analogRead(frontLeftLine)-edgeBase>EDGE_THRESHOLD){
    edgeSide=1;
    edgeDetected=true;
    Serial.println("frontLEFT");
    not_detected=false;
  }
  //if the front right sees black set edgeSide to 2
  else  if (analogRead(frontRightLine)-edgeBase>EDGE_THRESHOLD){
    edgeSide=2;
    edgeDetected=true;
    Serial.println("frontRIGHT");
    not_detected=false;
  }
  //if the front right seees black set edgeSide to 0
    if (analogRead(backLine)-edgeBase>EDGE_THRESHOLD){
    edgeSide=0;
    edgeDetected=true;
    Serial.println("back");
    not_detected=false;
  }
  //if we are on white set edgeSide to -1 and set edgeDetected to False
  if (not_detected){
    edgeDetected=false;
    edgeSide=-1;
  }
}
//avoidEdge() is a function that if we are currently detecting the edge avoids the edge
void avoidEdge(){
  if (edgeSide==3){
    //TODO: ADD turnAround() function
    //for now this is gonna just back up with both motors for like 1 second then one motor
    setMotors(SLOW_SPEED, -1, SLOW_SPEED, -1);
    //delay a second
    delay(1000);
    //basically then have the left go backwards and the right go forwards
    setMotors(SLOW_SPEED, -1, SLOW_SPEED, 1);
  }
}

