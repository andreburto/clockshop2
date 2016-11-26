// Numbers for the display found at the following address:
// https://lb-community.s3.amazonaws.com/uploads/supporting_file/asset/457/Dice.ino
// This array will contain the values for both numeric displays. Minutes
// start at 0 and go to 23; hours start at 24 and go to 83.
int numbers [] = { // HOURS
                   0, 3, 5, 7, 9,       //0-4
                  13,15,18,21,23,       //5-9
                  25,27,30,33,35,       //10-14
                  38,40,43,45,48,       //15-19
                  50,52,55,58,          //20-23
                  
                   // MINUTES
                  0, 2, 5, 7, 10,       //0-4
                  12,14,16,19,20,       //5-9
                  22,25,27,33,36,       //10-14
                  38,40,43,45,47,       //15-19
                  50,52,55,58,61,       //20-24
                  64,66,69,71,74,       //25-29
                  76,79,81,84,86,       //30-34
                  88,90,94,96,99,       //35-39 
                  102,104,106,108,110,  //40-44
                  113,115,118,120,123,  //45-49
                  126,129,132,134,136,  //50-54
                  138,141,143,145,147}; //55-59

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

// input
const int pinA = A1;
const int pinB = 0;

// output
const int digo = 1;
const int hour_out = 5;
const int minute_out = 9;

// State for the serial reading
int readState = 0;
int lastState = 0;

// Clock times
int the_hr = 0;
int the_mn = 0;

// Buffer timer
unsigned long buff_timer = 0;

// Diagnostic mode
const int DIAG_MODE_ON = 1;
const int DIAG_MODE_OFF = 0;
int diagnostic_mode;
unsigned long diag_timer_count;

// Display variables for diagnostic mode
const int MINUTE_MAX = 83;
const int MINUTE_START = 24;
const int HOUR_MAX = 23;
const int HOUR_START = 0;
// Starting positions
int MINUTE_COUNT = (int) MINUTE_START;
int HOUR_COUNT = (int) HOUR_START;

// Resets the REAL_VALS array to all 0 and the RDR_CNT to 0.
void cleanArray() {
  for(int c=0; c < MAX_LEN; c++) {
    READ_VALS[c] = 0;
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
    case 90:
      analogWrite(hour_out, numbers[valDat]);
      the_hr = valDat;
      break;
      
    case 91:
      analogWrite(minute_out, numbers[valDat + MINUTE_START]);
      the_mn = valDat;
      break;
      
    case 92:
      digitalWrite(digo, HIGH);
      break;
      
    case 93:
      digitalWrite(digo, LOW);
      break;

    case 99:
      diagnostic_mode = DIAG_MODE_ON;
      break;

    case 100:
      diagnostic_mode = DIAG_MODE_OFF;
      break;
      
    default:
      if (valCmd <= MINUTE_MAX && valCmd >= HOUR_START) {
        numbers[valCmd] = valDat;
      }
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

// In case bad or incomplete data is sent, clean out the buffer every 500 millis if any data.
void checkBuffer() {
  if (valCmd > -1 || valDat > -1) {
    if (buff_timer == 0) {
        buff_timer = millis() + 500;
    }
    
    if (millis() >= buff_timer) {
      cleanArray();
      valCmd = -1;
      valDat = -1;
    }
  }
}

void diagnosticIncrement(int* incr_val, const int* max_val, const int* start_val) {
  *incr_val += 1;
  
  if (*incr_val == *max_val) {
    *incr_val = (int) *start_val;
  }
}

void diagnosticDisplay(){
  analogWrite(hour_out, numbers[HOUR_COUNT]);
  analogWrite(minute_out, numbers[MINUTE_COUNT]);
}

void diagnosticCheck() {
  if (diagnostic_mode == DIAG_MODE_OFF) { return; }

  if (millis() >= diag_timer_count) {
    diag_timer_count = millis() + 2000;

    diagnosticIncrement(&MINUTE_COUNT, &MINUTE_MAX, &MINUTE_START);

    if (MINUTE_COUNT == (int) MINUTE_START) {
      diagnosticIncrement(&HOUR_COUNT, &HOUR_MAX, &HOUR_START);
    }

    diagnosticDisplay();
  }
}

void setup() {
  // Input
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  
  // 7-segment display
  pinMode(digo, OUTPUT);
  pinMode(hour_out, OUTPUT);
  pinMode(minute_out, OUTPUT);

  // Initialize the array
  cleanArray();

  // Set diagnostic mode on while the system boots
  diagnostic_mode = DIAG_MODE_ON;
  diag_timer_count = millis() + 1000;

  diagnosticDisplay();

  Serial.begin(9600);
  
  // Wait half a second
  delay(500);
}

void loop() {
  // Read data coming in from the pins
  readSerial();

  // Diagnostic mode
  diagnosticCheck();

  // Check for malformed data in the buffer, clean if needed.
  //checkBuffer();
}
