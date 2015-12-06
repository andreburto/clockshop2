const int MAX_LEN = 8;
const int MAX_CNT = 255;
const int pinSig = A0;
const int pinDat = D0;

int count = 0;
int adder = 1;
const int flip = -1;


// Write out a true or false signal
void writeOut(int data) {
    // Start signal
    if (data == 1) {
        digitalWrite(pinDat, HIGH);
    }
    digitalWrite(pinSig, HIGH);
    
    // Hold for a bit
    delay(5);
    
    // Stop signal
    if (data == 1) {
        digitalWrite(pinDat, LOW);
    }
    digitalWrite(pinSig, LOW);
    
    delay(5);
}

void setup() {
    // Setup the mode of the pins
    pinMode(pinSig, OUTPUT);
    pinMode(pinDat, OUTPUT);
    
    // Start them low
    digitalWrite(pinSig, LOW);
    digitalWrite(pinDat, LOW);
}

void loop() {

  for (int x = 0; x < MAX_LEN; x++) {
    int v = (count >> x) & 1;
    writeOut(v);
  }

  count += adder;

  if (count < 1 || count > 255) {
    adder *= flip;
  }

  delay(1000);
}