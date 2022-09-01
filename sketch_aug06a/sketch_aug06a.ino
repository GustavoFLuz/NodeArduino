#include <string.h>
#define fr 6
#define tr 2
#define es 3
#define di 4
#define pe 5

int pos;
char baseString[100];
int t(){
  delay(500);
}
int gX = 0;
int gY = 0;
int moveRow(int pos,int dest){
  if(pos < dest){
    for(pos;pos<dest;pos++){
      digitalWrite(tr, 1);
      t();
      digitalWrite(tr, 0);
      t();
      gY++;
      Serial.print("C");
      Serial.print(gY);
      Serial.print("-");
      Serial.println(gX);
    }
  } else if(pos > dest){
    for(pos;pos>dest;pos--){
      digitalWrite(fr, 1);
      t();
      digitalWrite(fr, 0);
      t();
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
      digitalWrite(di, 1);
      t();
      digitalWrite(di, 0);
      t();
      gX++;
      Serial.print("C");
      Serial.print(gY);
      Serial.print("-");
      Serial.println(gX);
    }
  } else if(pos > dest){
    for(pos;pos>dest;pos--){
      digitalWrite(es, 1);
      t();
      digitalWrite(es, 0);
      t();
      gX --;
      Serial.print("C");
      Serial.print(gY);
      Serial.print("-");
      Serial.println(gX);
    }
  }
  return pos;
}

int pick(int qty){
  digitalWrite(5, 1);
  t();
  digitalWrite(5, 0);
  t();
  Serial.println("P");  
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
      pick(qty);
      
      inRow = getNextInt(str);
      inCol = getNextInt(str);
      qty = getNextInt(str);
      posX = moveCol(posX, inCol);
      posY = moveRow(posY, inRow);
      pick(qty);
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
  pinMode(fr, OUTPUT);
  pinMode(tr, OUTPUT);
  pinMode(di, OUTPUT);
  pinMode(es, OUTPUT);
  pinMode(pe, OUTPUT);
  Serial.begin(9600);   //Start serial communication @9600 bps

}

void loop() {
  digitalWrite(fr, 0);
  digitalWrite(tr, 0);
  digitalWrite(di, 0);
  digitalWrite(es, 0);
  digitalWrite(pe, 0);
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
