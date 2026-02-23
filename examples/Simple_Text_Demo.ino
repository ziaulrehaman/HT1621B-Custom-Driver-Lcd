#include <CustomDisplayControl.h>

// Pins: CS=10, WR=11, DATA=12
CustomDisplayControl lcd(10, 11, 12);

uint16_t allIcons[] = {
  CDC_BATT_NAME,  // "BATT" label
  CDC_PC_NAME,    // "PC" label
  CDC_OUT_NAME,   // "OUTPUT" label
  CDC_TEMP,       // Thermometer icon
  CDC_LOAD_STAT,  // Load status bar
  CDC_FAULT_DOT,  // Fault/Dot indicator
  CDC_UNIT_C,     // Celsius unit
  CDC_UNIT_K,     // Kelvin unit
  CDC_UNIT_W,     // Watt unit
  CDC_UNIT_A,     // Ampere unit
  CDC_BULB,       // Bulb icon
  CDC_WARNING,    // Alert triangle
  CDC_LOAD_ARR,   // Load flow arrow
  CDC_PERCENT,    // % sign
  CDC_VOLT_UNIT,  // V unit
  CDC_BATT_C1,    // Battery bar 1
  CDC_BATT_C2,    // Battery bar 2
  CDC_BATT_C3,    // Battery bar 3
  CDC_BATT_C4,    // Battery bar 4
  CDC_BATT_BOX,   // Battery outline
  CDC_CHG_ARR,    // Charging arrow
  CDC_PV_SUN      // Sun/Solar icon
};

void setup() {
  lcd.begin();
  
  // 1. Sab Icons ko Loop mein ON karein
  for (int i = 0; i < 22; i++) { // Including all segments from your list
    lcd.setIcon(allIcons[i], true);
  }

  // 2. Top Row par "ZIA " dikhayein (To check Alphanumeric segments)
  lcd.printString(0, "ZIA ");

  // 3. Bottom Row par "188.8" dikhayein (Sab se zyada segments use karta hai)
  lcd.printFloat(1, 188.8);
}

void loop() {
  // Static display: Sab kuch chamakta rahay ga!
}