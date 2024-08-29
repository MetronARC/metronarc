const int irSensor1Pin = 32; // Pin for the first IR sensor
const int irSensor2Pin = 33; // Pin for the second IR sensor

void setup() {
  Serial.begin(9600); // Initialize serial communication
  pinMode(irSensor1Pin, INPUT); // Set IR sensor 1 pin as input
  pinMode(irSensor2Pin, INPUT); // Set IR sensor 2 pin as input
}

void loop() {
  // Read the state of IR sensors
  int irSensor1State = digitalRead(irSensor1Pin);
  int irSensor2State = digitalRead(irSensor2Pin);
  
  // Check if sensor 1 is HIGH
  if (irSensor1State == LOW) {
    Serial.println("IR Sensor 1 is HIGH");
  }

  // Check if sensor 2 is HIGH
  if (irSensor2State == LOW) {
    Serial.println("IR Sensor 2 is HIGH");
  }
  
  delay(1000); // Delay for readability, adjust as needed
}
