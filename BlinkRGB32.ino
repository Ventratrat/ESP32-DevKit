#define RED 16
#define GREEN 17
#define BLUE 18

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}


void loop() {

digitalWrite(RED, LOW);
digitalWrite(BLUE, LOW);
digitalWrite(GREEN, HIGH);
delay(3000);
digitalWrite(RED, HIGH);
digitalWrite(BLUE, HIGH);
digitalWrite(GREEN, HIGH);
delay(2000);
digitalWrite(RED, LOW);
digitalWrite(BLUE, HIGH);
digitalWrite(GREEN, HIGH);
delay(3000);
digitalWrite(RED, HIGH);
digitalWrite(BLUE, HIGH);
digitalWrite(GREEN, HIGH);
delay(4000);
}
