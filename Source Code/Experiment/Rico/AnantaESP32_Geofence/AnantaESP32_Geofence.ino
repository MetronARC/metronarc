#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Inisialisasi objek GPS dan Serial
TinyGPSPlus gps;
SoftwareSerial gpsSerial(26, 25);  // RX=26, TX=25

// Inisialisasi koordinat inisial dan jarak geofencing (10 meter)
const double initialLat = 1.118970;
const double initialLon = 104.047923;
const double geofenceRadius = 10;  // dalam meter

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600);

  Serial.println("GPS Mulai");
}

void loop() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      
      // Cek apakah data GPS valid
      if (gps.location.isValid()) {
        Serial.println("Terhubung ke Satelit !");
        
        double currentLat = gps.location.lat();
        double currentLon = gps.location.lng();

        // Hitung jarak antara posisi saat ini dan posisi inisial
        double distance = TinyGPSPlus::distanceBetween
        (
          currentLat, currentLon,
          initialLat, initialLon
        );

        // Cek apakah jarak kurang dari geofenceRadius
        if (distance < geofenceRadius) {
          Serial.println("Anda berada di dalam geofence!");
        } else {
          Serial.println("Anda di luar geofence.");
        }

        // Tampilkan informasi posisi saat ini di Serial Monitor
        Serial.print("Lat: ");
        Serial.print(currentLat, 6);
        Serial.print(", Lon: ");
        Serial.print(currentLon, 6);
        Serial.print(" | Jarak ke inisial: ");
        Serial.print(distance);
        Serial.println(" meter");

        delay(1000);
      }
    }
  }
}
