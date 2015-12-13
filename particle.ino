// Data writing constants
const int MAX_LEN = 8;
const int MAX_CNT = 255;
// Pin constants
const int pinSig = A0;
const int pinDat = D0;
// Timer constants
unsigned long start;
unsigned long lastSync;
unsigned long oneHour = 60*60*1000;
// Auto-set boolean
int autoset = 1;

// Write out a serial true or false signal
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
    
    // Wait for a bit
    delay(5);
}

// Write out an integer value as bits
void intToOut(int nmbr) {
  if (nmbr > MAX_CNT) { nmbr = MAX_CNT; }
  for (int x = 0; x < MAX_LEN; x++) {
    int v = (nmbr >> x) & 1;
    writeOut(v);
  }
}

void setHour(int h) {
    intToOut(60);
    intToOut(h);
}

void setMinute(int m) {
    intToOut(61);
    intToOut(m);
}

int setTime(String data_load) {
    String t = data_load.substring(0,1);
    String v = data_load.substring(1);
    // Once you manually set auto is tured off
    autoset = false;
    // The value to return
    int retval = -1;
    // Set the hour
    if (t.equals("h")) {
        setHour(v.toInt());
        retval = v.toInt();
    }
    // Set the minutes
    else if (t.equals("m")) {
        setMinute(v.toInt());
        retval = v.toInt();
    }
    // Unrecognized code
    return retval;
}

void manualSync() {
    Particle.syncTime();
    // Wait for the return
    delay(1000);
    // Set the time
    setHour(Time.hour());
    setMinute(Time.minute());
    // Resets autoset to true
    autoset = 1;
}

void keepSync() {
    if (autoset != 1) return;
    if (millis() > start) {
        if (millis() > lastSync+oneHour) {
            manualSync();
        }
    }
    else {
        start = lastSync;
    }
}

int setManual(String val) {
    if (val.equals("1")) {
        manualSync();
    } else {
        autoset = 0;
    }
    return autoset;
}

void setup() {
    // Expose funtion
    Particle.function("settime", setTime);
    Particle.function("setmanual", setManual);
    
    // Expose variables
    Particle.variable("autoset", &autoset, INT);
    
    // Setup the mode of the pins
    pinMode(pinSig, OUTPUT);
    pinMode(pinDat, OUTPUT);
    
    // Start them low
    digitalWrite(pinSig, LOW);
    digitalWrite(pinDat, LOW);
    
    // Synchronize time
    Particle.syncTime();
    
    delay(1000);
    
    // Set Time
    Time.zone(-6);
    setHour(Time.hour());
    setMinute(Time.minute());
    
    // Start sync logic
    start = millis();
}

void loop() {
    // Sync with the Particle cloud
    keepSync();
    // Keep some time difference between sync, lastSync, and millis()
    delay(500);
}