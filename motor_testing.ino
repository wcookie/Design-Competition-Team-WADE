  #include<Servo.h> //including the servo library
  
  Servo laser_servo; //initializing the servo for the laser
  //////////////////////////////////////
  //these 4 pins relate to the h bridges for the drive motors
  int analogLeft = 5; //PWM out
  int digitalLeft = 4; // Digital out
  int analogRight = 3; //PWM out
  int digitalRight = 2; //Digital out
  //////////////////////////////////////
  int servo_pin = 6; //PWM out
  int prOneIn = A1 ; //Analog in for the first phototransistor
  int prOneValue = 0; //analog value of the first phototransistor
  int servo_pos = 0; // the servo position for the lasers
void setup() {
  // put your setup code here, to run once:
  //initializing pin modes
  pinMode(analogLeft, OUTPUT);
  pinMode(digitalLeft, OUTPUT);
  pinMode(analogRight, OUTPUT);
  pinMode(digitalRight, OUTPUT);

  pinMode(prOneIn, INPUT);
  laser_servo.attach(servo_pin); //attaching the servo
  Serial.begin(9600); //allowing serial communication 
}

void loop() {

  //can take out timing stuff probably; we will be using 555 circuit


  //left is the upper h bridge
  //left HIGH is for forward
  analogWrite(analogLeft, 128); //128 out of 255 (0-255 bc 8 bits 2 to the 8 eecs213 lol)
  digitalWrite(digitalLeft, HIGH);

  //right is the lower h bridge
  //right LOW is for forward
  analogWrite(analogRight, 128);
  digitalWrite(digitalRight, LOW);
  prOneValue = analogRead(prOneIn); //reading the value of the first photoresistor
  Serial.println(prOneValue);

  //moving the servo
  servo_pos = 10;
  laser_servo.write(servo_pos); //telling the servo to move to 100 degrees
  delay(200000);
  


  
}
