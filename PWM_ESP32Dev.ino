hw_timer_t * timer = NULL;
#define BUTTON_PIN 0
#define  RGB  16 

int i = 0;
bool button = false;
unsigned long count = 0;

void IRAM_ATTR run_it() {
  button = true;
}

int PWM_FREQUENCY = 45; 
int PWM_CHANNEL = 0;
int PWM_RESOUTION = 8; 

void IRAM_ATTR onTimer(){
 count++;
}

void setup() {

  attachInterrupt( BUTTON_PIN, run_it, FALLING);
  Serial.begin(9600);
  pinMode(RGB, OUTPUT);
  
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);
  
  ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOUTION);
  ledcAttachPin(RGB, PWM_CHANNEL);
  ledcWrite(PWM_CHANNEL, 255);
}

void loop() {

  if (button){
    
    count = 0;
    
    while(count < 4000){ // increase for 4 seconds
      ledcWrite(PWM_CHANNEL, map(count,4000,0,0,256));
    }

    count = 0;

    while(count <= 5000){ // max brightness for 5 seconds 
      Serial.println(count); 
    }

    count = 0;
    
    while(count <= 2000){ // decrease for 2 seconds 
      ledcWrite(PWM_CHANNEL, map(count,2000,0,256,0));

    }

    button = false;

  }

}
