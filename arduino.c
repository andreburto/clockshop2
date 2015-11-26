const int pinA = A1;
const int pinB = 0;

int readState = 0;
int lastState = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int ina = digitalRead(pinA);
  int inb = digitalRead(pinB);

  if (ina == HIGH) {
    if (lastState == LOW) {
      lastState = HIGH;
      Serial.print(ina);
      Serial.print(" - ");
      Serial.println(inb);
    }
  }
  else {
    lastState = LOW;
  }
}