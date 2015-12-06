const int MAX_LEN = 8;
const int MAX_CNT = 255;
const int BIN_VALS[8] = {1,2,4,8,16,32,64,128};
const int pinA = A1;
const int pinB = 0;

int READ_VALS[8];
int READ_VAL;
int readState = 0;
int lastState = 0;

int cnt = 0;
int rdr_cnt = 0;
int nmbr[MAX_LEN];

void sumArray() {
  READ_VAL = 0;
  
  for(int c=0; c < MAX_LEN; c++) {
    READ_VAL += READ_VALS[c] * BIN_VALS[c];
  }
  
  Serial.println(READ_VAL);
  analogWrite(9, READ_VAL);
  cleanArray();
}

void cleanArray() {
  for(int c=0; c < MAX_LEN; c++) {
    nmbr[c] = 0;
  }
  rdr_cnt = 0;
}

void readSerial() {
  // put your main code here, to run repeatedly:
  int ina = digitalRead(pinA);
  int inb = digitalRead(pinB);

  if (ina == HIGH) {
    if (lastState == LOW) {
      lastState = HIGH;

      if (inb == 1) {
        READ_VALS[rdr_cnt] = 1;
      } else {
        READ_VALS[rdr_cnt] = 0;
      }

      rdr_cnt += 1;

      if (rdr_cnt == 8) { sumArray(); }
    }
  }
  else {
    lastState = LOW;
  }  
}

void setup() {
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  pinMode(9, OUTPUT);

  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  cleanArray();
}

void loop() {
  readSerial();
}