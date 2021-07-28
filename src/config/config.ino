/*
 * config
*/
#include <ESP8266WiFi.h>          
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 
#include <ArduinoOTA.h>
#include <FS.h>
#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>

//U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0,/* clock=*/ D3, /* data=*/ D2, /* cs=*/ 10, /* dc=*/ D0, /* reset=*/ D1);
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ D7, /* data=*/ D6, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display


#define id "Weather Cube"
#define pw "dzd123456"

#define flash_format
#define wifi_clear
//#define OTA

void setup() {
  Serial.begin(9600);   Serial.println("");
  u8g2.begin();  
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.drawStr(40,13,"config");u8g2.sendBuffer();

  // 格式化SPIFFS
  #ifdef flash_format
  Serial.println("SPIFFS format start");
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0,24,"format start");u8g2.sendBuffer();
  SPIFFS.format();    
  Serial.println("SPIFFS format finish");
  u8g2.drawStr(0,24,"format finish");u8g2.sendBuffer();
  #endif

  WiFiManager wifiManager;// 建立WiFiManager对象
  
  //初始化WIFI
  #ifdef wifi_clear
  wifiManager.resetSettings();// 清除ESP8266所存储的WiFi连接信息
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0,34,"wifi clear");u8g2.sendBuffer();
  Serial.println("ESP8266 WiFi Settings Cleared");
  #endif

  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0,48,"ID:");u8g2.drawStr(20,48,id);u8g2.sendBuffer();
  u8g2.drawStr(0,60,"PW:");u8g2.drawStr(25,60,pw);u8g2.sendBuffer();
  wifiManager.autoConnect(id,pw);                   //开启配置服务
  Serial.print("ESP8266 Connected to ");Serial.println(WiFi.SSID());              // WiFi名称
  Serial.print("IP address:\t");Serial.println(WiFi.localIP());                   // IP  
  
  u8g2.clear();
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.drawStr(40,13,"config");u8g2.sendBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0,24,"ID:");u8g2.drawStr(10,34,WiFi.SSID().c_str());u8g2.sendBuffer();
  u8g2.drawStr(0,46,"IP:");u8g2.drawStr(10,56,WiFi.localIP().toString().c_str());u8g2.sendBuffer();

  #ifdef OTA
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(90,64,"ota");u8g2.sendBuffer();
  ArduinoOTA.setHostname(id);
  ArduinoOTA.setPassword(pw);
  ArduinoOTA.begin();
  #endif
}
 
void loop() {
  #ifdef OTA
  ArduinoOTA.handle();
  #endif
}
