#include <LiquidCrystal.h>

// Arduino PIN kiosztás
#define BI_PIN A4
#define MI_PIN A5
#define LCD_D7  2
#define LCD_D6  3
#define LCD_D5  4
#define LCD_D4  5
#define LCD_E  11
#define LCD_RS 12


short beallitottIrany, mertIrany, elteres;

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
  // ADATOK BEOLVASÁSA
  // az irányítűt és a GPS mérését két potméter helyettesíti ezért van a map(), egyébként nem kellene
  beallitottIrany = map(analogRead(BI_PIN), 0, 1023, 0, 360);
  mertIrany = map(analogRead(MI_PIN), 0, 1023, 0, 360);

  // SZÉLSŐ ÉRTÉKEK LEVÁGÁSA
  // Elvileg megy 360-nal is a számítás, de érdemes azt nullára átírni
  // (a constrain-es sorok elhagyhatók, azok csak a potméterek bizonytalanságai miatt vannak!)
  if (beallitottIrany > 359) beallitottIrany = 0;
  constrain (beallitottIrany, 0, 359);
  if (mertIrany > 359) mertIrany = 0;
  constrain (mertIrany, 0, 359);

  // ELTÉRÉS KISZÁMÍTÁSA
  // más a képlet, ha az irányok különbségének abszolútértéke nagyobb, mint 180
  if (abs(mertIrany - beallitottIrany) > 180) elteres = mertIrany - beallitottIrany - (360 * (abs(mertIrany - beallitottIrany) / (mertIrany - beallitottIrany)));
  else elteres = mertIrany - beallitottIrany;

  // KIÍRANDÓ ADATOK ÖSSZEÁLLÍTÁSA
  // ez a rész nem fontos, csak az egyszerűbb pozícionálás miatt van
  String kiC = "C:";
  if (beallitottIrany > 99) {
    kiC += beallitottIrany;
  }
  else if (beallitottIrany > 9) {
    kiC += "0";
    kiC += beallitottIrany;
  }
  else {
    kiC += "00";
    kiC += beallitottIrany;
  }
  String kiM = "M:";
  if (mertIrany > 99) {
    kiM += mertIrany;
  }
  else if (mertIrany > 9) {
    kiM += "0";
    kiM += mertIrany;
  }
  else {
    kiM += "00";
    kiM += mertIrany;
  }
  String kiE = "elteres:";
  kiE += elteres;

  // ADATOK KIÍRÁSA
  lcd.setCursor(0, 0);
  lcd.print(kiC);
  lcd.write(byte(0));
  lcd.setCursor(9, 0);
  lcd.print(kiM);
  lcd.write(byte(0));
  lcd.setCursor(0, 1);
  lcd.print(kiE);
  lcd.write(byte(0));
  lcd.print("     ");   // + 5 'space' takarítani
}
