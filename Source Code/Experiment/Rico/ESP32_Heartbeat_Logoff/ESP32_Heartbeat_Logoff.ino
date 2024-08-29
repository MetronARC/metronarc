#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <MFRC522.h>
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
String uid = "MACHINE-1";
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
const char *host1 = "sparcmonitoring.com";

const char *ssid2 = "MetronARC Technologies";
const char *password2 = "metronarc123";
const char *host2 = "sparcmonitoring.com";

const char *ssid3 = "ico";
const char *password3 = "19403054";
const char *host3 = "sparcmonitoring.com";

const char* host = "";
String machineID = "MACHINE-7";
byte statusKoneksi;

int area = 1;

const float FACTOR = 50; //50A/1V from teh CT
const float multiplier = 0.0000394;
int16_t currentRMS, currentDC, newCurrentDC;

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
const unsigned long checkTimeInterval = 10000; // 10 seconds in milliseconds

bool shiftDoneHandled = false;

bool functionsExecuted = false;
bool voltageBelowThreshold = false; // Flag to check if voltage was below 1V
unsigned long previousVoltageReadMillis = 0;  // Variable to store the last time the voltage was read
const unsigned long voltageReadInterval = 2000; // Interval to read voltage (2 seconds)

void connectToWiFi() {
  // Add the SSIDs and passwords for all networks
  wifiMulti.addAP(ssid1, password1);
  wifiMulti.addAP(ssid2, password2);
  wifiMulti.addAP(ssid3, password3);

  Serial.println("Connecting to WiFi...");

  // Continuously attempt to connect to WiFi networks
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("Trying to connect to WiFi...");
    displayMessage("Trying to connect to WiFi...");
    delay(5000); // Wait for the connection
  }

  // Check the connection result
  if (WiFi.SSID() == ssid1) {
    host = host1;
  } else if (WiFi.SSID() == ssid2) {
    host = host2;
  } else if (WiFi.SSID() == ssid3) {
    host = host3;
  } 

  Serial.println("WiFi connected");
  displayMessage("WiFi Connected");
}

void checkWeldID() {
  String startReading = "Start";

  Link = "https://" + String(host) + "/checkWeldID.php?State=" + startReading + "&MachineID=" + machineID;

  while (true) {
    http.begin(Link);
    httpCode = http.GET();

    if (httpCode == 200) {
      payload = http.getString();
      Serial.println("HTTP GET response: " + String(httpCode));
      
      payload.trim();

      Serial.println("Payload: " + payload);

      weldID = payload;
      http.end();
      break;  // Exit the loop if the response is successful
    } else {
      Serial.println("HTTP GET failed. Retrying...");
      delay(1000);  // Wait for 5 seconds before retrying
    }
  }
}

void httpGet()
{
  http.begin(Link);
  httpCode = http.GET();
  payload = http.getString();
  Serial.println("HTTP GET response: " + String(httpCode));
  Serial.println(payload);
  http.end();
}

void kirimDone() {
  Status = "Done";
  Mode = "";
  Link = "https://" + String(host) + "/kirimInfo2.php?UID=" + uid + "&Status=" + Status + "&Mode=" + Mode + "&Area=" + area
  + "&weldID=" + weldID + "&MachineID=" + machineID + "&Timing=" + jmdS;

  // Loop until a successful HTTP response is received
    http.begin(Link);
    httpCode = http.GET();

      payload = http.getString();
      Serial.println("HTTP GET response: " + String(httpCode));
      Serial.println(payload);
      http.end();
}

void sendLogin() {
  Status = "Active";
  Mode = "Login";

  Serial.println("First Tap");

  Link = "https://" + String(host) + "/kirimInfo2.php?UID=" + uid + "&Status=" + Status + "&Mode=" + Mode + "&Area=" 
  + area + "&weldID=" + weldID+ "&MachineID=" + machineID + "&Timing=" + jmdS;

  // Loop until a successful HTTP response is received
  while (true) {
    http.begin(Link);
    httpCode = http.GET();

    if (httpCode == 200) {
      payload = http.getString();
      Serial.println("HTTP GET response: " + String(httpCode));
      Serial.println(payload);
      http.end();
      break;  // Exit the loop if the response is successful
    } else {
      Serial.println("HTTP GET failed. Retrying...");
      delay(1000);  // Wait for 5 seconds before retrying
    }
  }
}

void sendLogout() {
  Status = "Active";
  Mode = "Logout";

  Link = "https://" + String(host) + "/kirimInfo2.php?UID=" + uid + "&Status=" + Status + "&Mode=" 
  + Mode + "&Area=" + area + "&weldID=" + weldID + "&MachineID=" + machineID + "&Timing=" + jmdS;

  // Loop until a successful HTTP response is received
  while (true) {
    http.begin(Link);
    httpCode = http.GET();

    if (httpCode == 200) {
      payload = http.getString();
      Serial.println("HTTP GET response: " + String(httpCode));
      Serial.println(payload);
      http.end();
      break;  // Exit the loop if the response is successful
    } else {
      Serial.println("HTTP GET failed. Retrying...");
      delay(1000);  // Wait for 1 seconds before retrying
    }
  }
}

void kirimAwal() {
  Status = "Inactive";
  Mode = "";
  Link = "https://" + String(host) + "/kirimInfo2.php?UID=" + uid + "&Status=" + Status + "&Mode=" + Mode + "&Area=" + area + 
  "&weldID=" + weldID + "&MachineID=" + machineID + "&Timing=" + jmdS;

    http.begin(Link);
    httpCode = http.GET();

      payload = http.getString();
      Serial.println("HTTP GET response: " + String(httpCode));
      Serial.println(payload);
      http.end();
}

void heartBeat() {
  String startReading = "heartBeat";

  Link = "https://" + String(host) + "/checkTimes.php?State=" + startReading + "&MachineID=" + machineID;

  while (true) {
    http.begin(Link);
    httpCode = http.GET();

    if (httpCode == 200) {
      payload = http.getString();
      Serial.println("HTTP GET response: " + String(httpCode));
      
      payload.trim();

      Serial.println("Payload: " + payload);

      currentTime = payload;
      http.end();
      break;  // Exit the loop if the response is successful
    } else {
      Serial.println("HTTP GET failed. Retrying...");
      delay(1000);  // Wait for 5 seconds before retrying
    }
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);

  pinMode(14, INPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  // Initialize voltage sensor pin
  pinMode(32, INPUT);

  // Connect to WiFi
  connectToWiFi();

  // Initialize RFID
  SPI.begin();
  rfid.PCD_Init();

  ads.setGain(GAIN_FOUR);      // +/- 1.024V 1bit = 0.5mV
  ads.begin();

  checkWeldID();

  kirimAwal();
  delay(1000);
  sendLogin();
}

void loop() {

  unsigned long currentMillis = millis();
  
  if (currentMillis - previousCheckTimeMillis >= checkTimeInterval) {
    heartBeat();
    previousCheckTimeMillis = currentMillis;
  }
}

void displayMessage(String message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(message);
  display.display();
}
