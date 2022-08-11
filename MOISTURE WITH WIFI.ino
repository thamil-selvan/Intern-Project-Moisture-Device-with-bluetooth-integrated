#include "ThingSpeak.h"
#include <WiFi.h>
const byte pin = 25;
float val_ar[200],av=0;
int c = 0;
const char* ssid     = "Thamil Ph";
const char* password = "mmtt2002";


unsigned long myChannelNumber = 1799949;
const char * myWriteAPIKey = "6MI3FGB4GOARNX5M";


void setup()
{
  Serial.begin(9600);
  pinMode(pin, INPUT_PULLUP);
   Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    pinMode(pin, INPUT_PULLUP);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

}
void loop()
{
  unsigned long high = pulseInLong(pin, HIGH);
  unsigned long low = pulseInLong(pin, LOW);
  unsigned long duration = high + low;
  if (high > 0 && low > 0)
  {
    double cap=high*10/(log(2)*2.0);
    val_ar[c]=cap;
    c++;
    if(c>199){
      for(int i=0;i<200;i++){
        av+=val_ar[i];
      }
      av=av/200.0;
      
      float per=((10.28571428*av+2.5714286))/100.0;
      float dec=per-int(per);
      if(dec>=0.00 && dec<=0.25){
        per=int(per);
      }
      else if(dec>0.25 && dec<=0.75 ){
        per=int(per)+0.5;
      }
      else{
        per=int(per)+1;
      }
      Serial.print("Capacitance of the grain measured (in pF): ");
    Serial.print(av);
    Serial.print("  Moisture Percentage: ");
    Serial.println(per);
    c=0;
    ThingSpeak.writeField(myChannelNumber,1, per ,myWriteAPIKey);
    }

    
    // Serial.print("\tFrequency (Hz): ");
    // Serial.println(1000000.0 / duration);
  }
  else
  {
    c=0;
    Serial.println("No signal found.");
    delay(1000);
  }
  
  delay(5);
}