#include <LiquidCrystal.h>

const int btPinStart = 14;
const int btCount = 5;
const int pauseLength = 500;

//btValues contain value about specific button in this order: vUp, vDown, cUp, cDown, start
int btValues[btCount];

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int vol = 0;
String cupList[3] {
  "Lite", "Middels", "Stort"
};     
int conc  = 20;





void setup() {
  int i;

  //INITIALIZING SERIAL
  Serial.begin(9600);


  //LCD SET-UP
  lcd.begin(16, 2);                                     // set up the LCD's number of columns and rows:
  
  
  //INITITALIZING PINS (PULLUPP for buttons (internal 10k resistor))
  //Using analog pins as Digital I/O

  for (int pinNumber=0; pinNumber<btCount; pinNumber++) {
    pinMode(btPinStart + pinNumber, INPUT_PULLUP);
  }

  
}

void loop() {

  int btValues[5];

  

  // CLEARS SCREEN
  lcd.clear();
  lcd.print(cupList[vol] + " glass");
  lcd.setCursor(0,1);
  lcd.print("Styrke: " + String(conc) + "%");




  for (int i=0; i<btCount;i++) {
    btValues[i] = digitalRead(btPinStart + i);
  }



  if (btValues[0] == LOW && vol < 2) {
    vol += 1;
    pause();
  }
  if (btValues[1] == LOW && vol > 0) {
    vol -= 1;
    pause();
  }
  if (btValues[2] == LOW && conc < 100) {
    conc  += 2;
    pause();
  }
  if (btValues[3] == LOW && conc  > 0) {
    conc -= 2;
    pause();
  }
  if (btValues[4] == LOW) {
    juice(); // INSERT START FUNCTION HERE
  }

  delay(100);

 
   

  
}

void pause() {
  delay(pauseLength);
}


void juice() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Lager din saft..");
  delay(2000);
  lcd.clear();
  lcd.print("Glass: " + cupList[vol]);
  lcd.setCursor(0,1);
  lcd.print("Styrke: " + String(conc) + "%");
  delay(1500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Lager din saft..");
  delay(2000);
  lcd.clear();
  lcd.print("Saften er ferdig");
  delay(2000);
}

