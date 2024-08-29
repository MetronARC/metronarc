#include <WiFi.h>

const char* ssid1 = "MetronARC";
const char* password1 = "2468g0a7";
const char* ssid2 = "ico";
const char* password2 = "19403054";

String area;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  scanAndConnect();
}

void loop() {
  // Keep the loop function empty
}

void scanAndConnect() {
  Serial.println("Scanning for WiFi networks...");
  int n = WiFi.scanNetworks();
  Serial.println("Scan complete");

  int strongestRSSI1 = -100;
  int strongestRSSI2 = -100;

  for (int i = 0; i < n; ++i) {
    if (WiFi.SSID(i) == ssid1) {
      strongestRSSI1 = WiFi.RSSI(i);
    } else if (WiFi.SSID(i) == ssid2) {
      strongestRSSI2 = WiFi.RSSI(i);
    }
  }

  Serial.print("RSSI for ");
  Serial.print(ssid1);
  Serial.print(": ");
  Serial.println(strongestRSSI1);

  Serial.print("RSSI for ");
  Serial.print(ssid2);
  Serial.print(": ");
  Serial.println(strongestRSSI2);

  if (strongestRSSI1 > strongestRSSI2) {
    connectToWiFi(ssid1, password1);
    area = "1";
  } else if (strongestRSSI2 > strongestRSSI1) {
    connectToWiFi(ssid2, password2);
    area = "2";
  } else {
    Serial.println("Both SSIDs have the same RSSI, connecting to the first one.");
    connectToWiFi(ssid1, password1);
    area = "1";
  }

  Serial.print("Connected to area: ");
  Serial.println(area);
}

void connectToWiFi(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
