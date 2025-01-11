/*
 * Table Lights
 * 
 * Controller Code
 *
 * An Object-Oriented Approach with memory functionality!
 * Last state of system is stored in EEPROM before a power off.
 *
 */
#include <EEPROM.h>

#define NO_LIGHTS 3
#define NO_STATES 5
#define POWER_DELAY 500  // To tackle power-hungry relay module
#define DEBOUNCE_VAL 200  // in ms
#define ON_STR " I " // What to display while giving status of LED when it is ON
#define OFF_STR " _ "
#define ADDR 0  // Memory address to read data from

class light {
  private:
    int id;
    bool isOn;
    bool isReverse; // to check if the wiring is reverse or not

  public:
    // Default ctor for initializing objects with null or equivalent values
    light() {
      id = 0;
      isOn = false;
      isReverse = false;
    }

    // Parameterized ctor for initializing the object with real values.
    light(int idVal, bool isOnVal, bool isReverseVal = false) : id(idVal), isOn(isOnVal), isReverse(isReverseVal) {
      pinMode(id, OUTPUT);
      digitalWrite(id, isReverse ? !isOn : isOn);
    }

    void turnOn(bool toState) {
      if(isOn != toState) {
        digitalWrite(id, isReverse ? !toState : toState);
        isOn = toState;
        delay(POWER_DELAY);
      }
    }

    void status(bool verbose = true) {
      // Serial.print("Light ID: ");
      if(verbose) {
        Serial.print(id);
        Serial.println(isOn ? ON_STR : OFF_STR);
      }
      else {
        Serial.print(isOn ? ON_STR : OFF_STR);
      }
    }
};

class button {
  private:
    int id;
    int currentState;

  public:
    button() {
      id = 0;
      currentState = LOW;
    }

    button(int idVal) : id(idVal) {
      pinMode(id, INPUT);
      currentState = digitalRead(id);
    }

    int read() {
      currentState = digitalRead(id);
      delay(DEBOUNCE_VAL);
      return currentState;
    }
};

// Global Variables

// Entities
light Light[NO_LIGHTS];
button Button;

// Knobs
bool showDebugInfo = true;

// Helper variables
int cycleState = 0; // Current state of our light setup; Each state gives the mode of the lights
int prevState = cycleState;
bool readedFromMemory = false;

void pickState();
void setState(int stateVal = cycleState);
void myDebugln(char*);
void myDebug(char*);
void statusAllLights();
void updateMemory();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Serial communication established!");

  // Give real values for entities
  Light[0] = light(8, true);
  Light[1] = light(9, true);
  Light[2] = light(10, false, true);

  // Initialze the button
  Button = button(11);

  // Give status for all lights
  statusAllLights();
}

void loop() {
  // put your main code here, to run repeatedly:
  doSomething();
}

void doSomething() {
  pickState();
  setState();
  // Give status for each light if state of the setup has changed
  if(cycleState != prevState) {
    // Write to memory only when state has changed
    updateMemory();
    // Print status only when state changes
    statusAllLights();
  }
  // Give current state's value to var prevState for it to store it
  prevState = cycleState;
}

void pickState() {
  int buttonVal = Button.read();
  myDebugln("Buttton value read – DONE");

  if(!readedFromMemory) {
    cycleState = EEPROM.read(ADDR);
    myDebugln("Historical state from memory read and updated – DONE");
    readedFromMemory = true;
  }

  if(buttonVal == HIGH) {
    myDebugln("Button pressed");
    cycleState = ((cycleState + 1) % NO_STATES);
    myDebugln("Cycle state var changed and updated – DONE");
  }
}

void setState(int stateVal = cycleState) {
  bool lightFate[NO_LIGHTS]; // Decide the light is on/off in a particular state
  // Decide what combination of lights should turn on at what state
  switch(stateVal) {
    case 0:
      // default state
      // ON ON OFF
      lightFate[0] = true;
      lightFate[1] = true;
      lightFate[2] = false;
      break;

    case 1:
      // Mode 1
      // OFF OFF ON
      lightFate[0] = false;
      lightFate[1] = false;
      lightFate[2] = true;
      break;

    case 2:
      // Mode 2
      // ON OFF OFF
      lightFate[0] = true;
      lightFate[1] = false;
      lightFate[2] = false;
      break;

    case 3:
      // Mode 3
      // OFF ON OFF
      lightFate[0] = false;
      lightFate[1] = true;
      lightFate[2] = false;
      break;

    case 4:
      // Mode 4
      // ON ON ON
      lightFate[0] = true;
      lightFate[1] = true;
      lightFate[2] = true;
      break;
  }

  // Command that light to turn on below
  for(int i = 0; i < NO_LIGHTS; i++) {
    Light[i].turnOn(lightFate[i]);
    myDebugln("Light fate changed");
  }
}

void updateMemory() {
  EEPROM.write(ADDR, cycleState);
    myDebugln("Written state change to memory");
}

void statusAllLights() {
  Serial.print("State: ");
  Serial.print(cycleState);
  Serial.print(" >> ");
  for(int i = 0; i < NO_LIGHTS; i++) { 
    Light[i].status(false);
  }
  Serial.println();
}

void myDebugln(char* str) {
  if(showDebugInfo == true)
    Serial.println(str);
}

void myDebug(char* str) {
  if(showDebugInfo)
    Serial.print(str);
}
