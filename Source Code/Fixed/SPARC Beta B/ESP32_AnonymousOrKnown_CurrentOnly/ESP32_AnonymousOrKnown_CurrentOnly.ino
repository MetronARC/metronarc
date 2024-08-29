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
String uid = "MACHINE-4";
String weldID;
unsigned long arcInitiatedTime = 0;
bool arcInitiated = false;

float sumCurrentDC = 0;
int counterCurrentDC = 0;
bool initializationDone = false;

float averageCurrentDC = 0.0;  // Variable to store the average currentDC during arc-on period

const char *ssid1 = "Orbit 1";
const char *password1 = "2468g0a7";
const char *host1 = "sparcmonitoring.com";

const char *ssid2 = "MetronARC Technology";
const char *password2 = "2468g0a7A7B7*";
const char *host2 = "sparcmonitoring.com";

const char *ssid3 = "ico";
const char *password3 = "19403054";
const char *host3 = "sparcmonitoring.com";

const char* host = "";
String machineID = "MACHINE-4";
byte statusKoneksi;

int area = 2;

const float FACTOR = 50; //50A/1V from teh CT
const float multiplier = 0.0000394;
int16_t currentRMS, currentDC;

long millisSebelum;
byte detik, menit, jam;
String detikS, menitS, jamS, jmdS;
byte statusWaktu;
String statusT;

unsigned long previousMillis = 0;
const int period = 1;

