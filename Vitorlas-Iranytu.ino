#include <LiquidCrystal.h>

// Arduino PIN kiosztás
#define BRG_PIN A4
#define HDG_PIN A5
#define LCD_D7   2
#define LCD_D6   3
#define LCD_D5   4
#define LCD_D4   5
#define LCD_E   11
#define LCD_RS  12


short bearing, heading, turn;

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// fok ° jel az LCD-re
byte fok[8] = {
  B01110,
  B01010,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
};


void setup() {
  lcd.begin(16, 2);
  lcd.createChar(0, fok);
}

void loop() {
  
  // ADATOK BEOLVASÁSA ÉS SZÉLSŐ ÉRTÉKEK LEVÁGÁSA
  // Az iránytűt és a GPS mérését két potméter helyettesíti ezért van a map() és a constrain(),
  // egyébként, ha a GPS adatok 0 és 359 fok közötti értéket adnak, akkor azok elhagyhatók.
  // A constrain()-t nem lehet kombinálni semmivel, annak külön utasításnak kell lennie.
  // A végén az If() kell és a végén kell, tehát nem jó, ha a map()-ban 360 helyett 359 van.
  
  bearing = analogRead(BRG_PIN);
  heading = analogRead(HDG_PIN);
  bearing = constrain(bearing, 0, 1023);
  heading = constrain(heading, 0, 1023);
  bearing = map(bearing, 0, 1023, 0, 360);
  heading = map(heading, 0, 1023, 0, 360);
  if (bearing > 359) bearing = 0;
  if (heading > 359) heading = 0;

  // ELTÉRÉS (TURN) SZÁMÍTÁSA
  // más a képlet, ha az irányok különbségének abszolútértéke nagyobb, mint 180
  
  if (abs(heading - bearing) > 180) turn = heading - bearing - (360 * (abs(heading - bearing) / (heading - bearing)));
  else turn = heading - bearing;



  // ADATOK KIÍRÁSA LCD-RE
  // ez a rész rakja össze a kiírandó szöveget
  String kiB = "B: ";
  if (bearing > 99) {
    kiB += bearing;
  }
  else if (bearing > 9) {
    kiB += "0";
    kiB += bearing;
  }
  else {
    kiB += "00";
    kiB += bearing;
  }

  String kiH = "H: ";
  if (heading > 99) {
    kiH += heading;
  }
  else if (heading > 9) {
    kiH += "0";
    kiH += heading;
  }
  else {
    kiH += "00";
    kiH += heading;
  }

  String kiT = "TRN: ";
  kiT += turn;

  // ez kiírja LCD-re
  lcd.setCursor(0, 0);
  lcd.print(kiB);
  lcd.write(byte(0));
  lcd.setCursor(9, 0);
  lcd.print(kiH);
  lcd.write(byte(0));
  lcd.setCursor(0, 1);
  lcd.print(kiT);
  lcd.write(byte(0));
  lcd.print("     ");   // + 5 'space' takarítani
}
