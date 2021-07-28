#line 1 "c:\\Users\\ASUS\\Desktop\\weather_cube\\src\\src.ino"
#include <ESP8266WiFi.h>          
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 
#include <ArduinoOTA.h>
#include <FS.h>
#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0,/* clock=*/ D3, /* data=*/ D2, /* cs=*/ 10, /* dc=*/ D0, /* reset=*/ D1);

#define id "Weather Cube"
#define pw "dzd123456"

#line 16 "c:\\Users\\ASUS\\Desktop\\weather_cube\\src\\src.ino"
void setup();
#line 36 "c:\\Users\\ASUS\\Desktop\\weather_cube\\src\\src.ino"
void loop();
#line 16 "c:\\Users\\ASUS\\Desktop\\weather_cube\\src\\src.ino"
void setup() {
    Serial.begin(9600);
    u8g2.begin();
           
    WiFiManager wifiManager;
    u8g2.drawStr(0,24,"ID:");u8g2.drawStr(10,34,id);u8g2.sendBuffer();
    u8g2.drawStr(0,46,"PW:");u8g2.drawStr(10,56,pw);u8g2.sendBuffer();
    wifiManager.autoConnect(id,pw);
    Serial.print("ESP8266 Connected to ");Serial.println(WiFi.SSID());              // WiFi名称
    Serial.print("IP address:\t");Serial.println(WiFi.localIP());           // IP
    u8g2.clear();
    u8g2.drawStr(0,24,"ID:");u8g2.drawStr(10,34,WiFi.SSID().c_str());u8g2.sendBuffer();
    u8g2.drawStr(0,46,"IP:");u8g2.drawStr(10,56,WiFi.localIP().toString().c_str());u8g2.sendBuffer();

    ArduinoOTA.setHostname(id);
    ArduinoOTA.setPassword(pw);
    ArduinoOTA.begin();
    
}
 
void loop() {
  ArduinoOTA.handle();
}

