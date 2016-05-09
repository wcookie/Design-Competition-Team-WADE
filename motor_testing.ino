  int analogLeft = 5; //PWM out
  int digitalLeft = 4; // Digital out
  int analogRight = 3; //PWM out
  int digitalRight = 2; //Digital out
  int laserOneOut = 7; //Digital out
  int prOneIn = A1 ; //Analog in
  int prOneValue = 0; //analog value of the first photoresistor
  unsigned long newtime;
  bool flagged = true;
  unsigned long oldtime;
  unsigned long temptime;
void setup() {
  // put your setup code here, to run once:
  pinMode(analogLeft, OUTPUT);
  pinMode(digitalLeft, OUTPUT);
  pinMode(analogRight, OUTPUT);
  pinMode(digitalRight, OUTPUT);
  pinMode(laserOneOut, OUTPUT);
  pinMode(prOneIn, INPUT);
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
}
