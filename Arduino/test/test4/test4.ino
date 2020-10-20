const PROGMEM int emergencyBtnPin = 8; //Digital
const PROGMEM int resetBtnPin = 10; //Digital
const int pinBuz = 6;
// Status variables for output
bool emergencyStatus = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(emergencyBtnPin, INPUT);
  pinMode(resetBtnPin, INPUT);
  
  pinMode(pinBuz,OUTPUT); //Defines pinBuz as an Output

  Serial.println(F("Buttons initalized"));

}

void loop() {
  // put your main code here, to run repeatedly:
int buttonState = digitalRead(emergencyBtnPin); // read new state

  Serial.println(buttonState);
  
  if (buttonState == HIGH) {
    //emergency();
    Serial.println("The button is being pressed");
    digitalWrite(pinBuz, HIGH); // turn on
    Serial.println("EMERGENCY");
  }
  else {
    Serial.println("The button is unpressed");
    digitalWrite(pinBuz, LOW);  // turn off
  }
  
   //Reset button press detetion
  if (digitalRead(resetBtnPin) == LOW) {
     //reset();
     digitalWrite(pinBuz, LOW);
     Serial.println("I'M FINE");
  }

  delay(1000);
}

void reset(){
  emergencyStatus = false; // Resets emergency status variable
  //sendMessage(F("False Alarm"), F("Do not worry, the Humpty Dumpty device gave off a false alarm. I have been helped or been able to help myself."));
}

// Called when emergency button is pressed
void emergency(){
    //sendMessage(F("Emergency!"), F("I am in an emergency and am in need of assistance!"));
    emergencyStatus = true;
}
