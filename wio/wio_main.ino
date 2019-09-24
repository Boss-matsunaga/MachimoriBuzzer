#include <TinyGPS++.h>
#include <WioLTEforArduino.h>
#include <stdio.h>
//#include <MsTimer2.h>

#define BUZZER_PIN (WIOLTE_D38)
#define BUTTON_PIN (WIOLTE_A6)
#define INTERVAL        (60000)
#define RECEIVE_TIMEOUT (10000)

TinyGPSPlus gps;
WioLTE Wio;

char data[100];
char url[200];
HardwareSerial* GpsSerial;
char GpsData[100];
char GpsDataLength;
char incoming;
int flag=0;

void GpsBegin(HardwareSerial* serial)
{
  GpsSerial = serial;
  GpsSerial->begin(9600);
  GpsDataLength = 0;
}

void GpsCheck() {
  incoming = GpsSerial->read();
  gps.encode(incoming);
 // if (gps.location.isUpdated()) {
        sprintf(url, "http://machimori.japanwest.cloudapp.azure.com/wio?lat=32.476392&lon=130.607272&parent_ID=P10000&buzzer_num=B10000&flag=%d",flag);
        SerialUSB.println(gps.location.lat());
        SerialUSB.println(gps.location.lng());
        SerialUSB.println(url);
//  }else{
//    SerialUSB.println(###ERROR.notGPS###);
//  }
  
  
  int r = Wio.HttpGet(url, data,sizeof(data),6000);
  SerialUSB.println(r);
  SerialUSB.println("END");
  SerialUSB.println(data);
  if(r==1){
    digitalWrite(BUZZER_PIN,HIGH);
  }
  SerialUSB.println("wio-end");
}



void setup() {
  SerialUSB.println("");
  SerialUSB.println("--- START ---------------------------------------------------");
  GpsBegin(&Serial);
  Wio.Init();
  Wio.PowerSupplyLTE(true);
  Wio.PowerSupplyGrove(true);
  delay(500);
  pinMode(BUZZER_PIN,OUTPUT);
  pinMode(BUTTON_PIN,INPUT);
  Wio.TurnOnOrReset();
  Wio.Activate("soracom.io", "sora", "sora");
  delay(500);
  SerialUSB.println("setup-end");
  //MsTimer2::set(INTERVAL,GpsCheck);
}


void loop() {
  int i = digitalRead(BUTTON_PIN);
  if(i == HIGH){ 
    digitalWrite(BUZZER_PIN,HIGH);
    Wio.LedSetRGB(0,255,0);
    delay(500);
    digitalWrite(BUZZER_PIN,LOW);
    delay(100);
    GpsCheck();
  }else{                                                                                                                                                                                                                                                                                                                                                                     
    Wio.LedSetRGB(255,0,0);
  }
}
