#include <UniversalTelegramBot.h>
#include <ESP8266WiFi.h>
//#include<WiFi.h>

#include <WiFiClientSecure.h>

//------- WiFi Settings -------
char ssid[] = "XXX";       // your network SSID (name)
char password[] = "XXX";  // your network key

// ------- Telegram config --------
#define BOT_TOKEN "XXXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXX"  // your Bot Token (Get from Botfather)

#define CHAT_ID "-XXXXXXXXXXX" // Chat ID of where you want the message to go (You can use MyIdBot to get the chat ID)

// SSL client needed
WiFiClientSecure client;

UniversalTelegramBot bot(BOT_TOKEN, client);

String ipAddress = "";


void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);


  Serial.println("START");
}

void loop() {

  Serial.println("ALERT");
  delay(2000);
  Serial.println("STILL ALERT");

  String message = "HELLOOO!";
  if(bot.sendMessage(CHAT_ID, message, "Markdown")){
    Serial.println("TELEGRAM Successfully sent");
  }
  else{
    Serial.println("TELEGRAM NOT Successfully sent");
  }

  delay(300000);

}
