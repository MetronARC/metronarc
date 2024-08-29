#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;

String uid = "21125516825";
String weldID;
unsigned long arcInitiatedTime = 0;
bool arcInitiated = false;

float sumCurrentDC = 0;
int counterCurrentDC = 0;
bool initializationDone = false;

float averageCurrentDC = 293.11;  // Variable to store the average currentDC during arc-on period

float voltageDC = 23.1;

//---- Website ----
#include <HTTPClient.h>

HTTPClient http;
String Link, LinkTiming, Mode, Status, payload;
int httpCode; 

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
String machineID = "MACHINE-2";
byte statusKoneksi;

int area = 2;

// Define the pins for the infrared sensors
const int sensorPin32 = 32;
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

// Define states for the different sessions
enum SessionState {
  IDLE,
  LOGIN,
  LOGOUT
};

// Initialize session state to IDLE
SessionState currentState = IDLE;

// Flag to track if initial tasks have been performed in IDLE state
bool idleTasksPerformed = false;

// Flag to track if ArcOn has been sent and ArcOff needs to be sent
bool arcOnSent = false;

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
         "&weldID=" + weldID + "&Area=" + area + "&UID=" + uid + "&Status=" + Status + "&currentDC=" + currentDC + "&Mode=" + Mode + "&voltageDC=" + voltageDC;

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
  // Set arcOnSent flag to true
  arcOnSent = true;
}

void kirimArcOff() {
  Status = "ArcOff";
  
  // Include the average CurrentDC in the request
  Link = "https://" + String(host) + "/kirimWaktu2.php?Timing=" + jmdS + "&Status=" + Status + "&MachineID=" + machineID + "&Area=" + area + 
         "&UID=" + uid + "&Status=" + Status + "&currentDC=" + averageCurrentDC + "&weldID=" + weldID + "&voltageDC=" + voltageDC;

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
  pinMode(sensorPin32, INPUT);
  pinMode(sensorPin33, INPUT);

  connectToWiFi();
}

// Define a flag to track if kirimArcOn has been executed
bool arcOnExecuted = false;

void loop() {
  // Check the current state
  switch (currentState) {
    case IDLE:
      if (!idleTasksPerformed) {
        // Perform initial tasks only if not already performed
        // Print "IDLE Mode" once
        Serial.println("IDLE Mode");
        checkWeldID();
        kirimAwal();
        idleTasksPerformed = true; // Set the flag to true
      }
      
      // Check if sensor 32 is triggered
      if (digitalRead(sensorPin32) == LOW) {
        // Move to LOGIN state
        currentState = LOGIN;
        // Print "Login Mode" once
        Serial.println("Login Mode");
        sendLogin();
      }
      break;

    case LOGIN:
      // Check if sensor 33 is triggered
      if (digitalRead(sensorPin33) == LOW) {
        // If kirimArcOn hasn't been executed yet
        if (!arcOnExecuted) {
          // Print "Arc On" and execute kirimArcOn
          Serial.println("Arc On");
          kirimArcOn();
          // Set arcOnExecuted to true
          arcOnExecuted = true;
        }
      } else {
        // If kirimArcOn has been executed and the state changes to HIGH
        if (arcOnExecuted && digitalRead(sensorPin33) == HIGH) {
          // Print "Arc Off" and execute kirimArcOff
          Serial.println("Arc Off");
          kirimArcOff();
          // Reset arcOnExecuted flag
          arcOnExecuted = false;
        }
      }
      
      // Check if sensor 32 is triggered again
      if (digitalRead(sensorPin32) == LOW) {
        // Move to LOGOUT state
        currentState = LOGOUT;
        // Print "Logout Mode" once
        Serial.println("Logout Mode");
        sendLogout();
      }
      break;

    case LOGOUT:
      // Reset state to IDLE
      currentState = IDLE;
      kirimDone();
      // Reset the flag so that initial tasks can be performed again in IDLE state
      idleTasksPerformed = false;
      break;
  }
  
  // Delay for stability
  delay(100);
}
