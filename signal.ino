// Pin definitions
const unsigned short pinLedA = 15;
const unsigned short pinLedB = 21;
const unsigned short pinBtn1 = 22;
const unsigned short pinBtn2 = 23;

// Global variables
const unsigned short a = 400;
const unsigned short b = 100;
const unsigned short c = 9;
const unsigned short d = 4500;

// Count number of pulses in one waveform
unsigned char countPulses = 0;

// Switch selected
bool state1 = 1;
bool state2 = 1;

// Button state
unsigned short btn1 = LOW;
unsigned short btn2 = LOW;
bool oldBtn1 = 0;
bool oldBtn2 = 0;

// Remember that a button have been pressed to change the state
// only at the end of the waveform
bool pressed1 = 0;
bool pressed2 = 0;

// Store previous counters of time
unsigned int currentMicros;
unsigned int previousMicros = 0;

// A value to check if the waveform is playing now
bool isPlaying = 0;

// Pin configuration
void setup() {
  pinMode(pinLedA, OUTPUT);
  pinMode(pinLedB, OUTPUT);
  pinMode(pinBtn1, INPUT_PULLUP);
  pinMode(pinBtn2, INPUT_PULLUP);
}

// Check if the buttons have been pressed and change the states
void updateBtnState() {
  // Read state of the buttons
  btn1 = digitalRead(pinBtn1);
  btn2 = digitalRead(pinBtn2);
  // Delay in between reads for stability
  delay(10);
  
  // When button 1 is pressed, we change state beetween 0 and 1
  if(btn1 && !oldBtn1) { 
    oldBtn1 = 1;
    pressed1 = 1;
  }
  // If the waveform is finished, we change the state
  // if the button have been pressed
  if(!isPlaying && pressed1) {
    state1 = (state1 + 1) % 2;
    pressed1 = 0;
  }
  else if(!btn1 && oldBtn1) {
    oldBtn1 = 0;
  }

  // When button 2 is pressed, we change state beetween 0 and 1
  if(btn2 && !oldBtn2) {
    oldBtn2 = 1;
    pressed2 = 1;
  }
  // If the waveform is finished, we change the state
  // if the button have been pressed
  if(!isPlaying && pressed2) {
    state2 = (state2 + 1) % 2;
    pressed2 = 0;
  }
  else if(!btn2 && oldBtn2) {
    oldBtn2 = 0;
  }
}

// Function that emmit the signal
void signal(int nbOfPulses = c) {
  // We use a value to count the duration of the pulses
  currentMicros = micros();

  // Signal B
  if(currentMicros - previousMicros < 50 * 1000 && countPulses == 0) {
    digitalWrite(pinLedB, HIGH);
    digitalWrite(pinLedA, LOW);
    isPlaying = true;
  }

  // Signal A
  // 1
  else if(currentMicros - previousMicros < 50 + (a + 50 * countPulses) * 1000) {
    digitalWrite(pinLedB, LOW);
    digitalWrite(pinLedA, HIGH);
    isPlaying = true;
  }

  // 0
  else if(currentMicros - previousMicros < 50 + (a + 50 * countPulses + b) * 1000) {
    digitalWrite(pinLedB, LOW);
    digitalWrite(pinLedA, LOW);
    isPlaying = true;
  }

  // After c pulses we reset countPulses and we wait for d us
  else if(countPulses == (nbOfPulses - 1)) {
    if(currentMicros - previousMicros < d * 1000) {
      digitalWrite(pinLedB, LOW);
      digitalWrite(pinLedA, LOW);
      isPlaying = true;
    }
    else {
      countPulses = 0;
      previousMicros = currentMicros;
      isPlaying = false;
    }
  }

  // Next pulse
  else {
    countPulses++;
    previousMicros = currentMicros;
  }
}

void loop() {
  // Check state of the buttons
  updateBtnState();

  // When we press the button 1, the sequence is played
  if(state1) {
    // Reset Timer and enable stream of pulses,
    // if switch 2 is enable, we are in mode 1 with 3 pulses, 9 pulses otherwise
    if(state2) {
      signal(6);
    }
    else {
      signal();
    }
  }
  else {
    // Disable stream of pulses
    digitalWrite(pinLedA, LOW);
  }
}
