#include <driver/adc.h>
#include <esp_adc_cal.h>
#include <esp_sleep.h>

// Define analog input and infrared sensor pin
#define ANALOG_IN_PIN 32
#define IR_SENSOR_PIN 35

// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0;
float in_voltage = 0.0;

// Floats for resistor values in divider (in ohms)
float R1 = 30000.0;
float R2 = 7500.0;

// Float for Reference Voltage
float ref_voltage = 5.0;

// Integer for ADC value
int adc_value = 0;

// Flags
bool ir_triggered = false;

void setup()
{
    // Setup Serial Monitor
    Serial.begin(115200);
    Serial.println("DC Voltage Test");

    // Configure the IR sensor pin
    pinMode(IR_SENSOR_PIN, INPUT);

    // Read voltage on startup
    checkVoltage();
}

void loop()
{
    checkVoltage();

    // Check the IR sensor state
    int irState = digitalRead(IR_SENSOR_PIN);
    if (irState == LOW && !ir_triggered)
    {
        Serial.println("Infrared sensor triggered");
        ir_triggered = true; // Set flag to ensure it prints only once
    }
    else if (irState == HIGH && ir_triggered)
    {
        Serial.println("Infrared sensor is now OFF");
        ir_triggered = false; // Reset flag
    }

    // Short delay
    delay(500);
}

void checkVoltage()
{
    // Read the Analog Input
    adc_value = analogRead(ANALOG_IN_PIN);

    // Determine voltage at ADC input
    adc_voltage = (adc_value * ref_voltage) / 1024.0;

    // Calculate voltage at divider input
    in_voltage = adc_voltage / (R2 / (R1 + R2));

    // Print voltage to Serial Monitor to 2 decimal places
    Serial.print("Input Voltage = ");
    Serial.println(in_voltage, 2);

    if (in_voltage < 1.0)
    {
        Serial.println("Voltage is below 1V, going back to sleep");
        // Sleep for 10 seconds and then wake up to check voltage again
        esp_sleep_enable_timer_wakeup(10 * 1000000);
        esp_deep_sleep_start();
    }
    else
    {
        Serial.println("Voltage is more than 1V, proceeding to the void loop");
    }
}
