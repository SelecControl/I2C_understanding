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

const unsigned char tree_bitmap[] = {
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
  0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
  0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30
};

void loop() {
  // nothing needs to repeat for this simple example
  // static int counter = 0; // declare a static counter variable to keep track of the number of times the loop has run
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Hello ESP32!");
  display.println(counter); // print the current value of the counter to the display buffer and increment it
  // draws a solid 10-wide, 12-tall box with its top-left corner at (10, 40)
  display.fillRect(10, 40, 10, 12, SH110X_WHITE); // draw a filled rectangle on the display buffer with the specified dimensions and color
  // draws a hollow 10-wide, 12-tall box with its top-left corner at (30, 40)
  display.drawRect(30, 40, 10, 12, SH110X_WHITE); // draw a rectangle on the display buffer with the specified dimensions and color
  // draw a filled circle with a radius of 6 pixels and its center at (60, 46)
  display.fillCircle(60, 46, 6, SH110X_WHITE); // draw a filled circle on the display buffer with the specified center coordinates, radius, and color
  // draw a hollow circle with a radius of 6 pixels and its center at (90, 46)
  display.drawCircle(90, 46, 6, SH110X_WHITE); // draw a circle on the display buffer with the specified center coordinates, radius, and color
  // draw a line from (110, 40) to (120, 52)
  display.drawLine(110, 40, 120, 52, SH110X_WHITE); // draw a line on the display buffer from the specified start coordinates to the specified end coordinates with the specified color
  // draw a triangle with vertices at (10, 60), (20, 60), and (15, 52)
  display.drawTriangle(10, 60, 20, 60, 15, 52, SH110X_WHITE); // draw a triangle on the display buffer with the specified vertex coordinates and color
  // draw a filled triangle with vertices at (30, 60), (40, 60), and (35, 52)
  display.fillTriangle(30, 60, 40, 60, 35, 52, SH110X_WHITE); // draw a filled triangle on the display buffer with the specified vertex coordinates and color
  // draw a rounded rectangle with top-left corner at (50, 52), width of 20 pixels, height of 12 pixels, and corner radius of 3 pixels
  display.drawRoundRect(50, 52, 20, 12, 3, SH110X_WHITE); // draw a rounded rectangle on the display buffer with the specified top-left corner coordinates, width, height, corner radius, and color
  // draw a filled rounded rectangle with top-left corner at (80, 52), width of 20 pixels, height of 12 pixels, and corner radius of 3 pixels
  display.fillRoundRect(80, 52, 20, 12, 3, SH110X_WHITE); // draw a filled rounded rectangle on the display buffer with the specified top-left corner coordinates, width, height, corner radius, and color
  // draw a horizontal line from (110, 52) to (120, 52)
  display.drawFastHLine(110, 52, 10, SH110X_WHITE); // draw a horizontal line on the display buffer from the specified start coordinates to the specified end coordinates with the specified color
  // draw a tree shape with a base at (110, 60), width of 10 pixels, and height of 8 pixels
  display.drawBitmap(110, 60, tree_bitmap, 10, 8, SH110X_WHITE); // draw a bitmap image on the display buffer with the specified top-left corner coordinates, width, height, and color
  display.display(); // send the updated display buffer to the OLED display
  // counter++; // increment the counter variable
  
  delay(1000); // wait for 1 second before the next loop iteration
  
  // display.display(); // send the updated display buffer to the OLED display
  // delay(1000); // wait for 1 second before the next loop iteration
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
  if (now - lastPress > 200) {   // only if >200ms since last accepted press
    counter++;
    if (counter > 10) counter = 0;
    lastPress = now;            // remember this moment for next time
  }
}