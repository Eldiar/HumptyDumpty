// Importing Libraries
#include <WiFiEsp.h>
#include <SoftwareSerial.h>
#include <EMailSender.h>
//#include "Wire.h"

//Defining pin functions
SoftwareSerial EspSerial(2, 3); // RX, TX

#define SERIAL_BUFFER_SIZE 64

const PROGMEM int emergencyBtnPin = 8; //Digital
const PROGMEM int resetBtnPin = 10; //Digital

//creating of object emailSend, that sends emails, 
const EMailSender *emailSend;

// GYRO PINS// SDA pin = Analog4// SCL pin = Analog5
// Wifi Credentials
const PROGMEM char ssid[] = "AndroidAP";    // your network SSID (name)
const PROGMEM char pass[] = "skkj1168";        // your network password
int status = WL_IDLE_STATUS;                   // the Wifi radio's status

// Status variables for output
//bool emergencyStatus = false;

// ====================
//      Setup code
// ====================
void setup(){
  //initialize serial for debugging
  Serial.begin(9600);

  //initialize serial for ESP module
  EspSerial.begin(9600);

  //initialize ESP module
  WiFi.init(&EspSerial);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println(F("WiFi shield not present"));
    // don't continue
    while (true);

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print(F("Attempting to connect to WPA SSID:"));
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // Send wifi connection status message
  Serial.println(F("You're connected to the network"));

  //Button setup
  pinMode(emergencyBtnPin, INPUT);
  pinMode(resetBtnPin, INPUT);

  emailSend =  new EMailSender("smtp.michaelidesandreas1801@gmail.com", "tops3v3n");
  
  Serial.println(F("Buttons initalized"));
}
}

void loop()
{
 // -------------------
 // Button presses
 // -------------------

  // Emergency button press detection
  if (digitalRead(emergencyBtnPin) == HIGH) {
     emergency();
     //sendMessage(F("Emergency!"), F("I am in an emergency and am in need of assistance!"));
     digitalWrite(6, HIGH);
     Serial.println("Emergency");
  }
  
//   Reset button press detetion
  if (digitalRead(resetBtnPin) == HIGH) {
     reset();
     digitalWrite(6, LOW);
     Serial.println("I'M FINE");
  }
 
//  if (emergencyStatus == true){
//    digitalWrite(6, HIGH);
//  }

  delay(100);
}

 //Called when reset button is pressed
void reset(){
  //emergencyStatus = false; // Resets emergency status variable
  sendMessage(F("False Alarm"), F("Do not worry, the Humpty Dumpty device gave off a false alarm. I have been helped or been able to help myself."));
}

// Called when emergency button is pressed
void emergency(){
    sendMessage(F("Emergency!"), F("I am in an emergency and am in need of assistance!"));
    //emergencyStatus = true;
}



// Email Sending function
void sendMessage(const String& sbj, const String& msg){
  
  //EMailSender emailSend("smtp.michaelidesandreas1801@gmail.com", "password");
  EMailSender::EMailMessage message;
  message.subject = sbj;
  message.message = msg;
  
  EMailSender::Response resp = emailSend->send("michaelidesandreas1801@gmail.com", message);

  delete &message;
}
