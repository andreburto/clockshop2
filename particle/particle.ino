// This array will contain the values for both numeric displays. Minutes
// start at 0 and go to 23; hours start at 24 and go to 83.
int numbers [] = { // HOURS
                  0, 3, 6, 8, 10,       //0-4
                  13,15,18,21,23,       //5-9
                  25,27,30,32,34,       //10-14
                  38,40,43,44,46,       //15-19
                  50,52,55,57,          //20-23
                  
                   // MINUTES
                  0, 2, 5, 7, 10,       //0-4
                  12,14,16,18,20,       //5-9
                  22,25,27,33,36,       //10-14
                  37,40,42,43,46,       //15-19
                  50,52,55,58,61,       //20-24
                  64,66,69,71,74,       //25-29
                  76,79,81,84,86,       //30-34
                  88,90,94,96,99,       //35-39 
                  102,104,106,108,110,  //40-44
                  113,115,118,120,123,  //45-49
                  126,129,132,134,136,  //50-54
                  138,141,143,144,146}; //55-59

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

// Arduino Commands
const int HOUR_OUT = 90;
const int MINUTE_OUT = 91;
const int MUSIC_ON = 92;
const int MUSIC_OFF = 93;
const int DIAG_ON = 99;
const int DIAG_OFF = 100;

// Time range for variables
const int MINUTE_MAX = 83;
const int MINUTE_START = 24;
const int HOUR_MAX = 23;
const int HOUR_START = 0;

// Variables for keeping track of time
unsigned long LAST_TIME_CHECK;
unsigned long NEXT_TIME_CHECK;
unsigned long CHANGE_TIME_WAIT = 1000;
// Counters to track current time
int SECOND_COUNT = 0;
int MINUTE_COUNT = 0;
int HOUR_COUNT = 0;

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
    intToOut(HOUR_OUT);
    intToOut(h);
}

void setMinute(int m) {
    intToOut(MINUTE_OUT);
    intToOut(m);
}

int setTime(String data_load) {
    String t = data_load.substring(0,1);
    String v = data_load.substring(1);
    // Once you manually set auto is tured off
    autoset = 0;
    // Set the hour
    if (t.equals("h")) {
        setHour(v.toInt());
        HOUR_COUNT = v.toInt();
        return HOUR_COUNT;
    }
    // Set the minutes
    else if (t.equals("m")) {
        setMinute(v.toInt());
        MINUTE_COUNT = v.toInt();
        return MINUTE_COUNT;
    }
    // Unrecognized code
    return -1;
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
        intToOut(MUSIC_ON);
        intToOut(0);
    } else {
        playstop = 0;
        intToOut(MUSIC_OFF);
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

void keepTime() {
    unsigned long TIME_NOW = millis();
    
    if (TIME_NOW >= NEXT_TIME_CHECK) {
        LAST_TIME_CHECK = NEXT_TIME_CHECK;
        NEXT_TIME_CHECK = TIME_NOW + CHANGE_TIME_WAIT;
        
        SECOND_COUNT++;
        
        if (SECOND_COUNT > 59) {
            SECOND_COUNT = 0;
            
            MINUTE_COUNT++;
            
            if (MINUTE_COUNT > 59) {
                MINUTE_COUNT = 0;

                HOUR_COUNT++;
                
                if (HOUR_COUNT > 23) {
                    HOUR_COUNT = 0;
                }
            }
            
            setMinute(MINUTE_COUNT);
            setHour(HOUR_COUNT);
        }
    }
    
    else {
        if (TIME_NOW < LAST_TIME_CHECK) {
            LAST_TIME_CHECK = NEXT_TIME_CHECK;
            NEXT_TIME_CHECK = TIME_NOW + CHANGE_TIME_WAIT;
        }
    }
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
    //Particle.syncTime();
    
    delay(1000);

    // Setup display
    for (int count = 0; count <= 83; count++) {
        intToOut(count);
        intToOut(numbers[count]);
    }
    
    // Exit diagnostic mode
    intToOut(100);
    intToOut(1);
    
    // Set Time
    Time.zone(-6);
    HOUR_COUNT = Time.hour();
    MINUTE_COUNT = Time.minute();
    // Set clock display
    setHour(HOUR_COUNT);
    setMinute(MINUTE_COUNT);
    
    // Set the checks for time
    LAST_TIME_CHECK = 0;
    NEXT_TIME_CHECK = millis() + CHANGE_TIME_WAIT;
}

void loop() {
    // Sync with the Particle cloud
    // keepSync();
    keepTime();
}
