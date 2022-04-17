#line 1 "e:\\arduino\\code\\code\\weather_cube\\src\\config\\build\\sketch\\sketch_jun03a.ino.cpp"
#include <Arduino.h>
#line 1 "c:\\Users\\ASUS\\Desktop\\weather_cube\\sketch_jun03a\\sketch_jun03a.ino"
#line 1 "c:\\Users\\ASUS\\Desktop\\weather_cube\\sketch_jun03a\\sketch_jun03a.ino"
void setup();
#line 5 "c:\\Users\\ASUS\\Desktop\\weather_cube\\sketch_jun03a\\sketch_jun03a.ino"
void loop();
#line 1 "c:\\Users\\ASUS\\Desktop\\weather_cube\\sketch_jun03a\\sketch_jun03a.ino"
void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("ADC Value: ");Serial.println(map(analogRead(A0),0,1024,0,330)/100.0);
  delay(300);
}
