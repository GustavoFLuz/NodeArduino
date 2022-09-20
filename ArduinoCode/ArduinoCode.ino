#include <string.h>
#include <Stepper.h>

#define MOTOR_01 22
#define MOTOR_02 24
#define MOTOR_03 26
#define MOTOR_04 28
//4x4-.0:1-0:2=5-0:3=10-.
//4x4-.3:3-0:0=5-.
#define MOTOR_11 32
#define MOTOR_12 34
#define MOTOR_13 36
#define MOTOR_14 38

//#define MOTOR_21 22 //ENTRADAS DO NOVO MOTOR
//#define MOTOR_22 24
//#define MOTOR_23 26
//#define MOTOR_24 28

int pos;
char baseString[100];
int gX = 0;
int gY = 0;
int stepsPerX = 15;
int stepsPerY = 12;
int stepsPerZ = 10;
const int d = 70;
int t(){
  delay(500);
}

Stepper stepper(100, 42, 44, 46, 48);

void moveDown(){
  int motor1, motor2, motor3, motor4;
  for(int i=0;i<stepsPerY;i++){
    motor1 = 0;
    motor2 = 0;
    motor3 = 0;
    motor4 = 0;
    if(!(i%4))   motor1 = 1;
    if(!(i%4-1)) motor2 = 1;
    if(!(i%4-2)) motor3 = 1;
    if(!(i%4-3)) motor4 = 1;
    digitalWrite(MOTOR_01, motor1);
    digitalWrite(MOTOR_02, motor2);
    digitalWrite(MOTOR_03, motor3);
    digitalWrite(MOTOR_04, motor4);
    delay(d);
  }
}
void moveUp(){
  int motor1, motor2, motor3, motor4;
  for(int i=0;i<stepsPerY;i++){
    motor1 = 0;
    motor2 = 0;
    motor3 = 0;
    motor4 = 0;
    if(!(i%4))   motor4 = 1;
    if(!(i%4-1)) motor3 = 1;
    if(!(i%4-2)) motor2 = 1;
    if(!(i%4-3)) motor1 = 1;
    digitalWrite(MOTOR_01, motor1);
    digitalWrite(MOTOR_02, motor2);
    digitalWrite(MOTOR_03, motor3);
    digitalWrite(MOTOR_04, motor4);
    delay(d);
  }
}
void moveRight(){
  int motor1, motor2, motor3, motor4;
  for(int i=0;i<stepsPerX;i++){
    motor1 = 0;
    motor2 = 0;
    motor3 = 0;
    motor4 = 0;
    if(!(i%4))   motor1 = 1;
    if(!(i%4-1)) motor2 = 1;
    if(!(i%4-2)) motor3 = 1;
    if(!(i%4-3)) motor4 = 1;
    digitalWrite(MOTOR_11, motor1);
    digitalWrite(MOTOR_12, motor2);
    digitalWrite(MOTOR_13, motor3);
    digitalWrite(MOTOR_14, motor4);
    delay(d);
  }
}
void moveLeft(){
  int motor1, motor2, motor3, motor4;
  for(int i=0;i<stepsPerX;i++){
    motor1 = 0;
    motor2 = 0;
    motor3 = 0;
    motor4 = 0;
    if(!(i%4))   motor4 = 1;
    if(!(i%4-1)) motor3 = 1;
    if(!(i%4-2)) motor2 = 1;
    if(!(i%4-3)) motor1 = 1;
    digitalWrite(MOTOR_11, motor1);
    digitalWrite(MOTOR_12, motor2);
    digitalWrite(MOTOR_13, motor3);
    digitalWrite(MOTOR_14, motor4);
    delay(d);
  }
}

