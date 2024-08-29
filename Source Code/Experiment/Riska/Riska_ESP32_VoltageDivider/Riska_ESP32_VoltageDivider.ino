const float arduinoVCC = 3.3;//Your Arduino voltage
unsigned long ValueR1 = 99900;
unsigned long ValueR2 = 3280;
double Voltage_Source = 103.8;
const int alanogPin = 34;//the pin connecting the voltage. 
const int inputResolution =4096;//works with most Arduino boards
const float average_of = 500;//Average of 500 readings
int A1Value;
int A0Value;
float voltage;
float Calibrasi;
void setup() {
  Serial.begin(9600);
  Serial.println("Robojax: Reading Any voltage with Arduino");
  delay(500);

}

void loop() {
  //Robojax.com ARDVC-01 Measure any voltage with Arduino
  readVoltage();

  Serial.print("Vin: ");
  Serial.print(A1Value);
  Serial.println("mV"); 

  //Serial.print(voltage);
  Serial.print("V Avg: ");
  Serial.println(getVoltageAverage());
  delay(500);
  
}


void readVoltage(){
   A0Value = analogRead(alanogPin);
    A1Value = A0Value +100;
    float voltage_sensed = A1Value * (arduinoVCC / (float)inputResolution); 
  voltage = voltage_sensed / ((float) ValueR2/((float) ValueR1 + (float) ValueR2)); 
  Calibrasi = (1.0447 * voltage) + 1.2885;
}

  float getVoltageAverage(){
    float voltage_temp_average=0;
      for(int i=0; i < average_of; i++)
      {
         readVoltage();
         voltage_temp_average +=Calibrasi;
      }
        
    return voltage_temp_average / average_of;
      }
