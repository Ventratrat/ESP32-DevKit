#include <Arduino.h>
#include <EEPROM.h>

const uint8_t BUTTON_PIN = 0;   // Button connected to GPIO 0
const uint8_t RED_PIN = 16;     // Red LED pin
const uint8_t GREEN_PIN = 17;   // Green LED pin
const uint8_t BLUE_PIN = 18;    // Blue LED pin
const int EEPROM_ADDR = 0;      // Fixed EEPROM address to store LED state

volatile bool buttonPressed = false;
volatile bool isFirstPress = true;
volatile bool isBlinking = false;
volatile bool isBlinking2 = false;
volatile int ledState;      // LED state: 0 = off, 1 = red, 2 = blinking blue

void IRAM_ATTR buttonISR() {
  buttonPressed = true;
}

void setup() {
  Serial.begin(9600);  // Initialize serial communication for debugging
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);
  EEPROM.begin(1);
  
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  digitalWrite(RED_PIN, HIGH); 
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, HIGH);

  // Read LED state from EEPROM
 ledState = EEPROM.read(EEPROM_ADDR);
  Serial.print("Read from EEPROM: ");
  Serial.println(ledState);
  
  if (ledState == 1) {
    digitalWrite(RED_PIN, HIGH); 
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, LOW);
    isFirstPress = false;
  } else if (ledState == 2) {
    isBlinking2 = true;
    digitalWrite(RED_PIN, HIGH);   // Turn off red LED
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, HIGH);  // Turn off blue LED
  }

  delay(1000);
}

void loop() {
  ledState = 0;
  if (buttonPressed) {
    if (isFirstPress) {
      digitalWrite(RED_PIN, LOW);   // Turn on red LED
      ledState = 1;
      isFirstPress = false;
    } else {
      isBlinking = !isBlinking;     // Toggle blinking state
      if (isBlinking) {
        digitalWrite(RED_PIN, HIGH);
        digitalWrite(GREEN_PIN, HIGH);
        digitalWrite(BLUE_PIN, HIGH); // Turn off blue LED initially
        delay(100);                   // Delay for stability
        ledState = 2;
      } else {
        // Button pressed again after blink, restart
        isFirstPress = true;
        digitalWrite(RED_PIN, HIGH);   // Turn off all LEDs
        digitalWrite(GREEN_PIN, HIGH);
        digitalWrite(BLUE_PIN, HIGH);
        ledState = 0;
      }
    }
    buttonPressed = false;           // Reset button press flag
    // Store LED state in EEPROM
    EEPROM.write(EEPROM_ADDR, ledState);
    EEPROM.commit();
    Serial.print("Stored in EEPROM: ");
    Serial.println(ledState);
  }

  if (isBlinking) {
    // Blink blue LED
    digitalWrite(BLUE_PIN, HIGH);
    delay(943);                      // Blink interval
    digitalWrite(BLUE_PIN, LOW);
    delay(57);
  }

   if (isBlinking2) {
    // Blink blue LED
    digitalWrite(RED_PIN, HIGH);
    delay(943);                      // Blink interval
    digitalWrite(RED_PIN, LOW);
    delay(57);
  }
}
