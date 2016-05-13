  #include<Servo.h> //including the servo library
  //////////////////////////////////////
  Servo laser_servo; //initializing the servo for the laser
  //////////////////////////////////////
  //these 4 pins relate to the h bridges for the drive motors
  int analogLeft = 3; //PWM out
  int digitalLeft = 2; // Digital out
  int analogRight = 5; //PWM out
  int digitalRight = 4; //Digital out
  //////////////////////////////////////
  //servo stuff
  int servo_pos = 0; // the servo position for the lasers
  int servo_pin = 6; //PWM out
  //////////////////////////////////////
  //Pins for edge detection
  int frontLeftLine = A3; //Analog in
  int frontRightLine = A2; //Analog in
  int backLine = A1; //Analog in
  //////////////////////////////////////
  //laser values
  //left
  int leftLaserPin = A4 ; //Analog in for the left phototransistor
  int leftLaserValue = 0; //Analog value of the left phototransistor
  //right
  int rightLaserPin = A5; //Analog in for the right phototransistor
  int rightLaserValue = 0; //Analog value of the right phototransistor
  //////////////////////////////////////
  //logic values
  short detectionSide = 1; //detectionSide can be either 0, 1, or 2.  
  //If it is 0, then we have last seen the cup on the left.  
  //if it is 1 we have last seen it in the middle
  //If it is 2 then we have last seen it on the right
  bool isDetected = 0; //isDetected sees if we are currently looking at the cup
  short edgeSide = 1l; //edgeSIde can be either 0, 1, or 2/
  //If it is 0, then it is on the back
  //If it is 1, then it is on the left
  //If it is 2, then it is on the right
  bool edgeDetected =0; //edgeDetected purely is whether or not we think we're on an edge.
  int edgeBase =0; //edgeBase is our base threshold for the line detection we find in setup
void setup() {
  // put your setup code here, to run once:
  //initializing pin modes
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

  //can take out timing stuff probably; we will be using 555 circuit
 // Serial.println(analogRead(frontRightLine));
    Serial.println(edgeDetected);
  /*if (analogRead(frontLeftLine)<50){
    edgeSide=1;
    edgeDetected=true;
    Serial.println("frontLEFT");
  }
 */ if (analogRead(frontRightLine)<50){
    edgeSide=2;
    edgeDetected=true;
    Serial.println("frontRIGHT");
  }
    if (analogRead(backLine)<50){
    edgeSide=0;
    edgeDetected=true;
    Serial.println("back");
  }
  //left is the upper h bridge
  //left HIGH is for forward
  if (edgeDetected == false){
    analogWrite(analogLeft, 50); //128 out of 255 (0-255 bc 8 bits 2 to the 8 eecs213 lol)
    digitalWrite(digitalLeft, HIGH);
    //right is the lower h bridge
    //right LOW is for forward
    analogWrite(analogRight, 50);
    digitalWrite(digitalRight, HIGH);
  }
  else{
    analogWrite(analogRight, 50);
    analogWrite(analogLeft, 50);
    digitalWrite(digitalRight, HIGH);
    digitalWrite(digitalLeft, HIGH);
  }
  //moving the servo
  servo_pos ++;
  laser_servo.write(servo_pos); //telling the servo to move to 100 degrees
  delay(10);
 
}
