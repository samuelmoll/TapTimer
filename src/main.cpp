#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <RTClib.h>
#include <DHT.h>
#include <avr/eeprom.h>
#include <string.h>

#include "defines.hpp"
#include "ValveController.hpp"
#include "valve.hpp"
#include "Keypad.h"
#include "DisplayMenu.hpp"
#include "LowPower.h"

#define RTC 1
#define LCD 1
#define DHT 0

LiquidCrystal_PCF8574 lcd(0x27);
RTC_DS3231 rtc;


uint16_t EEMEM signature;

uint8_t EEMEM valve1StartHour;
uint8_t EEMEM valve1StartMin;
uint8_t EEMEM valve1Duration;
uint8_t EEMEM valve1Period;
uint8_t EEMEM valve1Enable;

uint8_t EEMEM valve2StartHour;
uint8_t EEMEM valve2StartMin;
uint8_t EEMEM valve2Duration;
uint8_t EEMEM valve2Period;
uint8_t EEMEM valve2Enable;

uint8_t EEMEM valve3StartHour;
uint8_t EEMEM valve3StartMin;
uint8_t EEMEM valve3Duration;
uint8_t EEMEM valve3Period;
uint8_t EEMEM valve3Enable;

uint8_t EEMEM valve4StartHour;
uint8_t EEMEM valve4StartMin;
uint8_t EEMEM valve4Duration;
uint8_t EEMEM valve4Period;
uint8_t EEMEM valve4Enable;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

Valve valve1(0, RELAY1);
Valve valve2(1, RELAY2);
Valve valve3(2, RELAY3);
Valve valve4(3, RELAY4);
ValveController valveController(&valve1, &valve2, &valve3, &valve4, &rtc);

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

void init_valves(){
  uint16_t sig;
  sig = eeprom_read_word(&signature);
  Serial.print(sig);
  if (sig == 0x74EE) { 
    Serial.print("EEPROM data detected - loading valves."); 
    uint8_t v1StartHour;
    uint8_t v1StartMin;
    uint8_t v1Duration;
    uint8_t v1Period;
    uint8_t v1Enable;
    v1StartHour = eeprom_read_byte(&valve1StartHour);
    v1StartMin = eeprom_read_byte(&valve1StartMin);
    v1Duration = eeprom_read_byte(&valve1Duration);
    v1Period = eeprom_read_byte(&valve1Period);
    v1Enable = eeprom_read_byte(&valve1Enable);
    valve1.setStartTime(v1StartHour, v1StartMin);
    valve1.setDuration(v1Duration);
    valve1.setPeriod(v1Period);
    valve1.enableValve(v1Enable);
    
    uint8_t v2StartHour;
    uint8_t v2StartMin;
    uint8_t v2Duration;
    uint8_t v2Period;
    uint8_t v2Enable;
    v2StartHour = eeprom_read_byte(&valve2StartHour);
    v2StartMin = eeprom_read_byte(&valve2StartMin);
    v2Duration = eeprom_read_byte(&valve2Duration);
    v2Period = eeprom_read_byte(&valve2Period);
    v2Enable = eeprom_read_byte(&valve2Enable);
    valve2.setStartTime(v2StartHour, v2StartMin);
    valve2.setDuration(v2Duration);
    valve2.setPeriod(v2Period);
    valve2.enableValve(v2Enable);

    uint8_t v3StartHour;
    uint8_t v3StartMin;
    uint8_t v3Duration;
    uint8_t v3Period;
    uint8_t v3Enable;
    v3StartHour = eeprom_read_byte(&valve3StartHour);
    v3StartMin = eeprom_read_byte(&valve3StartMin);
    v3Duration = eeprom_read_byte(&valve3Duration);
    v3Period = eeprom_read_byte(&valve3Period);
    v3Enable = eeprom_read_byte(&valve3Enable);
    valve3.setStartTime(v3StartHour, v3StartMin);
    valve3.setDuration(v3Duration);
    valve3.setPeriod(v3Period);
    valve3.enableValve(v3Enable);

    uint8_t v4StartHour;
    uint8_t v4StartMin;
    uint8_t v4Duration;
    uint8_t v4Period;
    uint8_t v4Enable;
    v4StartHour = eeprom_read_byte(&valve4StartHour);
    v4StartMin = eeprom_read_byte(&valve4StartMin);
    v4Duration = eeprom_read_byte(&valve4Duration);
    v4Period = eeprom_read_byte(&valve4Period);
    v4Enable = eeprom_read_byte(&valve4Enable);
    valve4.setStartTime(v4StartHour, v4StartMin);
    valve4.setDuration(v4Duration);
    valve4.setPeriod(v4Period);
    valve4.enableValve(v4Enable);
  }
}

