// Numbers for the display found at the following address:
// https://lb-community.s3.amazonaws.com/uploads/supporting_file/asset/457/Dice.ino
int numbers [] = { 0, 3, 6, 8,10,       //0
                  13,15,18,20,23,       //9
                  25,27,30,33,35,       //14
                  38,40,43,45,48,       //19
                  50,52,55,58,61,       //24
                  64,66,69,71,74,       //29
                  76,78,81,84,86,       //34
                  89,91,94,96,99,       //39
                  102,104,106,108,111,  //44
                  113,115,118,120,123,  //49
                  126,129,132,134,136,  //54
                  139,141,143,146,149}; //59
// Constants for reading data.
const int BIN_VALS[8] = {1,2,4,8,16,32,64,128};
const int MAX_LEN = 8;
// For reading in values
int READ_VALS[8];
int READ_VAL = -1;
int RDR_CNT = 0;
// Command and data holders
int valCmd = -1;
int valDat = -1;
// Time count - 1000milli = 1sec
const unsigned long MLEN = 1000;
const unsigned int MAX_HOUR = 24;
const unsigned int MAX_MINS = 60;
const unsigned int MAX_SECS = 60;
// input
const int pinA = A1;
const int pinB = 0;
// output
const int digo = 1;
const int mo = 5;
const int so = 9;
// State for the serial reading
int readState = 0;
int lastState = 0;
// Clock times
unsigned long baset = 0;
unsigned long start = 0;
int the_hr = 0;
int the_mn = 0;
// Buffer timer
unsigned long buff_timer = 0;

// Resets the REAL_VALS array to all nulls and the RDR_CNT to 0.
void cleanArray() {
  for(int c=0; c < MAX_LEN; c++) {
    READ_VALS[c] = NULL;
  }
  RDR_CNT = 0;
}

// Convers an array of 1's and 0's to an integer.
int sumArray() {
  READ_VAL = 0;
  for(int c=0; c < MAX_LEN; c++) {
    READ_VAL += READ_VALS[c] * BIN_VALS[c];
  }
  return READ_VAL;
}

void execCommand() {
  switch (valCmd) {
    case 60:
      analogWrite(mo, numbers[valDat]);
      the_hr = valDat;
      break;
    case 61:
      analogWrite(so, numbers[valDat]);
      the_mn = valDat;
      break;
    case 70:
      digitalWrite(digo, HIGH);
      break;
    case 71:
      digitalWrite(digo, LOW);
      break;
  }
}

void setState() {
  // Blank slate state
  if (valCmd == -1 && valDat == -1) {
    valCmd = sumArray();
  }
  // Data has been read
  else if (valCmd > -1 && valDat == -1) {
    valDat = sumArray();
  }
  // Both have been read
  if (valCmd > -1 && valDat > -1) {
    // Execute commands
    execCommand();
    // Reset the values
    valCmd = -1;
    valDat = -1;
  }
}

void readSerial() {
  // Read the pins
  int ina = digitalRead(pinA);
  int inb = digitalRead(pinB);
  
  if (ina == HIGH) {
    if (lastState == LOW) {
      lastState = HIGH;
      // Read the data pin and fed its value into the array
      if (inb == 1) {
        READ_VALS[RDR_CNT] = 1;
      } else {
        READ_VALS[RDR_CNT] = 0;
      }
      // Increment the read counter
      RDR_CNT += 1;
      // If the last bit is read, clear the array
      if (RDR_CNT == MAX_LEN) {
        setState();
        cleanArray();
      }
    }
  }
  else {
    lastState = LOW;
  }  
}

// In case bad or incomplete data is sent, clean out the buffer every 500millis if any data.
void checkBuffer() {
  if (valCmd > -1 || valDat > -1) {
    if (buff_timer == 0) {
        buff_timer = millis() + 500;
    } else {
      if (millis() > buff_timer) {
        cleanArray();
        valCmd = -1;
        valDat = -1;
      }
    }
  }
}

// Updates the time every loop
void tickTock() {
  // If the milliseconds are more than the set time
  if (millis() > baset) {
    // Increment minute counter
    the_mn += 1;
    // Update the minute if ready
    if (the_mn == MAX_MINS) {
      // Reset minute counter
      the_mn = 0;
      // Increment hour counter
      the_hr += 1;
      // Update the hour if ready
      if (the_hr == MAX_HOUR) {
        // Reset hour counter
        the_hr = 0;
        // Reset start every hour
        start = baset;
      }
    }
    // Update the display
    analogWrite(mo, numbers[the_hr]);
    analogWrite(so, numbers[the_mn]);
    // Update baset
    baset = setBaseT();
  } else { // when millis() rolls over
    if (millis() < start) {
      baset = setBaseT();
    }
  }    
}

// Returns the start time
unsigned long setBaseT() {
  return millis() + (MAX_SECS * MLEN);
}

void setup() {
  // Input
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  pinMode(9, OUTPUT);
  // 7-segment display
  pinMode(digo, OUTPUT);
  pinMode(mo, OUTPUT);
  pinMode(so, OUTPUT);
  // Set the default
  baset = setBaseT();
  // Initialize the array
  cleanArray();
  // Wait half a second
  delay(500);
}

void loop() {
  // Read data coming in from the pins
  readSerial();

  // Check for malformed data in the buffer, clean if needed.
  //checkBuffer();

  // Update the time
  tickTock();
}