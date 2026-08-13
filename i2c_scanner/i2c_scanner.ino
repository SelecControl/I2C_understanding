#include <Wire.h> // include the Wire library for I2C communication

void setup() {
  Serial.begin(115200); // sets the baud rate for serial communication
  Wire.begin(); // initializes the I2C bus as a master
  Serial.println("I2C Scanner starting..."); // prints a message to the serial monitor indicating that the I2C scanner is starting
}

void loop() {
  byte count = 0; // variable to keep track of the number of I2C devices found

  for (byte address = 1; address < 127; address++) { // loop through all possible I2C addresses (1 to 126)
    Wire.beginTransmission(address); // begins a transmission to the I2C device at the specified address
    byte error = Wire.endTransmission(); // ends the transmission and returns an error code (0 if successful, 4 if no device responded)

    if (error == 0) { // if no error occurred, a device was found at the current address
      Serial.print("Device found at address 0x"); // prints the address of the found device in hexadecimal format
      Serial.println(address, HEX); // prints the address of the found device in hexadecimal format
      count++; // increments the count of found devices
    }
  }

  if (count == 0) { // if no devices were found, print a message indicating that
    Serial.println("No I2C devices found"); // prints a message indicating that no I2C devices were found
  }

  delay(5000);   // wait 5 seconds before scanning again
}
