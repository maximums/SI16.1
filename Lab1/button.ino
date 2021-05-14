#define Button 7
#define LED 5

int buttonState = 0;
void setup() {
  pinMode(Button, INPUT);
  pinMode(LED, OUTPUT);
 
}

void loop() {
 buttonState = digitalRead(Button);
// if(buttonState)
//    digitalWrite(LED, LOW);
// else digitalWrite(LED, HIGH);
  buttonState ? digitalWrite(LED, LOW) : digitalWrite(LED, HIGH);
}
