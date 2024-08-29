#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;

//---- Website ----
#include <HTTPClient.h>

HTTPClient http;
String Link, LinkTiming, Mode, Status, payload;
int httpCode; 

const char *ssid1 = "Area 1";
const char *password1 = "19403054";
const char *host1 = "sparcmonitoring.com";

const char *ssid2 = "ico";
const char *password2 = "19403054";
const char *host2 = "sparcmonitoring.com";

const char* host = "";

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define I2C_SDA 32 // change as needed
#define I2C_SCL 33 // change as needed
#define OLED_ADDR 0x3C
#define GPS_TX 35   // connect to the TX pin of Neo-7M
#define GPS_RX 34   // connect to the RX pin of Neo-7M

SoftwareSerial gpsSerial(GPS_TX, GPS_RX);
TinyGPSPlus gps;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

float Latitude = 0.0;
float Longitude = 0.0;
bool positionAcquired = false; // Flag to check if position has been acquired
unsigned long timeToAcquire = 0; // Variable to store the time taken to acquire the position

void connectToWiFi() {
  // Add the SSIDs and passwords for both networks
  wifiMulti.addAP(ssid1, password1);
  wifiMulti.addAP(ssid2, password2);

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
    }

    Serial.println("WiFi connected");
    displayMessage("WiFi Connected");
  } else {
    Serial.println("WiFi failed to connect after several tries");
    displayMessage("WiFi not Connected after several retries");
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

void kirimGPS() {
  Status = "checkGPS";
  // Store the Latitude and Longitude values in separate variables
  String latitudeString = String(Latitude, 6);
  String longitudeString = String(Longitude, 6);
  // Construct the Link with only "Latitude" and "Longitude"
  Link = "https://" + String(host) + "/checkGPS.php?Latitude=" + latitudeString + "&Longitude=" + longitudeString + "&Status=" + Status;

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

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600);
  Wire.begin(I2C_SDA, I2C_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  display.display(); // Clear the display buffer
  display.clearDisplay(); // Clear the display content

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  connectToWiFi();
}

void loop() {
  // Read GPS data
  if (!positionAcquired && gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      display.clearDisplay();
      display.setCursor(0, 0);
      Latitude = gps.location.lat();
      Longitude = gps.location.lng();

      // Check if the position is acquired for the first time
      if (Latitude > 0 && Longitude > 0 && !positionAcquired) {
        positionAcquired = true;

        // Store the current millis() value when position is acquired
        unsigned long positionAcquiredTime = millis();

        // Calculate the time taken to acquire the position in seconds
        timeToAcquire = (positionAcquiredTime - timeToAcquire) / 1000;

        // Display the time taken on OLED
        display.print(F("Position Acquired in: "));
        display.print(timeToAcquire);
        display.println(F(" seconds"));
        display.display();
        
        // Print to Serial Monitor
        Serial.print(F("Position Acquired in: "));
        Serial.print(timeToAcquire);
        Serial.println(F(" seconds"));

        // Call the function to send GPS data
        kirimGPS();
      }
    }
  }

  // Your other main code can go here if needed
}
