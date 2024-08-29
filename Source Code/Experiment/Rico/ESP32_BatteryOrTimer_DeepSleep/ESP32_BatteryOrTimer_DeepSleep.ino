#define ADC_PIN 39
#define ADC_THRESHOLD 2000
#define SLEEP_TIME 120000 // 2 minutes in milliseconds

void setup() {
    Serial.begin(115200);
    delay(1000);  // Allow time for Serial Monitor to open

    // Read ADC value
    int adcValue = analogRead(ADC_PIN);
    
    // Check if the ADC value exceeds the threshold
    if (adcValue >= ADC_THRESHOLD) {
        Serial.println("ESP32 wakes up because of passing the ADC threshold");
    } else {
        Serial.println("ESP32 is awake now!");
    }

    // Set the ESP32 to deep sleep mode
    Serial.println("Going to deep sleep for 2 minutes...");
    esp_sleep_enable_timer_wakeup(SLEEP_TIME * 1000); // Convert to microseconds
    esp_deep_sleep_start();
}

void loop() {
    // The loop will not run since the ESP32 will be in deep sleep mode
}
