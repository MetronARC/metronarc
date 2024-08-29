#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

String uid = "21125516825";
String weldID;
unsigned long arcInitiatedTime = 0;
bool arcInitiated = false;

float sumCurrentDC = 0;
int counterCurrentDC = 0;
bool initializationDone = false;

float averageCurrentDC = 293.11;  // Variable to store the average currentDC during arc-on period
float voltageDC = 23.1;

// Define arrays to store values of averageCurrentDC and voltageDC
const int ARRAY_SIZE = 100; // Adjust the size as needed
float storedAverageCurrentDC[ARRAY_SIZE];
float storedVoltageDC[ARRAY_SIZE];
int storedIndex = 0;

//---- Website ----
#include <ESP8266HTTPClient.h>

HTTPClient http;
WiFiClient wifiClient;  // Create a WiFiClient object
String Link, LinkTiming, Mode, Status, payload;
int httpCode;

const char *ssid1 = "Orbit 1";
const char *password1 = "2468g0a7";
const char *host1 = "sparcmonitoring.com";

const char *ssid2 = "MetronARCs Technologies";
const char *password2 = "19403054";
const char *host2 = "sparcmonitoring.com";

const char *ssid3 = "MetronARCs Technology";
const char *password3 = "2468g0a7A7B7*";
const char *host3 = "sparcmonitoring.com";

const char* host = "";
String machineID = "MACHINE-2";
byte statusKoneksi;

int area = 2;

// Define the pin for the infrared sensor
const int sensorPinD1 = 5; // D1 corresponds to GPIO5 on the ESP8266

const float FACTOR = 50; // 50A/1V from the CT
const float multiplier = 0.0000394;
int16_t currentRMS, currentDC;

long millisSebelum;
byte detik, menit, jam;
String detikS, menitS, jamS, jmdS;
byte statusWaktu;
String statusT;

unsigned long previousMillis = 0;
const int period = 1;

// Flag to track if initial tasks have been performed in IDLE state
bool idleTasksPerformed = false;

// Flag to track if ArcOn has been sent and ArcOff needs to be sent
bool arcOnSent = false;

// Define a flag to track if kirimArcOn has been executed
bool arcOnExecuted = false;

void checkWeldID() {
  String startReading = "Start";

  Link = "https://" + String(host) + "/proclad/checkWeldID.php?State=" + startReading + "&MachineID=" + machineID;

  while (true) {
    http.begin(wifiClient, Link);
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
      delay(1000);  // Wait for 1 second before retrying
    }
  }
}

void httpGet()
{
  http.begin(wifiClient, Link);
  httpCode = http.GET();
  payload = http.getString();
  Serial.println("HTTP GET response: " + String(httpCode));
  Serial.println(payload);
  http.end();
}

