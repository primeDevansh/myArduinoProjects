/*
 * Table Lights
 * 
 * Controller Code; To be uploaded on Arduino board.
 * 
 * My table consists of 3 lights (1 Yellow + 2 White).
 * The current setup is completely based on traditional electronic circuits.
 * I want to bring it on IOT (Internet of Things) using Arduino.
 * I want to control all the lights using a single button.
 * 
 */

#define NO_LEDS 3 

const int lights[NO_LEDS] = {8, 9, 10};
const int button = 11;

int cycleState = 0;

void turnLight(int light, bool val) {
  switch(light) {
    case 8:
    case 9:
      if(val == true)
        digitalWrite(light, HIGH);
      else
        digitalWrite(light, LOW);
      break;

    case 10:
      if(val == true)
        digitalWrite(light, LOW);
      else
        digitalWrite(light, HIGH);
      break;
  }
}

void setup() {
  // put your setup code here, to run once:

  // Begin Serial communication
  Serial.begin(9600);
  Serial.println("Serial communication established.");

  // Set the pin mode for each I/O
  for(int i = 0; i < NO_LEDS; i++) {
    pinMode(lights[i], OUTPUT);
  }
  pinMode(button, INPUT);

  // Present the Output pins
  turnLight(lights[0], true);  // default for light 1 is ON
  turnLight(lights[1], true);  // default for light 2 is ON
  turnLight(lights[2], false); // default for light 3 is OFF
}

void loop() {
  // put your main code here, to run repeatedly:

  int buttonVal = digitalRead(button);

  if(buttonVal == HIGH)
    cycleState++;

  switch(cycleState) {
    case 0:
      // default state
      // ON ON OFF
      turnLight(lights[0], true);
      turnLight(lights[1], true);
      turnLight(lights[2], false);
      break;

    case 1:
      // Mode 1
      // ON OFF OFF
      turnLight(lights[0], true);
      turnLight(lights[1], false);
      turnLight(lights[2], false);
      break;

    case 2:
      // Mode 2
      // OFF ON OFF
      turnLight(lights[0], false);
      turnLight(lights[1], true);
      turnLight(lights[2], false);
      break;

    case 3:
      // Mode 3
      // OFF OFF ON
      turnLight(lights[0], false);
      turnLight(lights[1], false);
      turnLight(lights[2], true);
      break;

    case 4:
      // Mode 4
      // ON ON ON
      turnLight(lights[0], true);
      turnLight(lights[1], true);
      turnLight(lights[2], true);
      break;

    default:
      // Mode RESET
      // cycleState = 0 again
      cycleState = 0;
  }

  delay(200);
}



















