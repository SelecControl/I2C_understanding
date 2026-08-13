#include <Wire.h> // include the Wire library for I2C communication
#include <Adafruit_GFX.h> // include the Adafruit GFX library for graphics support
#include <Adafruit_SH110X.h> // include the Adafruit SH110X library for OLED display support

Adafruit_SH1106G display(128, 64, &Wire, -1); // create an instance of the Adafruit_SH1106G class with a 128x64 pixel display, using the Wire library for I2C communication and no reset pin

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
}

void loop() {
  // nothing needs to repeat for this simple example
  static int counter = 0; // declare a static counter variable to keep track of the number of times the loop has run
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Hello ESP32!");
  display.println(counter); // print the current value of the counter to the display buffer and increment it
  display.display(); // send the updated display buffer to the OLED display
  counter++; // increment the counter variable
  delay(1000); // wait for 1 second before the next loop iteration
  if(counter > 10) { // if the counter exceeds 10, reset it to 0
    counter = 0;
  }
}
