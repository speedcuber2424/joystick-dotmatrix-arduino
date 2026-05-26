# Joystick Dot Matrix Controller 

Control a 2×2 pixel cursor on an 8×8 LED dot matrix using a joystick module and Arduino UNO R4 Minima. Press the joystick button to light up all 64 LEDs at once.

Built for the **ARYAN BUILDS** YouTube channel.

---

## 📹 Watch the Video

> *Link coming soon — subscribe to [ARYAN BUILDS](https://youtube.com/@ARYANBUILDS) so you don't miss it!*

---

## 🛒 Parts List

| Component | Quantity |
|---|---|
| Arduino UNO R4 Minima | 1 |
| MAX7219 8×8 LED Dot Matrix Module | 1 |
| Joystick Module (KY-023 or similar) | 1 |
| Jumper wires (Male-to-Male) | ~10 |

---

## 🔌 Wiring

### Dot Matrix Module → Arduino R4 Minima
| Module Pin | Arduino Pin |
|---|---|
| VCC | 5V |
| GND | GND |
| DIN | D11 |
| CS | D10 |
| CLK | D13 |

> Use the **input side** of the module (the side labeled VCC GND DIN CS CLK). The other side (DOUT) is for chaining a second module — leave it unconnected.

### Joystick Module → Arduino R4 Minima
| Module Pin | Arduino Pin |
|---|---|
| VCC | 5V |
| GND | GND |
| VRX | A0 |
| VRY | A1 |
| SW | D2 |

---

##  Library Required

Install **LedControl** by Eberhard Fahle via the Arduino IDE Library Manager:

`Sketch → Include Library → Manage Libraries → search "LedControl" → Install`

---

##  Important Note for R4 Minima Users

The Arduino UNO R4 Minima has a **14-bit ADC**, meaning `analogRead()` returns values from **0 to 16383** — not 0 to 1023 like classic Arduino boards.

However, some joystick modules output a compressed voltage range. If your cursor is stuck in a corner on startup, open Serial Monitor at **115200 baud**, read your joystick's resting X and Y values, and update the `CENTER` constant in the code to match.

In this project, the joystick rested at ~530, so:
```cpp
const int CENTER    = 530;
const int DEAD_ZONE = 200;
```

---

##  How It Works

- Move the joystick → 2×2 cursor block travels across the 8×8 matrix
- Press the joystick button → all 64 LEDs light up
- Press again → back to cursor mode

---

##  Concepts Covered

- SPI communication with MAX7219 using the LedControl library
- Reading analog joystick X/Y values with 14-bit ADC on R4 Minima
- Dead zone implementation to prevent cursor drift
- Button debouncing with INPUT_PULLUP
- Constraining values to grid boundaries with `constrain()`

---

## 📜 License

MIT License — free to use, modify, and share. Credit appreciated!

---

*Please Drop a star if you like it*
