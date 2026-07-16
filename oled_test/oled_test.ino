#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

Adafruit_SH1106G display(128, 64, &Wire, -1);

void setup() {
  Wire.begin();

  if (!display.begin(0x3C, true)) {
    while (true);   // OLED not found — stop here forever
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("Hello ESP32!");
  display.display();
}

void loop() {
  // nothing needs to repeat for this simple example
}
