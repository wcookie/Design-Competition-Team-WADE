  #include<Servo.h> //including the servo library
  //////////////////////////////////////
  Servo laser_servo; //initializing the servo for the laser
  //////////////////////////////////////

  //////////////////////////////////////
  //some constants here
  #define EDGE_THRESHOLD 100
  #define STANDARD_SPEED 125
  #define SLOW_SPEED 75
  #define FAST_SPEED 150
  #define LASER_THRESHOLD 5
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
  short detectionSide = -1; //detectionSide can be either 0, 1, or 2.  -1 if we haven't seen it
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
  int frontLeftBase =0; //frontLeftBase is our base threshold for the line detection we find in setup left
  //same for right and back
  int frontRightBase =0;
  int backBase =0;
  ///////////////////////////////////////

  ///////////////////////////////////////
  //random detection stuff
  long leftRandom=0;
  long rightRandom=0;
  bool timeToTurn = false; //basically this says whether or not we are going to turn at the next millis() check
  long oldTime = 0;
  
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
  delay(1000);
  frontLeftBase=analogRead(frontLeftLine);
  delay(200);
  frontLeftBase=(frontLeftBase+analogRead(frontLeftLine))/2;
  delay(200);
  frontRightBase = analogRead(frontRightLine);
  delay(200);
  frontRightBase = (frontRightBase+analogRead(frontRightLine))/2;
  delay(200);
  backBase = analogRead(backLine);
  delay(200);
  backBase = (backBase+analogRead(backLine))/2;
  //basically creating the edgeBase as the initialitization of the first two values
  laser_servo.attach(servo_pin); //attaching the servo
  Serial.begin(9600); //allowing serial communication 
  //Now we wait for 4 seconds
  delay(3000);
  setMotors(STANDARD_SPEED, 1, STANDARD_SPEED,1);
  oldTime=millis();
  //AND We are gonna go go go go go
}

