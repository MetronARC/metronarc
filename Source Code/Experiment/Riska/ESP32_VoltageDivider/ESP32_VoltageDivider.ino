const float ESPVCC = 3.3; // Voltage of your Arduino
const unsigned long VALUE_R1 = 21500;
const unsigned long VALUE_R2 = 3280;
const double VOLTAGE_SOURCE = 25.4;
const int ANALOG_PIN = 32;
const int INPUT_RESOLUTION = 4096;
const float AVERAGE_OF = 500;

int analogValue;
int voltageMillivolts;
float voltage;
float calibration;
char voltageString[10]; // Global variable for voltage string
bool arcInitiated = false; // Variable to keep track of the arc state

void setup() {
  Serial.begin(115200);
  delay(500);
}

void loop() {
  readVoltage();

  // Check if the arc is initiated or stopped
  if (voltageMillivolts < 2200 && !arcInitiated) {
    Serial.println("Arc Initiated");
    arcInitiated = true;
  } else if (voltageMillivolts >= 2200 && arcInitiated) {
    Serial.println("Arc Stopped");
    arcInitiated = false;
  }

  delay(100); // Adding a small delay to make the serial output readable
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
  }
  return tempAverageVoltage / AVERAGE_OF;
}
