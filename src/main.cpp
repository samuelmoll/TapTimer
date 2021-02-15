#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <RTClib.h>
#include <DHT.h>
#include <avr/eeprom.h>
#include <string.h>
#include <stdio.h>

#include "defines.hpp"
#include "EEPROMController.hpp"
#include "StateMachine.hpp"
#include "States.hpp"
#include "ValveController.hpp"
#include "valve.hpp"
#include "Keypad.h"
#include "DisplayMenu.hpp"
#include "LowPower.h"

#define RTC 1
#define LCD 1
#define DHT 0






char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Declare instances
LiquidCrystal_PCF8574 lcd(0x27);
RTC_DS3231 rtc;

Valve valve1(0, RELAY1);
Valve valve2(1, RELAY2);
Valve valve3(2, RELAY3);
Valve valve4(3, RELAY4);
ValveController valveController(&valve1, &valve2, &valve3, &valve4, &rtc);

EEPROMController eepromController(&valve1, &valve2, &valve3, &valve4);

EEMEM uint16_t EEPROMController::signature;

EEMEM uint8_t EEPROMController::valve1StartHour;
EEMEM uint8_t EEPROMController::valve1StartMin;
EEMEM uint8_t EEPROMController::valve1Duration;
EEMEM uint8_t EEPROMController::valve1Period;
EEMEM uint8_t EEPROMController::valve1Enable;

EEMEM uint8_t EEPROMController::valve2StartHour;
EEMEM uint8_t EEPROMController::valve2StartMin;
EEMEM uint8_t EEPROMController::valve2Duration;
EEMEM uint8_t EEPROMController::valve2Period;
EEMEM uint8_t EEPROMController::valve2Enable;

EEMEM uint8_t EEPROMController::valve3StartHour;
EEMEM uint8_t EEPROMController::valve3StartMin;
EEMEM uint8_t EEPROMController::valve3Duration;
EEMEM uint8_t EEPROMController::valve3Period;
EEMEM uint8_t EEPROMController::valve3Enable;

EEMEM uint8_t EEPROMController::valve4StartHour;
EEMEM uint8_t EEPROMController::valve4StartMin;
EEMEM uint8_t EEPROMController::valve4Duration;
EEMEM uint8_t EEPROMController::valve4Period;
EEMEM uint8_t EEPROMController::valve4Enable;


const byte numRows = 4;
const byte numCols = 4;
char keymap[numRows][numCols]= { {'1', '2', '3', 'A'},
                                 {'4', '5', '6', 'B'},
                                 {'7', '8', '9', 'C'},
                                 {'*', '0', '#', 'D'} };
byte rowPins[numRows] = {ROW1, ROW2, ROW3, ROW4};
byte colPins[numCols] = {COL1, COL2, COL3, COL4};
Keypad keypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

DisplayMenu displayMenu(&valveController, &lcd, &keypad, &rtc);

StateMachine machine(&displayMenu, &valveController, &eepromController, new RunScheduleState());


void init_LCD() {
  Serial.println("Dose: check for LCD");
  int error;
  // See http://playground.arduino.cc/Main/I2cScanner
  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");

  } else {
    Serial.println(": LCD not found.");
  } // if

  lcd.begin(16, 2); // initialize the lcd
}



void init_RTC() {
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(5000);

  while (! Serial);
  #if LCD
    init_LCD();
  #endif
  #if RTC
    init_RTC();
  #endif
  #if DHT
    dht.begin();
  #endif
  
  pinMode(DHTPIN, INPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
  pinMode(ROW1, INPUT);
  pinMode(ROW2, INPUT);
  pinMode(ROW3, INPUT);
  pinMode(ROW4, INPUT);
  pinMode(COL1, INPUT);
  pinMode(COL2, INPUT);
  pinMode(COL3, INPUT);
  pinMode(COL4, INPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  
  eepromController.initValves();
  valveController.initSchedule();

  lcd.setBacklight(0);
  lcd.noDisplay();
  LowPower.powerStandby(SLEEP_1S, ADC_OFF, BOD_ON);
}

void loop() {
  machine.update();
  LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
}









