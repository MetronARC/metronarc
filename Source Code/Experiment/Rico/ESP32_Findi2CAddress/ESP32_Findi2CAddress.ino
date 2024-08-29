#include <Wire.h>

#define SDA_PIN 16
#define SCL_PIN 17

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.println("I2C Scanner");
}

void loop() {
  byte error, address;
  int deviceCount = 0;

  Serial.println("Scanning...");

  for(address = 1; address < 127; address++ ) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      deviceCount++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }    
  }

  if (deviceCount == 0) {
    Serial.println("No I2C devices found");
  }

  delay(5000);  // Wait 5 seconds for next scan
}
