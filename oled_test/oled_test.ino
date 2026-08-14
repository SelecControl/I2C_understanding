#include <Wire.h> // include the Wire library for I2C communication
#include <Adafruit_GFX.h> // include the Adafruit GFX library for graphics support
#include <Adafruit_SH110X.h> // include the Adafruit SH110X library for OLED display support

Adafruit_SH1106G display(128, 64, &Wire, -1); // create an instance of the Adafruit_SH1106G class with a 128x64 pixel display, using the Wire library for I2C communication and no reset pin

volatile int counter = 0;   // shared with the ISR → must be volatile
volatile unsigned long lastPress = 0;   // time of last accepted press
void onButtonPress();

void setup() {
  Wire.begin(); // initializes the I2C bus as a master

  if (!display.begin(0x3C, true)) { // initialize the display with I2C address 0x3C and reset the display
    while (true);   // OLED not found — stop here forever
  }

  display.clearDisplay(); // clear the display buffer
  display.setTextSize(1); // set the text size to 1 (default size)
  display.setTextColor(SH110X_WHITE); // set the text color to white (for monochrome displays)
  display.setCursor(0, 0); // set the cursor position to the top-left corner of the display
  display.println("Hello ESP32!"); // print "Hello ESP32!" to the display buffer // ln shift the cursor to the next line after printing
  //display.setCursor(0, 20); // Move cursor to the next line
  display.display(); // send the display buffer to the OLED display, making the text visible
  pinMode(27, INPUT_PULLUP); // set GPIO 27 as an input with an internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(27), onButtonPress, FALLING);
}

void loop() {
  // nothing needs to repeat for this simple example
  // static int counter = 0; // declare a static counter variable to keep track of the number of times the loop has run
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Hello ESP32!");
  display.println(counter); // print the current value of the counter to the display buffer and increment it
  display.display(); // send the updated display buffer to the OLED display
  // counter++; // increment the counter variable
  delay(1000); // wait for 1 second before the next loop iteration
  // int pb_button = digitalRead(27); // read the state of the push button connected to GPIO 27
  // if(pb_button == LOW) { // check if the push button is pressed (active low)
  //   counter++; // increment the counter variable
  // }
  // if(counter > 10) { // if the counter exceeds 10, reset it to 0
  //   counter = 0;
  // }

}

void onButtonPress() {      // the ISR — tiny!
  unsigned long now = millis();
  if (now - lastPress > 500) {   // only if >500ms since last accepted press
    counter++;
    if (counter > 10) counter = 0;
    lastPress = now;            // remember this moment for next time
  }
}