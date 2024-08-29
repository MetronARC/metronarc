const float ESPVCC = 3.3; // Voltage of your Arduino
const unsigned long VALUE_R1 = 21500;
const unsigned long VALUE_R2 = 3280;
const double VOLTAGE_SOURCE = 25.4;
const int ANALOG_PIN = 32;
const int INPUT_RESOLUTION = 4096;
const int AVERAGE_OF = 500;

int analogValue;
int voltageMillivolts;
float voltage;
float calibration;
float averageVoltage; // Global variable for average voltage
char voltageString[10]; // Global variable for voltage string

void setup() {
  Serial.begin(115200);
  delay(500);
}

void loop() {
  averageVoltage = getVoltageAverage();
  snprintf(voltageString, sizeof(voltageString), "%.2f", averageVoltage); // Convert average voltage to string for display

  Serial.print("Vin: ");
  Serial.print(voltageMillivolts);
  Serial.println("mV");

  Serial.print("V Avg: ");
  Serial.println(voltageString); // Print the string
  
  // Example of using averageVoltage in an if-else statement
  if (averageVoltage > 10.0) {
    Serial.println("Voltage is higher than 10V");
  } else {
    Serial.println("Voltage is 10V or lower");
  }

  delay(500);
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
