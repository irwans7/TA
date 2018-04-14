\
/*
>> Pulse Sensor Amped 1.2 <<
This code is for Pulse Sensor Amped by Joel Murphy and Yury Gitman
    www.pulsesensor.com 
    >>> Pulse Sensor purple wire goes to Analog Pin 0 <<<
Pulse Sensor sample aquisition and processing happens in the background via Timer 2 interrupt. 2mS sample rate.
PWM on pins 3 and 11 will not work when using this code, because we are using Timer 2!
The following variables are automatically updated:
Signal :    int that holds the analog signal data straight from the sensor. updated every 2mS.
IBI  :      int that holds the time interval between beats. 2mS resolution.
BPM  :      int that holds the heart rate value, derived every beat, from averaging previous 10 IBI values.
QS  :       boolean that is made true whenever Pulse is found and BPM is updated. User must reset.
Pulse :     boolean that is true when a heartbeat is sensed then false in time with pin13 LED going out.

This code is designed with output serial data to Processing sketch "PulseSensorAmped_Processing-xx"
The Processing sketch is a simple data visualizer. 
All the work to find the heartbeat and determine the heartrate happens in the code below.
Pin 13 LED will blink with heartbeat.
If you want to use pin 13 for something else, adjust the interrupt handler
It will also fade an LED on pin fadePin with every beat. Put an LED and series resistor from fadePin to GND.
Check here for detailed code walkthrough:
http://pulsesensor.myshopify.com/pages/pulse-sensor-amped-arduino-v1dot1

Code Version 1.2 by Joel Murphy & Yury Gitman  Spring 2013
This update fixes the firstBeat and secondBeat flag usage so that realistic BPM is reported.

Environment Monitor modified for ESP8266

*/
#define BLYNK_PRINT Serial 
#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>
//#include <SimpleTimer.h>
#define temp1 D1

Ticker flipper;
// setup sensor
OneWire oneWire_1(temp1);
// berikan nama variabel,masukkan ke pustaka Dallas
DallasTemperature sensorSuhu1(&oneWire_1);
 
char auth[] = "84c5f6a7d8fa4e718a1e236306144d35"; 
//Ganti dengan WiFi dan Password anda
char ssid[] = "Irwan iphone";
char pass[] = "irwanaja";

//  VARIABLES
//SimpleTimer timer;
int pulsePin =0;
float suhuSekarang;
 
//int blinkPin = 15;                // pin to blink led at each beat
//int fadePin = 12;                 // pin to do fancy classy fading blink at each beat
//int fadeRate = 0;                 // used to fade LED on with PWM on fadePin


// these variables are volatile because they are used during the interrupt service routine!
volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, must be seeded! 
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.


void setup(){
  //pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
//  pinMode(fadePin,OUTPUT);          // pin that will fade to your heartbeat!
  Serial.begin(115200);             // we agree to talk fast!
  delay(10);
   Blynk.begin(auth, ssid, pass);
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS 
   // UN-COMMENT THE NEXT LINE IF YOU ARE POWERING The Pulse Sensor AT LOW VOLTAGE, 
   // AND APPLY THAT VOLTAGE TO THE A-REF PIN
   //analogReference(EXTERNAL);  
   //Blynk.begin(auth, ssid, pass);
   //timer.setInterval(1000, sendData);
   sensorSuhu1.begin();
}


 
float ambilSuhu()
{
   sensorSuhu1.requestTemperatures();
   float suhu = sensorSuhu1.getTempCByIndex(0);
   //return suhu;
}


void loop(){
  
 if (QS == true){                       // Quantified Self flag is true when arduino finds a heartbeat
       Serial.print("Heart rate: ");            
       Serial.println(BPM);
        
        QS = false;                      // reset the Quantified Self flag for next time    
     }
   
                            //  take a break
  Blynk.virtualWrite(0,BPM);
  suhuSekarang=ambilSuhu();
  Serial.println(suhuSekarang);
  Blynk.virtualWrite(1,suhuSekarang);
 
  Blynk.run();
 delay(100);
}











