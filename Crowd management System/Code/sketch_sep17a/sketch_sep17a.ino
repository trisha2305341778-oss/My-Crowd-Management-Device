#include <Wire.h>
#include <LiquidCrystal_I2C.h>


const int trig1 = 2;
const int echo1 = 3;
const int trig2 = 4;
const int echo2 = 5;


LiquidCrystal_I2C lcd(0x27, 16, 2);

int peopleCount = 0;
int maxPeople = 4;
unsigned long personAtDoorSince = 0;

long getDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  return pulseIn(echo, HIGH) * 0.034 / 2;
}

void setup() {
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);

  lcd.init();         // Initialize the I2C LCD
  lcd.backlight();    // Turn on the LCD backlight
  Serial.begin(9600);

  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
  long d1 = getDistance(trig1, echo1);
  long d2 = getDistance(trig2, echo2);

 
  if (d1 < 10 || d2 < 10) {
    if (personAtDoorSince == 0) {
      personAtDoorSince = millis();
    } else if (millis() - personAtDoorSince > 10000) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Don't block door");
      lcd.setCursor(0, 1);
      lcd.print("Move please!");
      delay(2000);
    }
  } else {
    personAtDoorSince = 0;
  }


  if (d1 < 10) {
    unsigned long tStart = millis();
    while (millis() - tStart < 2000) {
      long d2Check = getDistance(trig2, echo2);
      if (d2Check < 10) {
        peopleCount++;
        break;
      }
    }
  }

  else if (d2 < 10) {
    unsigned long tStart = millis();
    while (millis() - tStart < 2000) {
      long d1Check = getDistance(trig1, echo1);
      if (d1Check < 10) {
        if (peopleCount > 0) peopleCount--;
        break;
      }
    }
  }

  lcd.clear();
  if (peopleCount >= maxPeople) {
    lcd.setCursor(0, 0);
    lcd.print("DO Not Enter");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Now Get In");
  }

  lcd.setCursor(0, 1);
  lcd.print("People: ");
  lcd.print(peopleCount);

  Serial.println(peopleCount);

  delay(500);
}