void updateEEPROM(){
Serial.println("updating EEprom");

    uint8_t v1StartHour = valve1.getStartHour();
    uint8_t v1StartMin = valve1.getStartMinute();
    uint8_t v1Duration = valve1.getDuration();
    uint8_t v1Period = valve1.getPeriod();
    uint8_t v1Enable = valve1.isEnabled();
    eeprom_update_byte((uint8_t*)&valve1StartHour, v1StartHour);
    eeprom_update_byte((uint8_t*)&valve1StartMin, v1StartMin);
    eeprom_update_byte((uint8_t*)&valve1Duration, v1Duration);
    eeprom_update_byte((uint8_t*)&valve1Period, v1Period);
    eeprom_update_byte((uint8_t*)&valve1Enable, v1Enable);
    
    uint8_t v2StartHour = valve2.getStartHour();
    uint8_t v2StartMin = valve2.getStartMinute();
    uint8_t v2Duration = valve2.getDuration();
    uint8_t v2Period = valve2.getPeriod();
    uint8_t v2Enable = valve2.isEnabled();
    eeprom_update_byte((uint8_t*)&valve2StartHour, v2StartHour);
    eeprom_update_byte((uint8_t*)&valve2StartMin, v2StartMin);
    eeprom_update_byte((uint8_t*)&valve2Duration, v2Duration);
    eeprom_update_byte((uint8_t*)&valve2Period, v2Period);
    eeprom_update_byte((uint8_t*)&valve2Enable, v2Enable);

    uint8_t v3StartHour = valve3.getStartHour();
    uint8_t v3StartMin = valve3.getStartMinute();
    uint8_t v3Duration = valve3.getDuration();
    uint8_t v3Period = valve3.getPeriod();
    uint8_t v3Enable = valve3.isEnabled();
    eeprom_update_byte((uint8_t*)&valve3StartHour, v3StartHour);
    eeprom_update_byte((uint8_t*)&valve3StartMin, v3StartMin);
    eeprom_update_byte((uint8_t*)&valve3Duration, v3Duration);
    eeprom_update_byte((uint8_t*)&valve3Period, v3Period);
    eeprom_update_byte((uint8_t*)&valve3Enable, v3Enable);

    uint8_t v4StartHour = valve4.getStartHour();
    uint8_t v4StartMin = valve4.getStartMinute();
    uint8_t v4Duration = valve4.getDuration();
    uint8_t v4Period = valve4.getPeriod();
    uint8_t v4Enable = valve4.isEnabled();
    eeprom_update_byte((uint8_t*)&valve4StartHour, v4StartHour);
    eeprom_update_byte((uint8_t*)&valve4StartMin, v4StartMin);
    eeprom_update_byte((uint8_t*)&valve4Duration, v4Duration);
    eeprom_update_byte((uint8_t*)&valve4Period, v4Period);
    eeprom_update_byte((uint8_t*)&valve4Enable, v4Enable);

    eeprom_update_word((uint16_t*)&signature, 0x74EE);
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
  init_valves();
  
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

  valveController.initSchedule();

  lcd.setBacklight(0);
  lcd.noDisplay();
}

void loop() {

  bool saveToEEPROM = false;
  
  char keypressed = keypad.getKey();
  if (keypressed != NO_KEY) {
    saveToEEPROM = displayMenu.mainMenu();
  }
  if(saveToEEPROM) {
    updateEEPROM();
    valveController.initSchedule();
    }
  valveController.runSchedule();
}









