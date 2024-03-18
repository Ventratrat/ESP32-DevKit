#include <Arduino.h>
#include <EEPROM.h>

const uint8_t BUTTON_PIN = 0;   // Button connected to GPIO 0
const uint8_t RED_PIN = 16;     // Red LED pin
const uint8_t GREEN_PIN = 17;   // Green LED pin
const uint8_t BLUE_PIN = 18;    // Blue LED pin
const int EEPROM_STATE_ADDR = 0;    // EEPROM address to store LED state

bool buttonState = false;
bool lastButtonState = false;
bool isFirstPress = true;
bool isBlinking = false;
int currentLedState = 0;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(9600); // Initialize serial communication for debugging
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Read LED state from EEPROM
  currentLedState = EEPROM.read(EEPROM_STATE_ADDR);
  
  // Set LED states based on EEPROM value
  if (currentLedState == 1) {
    digitalWrite(RED_PIN, LOW);   // Turn on red LED
    isFirstPress = false;
  } else if (currentLedState == 2) {
    isBlinking = true;
    digitalWrite(RED_PIN, HIGH);   // Turn off red LED
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, HIGH);  // Turn off blue LED
  } else {
    digitalWrite(RED_PIN, HIGH);   // Turn off LEDs initially
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, HIGH);
  }
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastDebounceTime > debounceDelay) {
    lastButtonState = buttonState;
    buttonState = digitalRead(BUTTON_PIN);

    if (lastButtonState == HIGH && buttonState == LOW) {
      buttonPressed();
    }
    lastDebounceTime = currentTime;
  }

  if (isBlinking) {
    // Blink blue LED
    digitalWrite(BLUE_PIN, LOW);
    delay(57);                      // Blink interval
    digitalWrite(BLUE_PIN, HIGH);
    delay(943);
  }
}

void buttonPressed() {
  if (isFirstPress) {
    digitalWrite(RED_PIN, LOW);   // Turn on red LED
    isFirstPress = false;
    currentLedState = 1;
  } else {
    isBlinking = !isBlinking;     // Toggle blinking state
    if (isBlinking) {
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, HIGH); // Turn off blue LED initially
      delay(100);                   // Delay for stability
      currentLedState = 2;
    } else {
      // Button pressed again after blink, restart
      isFirstPress = true;
      digitalWrite(RED_PIN, HIGH);   // Turn off all LEDs
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, HIGH);
      currentLedState = 0;
    }
  }
  
  // Store LED state in EEPROM
  EEPROM.write(EEPROM_STATE_ADDR, currentLedState);
  EEPROM.commit();
}
