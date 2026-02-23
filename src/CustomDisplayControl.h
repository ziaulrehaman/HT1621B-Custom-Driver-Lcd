#ifndef CUSTOM_DISPLAY_CONTROL_H
#define CUSTOM_DISPLAY_CONTROL_H

#include <Arduino.h>

// --- ALL 19 ICONS (Complete Zia's Set) ---
#define CDC_BATT_NAME  (14 << 4 | 0)
#define CDC_PC_NAME    (16 << 4 | 0)
#define CDC_OUT_NAME   (20 << 4 | 0)

#define CDC_TEMP       (1  << 4 | 0)
#define CDC_LOAD_STAT  (3  << 4 | 0)
#define CDC_FAULT_DOT  (7  << 4 | 0)

#define CDC_UNIT_C     (8  << 4 | 0)
#define CDC_UNIT_K     (8  << 4 | 1)
#define CDC_UNIT_W     (8  << 4 | 2)
#define CDC_UNIT_A     (8  << 4 | 3)

#define CDC_BULB       (9  << 4 | 0)
#define CDC_WARNING    (9  << 4 | 2)
#define CDC_LOAD_ARR   (10 << 4 | 0)
#define CDC_PERCENT    (10 << 4 | 1)
#define CDC_VOLT_UNIT  (10 << 4 | 3)

// Battery Segments (The ones that caused the error)
#define CDC_BATT_C1    (11 << 4 | 0)
#define CDC_BATT_C2    (11 << 4 | 1)
#define CDC_BATT_C3    (11 << 4 | 2)
#define CDC_BATT_C4    (11 << 4 | 3)
#define CDC_BATT_BOX   (12 << 4 | 0)

#define CDC_CHG_ARR    (12 << 4 | 1)
#define CDC_PV_SUN     (12 << 4 | 3)

class CustomDisplayControl {
  private:
    uint8_t _cs, _wr, _data;
    uint8_t buffer[32];
    static const uint8_t font[10][2];
    static const uint8_t alphaFont[26][2];

    void sendBits(uint16_t data, uint8_t bits);
    void sendCmd(uint8_t cmd);

  public:
    CustomDisplayControl(int cs, int wr, int data);
    void begin();
    void clear();
    void writeMem(uint8_t addr, uint8_t data);
    void setIcon(uint16_t icon, bool state);
    void updateBattery(int level);
    void setDigitRaw(int pos, uint8_t s0, uint8_t s1, bool dot = false);
    void printFloat(int row, float value);
    void printString(int row, const char* str);
};

#endif