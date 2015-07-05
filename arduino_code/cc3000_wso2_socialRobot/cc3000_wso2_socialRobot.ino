
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <PubSubClient.h>
#include <utility/netapp.h>

#include <avr/pgmspace.h>

#define aref_voltage 3.3

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!

// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIVIDER);

#define WLAN_SSID       "chan_mobile"
#define WLAN_PASS       ""

// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_UNSEC

unsigned long aucDHCP = 14400;
unsigned long aucARP = 3600;
unsigned long aucKeepalive = 25;
unsigned long aucInactivity = 0;

const unsigned long
  connectTimeout  = 15L * 1000L, // Max time to wait for server connection
  responseTimeout = 15L * 1000L; // Max time to wait for data from server

Adafruit_CC3000_Client client = Adafruit_CC3000_Client();
   
union ArrayToIp {
  byte array[4];
  uint32_t ip;
};

uint32_t ip;
//uint32_t broker_ip = {83, 212, 96, 61}; //172.17.163.242
//ArrayToIp server = { 242, 163, 17, 172 };

#define sensor_pin 6

int led = 12;

PubSubClient mqttclient("iot.eclipse.org", 1883, callback, client);


//motor stuff:
#define TURN_DELAY 200

int motion_global = 0;

//motors
int motor_right[] = {9, 11};
int motor_left[] = {7, 8};

int enA=12;
int enB=13;
 


void callback (char* topic, byte* payload, unsigned int length) {
 
 //check for payload and set dance accordingly 
  if(payload[0]=='1') {
    Serial.println(F("short dance"));
    short_dance();
        
  }
      if(payload[0]=='2') {
        Serial.println(F("long dance"));
        long_dance();
        
      }
      if(payload[0]=='3')
        Serial.println("right");
      if(payload[0]=='4')
        Serial.println("stop");
        
  Serial.println(topic);
  Serial.write(payload, length);
  Serial.println("");
  
}

void setup(void)
{
  Serial.begin(9600);
  Serial.println(F("Hello, CC3000!\n"));
  
  pinMode(led, OUTPUT);
  
  // If you want to set the aref to something other than 5v
  analogReference(EXTERNAL);
  
  
  
  Serial.println(F("\nInitialising the CC3000 ..."));
  if (!cc3000.begin()) {
    Serial.println(F("Unable to initialise the CC3000! Check your wiring?"));
    for(;;);
  }

 
   if (netapp_timeout_values(&aucDHCP, &aucARP, &aucKeepalive, &aucInactivity) != 0) {
      Serial.println(F("Error setting inactivity timeout!"));
    }


  /* Attempt to connect to an access point */
  char *ssid = WLAN_SSID;             /* Max 32 chars */
  Serial.print(F("\nAttempting to connect to ")); Serial.println(ssid);
  
  /* NOTE: Secure connections are not available in 'Tiny' mode! */
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }
   
  Serial.println(F("Connected!"));
  
  /* Wait for DHCP to complete */
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP()) {
    delay(100); // ToDo: Insert a DHCP timeout!
  }
  
  
  
  delay(1000);

 Serial.println(F("connecting to the broker..."));
 
   // connect to the broker
   if (mqttclient.connect("cc3000", "", "")) {
     Serial.println(F("MQTT Connected"));
     mqttclient.subscribe("wso2con");
     digitalWrite(led, HIGH);
     delay(2000);
     digitalWrite(led, LOW);
   }
   
}

boolean error = false;

long time = millis();

int sense_counter  = 0;

void loop(void) {

  mqttclient.loop();
}


void long_dance() {
  digitalWrite(led, HIGH);
  drive_forward();
  turn_right();
  turn_left();
  drive_backward();
  delay(100);
  drive_forward();
  turn_right();
  turn_left();
  drive_backward();
  motor_stop();
  spin();
  delay(2000);
  digitalWrite(led, LOW);
  motor_stop();
}

void short_dance(){
  digitalWrite(led, HIGH);
  spin();
  delay(2000);
  digitalWrite(led, LOW);
  motor_stop();
}


void motor_stop(){
digitalWrite(motor_left[0], LOW); 
digitalWrite(motor_left[1], LOW); 

digitalWrite(motor_right[0], LOW); 
digitalWrite(motor_right[1], LOW);
unsigned long  motorStop= millis() + 25;  
while (!(motorStop<= millis())){
//delay 25ms
}
}

void drive_backward(){
//motor_stop();
digitalWrite(motor_left[0], LOW); 
digitalWrite(motor_left[1], HIGH); 

digitalWrite(motor_right[0], LOW); 
digitalWrite(motor_right[1], HIGH); 

delay(500);
motor_stop();
}

void drive_forward(){
//motor_stop();

digitalWrite(motor_left[0], HIGH); 
digitalWrite(motor_left[1], LOW); 

digitalWrite(motor_right[0], HIGH); 
digitalWrite(motor_right[1], LOW); 

delay(500);
motor_stop();
}

void turn_right(){
motor_stop();
digitalWrite(motor_left[0], LOW); 
digitalWrite(motor_left[1], HIGH); 
unsigned long  motorStop= millis() + TURN_DELAY;  
while (!(motorStop<= millis())){
//delay 300ms
}
motor_stop();

}

void turn_left(){
motor_stop();

digitalWrite(motor_right[0], LOW); 
digitalWrite(motor_right[1], HIGH); 
unsigned long  motorStop= millis() + TURN_DELAY;  
while (!(motorStop<= millis())){
//delay 300ms
}
motor_stop();
}

void spin() {
  digitalWrite(motor_left[0], LOW); 
  digitalWrite(motor_left[1], HIGH); 
  digitalWrite(motor_right[0], LOW); 
  digitalWrite(motor_right[1], HIGH); 
  
}

