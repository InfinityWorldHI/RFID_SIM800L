/* -----------------------------------------------------------------------------
  - Project: RFID attendance system using SIM800L
  - Author:  https://www.youtube.com/ElectronicsTechHaIs
  - Date:  25/09/2019
   -----------------------------------------------------------------------------
  This code created by Electronics Tech channel for 
  the RFID attendance project with arduino and SIM800L.
  Please read the comments for a better understanding.

  Be noticed, using this code the website must be hosted using a hosting
  service like 000webhost or InfinityFree or any web hosting service.
  
  The project with the SIM800/SIM900 library: https://github.com/InfinityWorldHI/RFID_SIM800L
   ---------------------------------------------------------------------------*/
//*******************************libraries***********************************
#include<SPI.h>
#include<MFRC522.h> 
#include<avr/pgmspace.h>
#include<SoftwareSerial.h>
#include<GPRS_Shield_Arduino.h>
//***************************************************************************
//RFID RC522
#define SS_PIN 10
#define RST_PIN 9
//LEDs Buzzer
#define RedLed_PIN 8  //Red LED for login
#define GreLed_PIN 7  //Green LED for logout
#define Buzzer_PIN 6
//SIM800L pins
#define SIM_Rx 2
#define SIM_Tx 3
#define BAUDRATE  9600
#define RETRY_COUNT 5
//***************************************************************************
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
GPRS gprs(SIM_Tx, SIM_Rx, BAUDRATE);
//***************************************************************************
String card_ID = ""; //card UID
String payload = ""; //Response from the website
//*********************** For SIM800L ***************************************
//TODO adjust this
const char apn[] PROGMEM = {"**********"}; //your telecom service name
const char link[100];
char responseBuffer[32];
//***************************************************************************
int i;
int card;
//***************************************************************************
void setup() {
  
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();        // Initialize SPI bus
  mfrc522.PCD_Init(); // Initialize MFRC522 card
  //--------------------------------------------------------
  Serial.println(F("Start HTTP request!\r\n"));
  //--------------------------------------------------------
  for (i = 0; i < RETRY_COUNT; i++) {
      Serial.println(F("Initializing ..."));
      if (gprs.init() != false) {
          break;
      }
      delay(500);
  }
  //--------------------------------------------------------
  if (i < RETRY_COUNT) {
      Serial.println(F("OK"));
  } 
  else {
      Serial.println(F("failed"));
      return;
  }
  //--------------------------------------------------------
  for (i = 0; i < RETRY_COUNT; i++) {
      Serial.println(F("Check if network is registered..."));
      if (gprs.isNetworkRegistered() != false) {
          break;
      }
      delay(1000);
  }
  //--------------------------------------------------------
  if (i < RETRY_COUNT) {
      Serial.println(F("OK"));
  } 
  else {
      Serial.println(F("failed"));
      return;
  }
  //--------------------------------------------------------
  Serial.println(F("opening bearer ..."));
  if (gprs.openBearer((const __FlashStringHelper *)apn) != false) {
      Serial.print(F("OK, ip address is: "));
      Serial.println(gprs.getIPAddress());
  } 
  else {
      Serial.println(F("failed"));
      return;
  }
  //--------------------------------------------------------
  Serial.println(F("initializing HTTP service ..."));
  if (gprs.httpInitialize() != false) {
      Serial.println(F("OK"));
  } 
  else {
      Serial.println(F("failed"));
      return;
  }
  //--------------------------------------------------------
  for (i = 0; i < RETRY_COUNT; i++) {
      Serial.println(F("sending HTTP GET request to http://yourWebsite/postdemo.php..."));
      if (gprs.httpSendGetRequest("http://yourWebsite/postdemo.php?test=test") != -1) {
          break;
      }   
  }
  //--------------------------------------------------------
  if (i < RETRY_COUNT) {
      Serial.println(F("OK"));
  } 
  else {
      Serial.println(F("failed"));
      return;
  }
  //--------------------------------------------------------
  Serial.println(F("fetching HTTP GET response..."));
  if (gprs.httpReadResponseData(responseBuffer, sizeof(responseBuffer)) != false) {
      Serial.println(F("OK, received data:"));
      Serial.println(responseBuffer);
      
  } 
  else {
      Serial.println(F("failed"));
      return;
  }
  closeHTTP();
  //Pins
  pinMode(RedLed_PIN, OUTPUT);
  pinMode(GreLed_PIN, OUTPUT);
  pinMode(Buzzer_PIN, OUTPUT);
}
//***************************************************************************
void loop() {
  
  //look for new card
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;//got to start of loop if there is no card present
  }
 // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;//if read card serial(0) returns 1, the uid struct contians the ID of the read card.
  }

  for (byte i = 0; i < mfrc522.uid.size; i++) {
      card_ID += mfrc522.uid.uidByte[i];
  }
  //Buzzer Turn ON
  tone(Buzzer_PIN, 800, 500);
  delay(100);
  noTone(Buzzer_PIN);

  int n = card_ID.length();
  char card[n + 1];
  strcpy(card, card_ID.c_str());
  
  sendDataSIM( card );
  
  Serial.println(card_ID);
  Serial.println(card);
  
  card_ID = "";
  digitalWrite(RedLed_PIN, LOW);
  digitalWrite(GreLed_PIN, LOW);
}
//***************************************************************************
void sendDataSIM( char cardID[] ){
  closeHTTP();
  delay(100);
  //--------------------------------------------------------
  for (i = 0; i < RETRY_COUNT; i++) {
      Serial.println(F("Initializing ..."));
      if (gprs.init() != false) {
          break;
      }
      delay(500);
  }
  //--------------------------------------------------------
  if (i < RETRY_COUNT) {
      Serial.println(F("OK"));
  } 
  else {
      Serial.println(F("failed"));
      return;
  }
  //--------------------------------------------------------
  for (i = 0; i < RETRY_COUNT; i++) {
      Serial.println(F("Check if network is registered..."));
      if (gprs.isNetworkRegistered() != false) {
          break;
      }
      delay(1000);
  }
  //--------------------------------------------------------
  if (i < RETRY_COUNT) {
      Serial.println(F("OK"));
  } 
  else {
      Serial.println(F("failed"));
      return;
  }
  //--------------------------------------------------------
  Serial.println(F("opening bearer ..."));
  if (gprs.openBearer((const __FlashStringHelper *)apn) != false) {
      Serial.print(F("OK, ip address is: "));
      Serial.println(gprs.getIPAddress());
  } 
  else {
      Serial.println(F("failed"));
      return;
  }
  //--------------------------------------------------------
  Serial.println(F("initializing HTTP service ..."));
  if (gprs.httpInitialize() != false) {
      Serial.println(F("OK"));
  } 
  else {
      Serial.println(F("failed"));
      return;
  }
  //--------------------------------------------------------
  sprintf( link, "http://yourWebsite/postdemo.php?CardID=%s", cardID );
  //--------------------------------------------------------
  for (i = 0; i < RETRY_COUNT; i++) {
      Serial.println( link );
      if (gprs.httpSendGetRequest( link ) != -1) {
          break;
      }   
  }
  //--------------------------------------------------------
  if (i < RETRY_COUNT) {
      Serial.println(F("OK"));
  } 
  else {
      Serial.println(F("failed"));
      return;
  }
  //--------------------------------------------------------
  Serial.println(F("fetching HTTP GET response ..."));
  if (gprs.httpReadResponseData(responseBuffer, sizeof(responseBuffer)) != false) {
      Serial.println(F("OK, received data:"));
      Serial.println(responseBuffer);
      payload = responseBuffer;
      Serial.println(payload);
      if (payload.substring(0, 5) == "login") {
        digitalWrite(RedLed_PIN, HIGH);
        String user_name = payload.substring(5);
        Serial.print("Welcome, ");
        Serial.println(user_name);
      }
      else if (payload.substring(0, 6) == "logout") {
        digitalWrite(GreLed_PIN, HIGH);
        String user_name = payload.substring(6);
        Serial.print("Good Bye, ");
        Serial.println(user_name);
      }
      else if(payload == "succesful" || payload == "Cardavailable"){
        digitalWrite(RedLed_PIN,HIGH);
        digitalWrite(GreLed_PIN,HIGH);
        delay(500);  
        digitalWrite(RedLed_PIN,LOW);
        digitalWrite(GreLed_PIN,LOW);
        delay(500);  
        digitalWrite(RedLed_PIN,HIGH);
        digitalWrite(GreLed_PIN,HIGH);
      }
  } 
  else {
      Serial.println(F("failed"));
      return;
  }
  closeHTTP();
  delay(100);
}
//***************************************************************************
void closeHTTP(){
  //--------------------------------------------------------    
  Serial.println(F("terminating HTTP service ..."));
  if (gprs.httpTerminate() != false) {
     Serial.println(F("OK"));
  } else {
      Serial.println(F("failed"));
      return;
  }
  //--------------------------------------------------------    
  Serial.println(F("closing bearer ..."));
  if (gprs.closeBearer() != false) {
      Serial.println(F("OK"));
  } else {
      Serial.println(F("failed"));
      return;
  }
}
//***************************************************************************
