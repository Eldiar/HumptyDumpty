// Importing Libraries
#include <WiFiEsp.h>
#include <SoftwareSerial.h>
#include <EMailSender.h>
#include <Wire.h>

//Defining pin functions
SoftwareSerial EspSerial(2, 3); // RX, TX


//#define emergencyBtnPin 11
//#define resetBtnPin 10 //Digital
//#define buzzerPin 6 //Digital
//#define pulseSensorPin 3 //Analog

const PROGMEM int emergencyBtnPin = 11; //Digital
const PROGMEM int resetBtnPin = 10; //Digital
const PROGMEM int buzzerPin = 6; //Digital
const PROGMEM int pulseSensorPin = 3; //Analog

// GYRO PINS
// SDA pin = Analog4
// SCL pin = Analog5

// Wifi Credentials
const PROGMEM char ssid[] = "Network SSID";    // your network SSID (name)
const PROGMEM char pass[] = "12345678";        // your network password
int status = WL_IDLE_STATUS;                   // the Wifi radio's status

// Initialisation for GYRO
const PROGMEM int MPU_addr=0x68;               // I2C address of the MPU-6050

// Pulse detection variables


// Status variables for output
bool emergencyStatus = false;
boolean fall = false;       //stores if a fall has occurred
bool pulseMessageSent = false;
const EMailSender *emailSend;

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

  //setup for fall detection
   Wire.begin();
   Wire.beginTransmission(MPU_addr);
   Wire.write(0x6B);  // PWR_MGMT_1 register
   Wire.write(0);     // set to zero (wakes up the MPU-6050)
   Wire.endTransmission(true);
  }

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
  
  emailSend =  new EMailSender("smtp.michaelidesandreas1801@gmail.com", "password");
  
  Serial.println(F("Buttons initalized"));
}

// ====================
//      Main loop
// ====================

void loop()
{
 // -------------------
 // Button presses
 // -------------------

  // Emergency button press detection
  if (digitalRead(emergencyBtnPin) == HIGH) {
     emergency();
  }
  
  // Reset button press detetion
  if (digitalRead(resetBtnPin) == HIGH) {
     reset();
  }

 // -------------------
 // Pulse detection
 // -------------------

 if (checkPulse() == false){
  emergencyStatus = true;
 }

 // -------------------
 // Fall detection
 // -------------------

  // Check if a fall has been detected last loop
  if (fall==true){ //in event of a fall detection
    fallOccurance();
  }

 // -------------------
 // Buzzer
 // -------------------
 
  fall = FallDetected();
 
  if (emergencyStatus == true){
    digitalWrite(6, HIGH);
  }

  delay(100);
}

// ===================
// Functions
// ===================

// Check pulse
bool checkPulse(){
  int pulseSignal = analogRead(pulseSensorPin);             // Read the PulseSensor's value. 
                                                        // Assign this value to the "Signal" variable.
  //Serial.println(pulseSignal);                          // Send the Signal value to Serial Plotter.

  if (pulseSignal < 300){                 // Check if pulse is below lower treshhold
    return false;
  }
  if (pulseSignal > 700){                // Check if pulse exceeds higher treshold
    return false;
  }
  else{
    return true;
  }
}

// Called when checkPulse() == false
void wrongPulse(){
  if (pulseMessageSent == false){
  sendMessage(F("Missing Pulse!"), F("Something is wrong with my heartrate. Please send help!"));
  pulseMessageSent = true; //prevents message to be sent multiple times
  }
  
  
  emergencyStatus = true;  
}

// Called when reset button is pressed
void reset(){
  emergencyStatus = false; // Resets emergency status variable
  fall = false;
  pulseMessageSent = false; // Resets pulse message status variable
  sendMessage(F("False Alarm"), F("Do not worry, the Humpty Dumpty device gave off a false alarm. I have been helped or been able to help myself."));
}

// Called when emergency button is pressed
void emergency(){
    sendMessage(F("Emergency!"), F("I am in an emergency and am in need of assitance!"));
    emergencyStatus = true;
}

// Called when fall has been detected
void fallOccurance(){
   Serial.println(F("FALL DETECTED"));
   emergencyStatus = true;
   sendMessage(F("Fall Detected!"), F("I have fallen, if no reset message appears within a minute, please send help!")); 
   fall = false;
}

