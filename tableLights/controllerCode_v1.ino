/*
 * Table Lights
 * 
 * Controller Code
 *
 * An Object-Oriented Approach
 *
 */
#define NO_LIGHTS 3
#define DEBOUNCE_VAL 200  // in ms
#define NO_STATES 5  // 0 to 4
#define ON_STR " I " // What char/str to display while giving status of LED when it is ON
#define OFF_STR " _ "

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

// Helper variables
int cycleState = 0; // Current state of our light setup; Each state gives the mode of the lights
int prevState = cycleState;

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
  statusAll();
}

void loop() {
  // put your main code here, to run repeatedly:
  doSomething();
}

void doSomething() {
  int buttonVal = Button.read();
  bool lightFate[NO_LIGHTS]; // Decide the fate of each light; whether it is off in a particular state or on

  if(buttonVal == HIGH)
    cycleState = ((cycleState + 1) % NO_STATES);

  // Decide what combination of lights should turn on at what state
  switch(cycleState) {
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
  }

  // Give status for each light if state of the setup has changed
  // Should also print for the initial state when button hasnt been pressed yet!
  if(cycleState != prevState) { // Do not print in case state has not changed (i.e., dont print anything unless button pressed)
    statusAll();
  }
  // Give current state's value to var prevState for it to store it
  prevState = cycleState;
}

void statusAll() {
  Serial.print("State: ");
  Serial.print(cycleState);
  Serial.print(" >> ");
  for(int i = 0; i < NO_LIGHTS; i++) { 
    Light[i].status(false);
  }
  Serial.println();
}
