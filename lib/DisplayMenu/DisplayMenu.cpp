#include "DisplayMenu.hpp"

DisplayMenu::DisplayMenu(ValveController* VC, LiquidCrystal_PCF8574* LCD, Keypad* KP, RTC_DS3231* RTC) {
    valveController = VC;
    lcd = LCD;
    keypad = KP;
    rtc = RTC;
}

char DisplayMenu::wait_for_keypad(bool scroll) {
  char keypress = keypad->getKey();
  uint32_t ms = millis();
  while(keypress == NO_KEY) {
    keypress = keypad->getKey();
    if(scroll && millis()-ms > 300)  {
        lcd->scrollDisplayLeft();
        ms = millis();
    }
  }
  return keypress;
}

char DisplayMenu::getKeypress() {
    return keypad->getKey();
}

void DisplayMenu::setLCDOff() {
    lcd->setBacklight(0);
    lcd->noDisplay();
}

void DisplayMenu::displayValveInfo(int valveID) {
    char startTime[6] = {};
    sprintf(startTime, "%2d:%02d", valveController->getValves()[valveID]->getStartHour(),
                                valveController->getValves()[valveID]->getStartMinute());
    lcd->clear(); lcd->home(); lcd->print(startTime);
    char dur[7];
    sprintf(dur, "%2dmins", valveController->getValves()[valveID]->getDuration());
    lcd->setCursor(0, 1); lcd->print(dur);
    uint8_t period = valveController->getValves()[valveID]->getPeriod();
    lcd->setCursor(8, 0);
    if (period & 0x80) {
        char days[8] = {'s','m','t','w','t','f','s', '\0'};
        for (int i = 0; i < 7; i++) {
            if(period & (1<<(6-i))) {
                days[i] = days[i] - 32;
            }
        } 
        lcd->print(days);
    } else {
        char s[7] = {};
        sprintf(s, "%2d day", period);
        lcd->print(s);
    }
    lcd->setCursor(8,1);
    if (valveController->getValves()[valveID]->isEnabled()) {
        lcd->print("Enabled");
    } else {
        lcd->println("Disabled");
    }
}

void DisplayMenu::setPeriod(int id) {
    lcd->clear(); lcd->home();
    lcd->print("1. Weekdays");
    lcd->setCursor(0, 1);
    lcd->print("2. Interval");
    lcd->noCursor();
    char keypressed;
    uint8_t period = valveController->getValves()[id]->getPeriod();
    keypressed = wait_for_keypad(0);
    lcd->clear();
    if (keypressed == '1') {
        char days[8] = {'s','m','t','w','t','f','s', '\0'};
        lcd->setCursor(1,0);
        for (int i = 0; i < 7; i++) {
            if(period & (1<<(6-i))) {
                days[i] = days[i] - 32;
            }
        } 
        lcd->print(days);
        while(keypressed != '#' || keypressed != '*') {
            char day[2] = {};
            keypressed = wait_for_keypad(0);
            if(keypressed == '#') {
                period |= 0x80;
                valveController->getValves()[id]->setPeriod(period);
                break;
            } else if (keypressed == '*') {
                break;
            }
            day[0] = keypressed;
            int dayNum = atoi(day);
            Serial.print(keypressed);
            Serial.print(dayNum);
            period = period ^ (1<<(7-dayNum));
            char days[8] = {'s','m','t','w','t','f','s', '\0'};
            lcd->setCursor(1,0);
            for (int i = 0; i < 7; i++) {
                if(period & (1<<(6-i))) {
                    days[i] = days[i] - 32;
                }
            }
            lcd->print(days);
        }
    } else if (keypressed == '2') {
        char dur[3] = {};
        int count = 0;
        
        lcd->setCursor(4,0); lcd->print("day(s)"); lcd->setCursor(1,0);
        keypressed = wait_for_keypad(0);
        while(keypressed !='D' || keypressed != 'A') {
            dur[count] = keypressed;
            count++;
            lcd->write(keypressed); lcd->setCursor(1+count,0);
            keypressed = wait_for_keypad(0);
            if(keypressed == '#') {
                Serial.print(atoi(dur));
                valveController->getValves()[id]->setPeriod(atoi(dur));
                break;
            } else if (keypressed == '*') {
                break;
            }
        }   
    }
}

int DisplayMenu::enterValveID() {
    char keypressed;
    lcd->home(); lcd->clear(); lcd->print("Enter Valve #");
    keypressed = wait_for_keypad(0);
    int valveID = (int)keypressed-48-1;  // -48 for acsii conversion, -1 to get indexing right
    lcd->clear(); lcd->home(); lcd->print("Valve ");
    lcd->setCursor(6, 0); lcd->print(keypressed);
    return valveID;
}

