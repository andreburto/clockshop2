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
// Play or Stop indicator
int playstop = 0;

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
    autoset = 0;
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
    // Resets autoset to true
    autoset = 1;
    // Sets the time of the last sync
    lastSync = millis();
    // Syncs with the cloud
    Particle.syncTime();
    // Wait for the return
    delay(1000);
    // Set the time
    setHour(Time.hour());
    setMinute(Time.minute());
}

void keepSync() {
    if (autoset == 1 && millis() > start) {
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

int playStop(String val) {
    if (playstop == 0) {
        playstop = 1;
        intToOut(70);
        intToOut(0);
    } else {
        playstop = 0;
        intToOut(71);
        intToOut(0);
    }
    return playstop;
}

int setDisplay(String val) {
    int eq_pos = val.indexOf(',', 0);
    if (eq_pos == -1) return -1;
    String d = val.substring(0, eq_pos);
    String v = val.substring(eq_pos+1);
    intToOut(d.toInt());
    intToOut(v.toInt());
    return 1;
}

void setup() {
    // Expose funtion
    Particle.function("settime", setTime);
    Particle.function("setmanual", setManual);
    Particle.function("playmusic", playStop);
    Particle.function("setdisplay", setDisplay);
    
    // Expose variables
    Particle.variable("autoset", &autoset, INT);
    Particle.variable("playstop", &playstop, INT);
    
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