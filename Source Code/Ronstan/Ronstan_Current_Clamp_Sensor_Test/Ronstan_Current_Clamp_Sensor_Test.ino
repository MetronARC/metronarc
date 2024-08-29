#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <MFRC522.h>
//---- Website ----
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiMulti.h>

HTTPClient http;
String Link, LinkTiming, Mode, Status, payload;
int httpCode; 

WiFiMulti wifiMulti;

bool arcTimeoutHandled = false;

#define RST_PIN         22          // Configurable, see typical pin layout above
#define SS_PIN          21         // Configurable, see typical pin layout above
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define I2C_SDA 16 // ganti sesukamu
#define I2C_SCL 17 // ganti sesukamu
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(128, 64, &Wire, -1);

MFRC522 rfid(SS_PIN, RST_PIN);   // Create MFRC522 instance

#define ADC_ADDRESS     (0x48)      // ADC I2C Address
Adafruit_ADS1115 ads;

bool arcDetected = false;
bool uidRead = false;
String uid = "RONSTAN-1";
String weldID;
String currentTime;
unsigned long arcInitiatedTime = 0;
bool arcInitiated = false;

float sumCurrentDC = 0;
int counterCurrentDC = 0;
bool initializationDone = false;

float averageCurrentDC = 0.0;  // Variable to store the average currentDC during arc-on period

const char *ssid1 = "Orbit 1";
const char *password1 = "2468g0a7";
const char *host1 = "ronstan.sparcmonitoring.com";

const char *ssid2 = "MetronARC Technology";
const char *password2 = "2468g0a7A7B7*";
const char *host2 = "ronstan.sparcmonitoring.com";

const char *ssid3 = "ico";
const char *password3 = "19403054";
const char* host3 = "ronstan.sparcmonitoring.com";

const char* host = "";
String machineID = "RONSTAN-1";
byte statusKoneksi;

int area = 1;

const float FACTOR = 50; //50A/1V from teh CT
const float multiplier = 0.0000394;
float currentRMS = 0.0, previousCurrentRMS = -1.0;  // Initialize previousCurrentRMS to an impossible value

long millisSebelum;
byte detik, menit, jam;
String detikS, menitS, jamS, jmdS;
byte statusWaktu;
String statusT;

unsigned long previousMillis = 0;
const int period = 1;

const float ESPVCC = 3.3; // Voltage of your Arduino
const unsigned long VALUE_R1 = 99900;
const unsigned long VALUE_R2 = 3280;
const double VOLTAGE_SOURCE = 103.8;
const int ANALOG_PIN = 34;
const int INPUT_RESOLUTION = 4096;
const float AVERAGE_OF = 500;

int analogValue;
int voltageMillivolts;
float voltage;
float calibration;
float voltageAverage;

unsigned long previousCheckTimeMillis = 0;
const unsigned long checkTimeInterval = 60000; // 10 seconds in milliseconds

bool buttonPressed = false;
unsigned long pressStartTime = 0;
const unsigned long pressDuration = 5000; // 5 seconds in milliseconds

bool shiftDoneHandled = false;

void displayCurrentRMS(float current) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Current RMS: ");
  display.print(current, 2); // Print float with 2 decimal places
  display.display();
}

float getCurrent() {
  float voltage;
  float current;
  float sum = 0;
  int counter = 0;

  for (int i = 0; i < 10; i++) { //take 10 samples
    voltage = ads.readADC_Differential_0_1() * multiplier;
    current = voltage * FACTOR;

    // Apply adjusted calibration equation
    current = 2.0 * current - 0.5;

    sum += sq(current);
    counter++;
  }

  current = sqrt(sum / counter);
  return current;
}

void syaratTiming() {
  currentRMS = getCurrent();

  Serial.print("Current RMS: ");
  Serial.println(currentRMS, 2); // Print float with 2 decimal places

  if (currentRMS != previousCurrentRMS) {
    displayCurrentRMS(currentRMS);
    previousCurrentRMS = currentRMS;
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  ads.setGain(GAIN_FOUR);      // +/- 1.024V 1bit = 0.5mV
  ads.begin();
}

void loop() {
  syaratTiming();
  delay(1000);
}
