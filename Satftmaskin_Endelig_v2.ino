#include <LiquidCrystal.h>
#include <Servo.h>

// Definerer konstanter (Start for knappPin, antall knapper, pause mellom trykk, lcd-pins)
const int btPinStart = 14;
const int btCount = 5;
const int pauseLength = 500;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int refTime = 0; // #### Ikke satt enda, men tiden [ms] det tar for pumpa og pumpe 2cl (vårt referansevolum) ####

// Definerer flere konstanter (PINs for pumper (transistor-krets) og servo) 
const int pumpPin1 = 8;
const int pumpPin2 = 9;
const int servoPin = 10; //PWM pin

// Definerer variabler for volum og konsentrasjon, setter også startverdi.
int vol = 0;
int conc  = 20;

// Variabler tilknyttet pumpeegenskaper, basert på data fra testresultater
const float pMod1 = 22230; // Tiden det tar i [ms] for pumpe 1 og fylle opp 2 cl væske
const float pMod2 = 28500; // Tiden det tar i [ms] for pumpe 2 og fylle opp 2 cl væske
const float pMod3 = 4730; // Ekstra tid i [ms] for pumpe 1 sin "treghet"
const float pMod4 = 5600; // Ekstra tid i [ms] for pumpe 2 sin "treghet"

// btValues inneholder verdier om tilstanden til hhv. knappene volum opp, volum ned, konsentrasjon opp, konsentrasjon ned og start
int btValues[btCount];

// Setter opp LCD-display og servo
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo myservo;

// Lager et array med tre elementer (tre typer glass)
String cupList[3] {
  "Lite", "Middels", "Stort"
}; 

int pos;







void setup() {
  // Setter opp serial. Kun for debug.
  Serial.begin(9600);


  // Setter opp LCD-display
  lcd.begin(16, 2);                                     // set up the LCD's number of columns and rows:
 
  // Setter opp pins for knapper til INPUT_PULLUP (INPUT_PULLUP kobler til intern høy-resistans motstand)
  for (int pinNumber=0; pinNumber<btCount; pinNumber++) {
    pinMode(btPinStart + pinNumber, INPUT_PULLUP);
  }

  // Setter opp pins for pumper til OUTPUT. (Sende av/på-verdi til transistorkrets)
  pinMode(pumpPin1, OUTPUT);
  pinMode(pumpPin2, OUTPUT);
  

  // Setter opp pin for servo og setter start-verdi for servo
  myservo.attach(servoPin);
  myservo.write(160);
  

  
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
    juice();
  }

  delay(100);

 
   

  
}

void pause() {
  delay(pauseLength);
}


void juice() {
  
  // Definerer "1.00" som en float, for å unngå bugs
  float one = 1.00;
  
  // Definerer nyttige varibler basert på vol og conc
  float ratio_juice = conc/100; // Andel saft, desimaltall
  float ratio_water = one - ratio_juice; // Andel vann, desimaltall
  float n = pow(2, vol); // Antall enheter á 2 cl. Indeks {0,1,2} <=> Ant enheter {1,2,4} (Bruker 2-erpotenser)

  float tP1 = pMod1 * n * ratio_juice + pMod3; // Tiden pumpe 1 må stå på, med hensyn på vol og conc
  float tP2 = pMod2 * n * ratio_water + pMod4; // Tiden pumpe 2 må stå på, med hensyn på vol og conc
  
  
  // Skriver ny skjerm
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Forbereder..");


  // Flytter koppen tilbake
  servo_backward();

  
  // Skriver ny skjerm
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Lager saft..");
  

  // Skriver ny skjerm (KUN FOR DEBUG)
  //lcd.clear();
  //lcd.setCursor(0,0);
  //lcd.print("Pumpe 1: " + String(tP1));
  //lcd.setCursor(0,1);
  //lcd.print("Pumpe 2: " + String(tP2));


  // Skrur på pumpene
  digitalWrite(pumpPin1, HIGH);
  digitalWrite(pumpPin2, HIGH);

  if (tP2 > tP1) { // Om tiden "vannpumpa" skal stå på er større enn for "saftpumpa"
    // Venter så lenge som pumpe 1 skal være på
    delay(tP1);
    digitalWrite(pumpPin1, LOW); // Skrur av pumpe 1 (saftpumpa)

    //Venter så lenge som pumpe 2 ytterligere skal stå på (subtraherer tid allerede gått)
    delay(tP2-tP1);
    digitalWrite(pumpPin2, LOW); // Skrur av pumpe 2 (vannpumpa)
  }
  else { // Om tiden "saftpumpa" skal stå på er større enn for "vannpumpa" (eller om de er like, men da er rekkefølgen likegyldig)
    // Venter så lenge som pumpe 2 skal være på
    delay(tP2); 
    digitalWrite(pumpPin2, LOW); // Skrur av pumpe 2 (vannpumpa)

    // Venter så lenge som pumpe 1 ytterligere skal stå på (subtraherer tid allerede gått)
    delay(tP1-tP2);
    digitalWrite(pumpPin1, LOW);
    
  }
  
  

  // Skriver ny skjerm
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Saften er ferdig!");

  servo_forward();

  

  
  
  
  
  

  

  
}


void servo_forward() { // Servoen skyver platået fremover
  for (pos = 0; pos <= 150; pos += 1) { 
    myservo.write(pos);           
    delay(50);      
  }
}

void servo_backward() { // Servoen skyver platået bakover
  for (pos = 150; pos >= 0; pos -= 1) { 
    myservo.write(pos);             
    delay(50); 
  }
}


void testpump1() { digitalWrite(pumpPin1, HIGH); } // Tester pumpe 1. (Kun for debug)

void testpump2() { digitalWrite(pumpPin2, HIGH); } // Tester pumpe 2. (Kun for debug)


  

