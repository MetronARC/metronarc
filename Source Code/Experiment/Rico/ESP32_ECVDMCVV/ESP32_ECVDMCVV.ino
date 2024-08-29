#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ADS1X15.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;

String uid;
String weldID;
unsigned long arcInitiatedTime = 0;
bool arcInitiated = false;

float sumCurrentDC = 0;
int counterCurrentDC = 0;
bool initializationDone = false;

float averageCurrentDC = 0.0;  // Variable to store the average currentDC during arc-on period

//---- Website ----
#include <HTTPClient.h>

HTTPClient http;
String Link, LinkTiming, Mode, Status, payload;
int httpCode; 

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define I2C_SDA 16 // ganti sesukamu
#define I2C_SCL 17 // ganti sesukamu
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(128, 64, &Wire, -1);

#define SS_PIN 21     // Change these pins as needed
#define RST_PIN 22    // Change these pins as needed
MFRC522 rfid(SS_PIN, RST_PIN);

const int BUTTON_PING = 27; // Ping button pin
const int WiFi_LED = 32; // LED WiFi

const char *ssid1 = "Orbit 1";
const char *password1 = "2468g0a7";
const char *host1 = "sparcmonitoring.com";

const char *ssid2 = "Orbit 2";
const char *password2 = "2468g0a7";
const char *host2 = "sparcmonitoring.com";

const char *ssid3 = "ico";
const char *password3 = "19403054";
const char *host3 = "sparcmonitoring.com";

const char* host = "";
String machineID = "MACHINE-1";
byte statusKoneksi;

int area = 0;
bool areaConfirmed = false;

String previousUID = "";
unsigned long tapMillis = 0;
const int tapTimeout = 2000; // 2 seconds

Adafruit_ADS1115 ads;

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

//Voltage Divider
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

//Arrays for storing multiple current and voltage
const int ARRAY_SIZE = 100; // Adjust the size as needed
float storedAverageCurrentDC[ARRAY_SIZE];
float storedVoltageDC[ARRAY_SIZE];
int storedIndex = 0;

enum State {
  WIFI_CHECK,
  ASSIGN_AREA,
  IDLE,
  LOGIN,
  LOGOUT,
  UID_UNKNOWN
};

State currentState = WIFI_CHECK;

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

void readVoltage() {
  analogValue = analogRead(ANALOG_PIN);
  voltageMillivolts = analogValue + 100;
  
  float sensedVoltage = voltageMillivolts * (ESPVCC / (float)INPUT_RESOLUTION); 
  voltage = sensedVoltage / ((float)VALUE_R2 / ((float)VALUE_R1 + (float)VALUE_R2)); 
  calibration = (1.0447 * voltage) + 1.2885;
}

float getVoltageAverage() {
  float tempAverageVoltage = 0;
  for (int i = 0; i < AVERAGE_OF; i++) {
    readVoltage();
    tempAverageVoltage += calibration;
    voltageAverage = tempAverageVoltage / AVERAGE_OF;
  }
  return voltageAverage;
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
  
  // Include the stored values of averageCurrentDC and voltageDC in the request
  for (int i = 0; i < storedIndex; i++) {
    Link = "https://" + String(host) + "/kirimWaktu2.php?Timing=" + jmdS + "&Status=" + Status + "&MachineID=" + machineID + "&Area=" + area + 
           "&UID=" + uid + "&Status=" + Status + "&currentDC=" + storedAverageCurrentDC[i] + "&weldID=" + weldID + "&voltageDC=" + storedVoltageDC[i] 
           + "&StaticCurrentDC=" + averageCurrentDC + "&StaticVoltageDC=" + voltageAverage;

      http.begin(Link);
      int httpCode = http.GET();

        payload = http.getString();
        Serial.println("HTTP GET response: " + String(httpCode));
        Serial.println(payload);
  }

  // Reset storedIndex to 0 to indicate the array is now empty
  storedIndex = 0;
  averageCurrentDC = 0.0;
}


