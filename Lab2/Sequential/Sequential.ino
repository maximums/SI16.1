#include "timer-api.h"

#define BUTTON_1 13
#define BUTTON_2 10
#define BUTTON_3 9
#define LED_1 12
#define LED_2 4

#define OFST_A 200
#define OFST_B 0
#define OFST_C 300
#define OFST_IDLE 500
#define REC_A 300
#define REC_C 100
#define REC_IDLE 1000

//#define REC_B 500
int REC_B = 500;

int rec_cnt_A;
int rec_cnt_B;
int rec_cnt_C;
int rec_cnt_Idle;

void TaskA_ButtonLed();
void TaskB_BlinkLed();
void TaskC_Inc_Dec_Blink();
void TaskIdle();
void serial_putc(char c, FILE *);
char serial_getc(FILE *);

void setup() {
  Serial.begin(9600);
  rec_cnt_A = OFST_A;
  rec_cnt_B = OFST_B;
  rec_cnt_C = OFST_C;
  rec_cnt_Idle = OFST_IDLE;
  timer_init_ISR_1KHz(TIMER_DEFAULT);   // timer - 1ms
//  timer_init_ISR_10Hz(TIMER_DEFAULT);     // timer - 100ms
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_3, INPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  fdevopen(&serial_putc, &serial_getc);
}

void loop() {
//  do nothing (procastineaza)
}

void timer_handle_interrupts(int timer) {
    if (--rec_cnt_A <= 0) {
      TaskA_ButtonLed();
      rec_cnt_A = REC_A;
    }
    if (--rec_cnt_B <= 0) {
      TaskB_BlinkLed();
      rec_cnt_B = REC_B;
    }
    if (--rec_cnt_C <= 0) {
      TaskC_Inc_Dec_Blink();
      rec_cnt_C = REC_C; 
    }
    if (--rec_cnt_Idle <= 0) {
      TaskIdle();
      rec_cnt_Idle = REC_IDLE;
  }
}

void TaskIdle() {
//  if (!digitalRead(BUTTON_1)) {
//    printf("BUTTON PRESSED\r");
    if (digitalRead(LED_1)) printf("LedOn\r");
    else printf("LedOff\r");  
//  } 
}

void TaskA_ButtonLed() {
  digitalRead(BUTTON_1) ? digitalWrite(LED_1, HIGH) : digitalWrite(LED_1, LOW);
}

void TaskB_BlinkLed() {
  if (digitalRead(LED_1))
    digitalWrite(LED_2, !digitalRead(LED_2));
}

void TaskC_Inc_Dec_Blink() {
  if (!digitalRead(BUTTON_2)) REC_B+=100;
  if (!digitalRead(BUTTON_3)) REC_B-=100;
}

//    STDIO
void serial_putc(char c, FILE *){
  Serial.print(c);
}

char serial_getc(FILE *) {
  return Serial.read();
}
