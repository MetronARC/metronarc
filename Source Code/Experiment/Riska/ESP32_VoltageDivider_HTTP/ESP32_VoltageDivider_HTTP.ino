#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;

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

const char *ssid3 = "MetronARC Technologies";
const char *password3 = "19403054";
const char *host3 = "sparcmonitoring.com";

const char* host = "";

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

void kirimAwal() {
  Link = "https://" + String(host) + "/kirimVoltage.php?Status=" + Status;

  httpGet();
}

void kirimUpdate() {
  Link = "https://" + String(host) + "/kirimVoltage2.php?voltageAverage=" + String(voltageAverage);
  // Loop until a successful HTTP response is received
  httpGet();
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

void setup() {
  Serial.begin(115200);
  delay(500);

  connectToWiFi();
  delay(100);
  kirimAwal();
}

void loop() {
  readVoltage();
  Serial.print("Vin: ");
  Serial.print(voltageMillivolts);
  Serial.println("mV");

  Serial.print("V Avg: ");
  Serial.println(getVoltageAverage());
  kirimUpdate();
  delay(1000);
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
