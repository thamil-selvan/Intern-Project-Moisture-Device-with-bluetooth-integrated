//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

const byte pin = 25;
float val_ar[200],av=0;
int c = 0;
void setup()
{
  Serial.begin(115200);
  SerialBT.begin("Waycool_Moisture_device"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  pinMode(pin, INPUT_PULLUP);
  
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
  
  if(SerialBT.read()=='a'){
    SerialBT.println("56\n");
    Serial.println("Data sent");
  }
  delay(5);
}