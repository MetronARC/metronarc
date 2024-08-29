const float arduinoVCC = 3.3; // Your Arduino voltage
unsigned long ValueR1 = 3300;
unsigned long ValueR2 = 6200;
const int alanogPin = 34;        // the pin connecting the voltage.
const int inputResolution = 4095; // works with most Arduino boards
const float average_of = 500;    // Average of 500 readings

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define I2C_SDA 32 // ganti sesukamu
#define I2C_SCL 33 // ganti sesukamu
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(128, 64, &Wire, -1);

float voltage;

void displayMessage(String message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(message);
  display.display();
}

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  Serial.println("Robojax: Reading Any voltage with Arduino");
  delay(500);
}

void loop() {
  readVoltage();

  Serial.print("Vin: ");
  Serial.print(getVoltageAverage());
  Serial.println("V");

  // Display voltage on OLED
  displayMessage("Voltage: " + String(getVoltageAverage(), 2) + "V");

  delay(100);
}

void readVoltage() {
  int A0Value = analogRead(alanogPin);
  float voltage_sensed = A0Value * (arduinoVCC / (float)inputResolution);
  voltage = voltage_sensed * (1 + ((float)ValueR2 / (float)ValueR1));
}

float getVoltageAverage() {
  float voltage_temp_average = 0;
  for (int i = 0; i < average_of; i++) {
    readVoltage();
    voltage_temp_average += voltage;
  }

  return voltage_temp_average / average_of;
}
