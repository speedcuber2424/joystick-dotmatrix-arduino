// Project: Joystick Dot Matrix Cursor
// Board: Arduino UNO R4 Minima
// Author: Aryan (ARYAN BUILDS)
// Description: Move a glowing cursor across an 8x8 LED dot matrix using a joystick
// Library needed: LedControl by Eberhard Fahle (install via Library Manager)

#include <LedControl.h>   // The library that talks to our MAX7219 module

// ─────────────────────────────────────────
// PIN DEFINITIONS
// ─────────────────────────────────────────

// MAX7219 Dot Matrix pins
// LedControl(DIN, CLK, CS, numberOfDevices)
// DIN = D11, CLK = D13, CS = D10
// "1" means we only have one dot matrix module chained
LedControl lc = LedControl(11, 13, 10, 1);

// Joystick pins
const int JOY_X  = A0;   // Left = low value, Right = high value
const int JOY_Y  = A1;   // Up = low value, Down = high value
const int JOY_SW = 2;    // Push button (active LOW — reads LOW when pressed)

// ─────────────────────────────────────────
// JOYSTICK THRESHOLDS
// IMPORTANT: R4 Minima ADC is 14-bit → values are 0 to 16383, NOT 0 to 1023!
// Center resting position is roughly 8191
// Dead zone prevents cursor drift when joystick is released
// ─────────────────────────────────────────
const int CENTER    = 530;   // your actual resting X value
const int DEAD_ZONE = 200;
const int MOVE_DELAY = 180;   // threshold around center to ignore small drift   // ms between cursor steps — tune this for feel

// ─────────────────────────────────────────
// CURSOR STATE
// The 8x8 grid has rows 0–7 (top to bottom) and cols 0–7 (left to right)
// ─────────────────────────────────────────
int curRow = 3;   // Start near center of the 8x8
int curCol = 3;

// Button state (used to detect a fresh press, not a hold)
bool lastButtonState = HIGH;

// ─────────────────────────────────────────
// SPECIAL EFFECT: "Explode" all LEDs on button press
// ─────────────────────────────────────────
bool explodeMode = false;

void setup() {
  Serial.begin(115200);   // Always 115200 on R4 Minima!
  Serial.println("Joystick Dot Matrix — ARYAN BUILDS");

  // Wake up the MAX7219 — it starts in power-save mode by default
  lc.shutdown(0, false);      // 0 = device index (first module). false = wake up
  lc.setIntensity(0, 10);     // Brightness: 0 (dim) to 15 (brightest). 10 is good on camera
  lc.clearDisplay(0);         // Clear any leftover garbage on the display

  // Set joystick button with internal pull-up resistor
  // This means: pin is HIGH normally, goes LOW when you press the button
  // No external resistor needed!
  pinMode(JOY_SW, INPUT_PULLUP);

  // Draw the starting cursor position
  drawCursor();

  Serial.println("Ready! Move joystick to control cursor.");
  Serial.println("Press joystick button to explode all LEDs!");
}

void loop() {
  // ── Read joystick ────────────────────────
  int xVal = analogRead(JOY_X);   // 0–16383
  int yVal = analogRead(JOY_Y);   // 0–16383

  // Debug: watch values live in Serial Monitor (Tools → Serial Monitor → set to 115200)
  Serial.print("X: "); Serial.print(xVal);
  Serial.print("  Y: "); Serial.print(yVal);
  Serial.print("  Pos: ["); Serial.print(curRow);
  Serial.print(","); Serial.print(curCol); Serial.println("]");

  // ── Check button press ───────────────────
  bool buttonState = digitalRead(JOY_SW);
  if (buttonState == LOW && lastButtonState == HIGH) {
    // Fresh press detected (LOW = pressed because of INPUT_PULLUP)
    explodeMode = !explodeMode;   // Toggle the explosion effect
    Serial.println(explodeMode ? "EXPLODE MODE ON" : "Cursor mode");

    if (explodeMode) {
      // Fill every single LED on the display
      for (int r = 0; r < 8; r++) {
        lc.setRow(0, r, 0xFF);   // 0xFF = binary 11111111 = all 8 LEDs in a row ON
      }
    } else {
      // Go back to cursor mode — clear display and redraw cursor
      lc.clearDisplay(0);
      drawCursor();
    }
  }
  lastButtonState = buttonState;

  // If we're in explode mode, skip cursor movement
  if (explodeMode) {
    delay(MOVE_DELAY);
    return;
  }

  // ── Calculate new cursor position ────────
  int newRow = curRow;
  int newCol = curCol;

  // Check X axis (left / right)
  if (xVal < CENTER - DEAD_ZONE) {
    newCol = curCol - 1;   // Move LEFT
  } else if (xVal > CENTER + DEAD_ZONE) {
    newCol = curCol + 1;   // Move RIGHT
  }

  // Check Y axis (up / down)
  // Note: on most joystick modules, pushing UP gives a LOW value
  if (yVal < CENTER - DEAD_ZONE) {
    newRow = curRow - 1;   // Move UP
  } else if (yVal > CENTER + DEAD_ZONE) {
    newRow = curRow + 1;   // Move DOWN
  }

  // constrain() clamps the value so cursor can't go off the 8x8 grid
  // constrain(value, min, max) — built into Arduino
  newRow = constrain(newRow, 0, 7);
  newCol = constrain(newCol, 0, 7);

  // Only redraw if the cursor actually moved (saves unnecessary flickering)
  if (newRow != curRow || newCol != curCol) {
    curRow = newRow;
    curCol = newCol;
    drawCursor();
  }

  delay(MOVE_DELAY);
}

// ─────────────────────────────────────────
// drawCursor()
// Clears the whole display, then lights up just the cursor pixel
// ─────────────────────────────────────────
void drawCursor() {
  lc.clearDisplay(0);
  // Light up a 2x2 block of LEDs centered on the cursor
  for (int r = curRow; r <= curRow + 1; r++) {
    for (int c = curCol; c <= curCol + 1; c++) {
      // constrain keeps the block inside the 8x8 grid boundaries
      lc.setLed(0, constrain(r, 0, 7), constrain(c, 0, 7), true);
    }
  }
}      // Light up ONE pixel at cursor position
  // lc.setLed(device, row, col, state)
  // device = 0 (our only module)
  // row = 0 (top) to 7 (bottom)
  // col = 0 (left) to 7 (right)
  // state = true (ON) or false (OFF)

