#define ANALOG_PIN A1
#define VREF 5000 // ADC's reference voltage on your Arduino,typical value:5000mV
#define PRINT_INTERVAL 0
#define FULLLEVEL 1205    // 100 %
#define EMPTYLEVEL 460   // 0 %
#define comSerial Serial1

int16_t dataVoltage;
float dataCurrent, total, average; 
unsigned long timepoint_measure;
int counter;
int percentage;
char cstr[16];

// Initialize LED Pins
int ledGreen = 12;
int ledYellow = 11;

void setup()
{
  Serial.begin(9600);
  comSerial.begin(4800);
  pinMode(ANALOG_PIN, INPUT);
  timepoint_measure = millis();

  // LED 
  pinMode(ledGreen, OUTPUT);     
  pinMode(ledYellow, OUTPUT); 
  digitalWrite(ledGreen, HIGH);  

  counter = 0;
  total = 0;
}


void loop()
{
  // Turn the LED on
  digitalWrite(ledYellow, LOW);

  if (millis() - timepoint_measure > PRINT_INTERVAL) {
    counter++;
    timepoint_measure = millis();

    dataVoltage = analogRead(ANALOG_PIN)/ 1024.0 * VREF;
    dataCurrent = dataVoltage / 120.0; //Sense Resistor:120ohm
    
    total = total + dataVoltage;

  }
  
  // Only send every 1000
  if (counter == 1000) {
    digitalWrite(ledYellow, HIGH);
    delay(200);
    digitalWrite(ledYellow, LOW);

    // Calculation
    average = total/counter;
    percentage = (average-EMPTYLEVEL)/(FULLLEVEL - EMPTYLEVEL) * 100;
    itoa(percentage, cstr, 10);

    // Print some debug data
    Serial.print("Percentage:");
    Serial.println(percentage);
    Serial.println("====================================");
    
    // Send the data
    String test =  percentage +",";
    comSerial.write(percentage);

    // Reset the counter
    counter = 0;
    total = 0;
    delay(2000);
  }
}