void sendPing() {
  Link = "https://" + String(host) + "/receivePing.php?UID=" + uid + "&Area=" + area + "&MachineID=" + machineID;

  // Loop until a successful HTTP response is received
  httpGet();
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
      arcInitiatedTime = millis(); // Record the time when the arc is initiated
    }

    if (millis() - previousMillis >= 3000) {
          // Store the values in the arrays
          storedAverageCurrentDC[storedIndex] = averageCurrentDC;
          storedVoltageDC[storedIndex] = voltageAverage;
          storedIndex++; // Increment the index
          // Reset the timer
          previousMillis = millis();
        }

    // Accumulate the currentDC values for averaging
    sumCurrentDC += currentDC;
    counterCurrentDC++;
  } else {
    if (arcInitiated) {
      unsigned long elapsedTime = millis() - arcInitiatedTime;
      if (elapsedTime > 1000) { // Check if 1 seconds have passed
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



void assignArea() {
  if (!areaConfirmed) {
    handleButtonPress();
  } else {
    // Disable the buttons
    pinMode(BUTTON_PING, INPUT);

    displayMessage("Idle mode");
    currentState = IDLE;
  }
}

unsigned long countdownStartTime = 0;
const unsigned long countdownDuration = 10000;

void handleButtonPress() {
  if (digitalRead(BUTTON_PING) == 1) {
    if (area == 4) {
      area = 1;
    } else {
      area++;
    }
    display.clearDisplay();
    area = 1;
    displayMessage("Area Input initialized. Current area: " + String(area));
    Serial.println("Area Input initialized. Current area: " + String(area));
    delay(500);
    display.clearDisplay();
    displayMessage("Starting countdown...");
    Serial.println("Starting countdown...");
    countdownStartTime = millis();
    while (millis() - countdownStartTime < countdownDuration) {
      int remainingTime = (countdownDuration - (millis() - countdownStartTime)) / 1000 + 1;
      display.clearDisplay();
      displayMessage("Countdown: " + String(remainingTime) + " Area: " + String(area));
      Serial.println("Countdown: " + String(remainingTime) + " Area: " + String(area));
      if (digitalRead(BUTTON_PING) == 1) {
        if (area == 4) {
          area = 1;
        } else {
          area++;
        }
        display.clearDisplay();
        displayMessage("Area incremented. Current area: " + String(area));
        Serial.println("Area incremented. Current area: " + String(area));
        delay(500);
      }
    }
    areaConfirmed = true;
    display.clearDisplay();
    displayMessage("Area confirmed. Confirmed area: " + String(area));
    Serial.println("Area confirmed. Confirmed area: " + String(area));
    checkWeldID();
    kirimAwal();
    delay(3000);
  }
}


void waitForTap() {
  if (!initializationDone) {
    initializationDone = true;
  }

  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    uid = getUID();
    if (uid != previousUID) {
      currentState = LOGIN;
      // Reset the initialization flag when transitioning to LOGIN state
      initializationDone = false;
    } else {
      currentState = UID_UNKNOWN;
    }
  }
}

String getUID() {
  uid = "";
  rfid.PICC_HaltA();
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i]);
  }
  return uid;
}

void checkRFID() {
  if (statusKoneksi == 1) {
    Link = "http://" + String(host) + "/SPARK/cekKartu.php?UID=" + uid;
    httpGet();

    // Trim leading and trailing whitespaces from the payload
    payload.trim();

    if (payload == "OK") {
      // Continue to Login mode
      currentState = LOGIN;
    } else {
      // Display on OLED and stop from going to Login mode
      displayMessage("UID not recognized on database");
    }
  }
}


void loginMode() {
  static bool modeLogged = false;  // To track if "Mode Login" has been printed
  String uid = getUID();

  // Print "Mode Login" only once
  if (!modeLogged) {
    sendLogin();
    modeLogged = true;
  }

  // Print "Timing" in a loop
  while (currentState == LOGIN) {
    syaratTiming();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("UID: ");
    display.print(uid);
    display.setCursor(0, 10);
    display.print("Machine ID: ");
    display.print(machineID);
    display.setCursor(0, 20);
    display.print("Status: ");
    display.print(statusT);
    display.setCursor(0, 30);
    display.print("AC Current: ");
    display.print(currentRMS);
    display.setCursor(0, 40);
    display.print("DC Current: ");
    display.print(currentDC);
    display.setCursor(0, 50);
    display.print("DC Voltage: ");
    display.print(voltageAverage);
    display.display();
    syaratTiming();

    if (digitalRead(BUTTON_PING) == HIGH && currentState == LOGIN) {
      display.clearDisplay();
      sendPing();
      displayMessage("Ping initiated");
      delay(2000);
    }

    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      // Get the new UID
      String newUID = getUID();
      
      // Update the current UID to the new UID
      uid = newUID;

      // Change the state to LOGOUT
      currentState = LOGOUT;
    }
  }
}


void logoutMode() {
  static bool modeLogged = false;  // To track if "Logout Mode" has been printed
  displayMessage("Logout mode");

  // Print "Logout Mode" only once
  if (!modeLogged) {
    sendLogout();
    kirimDone();
    modeLogged = true;
  }

  delay(2000); // Wait for 2 seconds
  previousUID = "";
  currentState = IDLE;
  initializationDone = false;

  // Reset the ESP32
  ESP.restart();
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
    digitalWrite(WiFi_LED, HIGH);
    currentState = ASSIGN_AREA;
  } else {
    Serial.println("WiFi failed to connect after several tries");
    displayMessage("WiFi not Connected after several retries");
    currentState = WIFI_CHECK;
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

  // Set button pins as input
  pinMode(BUTTON_PING, INPUT);
  pinMode(WiFi_LED, OUTPUT);

  ads.setGain(GAIN_FOUR);      // +/- 1.024V 1bit = 0.5mV
  ads.begin();

}

bool inputAreaDisplayed = false;

void loop() {
  switch (currentState) {
    case WIFI_CHECK:
      // Check WiFi connection
      if (WiFi.status() == WL_CONNECTED) {
        // Clear the OLED screen when WiFi is connected
        display.clearDisplay();
        display.display();

        displayMessage("WiFi Connected");
        delay(2000);
        statusKoneksi = 1;

        currentState = ASSIGN_AREA;
      } else {
        connectToWiFi();
      }
      break;

    case ASSIGN_AREA:
      area = 1; // Initialize area to 1
      if (!inputAreaDisplayed) {
          displayMessage("Input your Area");

          // Set the flag to true to indicate that it has been displayed
          inputAreaDisplayed = true;
        }
      assignArea();
      break;

    case IDLE:
      displayMessage("Idle mode");
      waitForTap();
      break;

    case LOGIN:
      loginMode();
      break;

    case LOGOUT:
      logoutMode();
      break;

    case UID_UNKNOWN:
      displayMessage("UID unknown, please try again");
      delay(2000); // Wait for 2 seconds
      currentState = IDLE;
      break;
  }
}
