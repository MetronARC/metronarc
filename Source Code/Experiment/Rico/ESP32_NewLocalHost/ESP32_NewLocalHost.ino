#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ADS1X15.h>
#include <MFRC522.h>
#include <WiFi.h>

String uid;

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

const char *ssid = "Area 1";
const char *password = "1234abcd";
const char* host = "192.168.43.55";
String machineID = "MACHINE-2";
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

enum State {
  WIFI_CHECK,
  ASSIGN_AREA,
  IDLE,
  LOGIN,
  LOGOUT,
  UID_UNKNOWN
};

State currentState = WIFI_CHECK;

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

void httpGet()
{
  http.begin(Link);
  httpCode = http.GET();
  payload = http.getString();
  Serial.println("HTTP GET response: " + String(httpCode));
  Serial.println(payload);
  http.end();
}

void kirimAwal()
{
  Status = "Inactive";
  Mode = "";
  Link = "http://" + String(host) + "/SPARK/kirimInfo.php?UID=" + uid + "&Status=" + Status + "&Mode=" + Mode + "&Area=" + area + "&MachineID=" + machineID + "&Timing=" + jmdS;

  httpGet();
}

void kirimDone()
{
  Status = "Done";
  Mode = "";
  Link = "http://" + String(host) + "/SPARK/kirimInfo.php?UID=" + uid + "&Status=" + Status + "&Mode=" + Mode + "&Area=" + area + "&MachineID=" + machineID + "&Timing=" + jmdS;

  httpGet();
}

void sendLogin()
{            
  Status = "Active";
  Mode = "Login";
  Serial.println("First Tap");
            
  Link = "http://" + String(host) + "/SPARK/kirimInfo.php?UID=" + uid + "&Status=" + Status + "&Mode=" + Mode + "&Area=" + area + "&MachineID=" + machineID + "&Timing=" + jmdS;

  httpGet();
}

void sendLogout()
{
  Status = "Active";
  Mode = "Logout";
  Serial.println("Fourth Tap");
            
  Link = "http://" + String(host) + "/SPARK/kirimInfo.php?UID=" + uid + "&Status=" + Status + "&Mode=" + Mode + "&Area=" + area + "&MachineID=" + machineID + "&Timing=" + jmdS;

  httpGet();
}

void kirimWeb()
{
  //----- Ketika timing berjalan -----
  if (statusWaktu == 1)
  {
    Status = "Active";
    Link = "http://" + String(host) + "/SPARK/kirimWaktu.php?Timing=" + jmdS + "&Status=" + Status + "&MachineID=" + machineID + "&Area=" + area + "&CurrentDC=" + currentDC;

    httpGet();
  }
}

void sendPing()
{                
  Link = "http://" + String(host) + "/SPARK/receivePing.php?UID=" + uid + "&Area=" + area + "&MachineID=" + machineID;

  httpGet();
}

void printMeasure(String prefix, float value, String postfix)
{
  Serial.print(prefix);
  Serial.print(value, 3);
  Serial.println(postfix);
}

//----- Fungsi bagaimana syarat timing agar bisa aktif -----
void syaratTiming()
{
  currentRMS = getCurrent();
  currentDC = (14.109*currentRMS) + 6.3933;
  if (currentRMS > 1){
    statusWaktu = 1;
    statusT = "ON";
  }
  else{
    statusT = "LOW";
    statusWaktu = 0;
  }
}

//----- Fungsi timing -----
void timing()
{
  syaratTiming();


    if (statusWaktu == 1)
    {
        if (millis() > millisSebelum + 1000)
  {
      detik++;
          millisSebelum = millis();
  }

      if (detik >= 60)
      {
        detik = 0;
        menit++;

        if (menit >= 60)
        {
          menit = 0;
          jam++;
        }
      }
    }

    detikS = String(detik);
    menitS = String(menit);
    jamS = String(jam);
    jmdS = jamS + ":" + menitS + ":" + detikS;
    Serial.println(jmdS);
}

float getCurrent() {
  float voltage;
  float current;
  float sum = 0;
  int counter = 0;

  for (int i = 0; i < 10; i++) { //take 1000 samples
    voltage = ads.readADC_Differential_0_1() * multiplier;
    current = voltage * FACTOR;

    // Apply calibration equation
    current = 1.3362 * current + 0.1332;

    sum += sq(current);
    counter++;
  }

  current = sqrt(sum / counter);
  return current;
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
    kirimAwal();
    delay(3000);
  }
}


void waitForTap() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    uid = getUID();
    if (uid != previousUID) {
      currentState = LOGIN;
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
    timing();
    kirimWeb();
    display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Login mode");
  display.setCursor(0, 10);
  display.print("UID: ");
  display.print(uid);
  display.setCursor(0, 20);
  display.print("Machine ID: ");
  display.print(machineID);
  display.setCursor(0, 30);
  display.print("Status: ");
  display.print(statusT);
  display.setCursor(0, 40);
  display.print("AC Current: ");
  display.print(currentRMS);
  display.setCursor(0, 50);
  display.print("DC Current: ");
  display.print(currentDC);
  display.display();
  syaratTiming();

    if (digitalRead(BUTTON_PING) == HIGH && currentState == LOGIN) {
    display.clearDisplay();
    sendPing();
    displayMessage("Ping initiated");
    delay(2000);
  }

  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String newUID = getUID();
    if (newUID == uid) {
      currentState = LOGOUT;
    } else {
      currentState = UID_UNKNOWN;
    }
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
  int attempts = 0;
  const int maxAttempts = 5;

  // Attempt to connect to WiFi
  while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
    Serial.println("Connecting to WiFi...");
    displayMessage("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    delay(5000); // Wait for the connection
    attempts++;
  }

  // Check the connection result
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected to Area 1");
    displayMessage("WiFi Connected");
    digitalWrite(WiFi_LED, HIGH);
    currentState = ASSIGN_AREA;
  } else {
    Serial.println("WiFi failed to connect after several tries");
    displayMessage("WiFi not Connected after several retries");
    currentState = WIFI_CHECK;
  }
}
