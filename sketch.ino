#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

// Pins
#define SENSOR1_PIN 4
#define SENSOR2_PIN 5
#define BUZZER_PIN 15
#define LED_PIN 2

// Display
Adafruit_SSD1306 display(128, 64, &Wire);

// Temperature sensors
OneWire oneWire1(SENSOR1_PIN);
OneWire oneWire2(SENSOR2_PIN);
DallasTemperature sensor1(&oneWire1);
DallasTemperature sensor2(&oneWire2);

void setup() {
  Serial.begin(115200);
  
  // Initialize I/O pins
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // Initialize display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED display failed!");
    while(1);
  }
  
  // Start temperature sensors
  sensor1.begin();
  sensor2.begin();
  
  // Show startup message
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Dual-Zone Monitor");
  display.println("Initializing...");
  display.display();
  delay(1000);
}

void loop() {
  // Request temperatures
  sensor1.requestTemperatures();
  sensor2.requestTemperatures();
  
  // Read temperatures
  float temp1 = sensor1.getTempCByIndex(0);
  float temp2 = sensor2.getTempCByIndex(0);
  
  // Validate readings
  if(temp1 < -50 || temp1 > 125) temp1 = 0;
  if(temp2 < -50 || temp2 > 125) temp2 = 0;
  
  // Update display
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Dual-Zone Monitor");
  display.println("----------------");
  display.print("Zone 1: "); display.print(temp1); display.println("°C");
  display.print("Zone 2: "); display.print(temp2); display.println("°C");
  display.display();
  
  // Alert system
  if(temp1 > 30 || temp2 > 30) {
    digitalWrite(LED_PIN, LOW);
    tone(BUZZER_PIN, 1000, 500);
  } else {
    digitalWrite(LED_PIN, HIGH);
    noTone(BUZZER_PIN);
  }
  
  // Serial output for debugging
  Serial.print("Zone 1: "); Serial.print(temp1);
  Serial.print("°C\tZone 2: "); Serial.print(temp2); Serial.println("°C");
  
  delay(2000); // Update every 2 seconds
}