int moveRow(int pos,int dest){  
  if(pos < dest){
    for(pos;pos<dest;pos++){
      moveDown();
      gY++;
      Serial.print("C");
      Serial.print(gY);
      Serial.print("-");
      Serial.println(gX);
    }
  } else if(pos > dest){
    for(pos;pos>dest;pos--){
      moveUp();
      gY--; 
      Serial.print("C");
      Serial.print(gY);
      Serial.print("-");
      Serial.println(gX);  
   }
    
  }
  return pos;
}
int moveCol(int pos ,int dest){
    if(pos < dest){
    for(pos;pos<dest;pos++){
      moveLeft();
      gX++;
      Serial.print("C");
      Serial.print(gY);
      Serial.print("-");
      Serial.println(gX);
    }
  } else if(pos > dest){
    for(pos;pos>dest;pos--){
      moveRight();
      gX --;
      Serial.print("C");
      Serial.print(gY);
      Serial.print("-");
      Serial.println(gX);
    }
  }
  return pos;
}

int pick(int qty, bool pick){
  Serial.println("P");
  stepper.step(200);
  delay(500);
  stepper.step(-200);
  
}

int strLen;
int convertStrToInt(char *numberChar, int length)
{
  int number = 0;
  int digit;
  for (int i = 0; i < length; i++)
  {
    digit = numberChar[i] - '0';
    number = 10 * number + digit;
  }
  return number;
}

int getNextInt(char *str) {
  int posArray = 0;
  char number[5];
  while (+str[pos] >= 48 && +str[pos] <= 57)
  {
    number[posArray] = str[pos];
    posArray++;
    pos ++;
  }
  pos++;
  int digits = posArray ;
  return convertStrToInt(number, digits);
}

int runCode() {
  strLen = pos;
  pos = 0;
  char str[100];
  strcpy(str, baseString);
  
  Serial.println("Started");

  int rows = getNextInt(str);
  int cols = getNextInt(str);
  pos++;

  int posX = 0;
  int posY = 0;

  int outRow;
  int outCol;

  int inRow;
  int inCol;
  int qty;
  gX= 0;
  gY= 0;
  while (pos != strLen)
  {
    outRow = getNextInt(str);
    outCol = getNextInt(str);
    while (str[pos] != '.')
    {
      posX = moveCol(posX, outCol);
      posY = moveRow(posY, outRow);
      pick(qty, true);
      
      inRow = getNextInt(str);
      inCol = getNextInt(str);
      qty = getNextInt(str);
      posX = moveCol(posX, inCol);
      posY = moveRow(posY, inRow);
      delay(d*2);
      pick(qty, false);
    }
    posX = moveCol(posX, 0);
    posY = moveRow(posY, 0);
    pos ++;
  }
  Serial.print("Over");
  pos = 0;
  strLen =0;
}
void setup() {
  pinMode(MOTOR_01, OUTPUT);
  pinMode(MOTOR_02, OUTPUT);
  pinMode(MOTOR_03, OUTPUT);
  pinMode(MOTOR_04, OUTPUT);
  
  pinMode(MOTOR_11, OUTPUT);
  pinMode(MOTOR_12, OUTPUT);
  pinMode(MOTOR_13, OUTPUT);
  pinMode(MOTOR_14, OUTPUT);
  
  stepper.setSpeed(30);
  
  //pinMode(MOTOR_21, OUTPUT);
  //pinMode(MOTOR_22, OUTPUT);
  //pinMode(MOTOR_23, OUTPUT);
  //pinMode(MOTOR_24, OUTPUT);
  
  Serial.begin(9600);   //Start serial communication @9600 bps

}

void loop() {
  digitalWrite(MOTOR_01, 0);
  digitalWrite(MOTOR_02, 0);
  digitalWrite(MOTOR_03, 0);
  digitalWrite(MOTOR_04, 0);
  
  digitalWrite(MOTOR_11, 0);
  digitalWrite(MOTOR_12, 0);
  digitalWrite(MOTOR_13, 0);
  digitalWrite(MOTOR_14, 0);
  
  //digitalWrite(MOTOR_21, 0);
  //digitalWrite(MOTOR_22, 0);
  //digitalWrite(MOTOR_23, 0);
  //digitalWrite(MOTOR_24, 0);
  if (Serial.available()) {

    char val = (char)Serial.read();
    //Serial.print(val);
    if (val == '\n') runCode();
    else {
      baseString[pos] = val;
      pos++;
    }
    
  }
}