bool FallDetected(){
  
    int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
    float ax=0, ay=0, az=0, gx=0, gy=0, gz=0;
    
    //Fall detection variables
    boolean trigger1=false;     //stores if first trigger (lower threshold) has occurred
    boolean trigger2=false;     //stores if second trigger (upper threshold) has occurred
    boolean trigger3=false;     //stores if third trigger (orientation change) has occurred
    byte trigger1count=0;       //stores the counts past since trigger 1 was set true
    byte trigger2count=0;       //stores the counts past since trigger 2 was set true
    byte trigger3count=0;       //stores the counts past since trigger 3 was set true
    int angleChange=0;

    //Read Gyro Values
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
    AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
    AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L) 
    GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

     //2050, 77, 1947 are values for calibration of accelerometer
     // values may be different for you
     ax = (AcX-2050)/16384.00; // the accelerometer gives a value between 1 & 16384.00. 
                               //For the value of the accelerometer to be between the 0-1, the caliberation
                               //value is subracted from the position at the x/y/z axis and divided by the maximum value
     ay = (AcY-77)/16384.00;
     az = (AcZ-1947)/16384.00;
    
     //270, 351, 136 for gyroscope
     gx = (GyX+270)/131.07;
     gy = (GyY-351)/131.07;
     gz = (GyZ+136)/131.07;
    
     // calculating Amplitute factor for 3 axis
     float Raw_AM = pow(pow(ax,2)+pow(ay,2)+pow(az,2),0.5);
     int AM = Raw_AM * 10;  // as values are within 0 to 1, I multiplied 
                            // it by for using if else conditions 
    
     Serial.println(AM);
     //Serial.println(PM);
     //delay(500);
    
     if (trigger3==true){ //trigger3 = angle change 
        trigger3count++; //increment trigger 3
        //Serial.println(trigger3count);
        if (trigger3count>=10){ 
           angleChange = pow(pow(gx,2)+pow(gy,2)+pow(gz,2),0.5);// lenght of acceleration vector in 3 dimensions 
           //delay(10);
           Serial.println(angleChange); 
           
           if ((angleChange>=0) && (angleChange<=10)){ //if orientation changes remains between 0-10 degrees
               fall=true; trigger3=false; trigger3count=0;//reset triger count 
               Serial.println(angleChange);
                 }
                 
           else{ //user regained normal orientation
              trigger3=false; trigger3count=0;
              Serial.println(F("TRIGGER 3 DEACTIVATED"));
           }
         }
      }
    
     if (trigger2count>=6){ // breaking of upper threshold for acceleration //allow 0.5s for orientation change 
       trigger2=false; trigger2count=0;
       Serial.println(F("TRIGGER 2 DECACTIVATED"));
       }
     if (trigger1count>=6){ //lower threshold for acceleration //allow 0.5s for AM to break upper threshold 
       trigger1=false; trigger1count=0;
       Serial.println(F("TRIGGER 1 DECACTIVATED"));
       }
     if (trigger2==true){
       trigger2count++;
       //angleChange=acos(((double)x*(double)bx+(double)y*(double)by+(double)z*(double)bz)/(double)AM/(double)BM);
       angleChange = pow(pow(gx,2)+pow(gy,2)+pow(gz,2),0.5); //lenth og angle hage vector 
       Serial.println(angleChange);
       if (angleChange>=30 && angleChange<=400){ //if orientation changes by between 80-100 degrees
         trigger3=true; trigger2=false; trigger2count=0;
         Serial.println(angleChange);
         Serial.println(F("TRIGGER 3 ACTIVATED"));//activate notification 
           }
       }
     if (trigger1==true){
       trigger1count++;
       if (AM>=12){ //if AM breaks upper threshold (3g)
         trigger2=true;
        Serial.println(F("TRIGGER 2 ACTIVATED"));
         trigger1=false; trigger1count=0;
         }
       }
     if (AM<=2 && trigger2==false){ //if AM breaks lower threshold (0.4g)
       trigger1=true;
       Serial.println(F("TRIGGER 1 ACTIVATED"));
       }

  return fall; 
}

// Email Sending function
void sendMessage(String sbj, String msg){
  
 // EMailSender emailSend("smtp.michaelidesandreas1801@gmail.com", "password");
  EMailSender::EMailMessage message;
  message.subject = sbj;
  message.message = msg;
  
  EMailSender::Response resp = emailSend->send("michaelidesandreas1801@gmail.com", message);

  delete &message;
  
}
