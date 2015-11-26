const int pinSig = A0;
const int pinDat = D0;

// Write out a true or false signal
void writeOut(bool data) {
    // Start signal
    if (data == true) {
        digitalWrite(pinDat, HIGH);
    }
    digitalWrite(pinSig, HIGH);
    
    // Hold for a bit
    delay(5);
    
    // Stop signal
    if (data == true) {
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
  // put your main code here, to run repeatedly:
  writeOut(true);
  writeOut(false);
  writeOut(true);
  writeOut(true);
  delay(1000);
}