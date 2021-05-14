#include <LiquidCrystal.h>

enum direc {forward, backward, stop};

#define LCD_COLUMS 16
#define LCD_ROWS 2
#define MOTOR_PIN1 13
#define MOTOR_PIN2 12
#define MOTOR_Ena 11
#define RELAY_PIN 10

const byte d7 = 14, d6 = 15, d5 = 16, d4 = 17, en = 18, rs = 19;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
String input;

char getChar(FILE *) {
   return Serial.read();  
};
void putChar(char c, FILE *) {
  Serial.print(c);
};

class Relay{
  private:
    byte pin;
    byte state = 0;
  public:
    Relay(const byte pin) {
      this->pin = pin;
      pinMode(this->pin, OUTPUT);
    }
  public:
    void r_switch(String comd) {
      lcd.clear();
      if (comd.compareTo("on")) {
        this->state = 0;
        lcd.print("Bulb Off");
      }else {
        this->state = 1;
        lcd.print("Bulb On");
      }
    }
    void r_work(){
      state == 0 ? digitalWrite(this->pin, LOW) : digitalWrite(this->pin, HIGH);       
    }
    
};

class Motor{
  private:
    byte flag = 0;
    byte m_speed = 50;
    byte pin1;
    byte pin2;
    byte ena;
  public:
    Motor(const byte pin1, const byte pin2, const byte ena) {
      this->pin1 = pin1;
      this->pin2 = pin2;
      this->ena = ena;
    }
  public:
    void init(){
      pinMode(this->pin1, OUTPUT);
      pinMode(this->pin2, OUTPUT);
      pinMode(this->ena, OUTPUT);
    }
    void m_work(){
      analogWrite(this->ena, m_speed);
      if (this->flag == 1) {
        digitalWrite(this->pin1, LOW);
        digitalWrite(this->pin2, HIGH);
      }else if(this->flag == 2) {
        digitalWrite(this->pin1, HIGH);
        digitalWrite(this->pin2, LOW);
      }else {
        digitalWrite(this->pin1, LOW);
        digitalWrite(this->pin2, LOW);  
      }
    }
    void set_speed(byte new_speed){
      lcd.clear();
      if(new_speed <= 255 and new_speed >= 0) {
        this->m_speed = new_speed;
        int tmp = ((new_speed * 100) / 255);
        lcd.print(String("Power: ")+String(tmp)+String(" %"));
       } else lcd.print("0 < speed < 255");
    }
    void change_direction(String comd) {
      int tmp = ((this->m_speed * 100) / 255);
      lcd.clear();
      if (comd.equals("backward")) {
        flag = 1;
        lcd.print(String("Direct: backward"));
        lcd.setCursor(0,1);
        lcd.print(String("Power: ")+String(tmp)+String(" %"));
       }
      else if (comd.equals("forward")) {
        flag = 2;
        lcd.print(String("Direct: forward")+String(" %"));
        lcd.setCursor(0,1);
        lcd.print(String("Power: ")+String(tmp)+String(" %"));
       }
      else if (comd.equals("stop")) {
        flag = 0;
        lcd.print(String("Motor stoped"));
       }
    }
};

Motor motor(MOTOR_PIN1, MOTOR_PIN2, MOTOR_Ena);
Relay relay(RELAY_PIN);

void getInput() {
  char c;
  scanf("%c",&c);
  if(c == '\r') {
    lcd.clear();
    if (
      input.compareTo("forward") == 0 || 
      input.compareTo("backward") == 0 || 
      input.compareTo("stop") == 0
    ) motor.change_direction(input);
    else if (input.compareTo("on") ==0 || input.compareTo("off") == 0) 
      relay.r_switch(input);
    else motor.set_speed(input.toInt());
    input = "";      
  } else{
    printf("%c",c);
    input += c;
  }  
}

void setup() {
  Serial.begin(9600);
  lcd.begin(LCD_COLUMS, LCD_ROWS);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Laborator 4");
  lcd.setCursor(0,1);
  lcd.print("Dodi Cristian");
  motor.init();
  delay(1000);
  lcd.clear();
  fdevopen(&putChar, &getChar);
}

void loop() {
  if(Serial.available() > 0)
    getInput();
  motor.m_work();
  relay.r_work();
  delay(50);
}
