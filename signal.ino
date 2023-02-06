// Global variables
int a = 400;
int b = 100;
int c = 9;
int d = 4500;
int countPulses = 0;

// Pin definitions
const short pinLedA = 15;
const short pinLedB = 21;
const short pinBtn1 = 22;
const short pinBtn2 = 23;

// Switch selected
short state1 = 1;
short state2 = 1;

// Button state
int btn1 = LOW;
int btn2 = LOW;
int oldBtn1 = 0;
int oldBtn2 = 0;

// Store previous counters of time
int currentMicros;
int previousMicros = 0;

void setup() {
  // Pin configuration
  pinMode(pinLedA, OUTPUT);
  pinMode(pinLedB, OUTPUT);
  pinMode(pinBtn1, INPUT_PULLUP);
  pinMode(pinBtn2, INPUT_PULLUP);
}

void updateBtnState() {
  // Read state of the buttons
  btn1 = digitalRead(pinBtn1);
  btn2 = digitalRead(pinBtn2);
  // Delay in between reads for stability
  delay(10);
  
  // When button 1 is pressed, we change state beetween 0 and 1
  if(btn1 && !oldBtn1) {
    state1 = (state1 + 1) % 2; 
    oldBtn1 = 1;
    
    // By pressing the button it reset the signal
    countPulses = 0;
    previousMicros = 0;
  }
  else if(!btn1 && oldBtn1) {
    oldBtn1 = 0;
  }

  // When button 2 is pressed, we change state beetween 0 and 1
  if(btn2 && !oldBtn2) {
    state2 = (state2 + 1) % 2; 
    oldBtn2 = 1;
    
    // By pressing the button it reset the signal
    countPulses = 0;
    previousMicros = 0;
  }
  else if(!btn2 && oldBtn2) {
    oldBtn2 = 0;
  }
}

void signal(int nbOfPulses = c) {
  // We use a value to count the duration of the pulses
  currentMicros = micros();

  // State 1
  if(currentMicros - previousMicros < (a + 50 * countPulses) * 1000) {
    digitalWrite(pinLedA, HIGH);
  }

  // State 0
  else if(currentMicros - previousMicros < (a + 50 * countPulses + b) * 1000) {
    digitalWrite(pinLedA, LOW);
  }

  // After c pulses we reset countPulses and we wait for d us
  else if(countPulses == (nbOfPulses - 1)) {
    if(currentMicros - previousMicros < d * 1000) {
      digitalWrite(pinLedA, LOW);
    }
    else {
      countPulses = 0;
      previousMicros = currentMicros;
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

  // When we press the button, the sequence is played
  if(state1) {
    // Reset Timer and enable stream of pulses,
    // if switch 2 is enable, we are in mode 1 with 3 pulses, 9 pulses otherwise
    if(state2) {
      signal(3);
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
