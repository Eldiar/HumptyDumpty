#include <ArduinoJson.h>

#include <TelegramBot.h>
//#include <TelegramKeyboard.h>

#include <WiFiEsp.h>
//#include <WiFiClient.h>
//#include <WiFiServer.h>
//#include <WiFiUdp.h>

//#include <ESP8266WiFi.h> 
#include <WiFiClientSecure.h> 
//#include <TelegramBot.h> 

#define LED 1 //led pin number
// Initialize Wifi connection to the router 
const char* ssid     = "xxxxx"; 
const char* password = "yyyyy"; 
// Initialize Telegram BOT 
const char BotToken[] = "xxxxxxxxx"; 
WiFiClient net_ssl; 
TelegramBot bot (BotToken, net_ssl); 
// the number of the LED pin   
void setup()  
{   
Serial.begin(115200);   
while (!Serial) {}  //Start running when the serial is open  
delay(3000);   
// attempt to connect to Wifi network:   
Serial.print("Connecting Wifi: ");   
Serial.println(ssid);   
while (WiFi.begin(ssid, password) != WL_CONNECTED)  
      {   
  Serial.print(".");   
  delay(500);   
}   
Serial.println("");   
Serial.println("WiFi connected");   
bot.begin();   
pinMode(LED, OUTPUT);   
}   
void loop()  
{   
message m = bot.getUpdates(); // Read new messages   
if (m.text.equals("on"))  
      {   
  digitalWrite(LED, 1);    
  bot.sendMessage(m.chat_id, "The Led is now ON");   
}   
else if (m.text.equals("off"))  
      {   
  digitalWrite(LED, 0);    
  bot.sendMessage(m.chat_id, "The Led is now OFF");   
}   
}   
