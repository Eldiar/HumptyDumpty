#include <Blynk.h>

//#include <ESP8266_Lib.h>
#include <EMailSender.h>
#include <SoftwareSerial.h>
#define ESP8266_BAUD 9600

char auth[] = "6bae2d0e7bfa4b6096feeaffa2030e4f";
char ssid[] = "Enter wiFi Name"; //You can replace the wifi name to your wifi 

char pass[] = "Password";  //Type password of your wifi.

SoftwareSerial EspSerial(2, 3); // RX, TX
WidgetLCD lcd(V0);

ESP8266 wifi(&EspSerial);

const int buttonPin = 3;
int buttonState = 0; 

void setup(){
  
  Serial.begin(9600);
  EspSerial.begin(ESP8266_BAUD);
  pinMode(buttonPin, INPUT);
  
  
}

void loop() {
  
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    // send Message:
    sendMessage();
    Serial.println("Hello, World!");
    Serial.println("Sending status: ");
    Serial.println(resp.code);
    Serial.println(resp.desc);
    Serial.println(resp.status);
  }

  
  

}

void sendMessage(){
  
  EMailSender emailSend("smtp.michaelidesandreas1801@gmail.com", "password");
  EMailSender::EMailMessage message;
  message.subject = "Emergency!";
  message.message = "I have FALLEN DOWN/ I am in an emergency";
  
  EMailSender::Response resp = emailSend.send("michaelidesandreas1801@gmail.com", message);
}
