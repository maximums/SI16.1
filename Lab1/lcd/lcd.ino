#include <LiquidCrystal.h>
#include <Keypad.h>

#define KEYPAD_COLS 4
#define KEYPAD_ROWS 4
#define LCD_COLUMS 16
#define LCD_ROWS 2
#define GREEN_LED 2
#define RED_LED 5

char key;
String buff = "";
String validCode = "45215";

char getChar(FILE *);
void putChar(char c, FILE *);
void checkCode(String code);
void printResult(char c);

char keys[KEYPAD_COLS][KEYPAD_ROWS] = {
  {'7','8','9','/'},
  {'4','5','6','*'},
  {'1','2','3','-'},
  {'c','0','=','+'}
 };

const byte rowPins[KEYPAD_ROWS] = {17, 16, 15, 14};
const byte colPins[KEYPAD_COLS] = {11, 10, 9, 8};
const byte rs = 13, en = 12, d4 = 7, d5 = 6, d6 = 4, d7 = 3; 

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

void setup() {
  lcd.begin(LCD_COLUMS, LCD_ROWS);
  lcd.clear();
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  fdevopen(&putChar, &getChar);
}

void loop() {
  scanf("%c",&key);
  if (key) printResult(key);
}

char getChar(FILE *) {
   return keypad.getKey();  
};

void putChar(char str, FILE *) {
  lcd.print(str);
};

void checkCode(String code) {
    if (code == validCode) {
      lcd.clear();
      printf("Valid code.");
      digitalWrite(GREEN_LED, HIGH);
      delay(1000);
      lcd.clear();
      digitalWrite(GREEN_LED, LOW);
    } else {
      lcd.clear();
      printf("Invalid code.");
      digitalWrite(RED_LED, HIGH);
      delay(1000);
      lcd.clear();
      digitalWrite(RED_LED, LOW);
    }  
}

void printResult(char c) {
    if (c != '=') {
      buff+=c;
      printf("%c",c);  
  } else {
      checkCode(buff);
      buff = "";
    } 
}