void DisplayMenu::setStartTime(int valveID) {
    char keypressed;
    char newHourString[3] = {};
    char newMinString[3] = {};
    lcd->setCursor(0,0);
    keypressed = wait_for_keypad(0);
    newHourString[0] = keypressed;
    lcd->write(keypressed);
    lcd->setCursor(1, 0);
    keypressed = wait_for_keypad(0);
    newHourString[1] = keypressed;                    
    lcd->write(keypressed);
    int newHour = atoi(newHourString);
    lcd->setCursor(3, 0);
    keypressed = wait_for_keypad(0);
    newMinString[0] = keypressed;
    lcd->write(keypressed);
    lcd->setCursor(4, 0);
    keypressed = wait_for_keypad(0);
    newMinString[1] = keypressed;
    lcd->write(keypressed);
    int newMin = atoi(newMinString);
    valveController->getValves()[valveID]->setStartTime(newHour, newMin);
}

void DisplayMenu::setDuration(int valveID) {
    char keypressed;
    lcd->clear(); lcd->home();
    lcd->print("Duration in min:");
    lcd->setCursor(6, 1);
    keypressed = wait_for_keypad(0);
    int count =0;
    char dur[3] = {};
    while(keypressed !='D' || keypressed != 'A') {
        dur[count] = keypressed;
        count++;
        lcd->write(keypressed); lcd->setCursor(6+count,1);
        keypressed = wait_for_keypad(0);
        if(keypressed == '#') {
            Serial.print(atoi(dur));
            valveController->getValves()[valveID]->setDuration(atoi(dur));
            break;
        } else if (keypressed == '*') {
            break;
        }
    }
}

void DisplayMenu::displayOptions() {
    DateTime now = rtc->now();
    char time[6] = {};
    sprintf(time, "%2d:%02d", now.hour(), now.minute());
    lcd->home(); lcd->clear();
    lcd->print("A - Edit Schedule, B - Run Once");
    lcd->setCursor(1,1);
    lcd->print(time);
}

int DisplayMenu::mainMenu() {
    char keypressed;
    int valveID = -1;
    bool saveToEEPROM = false;
    lcd->display();
    lcd->setBacklight(150);
    displayOptions();
    keypressed = wait_for_keypad(1);
    Serial.print(keypressed);
    while(keypressed != '*'){
        switch(keypressed) {
            case 'A':
            {
                valveConfigurationMenu();
                return 1;
            }

            case 'B':
                return 2;                
            case '#':
                break;
        }
        displayOptions();
        keypressed = wait_for_keypad(0);
    }
    saveToEEPROM = true;
    lcd->setBacklight(0);
    lcd->noDisplay();
    return saveToEEPROM;
}

int DisplayMenu::valveConfigurationMenu() {
    int valveID = enterValveID();
    delay(1000);
    displayValveInfo(valveID);
    char keypressedEditMenu = wait_for_keypad(0);
    int cursorCol = 0; int cursorRow = 0;
    lcd->setCursor(cursorCol,cursorRow);lcd->cursor();
    do {
        switch(keypressedEditMenu) {
            case '1':
                setStartTime(valveID);
                break;
            case '3':
                setPeriod(valveID);
                break;
            case '7':
                setDuration(valveID);
                break;
            case '9':
                valveController->getValves()[valveID]->toggleValve();
                break;
            case '*':
                keypressedEditMenu = '*';
                break;
            default:
                ;
        }
        displayValveInfo(valveID);
        keypressedEditMenu = wait_for_keypad(0);
    } while (keypressedEditMenu != '*');
}

int DisplayMenu::valveSelectRunOnce() {
    char keypressed = NO_KEY;
    int select = 0;
    Serial.println("B pressed");
    lcd->home(); lcd->clear();
    lcd->print(" Enter Valves");
    lcd->setCursor(2,1);
    for (int i = 0; i < NUM_VALVES; i++) {
        lcd->print("X");
        lcd->setCursor(2+i, 1);  // ensures forward compatibility for more valves
    }
    
    keypressed = wait_for_keypad(0);
    while(keypressed !='*' || keypressed != '#') {
        int offset = (int)(keypressed) - 49;
        Serial.print("Offset: "); Serial.println(offset);
        select = select | (1<<(offset));
        lcd->setCursor(2+offset, 1); lcd->print('O');
        keypressed = wait_for_keypad(0);
        if(keypressed == '#') {
            return select;
        } else if (keypressed == '*') {
            return -1;
        }
    }
}
