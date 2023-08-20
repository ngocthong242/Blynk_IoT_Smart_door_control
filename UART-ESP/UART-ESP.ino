//#include <SoftwareSerial.h>
//const byte Rx = 14; //d5
//const byte Tx = 12;//d6
//SoftwareSerial esp = SoftwareSerial(Rx, Tx);

#define BLYNK_TEMPLATE_ID "TMPLpelD1A94"
#define BLYNK_DEVICE_NAME "KHÓA CỬA IOT"
#define BLYNK_AUTH_TOKEN "zWMlHXHRYUaWdgd8rZDRQkIzb2Nl2HKO"
#define BLYNK_FIRMWARE_VERSION        "0.1.0"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define KHOA    V0
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Redmi Thong";
char pass[] = "1234567890";
long last = 0;
boolean bt1_state = HIGH;
//int p;
int trangthailed;
void Read_Uno(void);
WidgetLED led(V1);
BlynkTimer timer;
void blinkLedWidget()
{
  if (led.getValue())
  {
    led.off();
  }
  else
  {
    led.on();
  }
}
void setup()
{
  Serial.begin(9600);
 // esp.begin(9600);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, blinkLedWidget);
  last = millis();
}

void loop()
{
  Blynk.run();
  timer.run();
  Read_Uno();

}
void Read_Uno(void)
{
  if (Serial.available() > 0)
  {
    char In = Serial.read();
    if (In == '1')
    {
      trangthailed = 1;
      // Serial.println("LED on");
      Blynk.virtualWrite(V0, trangthailed);
    }
    else if (In == '0')
    {
      trangthailed = 0;
      //  Serial.println("LED off");
      Blynk.virtualWrite(V0, trangthailed);
    }
  }

}
BLYNK_WRITE(V0)
{
  int p = param.asInt();
  Serial.println(String(p));

}
