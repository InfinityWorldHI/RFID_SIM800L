# RFID_SIM800L

# This code is for the RFID attendance system using any Arduino board with SIM800 or SIM900.

The instructions:
  * Include GPRS_SIM800_900 library into the Arduino IDE.
  * Open the RFID_SIM800_900 code and put your telecome host service (const char apn[] PROGMEM = {"**********"};) then compile it.
  * Download the website files to host it using a hosting service.
  
  The connections:
  
    *RFID:
      1- 3.3v to pin3.3v
      2- RST to pin9
      3- GND to GND
      4- MISO to pin12
      5- MOSI to pin11
      6- SCK to pin13
      7- SDA to pin10
      
    *LEDs Buzzer
      1- Red LED to pin8
      2- Green LED to pin7
      3- Buzzer tp pin6
      
    *SIM800L
      1- GND to GND of the Arduino and GND of the 3.7v battery(2Amps).
      2- Tx
      3- Rx
      4- Vcc to 3.7v of the 3.7v battery (2Amps).

SIM800L LED Status Indicators
 There is an LED on the top right side of the SIM800L Cellular Module which indicates the status of your cellular network. 
 It’ll blink at various rates to show what state it’s in:
 
 * Blink every 1s
     The module is running but hasn’t made connection to the cellular network yet.
 * Blink every 2s
     The GPRS data connection you requested is active.
 * Blink every 3s
     The module has made contact with the cellular network & can send/receive voice and SMS.
     
     
