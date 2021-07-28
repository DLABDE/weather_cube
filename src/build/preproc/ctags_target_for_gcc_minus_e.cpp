# 1 "c:\\Users\\ASUS\\Desktop\\weather_cube\\src\\src.ino"
# 2 "c:\\Users\\ASUS\\Desktop\\weather_cube\\src\\src.ino" 2
# 3 "c:\\Users\\ASUS\\Desktop\\weather_cube\\src\\src.ino" 2
# 4 "c:\\Users\\ASUS\\Desktop\\weather_cube\\src\\src.ino" 2
# 5 "c:\\Users\\ASUS\\Desktop\\weather_cube\\src\\src.ino" 2
# 6 "c:\\Users\\ASUS\\Desktop\\weather_cube\\src\\src.ino" 2
# 7 "c:\\Users\\ASUS\\Desktop\\weather_cube\\src\\src.ino" 2
# 8 "c:\\Users\\ASUS\\Desktop\\weather_cube\\src\\src.ino" 2
# 9 "c:\\Users\\ASUS\\Desktop\\weather_cube\\src\\src.ino" 2
# 10 "c:\\Users\\ASUS\\Desktop\\weather_cube\\src\\src.ino" 2

U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2((&u8g2_cb_r0),/* clock=*/ D3, /* data=*/ D2, /* cs=*/ 10, /* dc=*/ D0, /* reset=*/ D1);




void setup() {
    Serial.begin(9600);
    u8g2.begin();

    WiFiManager wifiManager;
    u8g2.drawStr(0,24,"ID:");u8g2.drawStr(10,34,"Weather Cube");u8g2.sendBuffer();
    u8g2.drawStr(0,46,"PW:");u8g2.drawStr(10,56,"dzd123456");u8g2.sendBuffer();
    wifiManager.autoConnect("Weather Cube","dzd123456");
    Serial.print("ESP8266 Connected to ");Serial.println(WiFi.SSID()); // WiFi名称
    Serial.print("IP address:\t");Serial.println(WiFi.localIP()); // IP
    u8g2.clear();
    u8g2.drawStr(0,24,"ID:");u8g2.drawStr(10,34,WiFi.SSID().c_str());u8g2.sendBuffer();
    u8g2.drawStr(0,46,"IP:");u8g2.drawStr(10,56,WiFi.localIP().toString().c_str());u8g2.sendBuffer();

    ArduinoOTA.setHostname("Weather Cube");
    ArduinoOTA.setPassword("dzd123456");
    ArduinoOTA.begin();

}

void loop() {
  ArduinoOTA.handle();
}
