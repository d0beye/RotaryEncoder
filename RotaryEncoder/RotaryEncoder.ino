#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 

const int encoderPin = 3;
const float PPR = 360.0; 

volatile long pulseCount = 0;
unsigned long lastTime = 0;
const int interval = 1000; 

void countPulse() {
  pulseCount++;
}

void setup() {
  Serial.begin(9600);
  pinMode(encoderPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPin), countPulse, RISING);

  lcd.init();
  lcd.backlight();
  
  // Welcome Screen
  lcd.setCursor(0, 0);
  lcd.print("Spin Module");
  lcd.setCursor(0, 1);
  lcd.print("Harun Temiz");
  delay(3000);
  lcd.clear();
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastTime >= interval) {
    // 1. Atomically grab the pulse count
    noInterrupts(); 
    long currentPulses = pulseCount;
    pulseCount = 0; 
    interrupts(); 

    // 2. Calculate RPM
    // (Pulses per second / PPR) * 60 seconds = RPM
    float rpm = (currentPulses / PPR) * 60.0;

    // 3. Update LCD
    lcd.clear(); // Clear old numbers to prevent overlap
    lcd.setCursor(0, 0);
    lcd.print("Pulses: ");
    lcd.print(currentPulses);
    
    lcd.setCursor(0, 1);
    lcd.print("RPM: ");
    lcd.print(rpm, 1); // Show 1 decimal place

    lastTime = currentTime;
  }
}