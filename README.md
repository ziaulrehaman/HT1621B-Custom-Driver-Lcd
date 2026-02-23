# CustomDisplayControl Library

[![Arduino Library](https://img.shields.io/badge/Arduino-Library-blue.svg)](https://www.arduino.cc/reference/en/libraries/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A lightweight Arduino library to control a custom 8‑digit alphanumeric segment display with a rich set of icons. The display is driven via a simple 3‑wire interface (CS, WR, DATA) and is ideal for projects that need to show numeric values, short text, and status symbols (battery, units, warnings, etc.) on a single compact module.

## Features

- **8 Seven‑Segment Digits** – Arranged in two rows of four digits each.
- **Alphanumeric Support** – Display digits 0‑9 and uppercase letters A‑Z using built‑in font tables.
- **Decimal Points** – Independently controllable decimal point for every digit.
- **19 Predefined Icons** – Including battery segments, unit symbols (V, A, W, °C, K, %), status indicators (sun, bulb, warning, arrows), and labels (BATT, PC, OUTPUT).
- **Simple 3‑Wire Interface** – Uses only three digital pins (Chip Select, Write Clock, Data).
- **Internal Buffer** – All writes update a shadow buffer, reducing communication overhead and allowing efficient partial updates.
- **Battery Level Visualization** – Helper function to show a 4‑segment battery gauge.
- **Easy to Extend** – Font tables and icon definitions are exposed, making it simple to add custom characters or icons.

## Hardware Requirements

- Any Arduino‑compatible board (Uno, Mega, ESP8266, ESP32, etc.)
- A custom display module that accepts the 3‑wire protocol described below.  
  *Typical driver ICs: HT1621, TM1621, or similar.*
- Three digital I/O pins for connection.

### Pinout

| Display Module | Arduino Pin |
|----------------|-------------|
| CS (Chip Select) | Any digital pin |
| WR (Write/Clock) | Any digital pin |
| DATA            | Any digital pin |

## Installation

1. **Download** the library as a ZIP file from the repository or clone it:
   ```
   git clone https://github.com/yourusername/CustomDisplayControl.git
   ```
2. In the Arduino IDE, go to **Sketch** → **Include Library** → **Add .ZIP Library** and select the downloaded file.
3. Alternatively, copy the `CustomDisplayControl` folder into your Arduino `libraries` directory.
4. Restart the Arduino IDE.

## Getting Started

Include the library and create a `CustomDisplayControl` object with your pin numbers:

```cpp
#include <CustomDisplayControl.h>

// Pins: CS=10, WR=11, DATA=12
CustomDisplayControl lcd(10, 11, 12);

void setup() {
  lcd.begin();                     // Initialize the display
  lcd.printString(0, "ZIA ");      // Top row: "ZIA "
  lcd.printFloat(1, 188.8);        // Bottom row: "188.8"
  lcd.setIcon(CDC_BULB, true);     // Turn on the bulb icon
}

void loop() {
  // Your code here
}
```

## API Reference

### Constructor

```cpp
CustomDisplayControl(int cs, int wr, int data);
```
Creates a new display object.  
- `cs`, `wr`, `data` – digital pin numbers connected to the display.

### Initialization and Control

```cpp
void begin();
```
Initializes the pins, sends the required startup commands to the display, and clears the screen.

```cpp
void clear();
```
Clears the entire display buffer (all segments off) and updates the hardware.

### Low‑Level Access

```cpp
void writeMem(uint8_t addr, uint8_t data);
```
Writes 4 bits of data to the specified memory address (0–31). The address corresponds to a physical segment group on the display. The internal buffer is also updated.

```cpp
void setIcon(uint16_t icon, bool state);
```
Turns an individual icon on (`true`) or off (`false`). Use one of the predefined icon macros (see table below).

```cpp
void setDigitRaw(int pos, uint8_t s0, uint8_t s1, bool dot = false);
```
Sets a digit at position `pos` (0–7). The two bytes `s0` and `s1` define the segment pattern for the two halves of the digit. If `dot` is `true`, the decimal point for that digit is lit.

### High‑Level Display Functions

```cpp
void printFloat(int row, float value);
```
Displays a floating‑point number with one decimal place on the specified row.  
- `row`: `0` for top row, `1` for bottom row.  
- The number is rounded to the nearest tenth and formatted as, e.g., `188.8`. Leading zeros are suppressed.

```cpp
void printString(int row, const char* str);
```
Displays up to 4 uppercase characters on the specified row.  
- The string is automatically converted to uppercase.  
- Only letters A‑Z are mapped; other characters are ignored.  
- Example: `lcd.printString(0, "Volt")` shows `"VOLT"`.

### Utility

```cpp
void updateBattery(int level);
```
Sets the battery icon segments according to `level` (0–4). Always enables the battery outline (`CDC_BATT_BOX`).  
- `level = 0` → no bars  
- `level = 4` → all four bars lit

## Icon Macros

The following macros are defined in the header for easy icon access:

| Macro            | Description              |
|------------------|--------------------------|
| `CDC_BATT_NAME`  | "BATT" label             |
| `CDC_PC_NAME`    | "PC" label               |
| `CDC_OUT_NAME`   | "OUTPUT" label           |
| `CDC_TEMP`       | Thermometer              |
| `CDC_LOAD_STAT`  | Load status bar          |
| `CDC_FAULT_DOT`  | Fault dot                |
| `CDC_UNIT_C`     | °C unit                  |
| `CDC_UNIT_K`     | K unit                   |
| `CDC_UNIT_W`     | W unit                   |
| `CDC_UNIT_A`     | A unit                   |
| `CDC_BULB`       | Bulb icon                |
| `CDC_WARNING`    | Warning triangle         |
| `CDC_LOAD_ARR`   | Load arrow               |
| `CDC_PERCENT`    | % sign                   |
| `CDC_VOLT_UNIT`  | V unit                   |
| `CDC_BATT_C1`    | Battery bar 1            |
| `CDC_BATT_C2`    | Battery bar 2            |
| `CDC_BATT_C3`    | Battery bar 3            |
| `CDC_BATT_C4`    | Battery bar 4            |
| `CDC_BATT_BOX`   | Battery outline          |
| `CDC_CHG_ARR`    | Charging arrow           |
| `CDC_PV_SUN`     | Sun / solar icon         |

## Example Sketch

The library comes with a complete example that:
- Turns on **all** icons.
- Displays `"ZIA "` on the top row.
- Displays `"188.8"` on the bottom row (this number lights every segment of a typical 7‑segment display).

```cpp
#include <CustomDisplayControl.h>

CustomDisplayControl lcd(10, 11, 12);

// Array of all icon macros
uint16_t allIcons[] = {
  CDC_BATT_NAME, CDC_PC_NAME, CDC_OUT_NAME,
  CDC_TEMP, CDC_LOAD_STAT, CDC_FAULT_DOT,
  CDC_UNIT_C, CDC_UNIT_K, CDC_UNIT_W, CDC_UNIT_A,
  CDC_BULB, CDC_WARNING, CDC_LOAD_ARR, CDC_PERCENT, CDC_VOLT_UNIT,
  CDC_BATT_C1, CDC_BATT_C2, CDC_BATT_C3, CDC_BATT_C4, CDC_BATT_BOX,
  CDC_CHG_ARR, CDC_PV_SUN
};

void setup() {
  lcd.begin();

  // Turn on all icons
  for (int i = 0; i < 22; i++) {
    lcd.setIcon(allIcons[i], true);
  }

  lcd.printString(0, "ZIA ");
  lcd.printFloat(1, 188.8);
}

void loop() {
  // Static display – everything stays on
}
```

## Font Tables

Two internal font tables are used:

- `font[10][2]` – patterns for digits `0` through `9`.
- `alphaFont[26][2]` – patterns for uppercase letters `A` through `Z`.

Each entry consists of two bytes that define the segment states for the two driver outputs per digit. This allows the library to support custom segment wiring. If you need to modify the character set, you can edit these tables in the `.cpp` file.

## Communication Protocol

The display uses a simple 3‑wire protocol similar to SPI but without a separate MISO line:

1. **Chip Select (CS)** is pulled **LOW** to start a transaction.
2. The first three bits form a command:
   - `0x04` – Command mode (used for initialization)
   - `0x05` – Memory write mode
3. For memory writes, the next six bits are the **address** (0–31), followed by four bits of **data**.
4. Data is clocked on the **rising edge** of the WR pin.
5. CS is pulled **HIGH** to end the transaction.

The initialization sequence sends:
- `0x01` – System enable
- `0x03` – RC oscillator on
- `0x29` – Display on

## Troubleshooting

- **Nothing displays** – Check wiring; verify that the display module power is correct (usually 3.3V or 5V). Use an oscilloscope or logic analyzer to confirm the signals on CS, WR, and DATA.
- **Wrong digits or icons** – The font tables may need adjustment for your specific segment mapping. Modify the byte values in `font` and `alphaFont` to match your hardware.
- **Flickering** – Ensure that `delay()` or other blocking code is not too long; the library itself does not introduce delays during normal writes.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request on GitHub.

## License

This library is released under the [MIT License](LICENSE). You are free to use, modify, and distribute it in personal and commercial projects.

---

*Happy making!*