void loop() {
  laser_servo.write(90);
  //delay(10000);
  //check if we are near the edge
  //Serial.println("LEFT");
  //Serial.println(analogRead(leftLaserPin));
  Serial.println("A5");
  Serial.println(analogRead(A5));
  Serial.println("A4");
  Serial.println(analogRead(A4));
  delay(10);
  //Serial.println("RIGHT");
 // Serial.println(analogRead(rightLaserPin));
  //delay(50);
  detectCup();
  nearTheEdge();
  //if we are avoid it
  if (edgeDetected){
    avoidEdge();
  }
  //if we detect the cup
  if (isDetected){
    //ATTACK CUP
   // Serial.println("ATTACK");
   // attackCup();
  }
  else{
    Serial.println(analogRead(leftLaserPin));
    setMotors(0,1,0,1);
  }
  //moving the servo
  servo_pos ++;
  //laser_servo.write(90); //telling the servo to move to 100 degrees
 // setMotors(STANDARD_SPEED, 1, STANDARD_SPEED, 1);
 //we want to search for the cup here
 // delay(10);
 searchForCup();
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

  if ((frontLeftBase-analogRead(frontLeftLine)>EDGE_THRESHOLD)&&(frontRightBase-analogRead(frontRightLine)>EDGE_THRESHOLD)){
    edgeSide=3;
    edgeDetected=true;
    Serial.println("IN THE FRONT");
  }
  //if the front left sees black set edgeSide to 1
  else if (frontLeftBase-analogRead(frontLeftLine)>EDGE_THRESHOLD){
    edgeSide=1;
    edgeDetected=true;
    Serial.println("frontLEFT");
    not_detected=false;
  }
  //if the front right sees black set edgeSide to 2
  else  if (frontRightBase-analogRead(frontRightLine)>EDGE_THRESHOLD){
    edgeSide=2;
    edgeDetected=true;
    Serial.println("frontRIGHT");
    not_detected=false;
  }
  //if the front right seees black set edgeSide to 0
    if (-backBase-analogRead(backLine)>EDGE_THRESHOLD){
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
  //if edge is right in front
  if (edgeSide==3){
    //TODO: ADD turnAround() function
    //for now this is gonna just back up with both motors for like 1 second then one motor
    setMotors(SLOW_SPEED, -1, SLOW_SPEED, -1);
    //delay a second
    delay(1000);
    //basically then have the left go backwards and the right go forwards
    setMotors(SLOW_SPEED, -1, SLOW_SPEED, 1);
    //wait 2 seconds
    delay(2000);
  }
  //if edge is to the right 
  else if (edgeSide==2){
    setMotors(0, -1, 0,-1);
    delay(100);
    //set the right one to faster than the left one slightly
    setMotors(STANDARD_SPEED+25, -1, STANDARD_SPEED+25, -1);
    //wait 550 milli second
    delay(550);
    //now we turn using the right wheel as our pivot
    setMotors(SLOW_SPEED+25, -1, STANDARD_SPEED, 1);
    delay(500);
  }
  //if edge is to the left
  else if (edgeSide==1){
    //vice versa of above
    setMotors(0,-1,0,-1);
    delay(100);
    setMotors(STANDARD_SPEED+25, -1, STANDARD_SPEED+25, -1);
    //wait 550 mili second
    delay(550);
    setMotors(STANDARD_SPEED, 1, SLOW_SPEED+25, -1);
    delay(500);
  }
  //it really shouldn't get to the back but whatever i'll take care of it
  else if (edgeSide==0){
    //basically just go forward at slow speed for a second
    setMotors(SLOW_SPEED, 1, SLOW_SPEED, 1);
    //the second delay
    delay(1000);
  }
  else{
    Serial.println("Why did we call this function.... there is no edge");
  }
}
//detectCup() at the end of the day looks for cup  
void detectCup(){
     //acquire laser values
     leftLaserValue = analogRead(leftLaserPin);
     rightLaserValue = analogRead(rightLaserPin);
     if((leftLaserValue > LASER_THRESHOLD) && (rightLaserValue > LASER_THRESHOLD)){
        //this case means that we are locked on
        isDetected=true;
        detectionSide=1;
     }
     //its only on the left
     else if(leftLaserValue > LASER_THRESHOLD){
        isDetected=true;
        detectionSide=0;
     }
     //its only on the right
     else if(rightLaserValue > LASER_THRESHOLD){
        isDetected=true;
        detectionSide=2;
     }
     //we didnt find it
     else{
        isDetected=false;
     }
}
//attackCup() determines where to go if we are hitting the cup
void attackCup(){
  //if we are in the middle locked on
  if(detectionSide==1){
    //if we are in the motors simply go faster and ATTACK
    setMotors(FAST_SPEED, 1, FAST_SPEED, 1);
  }
  //if we are on the left only but we still detect it
  //lets go at slightly off standard speed slightly left (motor on right is faster)
  else if(detectionSide==0){
    setMotors(STANDARD_SPEED-10, 1, STANDARD_SPEED+10, 1);
  }
  //we are looking on the right
  else{
   setMotors(STANDARD_SPEED+10, 1, STANDARD_SPEED-10, 1);
  }
  
}
//searchForCup() determines where to go when we dont see the cup
void searchForCup(){
  bool turn=false;
  if(timeToTurn){
    if ((millis()-oldTime)>500)
    {
      turn=true;
      timeToTurn=false;
    }
  }
  else{
    if((millis()-oldTime)>2000){
     setMotors(STANDARD_SPEED,1,STANDARD_SPEED,1);
     timeToTurn=true;
    }
  }
  if (turn){
    //basically need to do some weirder stuff here with random etc.
    //if we have last scene in the middle, have it balanced out randomly
    //We will turn for 500 milliseconds, then turn on for 2 seconds
    if(detectionSide==1){
      leftRandom=random(0,27); //random number between 0 and 26
      rightRandom=random(0,27); //same thing
      setMotors(STANDARD_SPEED-13+leftRandom, 1, STANDARD_SPEED-13+rightRandom, 1);  
    } 
    //if it was last scene on the left lets skew this value to the left mildly heavily
    else if (detectionSide==0){
      leftRandom=random(0,27); //same random numbers
      rightRandom=random(0,27);
      //however here instead of having the same offset, the left is going to skew on average 13 higher than the right
      setMotors(STANDARD_SPEED+leftRandom, 1, STANDARD_SPEED-13+rightRandom, 1);
    }
    //same thing on the right 
    else{
      leftRandom = random(0,27);
      rightRandom=random(0,27); //at this point i should have just made one set of this...
      //same as above but with swithced left/right
      setMotors(STANDARD_SPEED-13+leftRandom, 1, STANDARD_SPEED+rightRandom, 1);
    }
  }
  //if we aren't turning then we just go straight

}

