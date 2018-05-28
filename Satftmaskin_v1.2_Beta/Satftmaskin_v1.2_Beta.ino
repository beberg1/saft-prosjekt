#include <LiquidCrystal.h>


// Definerer konstanter (Start for knappPin, antall knapper, pause mellom trykk, lcd-pins)
const int btPinStart = 14;
const int btCount = 5;
const int pauseLength = 500;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

// Definerer variabler for volum og konsentrasjon, setter også startverdi.
int vol = 0;
int conc  = 20;

// btValues inneholder verdier om tilstanden til hhv. knappene vUp, vDown, cUp, cDown og start
int btValues[btCount];

// Setter opp LCD-display
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Lager et array med tre elementer (tre typer glass)
String cupList[3] {
  "Lite", "Middels", "Stort"
};    







void setup() {
  // Setter opp serial. Kun for debug.
  Serial.begin(9600);


  // Setter opp LCD-display
  lcd.begin(16, 2);                                     // set up the LCD's number of columns and rows:
 
  // Setter opp pins for knapper
  for (int pinNumber=0; pinNumber<btCount; pinNumber++) {
    pinMode(btPinStart + pinNumber, INPUT_PULLUP);
  }

  
}

void loop() {
  // Definerer et array med verdi for tilstandene til knappene
  int btValues[5];

  

  // Skriver oppdatert informasjon om volum og styrke til LCD
  lcd.clear();
  lcd.print(cupList[vol] + " glass");
  lcd.setCursor(0,1);
  lcd.print("Styrke: " + String(conc) + "%");


  // Leser av knappenes verdi og lagrer verdien i btValues[]
  for (int i=0; i<btCount;i++) {
    btValues[i] = digitalRead(btPinStart + i);
  }


  // KNAPP 0: Volum opp
  // KNAPP 1: Volum ned
  // KNAPP 2: Konsentrasjon opp
  // KNAPP 3: Konsentrasjon ned
  // KNAPP 4: Start

  // Øker verdien til volum om KNAPP 0 er trykt inne, verdien går ikke over 2.
  if (btValues[0] == LOW && vol < 2) {
    vol += 1;
    pause();
  }

  // Minker verdien til volum om KNAPP 1 er trykt inne, verdien blir ikke negativ.
  if (btValues[1] == LOW && vol > 0) {
    vol -= 1;
    pause();
  }

  // Øker verdien til konsentrasjonen om KNAPP 2 er trykt inne, verdien går ikke over 100
  if (btValues[2] == LOW && conc < 100) {
    conc  += 2;
    pause();
  }

  // Minker verdien til konsentrasjonen om KNAPP 3 er trykt inne, verdien blir ikke negativ.
  if (btValues[3] == LOW && conc  > 0) {
    conc -= 2;
    pause();
  }

  // Starter å lage saft
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