void kirimAwal() {
  Status = "Inactive";
  Mode = "";
  Link = "https://" + String(host) + "/proclad/kirimInfo2.php?UID=" + uid + "&Status=" + Status + "&Mode=" + Mode + "&Area=" + area + 
  "&weldID=" + weldID + "&MachineID=" + machineID + "&Timing=" + jmdS;

  // Loop until a successful HTTP response is received
  while (true) {
    http.begin(wifiClient, Link);
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

void kirimDone() {
  Status = "Done";
  Mode = "";
  Link = "https://" + String(host) + "/proclad/kirimInfo2.php?UID=" + uid + "&Status=" + Status + "&Mode=" + Mode + "&Area=" + area
  + "&weldID=" + weldID + "&MachineID=" + machineID + "&Timing=" + jmdS;

  // Loop until a successful HTTP response is received
  while (true) {
    http.begin(wifiClient, Link);
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

void sendLogin() {
  Status = "Active";
  Mode = "Login";

  Serial.println("First Tap");

  Link = "https://" + String(host) + "/proclad/kirimInfo2.php?UID=" + uid + "&Status=" + Status + "&Mode=" + Mode + "&Area=" 
  + area + "&weldID=" + weldID+ "&MachineID=" + machineID + "&Timing=" + jmdS;

  // Loop until a successful HTTP response is received
  while (true) {
    http.begin(wifiClient, Link);
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


void sendLogout() {
  Status = "Active";
  Mode = "Logout";

  Link = "https://" + String(host) + "/proclad/kirimInfo2.php?UID=" + uid + "&Status=" + Status + "&Mode=" 
  + Mode + "&Area=" + area + "&weldID=" + weldID + "&MachineID=" + machineID + "&Timing=" + jmdS;

  // Loop until a successful HTTP response is received
  while (true) {
    http.begin(wifiClient, Link);
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

void kirimArcOn() {
  Status = "ArcOn";
  Link = "https://" + String(host) + "/proclad/kirimWaktu2.php?Timing=" + jmdS + "&Status=" + Status + "&MachineID=" + machineID +
         "&weldID=" + weldID + "&Area=" + area + "&UID=" + uid + "&Status=" + Status + "&currentDC=" + currentDC + "&Mode=" + Mode + "&voltageDC=" + voltageDC;

  http.begin(wifiClient, Link);
  int httpCode = http.GET();

  payload = http.getString();
  Serial.println("HTTP GET response: " + String(httpCode));
  Serial.println(payload);

  // Set arcOnSent flag to true
  arcOnSent = true;
}

void kirimArcOff() {
  Status = "ArcOff";
  
  // Include the stored values of averageCurrentDC and voltageDC in the request
  for (int i = 0; i < storedIndex; i++) {
    Link = "https://" + String(host) + "/proclad/kirimWaktu2.php?Timing=" + jmdS + "&Status=" + Status + "&MachineID=" + machineID + "&Area=" + area + 
           "&UID=" + uid + "&Status=" + Status + "&currentDC=" + storedAverageCurrentDC[i] + "&weldID=" + weldID + "&voltageDC=" + storedVoltageDC[i];
    http.begin(wifiClient, Link);
    int httpCode = http.GET();
    payload = http.getString();
    Serial.println("HTTP GET response: " + String(httpCode));
    Serial.println(payload);
    http.end();
  }
  
  // Reset the storedIndex after sending the data
  storedIndex = 0;
}

void connectToWiFi() {
  WiFi.mode(WIFI_STA);

  Serial.println("Connecting to WiFi...");

  // Attempt to connect to each WiFi network
  WiFi.begin(ssid1, password1);
  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    host = host1;
    Serial.println("Connected to " + String(ssid1));
    return;
  }

  WiFi.begin(ssid2, password2);
  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    host = host2;
    Serial.println("Connected to " + String(ssid2));
    return;
  }

  WiFi.begin(ssid3, password3);
  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    host = host3;
    Serial.println("Connected to " + String(ssid3));
    return;
  }

  Serial.println("Failed to connect to WiFi");
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Set sensor pin as input
  pinMode(sensorPinD1, INPUT);

  connectToWiFi();

  Serial.println("IDLE Mode");
  checkWeldID();
  kirimAwal();
  delay(1000);
  sendLogin();
}

void loop() {
  if (digitalRead(sensorPinD1) == LOW) {
    // If kirimArcOn hasn't been executed yet
    if (!arcOnExecuted) {
      // Print "Arc On" and execute kirimArcOn
      Serial.println("Arc On");
      kirimArcOn();
      // Set arcOnExecuted to true
      arcOnExecuted = true;
    }

    // Store averageCurrentDC and voltageDC every 3 seconds
    if (millis() - previousMillis >= 3000) {
      // Store the values in the arrays
      storedAverageCurrentDC[storedIndex] = averageCurrentDC;
      storedVoltageDC[storedIndex] = voltageDC;
      storedIndex++; // Increment the index
      // Reset the timer
      previousMillis = millis();
    }

    // Print "6 seconds warning" every 6 seconds
    if ((millis() - arcInitiatedTime) >= 6000) {
      Serial.println("6 seconds warning");
      kirimArcOff();
      arcInitiatedTime = millis(); // Reset the timer
    }
  } else {
    // If kirimArcOn has been executed and the state changes to HIGH
    if (arcOnExecuted && digitalRead(sensorPinD1) == HIGH) {
      // Print the stored values before initiating kirimArcOff
      Serial.println("Stored Values before Arc Off:");
      for (int i = 0; i < storedIndex; i++) {
        Serial.print("Index ");
        Serial.print(i);
        Serial.print(": averageCurrentDC = ");
        Serial.print(storedAverageCurrentDC[i]);
        Serial.print(", voltageDC = ");
        Serial.println(storedVoltageDC[i]);
      }

      // Print "Arc Off" and execute kirimArcOff
      Serial.println("Arc Off");
      kirimArcOff();
      // Reset arcOnExecuted flag
      arcOnExecuted = false;
    }
  }
}