float getCurrent() {
  float voltage;
  float current;
  float sum = 0;
  int counter = 0;

  for (int i = 0; i < 10; i++) { //take 1000 samples
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

void kirimDone() {
  Status = "Done";
  Mode = "";
  Link = "https://" + String(host) + "/kirimInfo2.php?UID=" + uid + "&Status=" + Status + "&Mode=" + Mode + "&Area=" + area
  + "&weldID=" + weldID + "&MachineID=" + machineID + "&Timing=" + jmdS;

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

void kirimArcOn() {
  Status = "ArcOn";
  Link = "https://" + String(host) + "/kirimWaktu2.php?Timing=" + jmdS + "&Status=" + Status + "&MachineID=" + machineID +
         "&weldID=" + weldID + "&Area=" + area + "&UID=" + uid + "&Status=" + Status + "&currentDC=" + currentDC + "&Mode=" + Mode;

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
      delay(1000);  // Wait for 1 second before retrying
    }
  }
}

void kirimArcOff() {
  Status = "ArcOff";
  
  // Include the average CurrentDC in the request
  Link = "https://" + String(host) + "/kirimWaktu2.php?Timing=" + jmdS + "&Status=" + Status + "&MachineID=" + machineID + "&Area=" + area + 
         "&UID=" + uid + "&Status=" + Status + "&currentDC=" + averageCurrentDC + "&weldID=" + weldID;

  while (true) {
    http.begin(Link);
    int httpCode = http.GET();

    if (httpCode == 200) {
      payload = http.getString();
      Serial.println("HTTP GET response: " + String(httpCode));
      Serial.println(payload);
      http.end();
      break;  // Exit the loop if the response is successful
    } else {
      Serial.println("HTTP GET failed. Retrying...");
      delay(1000);  // Wait for 1 second before retrying
    }
  }
  // Reset averageCurrentDC for the next arc-on period
  averageCurrentDC = 0.0;
}

void printMeasure(String prefix, float value, String postfix)
{
  Serial.print(prefix);
  Serial.print(value, 3);
  Serial.println(postfix);
}

void syaratTiming() {
  currentRMS = getCurrent();
  currentDC = (14.109 * currentRMS) + 6.3933;

  if (currentRMS > 1) {
    if (!arcInitiated) {
      // If the arc is not initiated, set the flag and send ArcOn
      Serial.println("Arc initiated");
      kirimArcOn();
      arcInitiated = true;
      arcDetected = true;
      arcInitiatedTime = millis(); // Record the time when the arc is initiated
    }

    sumCurrentDC += currentDC;
    counterCurrentDC++;

  } else {
    if (arcInitiated) {
      unsigned long elapsedTime = millis() - arcInitiatedTime;
      if (elapsedTime > 1000) { // Check if 5 seconds have passed
        Serial.println("Arc stopped");
        arcInitiated = false; // Reset the arc initiated flag
        
        // Calculate the average currentDC  
        if (counterCurrentDC > 0) {
          averageCurrentDC = sumCurrentDC / counterCurrentDC;
          Serial.print("Average CurrentDC: ");
          Serial.println(averageCurrentDC);
          kirimArcOff();
        }

        // Reset the sumCurrentDC and counterCurrentDC for the next arc-on period
        sumCurrentDC = 0;
        counterCurrentDC = 0;
      }
    }
  }
}

void displayMessage(String message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(message);
  display.display();
}

void connectToWiFi() {
  // Add the SSIDs and passwords for both networks
  wifiMulti.addAP(ssid1, password1);
  wifiMulti.addAP(ssid2, password2);
  wifiMulti.addAP(ssid3, password3);

  int attempts = 0;
  const int maxAttempts = 5;

  Serial.println("Connecting to WiFi...");

  // Attempt to connect to WiFi networks
  while (wifiMulti.run() != WL_CONNECTED && attempts < maxAttempts) {
    Serial.println("Trying to connect to WiFi...");
    displayMessage("Trying to connect to WiFi...");
    delay(5000); // Wait for the connection
    attempts++;
  }

  // Check the connection result
  if (wifiMulti.run() == WL_CONNECTED) {
    if (WiFi.SSID() == ssid1) {
      host = host1;
    } else if (WiFi.SSID() == ssid2) {
      host = host2;
    } else if (WiFi.SSID() == ssid3) {
      host = host3;
    } 

    Serial.println("WiFi connected");
    displayMessage("WiFi Connected");
  } else {
    Serial.println("WiFi failed to connect after several tries");
    displayMessage("WiFi not Connected after several retries");
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
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
  // Reset flags
  arcDetected = false;
  uidRead = false;

  unsigned long currentMillis = millis();

  currentRMS = getCurrent();
  currentDC = (14.109 * currentRMS) + 6.3933;

  // Look for new cards
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    // Store the UID to the variable
    uid = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      uid += String(rfid.uid.uidByte[i]);
    }
    
    uidRead = true;
    sendLogin();
    displayMessage("Welcome, Welder");

    // Stop if an UID was read
    rfid.PICC_HaltA();
  }

  // Check if arc is detected
  syaratTiming();

  // Act based on conditions
  if (arcDetected) {
    if (uidRead) {
      // Print UID and message for known welder
      Serial.print("Known welder ");
      Serial.print(uid);
      Serial.println(", transmitting signal");
      displayMessage("Status: Welding, Known Welder");
    } else {
      // Print message for anonymous welder
      Serial.println("Anonymous Welder, transmitting signal");
      displayMessage("Status: Welding, MACHINE-4");
    }
    // Reset flags
    arcDetected = false;
    uidRead = false;
    
    // Reset the flag once arc is detected
    arcTimeoutHandled = false;
  } else {
    // No arc detected, check if 30 seconds have passed and the condition hasn't been handled yet
    if (!arcTimeoutHandled && currentMillis - arcInitiatedTime > 30000) {
      Serial.println("No Arc has been detected for the last 30 seconds");
      displayMessage("No Arc has been detected for the last 30 seconds");
      sendLogout();
      kirimDone();
      arcTimeoutHandled = true; // Set the flag to indicate that the condition has been handled
    }
    // Check if currentRMS is greater than 1 and uidRead is false
    else if (currentRMS > 1 && !uidRead) {
      // Display the message on the OLED
      displayMessage("Status: Welding, MACHINE-4");
    }
    else if (currentRMS < 1 && !uidRead) {
      // Display the message on the OLED
      displayMessage("Status: Not Welding");
    }
  }
}
