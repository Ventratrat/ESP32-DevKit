#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

typedef enum {
  LED_OFF_T = HIGH,
  LED_ON_T = LOW
} LED_STATE_T ;

#ifndef LED_BUILTIN
#define LED_BUILTIN 17
#endif 
#define LED3        16 // red 

const byte switch_pinG = 0; //corresponds to GPIO0
int PWM_FREQUENCY = 1000; 
int PWM_CHANNEL = 0; 
int PWM_RESOUTION = 8; 
unsigned long count_s = 0;
hw_timer_t * timer = NULL;

void IRAM_ATTR onTimer(){
 count_s++;
}

extern void FadeIn(void* pvParameters);
extern void Steady(void* pvParameters);
extern void FadeOut(void* pvParameters);

static void IRAM_ATTR button_pressed(void);

void setup() {
  
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(switch_pinG), button_pressed, FALLING);  
  pinMode(LED3, OUTPUT);

  ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOUTION);
  ledcAttachPin(LED3, PWM_CHANNEL);
  ledcWrite(PWM_CHANNEL, 255);

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);
  

  Serial.println("\nStarting the scheduler");
  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
  
}

void loop(){
  
  // Empty. Things are done in Tasks.
}



void vProcessInterface(void* pvParameter1, uint32_t ulParameter2){

  BaseType_t xInterfaceToService;

  xInterfaceToService = (BaseType_t) ulParameter2;

  Serial.println("\nStarting the FadeIn task now.");

  xTaskCreatePinnedToCore(
  FadeIn
  ,  "FadeIn"   // A name just for humans
  ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
  ,  NULL
  ,  3 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
  ,  NULL 
  ,  ARDUINO_RUNNING_CORE);

  
}

static void IRAM_ATTR button_pressed(void){ // deferred interrupt 

  BaseType_t xInterfaceToService, xHigherPriorityTaskWoken;

  xHigherPriorityTaskWoken = pdFALSE;
  xTimerPendFunctionCallFromISR(vProcessInterface, NULL, (uint32_t) xInterfaceToService, &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(); // RTOS functions must have ISR suffix 
  
}

void FadeIn(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  Serial.println("\nCurrently in the FadeIn task");

  count_s=0;
  
  while(count_s < 4000){ // increasing brightness for 4 seconds
    ledcWrite(PWM_CHANNEL, map(count_s,4000,0,0,256));
  }
  
  xTaskCreatePinnedToCore(
    Steady
    ,  "Steady"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);

  Serial.println("Finished fading in.");
  vTaskDelete(NULL);
}

void Steady(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  Serial.println("\nCurrently in the Steady task");
  count_s=0;
  
  while(count_s < 5000){
    Serial.println(count_s);
  }
    xTaskCreatePinnedToCore(
    FadeOut
    ,  "FadeOut"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);
    

  vTaskDelete(NULL);
}
void FadeOut(void *pvParameters) 
{
  (void) pvParameters;

  Serial.println("\nCurrently in the FadeOut task");
  
  count_s = 0;
  while(count_s <= 2000){ // decreasing brightness for 2 seconds 
    ledcWrite(PWM_CHANNEL, map(count_s,2000,0,256,0));
  }
    
  Serial.println("Finished fading out.");
  vTaskDelete(NULL);
}
