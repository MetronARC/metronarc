#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define I2C_SDA 32 // change as needed
#define I2C_SCL 33 // change as needed
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Define the RX and TX pins for the GPS module
#define GPS_RX_PIN    35
#define GPS_TX_PIN    34

// Define geofence parameters
#define GEOFENCE_OFFICE_LATITUDE  1.1023856
#define GEOFENCE_OFFICE_LONGITUDE 104.0336899
#define GEOFENCE_OFFICE_RADIUS    100

#define GEOFENCE_HOME_LATITUDE    1.127517
#define GEOFENCE_HOME_LONGITUDE   104.012899
#define GEOFENCE_HOME_RADIUS      100

#define GEOFENCE_NOV_LATITUDE     1.184392
#define GEOFENCE_NOV_LONGITUDE    104.006504
#define GEOFENCE_NOV_RADIUS       100

TinyGPSPlus gps;
SoftwareSerial gpsSerial(GPS_RX_PIN, GPS_TX_PIN);

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
}

void loop() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);

      // Print to OLED
      display.print(F("Latitude: "));
      display.println(String(gps.location.lat(), 6));

      display.print(F("Longitude: "));
      display.println(String(gps.location.lng(), 6));

      // Print to Serial
      Serial.print(F("Latitude: "));
      Serial.println(gps.location.lat(), 6);

      Serial.print(F("Longitude: "));
      Serial.println(gps.location.lng(), 6);

      // Check geofences
      checkGeofence("At the Office", GEOFENCE_OFFICE_LATITUDE, GEOFENCE_OFFICE_LONGITUDE, GEOFENCE_OFFICE_RADIUS);
      checkGeofence("At home", GEOFENCE_HOME_LATITUDE, GEOFENCE_HOME_LONGITUDE, GEOFENCE_HOME_RADIUS);
      checkGeofence("At NOV", GEOFENCE_NOV_LATITUDE, GEOFENCE_NOV_LONGITUDE, GEOFENCE_NOV_RADIUS);

      display.display();
    }
  }
}

void checkGeofence(const char* locationName, double geofenceLatitude, double geofenceLongitude, double geofenceRadius) {
  // Calculate distance between current location and geofence center
  double distance = TinyGPSPlus::distanceBetween(
    gps.location.lat(), gps.location.lng(),
    geofenceLatitude, geofenceLongitude
  );

  // Check if inside the geofence
  if (distance <= geofenceRadius) {
    Serial.println(locationName);
    display.println(locationName);
  }
}
