#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;

String uid = "RONSTAN-1";
String weldID;
unsigned long arcInitiatedTime = 0;
bool arcInitiated = false;

float sumCurrentDC = 0;
int counterCurrentDC = 0;
bool initializationDone = false;
String currentTime;

float averageCurrentDC = 293.11;  // Variable to store the average currentDC during arc-on period
float voltageDC = 23.1;

// Define arrays to store values of averageCurrentDC and voltageDC
const int ARRAY_SIZE = 100; // Adjust the size as needed
float storedAverageCurrentDC[ARRAY_SIZE];
float storedVoltageDC[ARRAY_SIZE];
int storedIndex = 0;

//---- Website ----
#include <HTTPClient.h>

HTTPClient http;
String Link, LinkTiming, Mode, Status, payload;
int httpCode; 

const char *ssid1 = "ico";
const char *password1 = "19403054";
const char *host1 = "ronstan.sparcmonitoring.com";

const char *ssid2 = "MetronARC";
const char *password2 = "2468g0a7";
const char *host2 = "ronstan.sparcmonitoring.com";

const char *ssid3 = "MetronARCs Technology";
const char *password3 = "2468g0a7A7B7*";
const char *host3 = "ronstan.sparcmonitoring.com";

const char* host = "";
String machineID = "RONSTAN-1";
byte statusKoneksi;

int area = 1;

// Define the pins for the infrared sensors
const int sensorPin33 = 33;

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

// Flag to track if initial tasks have been performed in IDLE state
bool idleTasksPerformed = false;

// Flag to track if ArcOn has been sent and ArcOff needs to be sent
bool arcOnSent = false;

// Define a flag to track if kirimArcOn has been executed
bool arcOnExecuted = false;

unsigned long previousCheckTimeMillis = 0;
const unsigned long checkTimeInterval = 60000; // 10 seconds in milliseconds

void firstHeartBeat(){
  String startReading = "firstHeartBeat";
  Link = "https://" + String(host) + "/backEnd/checkFirst.php?State=" + startReading + "&MachineID=" + machineID + "&Area=" + area + "&weldID=" + weldID + "&UID=" + uid;

  int retryCount = 0;
  const int maxRetries = 10;  // Maximum number of retries before resetting the ESP32

    http.begin(Link);
    httpCode = http.GET();


      payload = http.getString();
      Serial.println("HTTP GET response: " + String(httpCode));
      
      payload.trim();

      Serial.println("Payload: " + payload);

      currentTime = payload;

}

void heartBeat() {
  String startReading = "heartBeat";

  Link = "https://" + String(host) + "/backEnd/checkTime.php?State=" + startReading + "&MachineID=" + machineID;

  while (true) {
    http.begin(Link);
    httpCode = http.GET();

    if (httpCode == 200) {
      payload = http.getString();
      Serial.println("HTTP GET response: " + String(httpCode));
      f
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

void checkWeldID() {
  String startReading = "Start";

  Link = "https://" + String(host) + "/backEnd/checkWeldID.php?State=" + startReading + "&MachineID=" + machineID;

    http.begin(Link);
    httpCode = http.GET();


      payload = http.getString();
      Serial.println("HTTP GET response: " + String(httpCode));
      
      payload.trim();

      Serial.println("Payload: " + payload);

      weldID = payload;
      http.end();
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
  Link = "https://" + String(host) + "/backEnd/kirimInfo2.php?UID=" + uid + "&Status=" + Status + "&Mode=" + Mode + "&Area=" + area + 
  "&weldID=" + weldID + "&MachineID=" + machineID + "&Timing=" + jmdS;

  // Loop until a successful HTTP response is received
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
  Link = "https://" + String(host) + "/backEnd/kirimInfo2.php?UID=" + uid + "&Status=" + Status + "&Mode=" + Mode + "&Area=" + area
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

  Link = "https://" + String(host) + "/backEnd/kirimInfo2.php?UID=" + uid + "&Status=" + Status + "&Mode=" + Mode + "&Area=" 
  + area + "&weldID=" + weldID+ "&MachineID=" + machineID + "&Timing=" + jmdS;

  // Loop until a successful HTTP response is received

    http.begin(Link);
    httpCode = http.GET();

      payload = http.getString();
      Serial.println("HTTP GET response: " + String(httpCode));
      Serial.println(payload);
      http.end();
}


void sendLogout() {
  Status = "Active";
  Mode = "Logout";

  Link = "https://" + String(host) + "/backEnd/kirimInfo2.php?UID=" + uid + "&Status=" + Status + "&Mode=" 
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
  Link = "https://" + String(host) + "/backEnd/kirimWaktu2.php?Timing=" + jmdS + "&Status=" + Status + "&MachineID=" + machineID +
         "&weldID=" + weldID + "&Area=" + area + "&UID=" + uid + "&Status=" + Status + "&currentDC=" + currentDC + "&Mode=" + Mode + "&voltageDC=" + voltageDC;

    http.begin(Link);
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
    Link = "https://" + String(host) + "/backEnd/kirimWaktu2.php?Timing=" + jmdS + "&Status=" + Status + "&MachineID=" + machineID + "&Area=" + area + 
           "&UID=" + uid + "&Status=" + Status + "&currentDC=" + storedAverageCurrentDC[i] + "&weldID=" + weldID + "&voltageDC=" + storedVoltageDC[i] 
           + "&StaticCurrentDC=" + averageCurrentDC + "&StaticVoltageDC=" + voltageDC;

      http.begin(Link);
      int httpCode = http.GET();

        payload = http.getString();
        Serial.println("HTTP GET response: " + String(httpCode));
        Serial.println(payload);
  }

  // Reset storedIndex to 0 to indicate the array is now empty
  storedIndex = 0;
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
  } else {
    Serial.println("WiFi failed to connect after several tries");
  }
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Set sensor pins as inputs
  pinMode(sensorPin33, INPUT);

  connectToWiFi();

  Serial.println("IDLE Mode");
  checkWeldID();
  kirimAwal();
  delay(1000);
  sendLogin();
  delay(1000);
  firstHeartBeat();
}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousCheckTimeMillis >= checkTimeInterval) {
    heartBeat();
    previousCheckTimeMillis = currentMillis;
  }
  
  if (digitalRead(sensorPin33) == LOW) {
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
    if (arcOnExecuted && digitalRead(sensorPin33) == HIGH) {
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
