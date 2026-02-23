#include "CustomDisplayControl.h"

const uint8_t CustomDisplayControl::font[10][2] = {
    {0b1111, 0b1010}, {0b0000, 0b1010}, {0b1101, 0b0110}, {0b1001, 0b1110},
    {0b0010, 0b1110}, {0b1011, 0b1100}, {0b1111, 0b1100}, {0b0001, 0b1010},
    {0b1111, 0b1110}, {0b1011, 0b1110}
};

const uint8_t CustomDisplayControl::alphaFont[26][2] = {
    { 0b0111, 0b1110 }, { 0b1110, 0b1100 }, { 0b1111, 0b0000 }, { 0b1100, 0b1110 }, 
    { 0b1111, 0b0100 }, { 0b0111, 0b0100 }, { 0b1111, 0b1100 }, { 0b0110, 0b1110 }, 
    { 0b0000, 0b1010 }, { 0b1000, 0b1110 }, { 0b0110, 0b0110 }, { 0b1110, 0b0000 }, 
    { 0b0101, 0b1010 }, { 0b0100, 0b1100 }, { 0b1111, 0b1010 }, { 0b0111, 0b0110 }, 
    { 0b0011, 0b1110 }, { 0b0100, 0b0100 }, { 0b1011, 0b1100 }, { 0b1110, 0b0100 }, 
    { 0b1100, 0b1000 }, { 0b1110, 0b1010 }, { 0b1100, 0b1010 }, { 0b0110, 0b1010 }, 
    { 0b1011, 0b1110 }, { 0b1001, 0b0110 }
};

CustomDisplayControl::CustomDisplayControl(int cs, int wr, int data) : _cs(cs), _wr(wr), _data(data) {}

void CustomDisplayControl::begin() {
    pinMode(_cs, OUTPUT); pinMode(_wr, OUTPUT); pinMode(_data, OUTPUT);
    digitalWrite(_cs, HIGH); delay(100);
    sendCmd(0x01); sendCmd(0x03); sendCmd(0x29);
    clear();
}

void CustomDisplayControl::sendBits(uint16_t data, uint8_t bits) {
    for (int8_t i = bits - 1; i >= 0; i--) {
        digitalWrite(_wr, LOW);
        digitalWrite(_data, (data >> i) & 0x01);
        digitalWrite(_wr, HIGH);
    }
}

void CustomDisplayControl::sendCmd(uint8_t cmd) {
    digitalWrite(_cs, LOW); sendBits(0x04, 3); sendBits(cmd, 8); sendBits(0, 1); digitalWrite(_cs, HIGH);
}

void CustomDisplayControl::writeMem(uint8_t addr, uint8_t data) {
    buffer[addr] = data; digitalWrite(_cs, LOW); sendBits(0x05, 3); sendBits(addr, 6);
    for (int i = 0; i < 4; i++) { digitalWrite(_wr, LOW); digitalWrite(_data, (data >> i) & 0x01); digitalWrite(_wr, HIGH); }
    digitalWrite(_cs, HIGH);
}

void CustomDisplayControl::clear() { for (int i = 0; i < 32; i++) writeMem(i, 0); }

void CustomDisplayControl::setIcon(uint16_t icon, bool state) {
    uint8_t addr = icon >> 4; uint8_t bit = icon & 0x0F;
    uint8_t val = buffer[addr];
    if (state) val |= (1 << bit); else val &= ~(1 << bit);
    writeMem(addr, val);
}

void CustomDisplayControl::updateBattery(int level) {
    setIcon(CDC_BATT_BOX, true);
    setIcon(CDC_BATT_C1, level >= 1); setIcon(CDC_BATT_C2, level >= 2);
    setIcon(CDC_BATT_C3, level >= 3); setIcon(CDC_BATT_C4, level >= 4);
}

void CustomDisplayControl::setDigitRaw(int pos, uint8_t s0, uint8_t s1, bool dot) {
    int a0 = (pos < 4) ? pos * 2 : 13 + (pos - 4) * 2;
    int a1 = a0 + 1;
    uint8_t iconBit = buffer[a1] & 0x01;
    writeMem(a0, s0); writeMem(a1, s1 | (dot ? 0x01 : iconBit));
}

void CustomDisplayControl::printFloat(int row, float value) {
    int start = (row == 0) ? 0 : 4;
    int v = (int)(value * 10.0 + 0.5);
    setDigitRaw(start + 0, (v / 1000) % 10 == 0 ? 0 : font[(v / 1000) % 10][0], (v / 1000) % 10 == 0 ? 0 : font[(v / 1000) % 10][1]);
    setDigitRaw(start + 1, font[(v / 100) % 10][0], font[(v / 100) % 10][1]);
    setDigitRaw(start + 2, font[(v / 10) % 10][0], font[(v / 10) % 10][1], true);
    setDigitRaw(start + 3, font[v % 10][0], font[v % 10][1]);
}

void CustomDisplayControl::printString(int row, const char* str) {
    int start = (row == 0) ? 0 : 4;
    for (int i = 0; i < 4; i++) {
        if (str[i] == '\0') break;
        char c = str[i];
        if (c >= 'a' && c <= 'z') c -= 32;
        if (c >= 'A' && c <= 'Z') {
            int idx = c - 'A';
            setDigitRaw(start + i, alphaFont[idx][0], alphaFont[idx][1]);
        }
    }
}