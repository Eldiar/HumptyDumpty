// Importing Libraries
#include <WiFiEsp.h>
#include <SoftwareSerial.h>
#include <EMailSender.h>

//Defining pin functions

SoftwareSerial EspSerial(2, 3); // RX, TX
const int emergencyBtnPin = ;
const int resetBtnPin = ;

// Wifi shield serial
#define ESP8266_BAUD 9600

// Wifi Credentials
char ssid[] = "Network SSID";    // your network SSID (name)
char pass[] = "12345678";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status


// Setup code

void setup(){
  //initialize serial for debugging
  Serial.begin(9600);

  //initialize serial for ESP module
  EspSerial.begin(ESP8266_BAUD);

  //initialize ESP module
  WiFi.init(&EspSerial);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // Send wifi connection status message
  Serial.println("You're connected to the network");

  //Button setup
  pinMode(emergencyBtnPin, INPUT);
  pinMode(resetBtnPin, INPUT);
  
  Serial.println("Buttons initalized");
}

void loop()
{
  emergencyState = digitalRead(emergencyBtnPin);
  resetState = digitalRead(resetBtnPin);

  // Emergency button press detection
  if (emergencyState == HIGH) {

    // send Emergency Message:
    emergencySubject = "Emergency!";
    emergencyMessage = "I am in an emergency and am in need of assitance!";
    sendMessage(emergencySubject, emergencyMessage);

  }

  // Reset button press detetion
  if (resetState == HIGH) {

    // send false alarm / reset Message:
    Subject = "False Alarm";
    Message = "Don't worry, the Humpty Dumpty device gave a false alarm or I have been helped / been able to help myself.";
    sendMessage(Subject, Message);

  }


}

// Email Sending function
void sendMessage(String sbj, String msg){
  
  EMailSender emailSend("smtp.michaelidesandreas1801@gmail.com", "password");
  EMailSender::EMailMessage message;
  message.subject = sbj;
  message.message = msg;
  
  EMailSender::Response resp = emailSend.send("michaelidesandreas1801@gmail.com", message);
}
