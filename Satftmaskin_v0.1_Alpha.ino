#include <LiquidCrystal.h>



const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int vol = 0;
int strength = 0;

void setup() {

  //INITIALIZING SERIAL
  Serial.begin(9600);


  //LCD SET-UP
  lcd.begin(16, 2);                                     // set up the LCD's number of columns and rows:
  
  
  //INITITALIZING PINS (PULLUPP for buttons (internal 10k resistor))
  //Using analog pins as Digital I/O
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  
}

void loop() {
  int vUp;
  int vDown;
  int sUp;
  int sDown;
  int start;

  

  // CLEARS SCREEN
  lcd.clear();
  lcd.print("Volum: " + String(vol) + "mL");
  lcd.setCursor(0,1);
  lcd.print("Styrke: " + String(strength) + "%");

  start = digitalRead(A1);
  sDown = digitalRead(A2);
  sUp = digitalRead(A3);
  vDown = digitalRead(A4);
  vUp = digitalRead(A5);


  // CHECKS IF BUTTON 1 IS PRESSED - IN THAT CASE PRINTS TO SCREEN
  if (vUp == LOW) {
    vol += 1;
  }
  // CHECKS IF BUTTON 2 IS PRESSED - IN THAT CASE PRINTS TO SCREEN
  if (vDown == LOW && vol > 0) {
    vol -= 1;
  }
  // CHECKS IF BUTTON 3 IS PRESSED - IN THAT CASE PRINTS TO SCREEN
  if (sUp == LOW && strength < 100) {
    strength += 2;
  }
  // CHECKS IF BUTTON 4 IS PRESSED - IN THAT CASE PRINTS TO SCREEN
  if (sDown == LOW && strength > 0) {
    strength -= 2;
  }
  // CHECKS IF BUTTON 5 IS PRESSED - IN THAT CASE PRINTS TO SCREEN
  if (start == LOW) {
    delay(2000); // INSERT START FUNCTION HERE
  }

 delay(500);
   

  
}
