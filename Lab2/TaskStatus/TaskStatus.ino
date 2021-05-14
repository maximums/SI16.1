#include <Arduino_FreeRTOS.h>

#define BUTTON_1 13
#define BUTTON_2 10
#define BUTTON_3 9
#define LED_1 12
#define LED_2 4

int REC_B = 500;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_3, INPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  fdevopen(&serial_putc, &serial_getc);
  
  xTaskCreate(TaskIdle, // Task function
              "Blink", // Task name
              128, // Stack size 
              NULL, 
              1, // Priority
              NULL); // Task handler

  xTaskCreate(TaskAButtonLed,
              "TaskAButtonLed",
              128,
              NULL, 
              1,
              NULL);

  xTaskCreate(TaskB_BlinkLed,
              "TaskB_BlinkLed",
              128,
              NULL, 
              1,
              NULL);

  xTaskCreate(TaskC_Inc_Dec_Blink,
              "TaskC_Inc_Dec_Blink",
              128,
              NULL, 
              1,
              NULL);

}

void loop() { }

void TaskIdle(void *pvParameters) {
  (void) pvParameters;
  for (;;)
  {
//    if (!digitalRead(BUTTON_1)) {
//      printf("BUTTON PRESSED\r");
      if (digitalRead(LED_1)) printf("LedOn\r");
      else printf("LedOff\r");  
//    }
    vTaskDelay(200 / portTICK_PERIOD_MS);  
  } 
}

void TaskAButtonLed(void *pvParameters) {
  (void) pvParameters;
  for (;;)
  {
    digitalRead(BUTTON_1) ? digitalWrite(LED_1, HIGH) : digitalWrite(LED_1, LOW);
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

void TaskB_BlinkLed(void *pvParameters) {
  (void) pvParameters;
  for (;;)
  {
    if (digitalRead(LED_1))
      digitalWrite(LED_2, !digitalRead(LED_2));
    vTaskDelay(REC_B / portTICK_PERIOD_MS);
  }
}

void TaskC_Inc_Dec_Blink(void *pvParameters) {
  (void) pvParameters;
  for (;;)
  {
    if (!digitalRead(BUTTON_2)) REC_B+=100;
    if (!digitalRead(BUTTON_3)) REC_B-=100;
    vTaskDelay(100 / portTICK_PERIOD_MS);  
  }
}

//    STDIO
void serial_putc(char c, FILE *){
  Serial.print(c);
}

char serial_getc(FILE *) {
  return Serial.read();
}
