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

class light {
  private:
    int id;
    bool isOn;
    bool isReverse; // to check if the wiring is reverse or not

  public:
    // Default ctor for initializing objects will null or equivalent values
    light() {
      id = 0;
      isOn = false;
      isReverse = false;
    }

    // Parameterized ctor for initializing the object.
    light(int idVal, bool isOnVal, bool isReverseVal = false) : id(idVal), isOn(isOnVal), isReverse(isReverseVal) {
      pinMode(id, OUTPUT);
      if(!isReverse)
        if(isOn)
          digitalWrite(id, HIGH);
        else
          digitalWrite(id, LOW);

      else
        if(isOn)
          digitalWrite(id, LOW);
        else
          digitalWrite(id, HIGH);
    }

    void turnOn(bool toState) {
      if(isOn != toState) {
          if(toState)
            digitalWrite(id, isReverse ? LOW : HIGH);
          else
            digitalWrite(id, isReverse ? HIGH : LOW);
        isOn = toState;
      }
    }

    void status() {
      // Serial.print("Light ID: ");
      Serial.print(id);
      Serial.print(" ");
      Serial.println(isOn ? "ON" : "OFF");
    }

};

class button {
  private:
    int id;

    // following vars for read() function
    unsigned long lastPressedAtTime;
    bool lastButtonState;
    int currentState;

  public:
    button() {
      id = 0;
      lastPressedAtTime = 0;
      lastButtonState = LOW;
    }

    button(int idVal) : id(idVal) {
      pinMode(id, INPUT);
      lastPressedAtTime = 0;
      lastButtonState = LOW;
    }

    int read() {
      currentState = digitalRead(id);

      // if(currentState != lastButtonState) {
      //   lastPressedAtTime = millis();
      // }

      // if((millis() - lastPressedAtTime) > DEBOUNCE_VAL) {
      //   lastButtonState = currentState;
      //   return currentState;
      // }
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
  for(int i = 0; i < NO_LIGHTS; i++) {
    Light[i].status();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  doSomething();
}

void doSomething() {
  int buttonVal = Button.read();
  bool lightStateVal[NO_LIGHTS]; // State of light i

  if(buttonVal == HIGH)
    cycleState = ((cycleState + 1) % NO_STATES);

  switch(cycleState) {
    case 0:
      // default state
      // ON ON OFF
      lightStateVal[0] = true;
      lightStateVal[1] = true;
      lightStateVal[2] = false;
      break;

    case 1:
      // Mode 1
      // OFF OFF ON
      lightStateVal[0] = false;
      lightStateVal[1] = false;
      lightStateVal[2] = true;
      break;

    case 2:
      // Mode 2
      // ON OFF OFF
      lightStateVal[0] = true;
      lightStateVal[1] = false;
      lightStateVal[2] = false;
      break;

    case 3:
      // Mode 3
      // OFF ON OFF
      lightStateVal[0] = false;
      lightStateVal[1] = true;
      lightStateVal[2] = false;
      break;

    case 4:
      // Mode 4
      // ON ON ON
      lightStateVal[0] = true;
      lightStateVal[1] = true;
      lightStateVal[2] = true;
      break;
  }

  for(int i = 0; i < NO_LIGHTS; i++) {
    Light[i].turnOn(lightStateVal[i]);
    if(prevState != cycleState) {// Do not print in case state not changes
      Serial.print("Mode: ");
      Serial.print(cycleState);
      Serial.print(", ");
      Light[i].status();
    }
  }

  prevState = cycleState;
}
