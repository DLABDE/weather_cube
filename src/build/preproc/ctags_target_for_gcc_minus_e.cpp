# 1 "e:\\arduino\\code\\code\\weather_cube\\src\\src.ino"
# 2 "e:\\arduino\\code\\code\\weather_cube\\src\\src.ino" 2

# 4 "e:\\arduino\\code\\code\\weather_cube\\src\\src.ino" 2
# 5 "e:\\arduino\\code\\code\\weather_cube\\src\\src.ino" 2

# 7 "e:\\arduino\\code\\code\\weather_cube\\src\\src.ino" 2
# 8 "e:\\arduino\\code\\code\\weather_cube\\src\\src.ino" 2
# 9 "e:\\arduino\\code\\code\\weather_cube\\src\\src.ino" 2
# 10 "e:\\arduino\\code\\code\\weather_cube\\src\\src.ino" 2
# 11 "e:\\arduino\\code\\code\\weather_cube\\src\\src.ino" 2
# 12 "e:\\arduino\\code\\code\\weather_cube\\src\\src.ino" 2

# 14 "e:\\arduino\\code\\code\\weather_cube\\src\\src.ino" 2
# 15 "e:\\arduino\\code\\code\\weather_cube\\src\\src.ino" 2

# 17 "e:\\arduino\\code\\code\\weather_cube\\src\\src.ino" 2
//#include <ArduinoOTA.h>        

//网络时钟API参数(来源可能不稳定，可以更改稳定源)
const char* host = "http://quan.suning.com/getSysTime.do";
const char* sig="/";

//配置CUBE参数



//心知天气API参数
String reqUserKey = "SqXFTWdyiyNMx4dBr"; // 私钥(请使用自己的私匙)
String reqLocation = "ip"; // 城市
String reqUnit = "c"; // 摄氏/华氏

//OLED显示屏配置(1306-128*64-F)
//U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0,/* clock=*/ D3, /* data=*/ D2, /* cs=*/ 10, /* dc=*/ D0, /* reset=*/ D1);
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2((&u8g2_cb_r0), /* clock=*/ D7, /* data=*/ D6, /* reset=*/ 255); // All Boards without Reset of the Display



//系统数据结构体
struct system_msg
{
    float bat; //电池信息
}sys_msg;

//天气信息结构体
struct weathwer_msg
{
    String sercode; //服务器代码
    int weacode; //天气代码
    String wea; //天气
    int degree; //温度(C)
    String dress; //穿衣建议
    String facf; //流感建议
    String exer; //运动建议
    String uv; //紫外线建议
    //String lastime;   //上次更新时间

    int daycode[3]; //白天天气
    String daytxt[3];
    int nightcode[3]; //夜晚天气
    String nighttxt[3];
    int high[3]; //最高温
    int low[3]; //最低温
    float rain[3]; //降水概率
    String winddir[3]; //风向
    int windsca[3]; //风力
    int hum[3]; //湿度
}wea_msg;

//时间结构体
struct time_now
{
    int year;
    int mon;
    int day;

    int hour;
    int min;
    int sec;
}time_msg,time_old;

//心知天气对象
WeatherNow weatherNow;
LifeInfo lifeInfo;
Forecast forecast;

//定时器
Ticker tickersec;// 建立一秒定时器
unsigned int count=1;
unsigned int state = 0;


//开机动画
void open_face()
{
    Serial.println("open_face");
    u8g2.clear();
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(20,20,"Weather");
    u8g2.drawStr(35,45,"Cube");
    u8g2.sendBuffer();
    delay(200);

    u8g2.setFont(u8g2_font_open_iconic_weather_1x_t); //字体字集
    for (int i = 0,k=0; i < 6; i++,k+=20)
    {
        u8g2.drawGlyph(k,60,0x41);u8g2.sendBuffer();
        delay(200);
    }
    delay(300);
}

//网络连接初始化
void web_connect_config()
{
    digitalWrite(2,0x0);
    WiFiManager wifiManager;
    u8g2.clear();
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(40,13,"config");u8g2.sendBuffer();Serial.println("web_config");
    u8g2.setFont(u8g2_font_ncenR10_tr);
    u8g2.drawStr(0,30,"ID:");u8g2.drawStr(20,42,"Weather Cube");u8g2.sendBuffer();
    u8g2.drawStr(0,52,"PW:");u8g2.drawStr(25,64,"dzd123456");u8g2.sendBuffer();
    wifiManager.autoConnect("Weather Cube","dzd123456"); //开启wifi连接服务
    Serial.print("ESP8266 Connected to ");Serial.println(WiFi.SSID()); // WiFi名称
    Serial.print("IP address:\t");Serial.println(WiFi.localIP()); // IP
    u8g2.clearBuffer();
    //u8g2.clear();
    u8g2.drawStr(0,30,"ID:");u8g2.drawStr(10,42,WiFi.SSID().c_str());u8g2.sendBuffer();
    u8g2.drawStr(0,52,"IP:");u8g2.drawStr(10,64,WiFi.localIP().toString().c_str());u8g2.sendBuffer();
    digitalWrite(2,0x1);
}

//更新电池信息(百分比)
float updata_bat()
{
    float bat=0;
    for(int i=0;i<=5;i++)
        bat+=analogRead(A0);
    return ((map(bat/5.0,0,1024,0,330))-267.0)/(345.0-267.0)*100;
}

//系统初始化
void system_config()
{
    Serial.begin(115200);Serial.println("system_config");
    u8g2.begin();
    open_face();

    pinMode(2, 0x01);
    //pinMode(D3, OUTPUT);
    //digitalWrite(D3,LOW); 
    pinMode(D5 /*按键，高电平触发*/,0x00);

    tickersec.attach(1,onesec_tick);
}

void setup()
{
    system_config();
    web_connect_config();

    // 配置心知天气请求信息
    weatherNow.config(reqUserKey, reqLocation, reqUnit);
    forecast.config(reqUserKey, reqLocation, reqUnit);
    lifeInfo.config(reqUserKey, reqLocation, reqUnit);

    get_time();
    get_weather(0);
    sys_msg.bat=updata_bat();

    u8g2.clear();
}

void time_face()
{
    if (time_old.sec!=time_msg.sec) //时钟
    {
        u8g2.setFont(u8g2_font_courB24_tn);
        u8g2.setCursor(15,55);
        time_msg.hour<10?u8g2.print("0"):0;u8g2.print(time_msg.hour);
        time_msg.sec%2==0?u8g2.print(":"):u8g2.print(".");
        time_msg.min<10?u8g2.print("0"):0;u8g2.print(time_msg.min);
        u8g2.sendBuffer();
        //u8g2.clearBuffer();
    }
    u8g2.drawHLine(0,15,128);//分割线

    //小天气
    u8g2.setFont(u8g2_font_open_iconic_weather_1x_t);//8层高
    if (wea_msg.weacode==0 ||wea_msg.weacode==2)//晴天,白
        u8g2.drawGlyph(60,12,0x45);
    else if (wea_msg.weacode==1 ||wea_msg.weacode==3)//晴天,夜
        u8g2.drawGlyph(60,12,0x42);
    else if (wea_msg.weacode==4 ||wea_msg.weacode==9)//多云
        u8g2.drawGlyph(60,12,0x40);
    else if (wea_msg.weacode>=10 && wea_msg.weacode<=25)//下雨
        u8g2.drawGlyph(60,12,0x43);
    else
        u8g2.drawGlyph(60,12,0x41);

    u8g2.sendBuffer();

    //小温度
    u8g2.setFont(u8g2_font_courB08_tr);
    u8g2.setCursor(100,12);
    u8g2.print(wea_msg.degree);u8g2.print("C");

    //小日期
    u8g2.setCursor(5,12);
    u8g2.print(time_msg.mon);u8g2.print("/");u8g2.print(time_msg.day);
    u8g2.sendBuffer();

}
void wea_face()
{
    //u8g2.clearBuffer();
    //温度范围
    u8g2.setFont(u8g2_font_courB08_tr);
    u8g2.setCursor(10,11);
    u8g2.print(wea_msg.low[0]);u8g2.print("-");u8g2.print(wea_msg.high[0]);u8g2.print("C");

    //降水概率,相对湿度
    u8g2.setFont(u8g2_font_open_iconic_weather_1x_t);u8g2.drawGlyph(52,11,0x43);
    u8g2.setFont(u8g2_font_courB08_tr);
    u8g2.setCursor(60,11);
    u8g2.print(wea_msg.rain[0]);u8g2.print("%");
    u8g2.setCursor(100,11);
    u8g2.print(wea_msg.hum[0]);u8g2.print("%");

    u8g2.sendBuffer();


    //大天气图标
    if (wea_msg.weacode==0 || wea_msg.weacode==2 || wea_msg.weacode==38)
        u8g2.drawXBMP( 7 , 20 , 45 , 45 , sun );
    else if (wea_msg.weacode==1 || wea_msg.weacode==3)
        u8g2.drawXBMP( 7 , 20 , 45 , 46 ,Clear);
    else if (wea_msg.weacode==13)
        u8g2.drawXBMP( 5 , 20 , 47 , 45 , LightRain );
    else if (wea_msg.weacode>=14 && wea_msg.weacode<=18)
        u8g2.drawXBMP( 5 , 20 , 47 , 45 , rain );
    else if (wea_msg.weacode==4)
        u8g2.drawXBMP( 5 , 25 , 47 , 37 , cloudy );
    else if (wea_msg.weacode==9)
        u8g2.drawXBMP( 5 , 25 , 47 , 31 , Overcast );
    else if (wea_msg.weacode==5 || wea_msg.weacode==7)
        u8g2.drawXBMP( 5 , 25 , 47 , 33 , suncloudy );
    else if (wea_msg.weacode==10)
        u8g2.drawXBMP( 5 , 20 , 46 , 45 , Shower );
    else if (wea_msg.weacode==11)
        u8g2.drawXBMP( 5 , 20 , 45 , 45 , Thundershower );
    else if (wea_msg.weacode==30)
        u8g2.drawXBMP( 5 , 17 , 47 , 44 , Foggy);
    else if (wea_msg.weacode==31)
        u8g2.drawXBMP( 5 , 17 , 47 , 42 , Haze);
    else if (wea_msg.weacode==99)
        u8g2.drawXBMP( 5 , 25 , 47 , 20 , Unknown);

    u8g2.sendBuffer();

    //天气名称
    u8g2.setFont(u8g2_font_ncenB12_tr);
    u8g2.drawStr(50,28,wea_msg.wea.c_str());

    //大温度
    u8g2.setFont(u8g2_font_ncenB24_tr);
    u8g2.setCursor(65,63);
    u8g2.print(wea_msg.degree);u8g2.print("C");

    u8g2.sendBuffer();
}

void sys_face()
{
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(45,12,"MSG");

    u8g2.setFont(u8g2_font_courB08_tr);
    u8g2.setCursor(5,25);
    u8g2.print("bat:");u8g2.print(sys_msg.bat);u8g2.print("%");


    u8g2.sendBuffer();
}


void (*func_table[3])() = {time_face,wea_face,sys_face};//指向函数的指针数组
int page=0;

void loop()
{
    if (digitalRead(D5 /*按键，高电平触发*/)==0x1)//按键按下
    {
        state=count;
        if (count-state<=30)
        {
            if (page<3 -1)page++;
            else page=0;
            u8g2.clear();
        }
    }
    if (count-state<=30)//刷新界面
    {
        (*func_table[page])();
    }
    if (count%240==0)//天气内容更新
    {
        get_weather(0);
    }
    if (count%30==0)//电池信息更新
    {
        sys_msg.bat=sys_msg.bat=updata_bat();
    }

}

//获取天气信息
void get_weather(int a)
{
    digitalWrite(2,0x0);Serial.println("get_weather");
    if(weatherNow.update() || a==0) // 更新当前天气信息
    {
        // 心知天气服务器错误代码说明可通过以下网址获取
        // https://docs.seniverse.com/api/start/error.html
        wea_msg.sercode=weatherNow.getServerCode();
        wea_msg.weacode=weatherNow.getWeatherCode();
        wea_msg.wea=weatherNow.getWeatherText();
        wea_msg.degree=weatherNow.getDegree();
        //wea_msg.lastime=weatherNow.getLastUpdate(); 
    }
    if (forecast.update() || a==1) //更新未来天气信息
    {
        for (char i = 0; i < 3; i++)
        {
            wea_msg.sercode=forecast.getServerCode();
            wea_msg.daycode[i]=forecast.getDayCode(i);
            wea_msg.daytxt[i]=forecast.getDayText(i);
            wea_msg.nightcode[i]=forecast.getNightCode(i);
            wea_msg.nighttxt[i]=forecast.getNightText(i);
            wea_msg.high[i]=forecast.getHigh(i);
            wea_msg.low[i]=forecast.getLow(i);
            wea_msg.rain[i]=forecast.getRain(i);
            wea_msg.winddir[i]=forecast.getWindDirection(i);
            wea_msg.windsca[i]=forecast.getWindScale(i);
            wea_msg.hum[i]=forecast.getHumidity(i);
        }
    }
    if (lifeInfo.update() || a==2) //更新生活指数信息
    {
        wea_msg.dress=lifeInfo.getDressing();
        wea_msg.facf=lifeInfo.getFactorFlu();
        wea_msg.exer=lifeInfo.getExercise();
        wea_msg.uv=lifeInfo.getUV();
    }
    digitalWrite(2,0x1);
}

//获取网络时间
void get_time()
{
    digitalWrite(2,0x0);Serial.println("get_time");
    /*

    WiFiClient client;

    String httpRequest = String("GET ") +sig+ " HTTP/1.1\r\n" + 

                                "Host: " + host + "\r\n" + 

                                "Connection: close\r\n\r\n";

    if (client.connect(host, 80))

    {

        client.print(httpRequest);// 向服务器发送http请求信息

        String status_response = client.readStringUntil('\n');// 获取并服务器响应状态行 

        Serial.print("status_response: ");Serial.println(status_response);

        client.find("\r\n\r\n");// 使用find跳过HTTP响应头

        client.find("datetime_1\":\"");

        time_read(client.readString());      //解析时间

    }

    else {Serial.println(" connection failed!");}   

    client.stop(); //断开客户端与服务器连接工作

    digitalWrite(LED_BUILTIN,HIGH);

    */
# 380 "e:\\arduino\\code\\code\\weather_cube\\src\\src.ino"
   HTTPClient http;
   http.setTimeout(5000);
   http.begin(host);
   int httpCode = http.GET();
   if (httpCode > 0)
   {
       Serial.printf("[HTTP] GET... code: %d\n", httpCode);
       if (httpCode == HTTP_CODE_OK)
       {
           String response = http.getString();
           Serial.println(response);
           //Serial.println(response.substring(13, 23));
           time_read(response.substring(13, 31));
       }
   }
}

//网络时间解析
void time_read(String timea)//2021-06-05 14:05:17
{
    time_msg.year=timea.substring(0,5).toInt();
    //Serial.println(timea.substring(0, 5));
    time_msg.mon=timea.substring(5,8).toInt();
    time_msg.day=timea.substring(8,11).toInt();
    time_msg.hour=timea.substring(11,14).toInt();
    time_msg.min=timea.substring(14,17).toInt();
    time_msg.sec=timea.substring(17,20).toInt();
}

//定时器(1s)
void onesec_tick()
{
    count++;

    //更新页码信息
    if (count%10==0)
    {
        page++;
        u8g2.clear();
        if (page>3 -1)page=0;
    }

    //时间更新
    time_old.sec=time_msg.sec;
    time_old.min=time_msg.min;
    time_old.hour=time_msg.hour;
    //time_old.day=time_msg.day;
    //time_old.mon=time_msg.mon;
    //time_old.year=time_msg.year;
    if (time_msg.sec==59)
    {
        time_msg.sec=0;
        if (time_msg.min==59)
        {
            time_msg.min=0;
            if (time_msg.hour==23)
            {
                time_msg.hour=0;
                //get_time();
            }else{time_msg.hour+=1;}
        }else{time_msg.min+=1;}
    }else{time_msg.sec+=1;}
}


/*

0   晴（国内城市白天晴）    Sunny

1   晴（国内城市夜晚晴）    Clear

2   晴（国外城市白天晴）    Fair

3   晴（国外城市夜晚晴）    Fair

4   多云                 Cloudy

5   晴间多云            Partly Cloudy

6   晴间多云            Partly Cloudy

7   大部多云            Mostly Cloudy

8   大部多云            Mostly Cloudy

9   阴                  Overcast

10  阵雨                Shower

11  雷阵雨              Thundershower

12  雷阵雨伴有冰雹       Thundershower with Hail

13  小雨                Light Rain

14  中雨                Moderate Rain

15  大雨                Heavy Rain

16  暴雨                Storm

17  大暴雨              Heavy Storm

18  特大暴雨            Severe Storm

19  冻雨                Ice Rain

20  雨夹雪              Sleet

21  阵雪                Snow Flurry

22  小雪                Light Snow

23  中雪                Moderate Snow

24  大雪                Heavy Snow

25  暴雪                Snowstorm

26  浮尘                Dust

27  扬沙                Sand

28  沙尘暴              Duststorm

29  强沙尘暴            Sandstorm

30  雾                  Foggy

31  霾                  Haze

32  风                  Windy

33  大风                Blustery

34  飓风                Hurricane

35  热带风暴            Tropical Storm

36  龙卷风              Tornado

37  冷                  Cold

38  热                  Hot

99  未知                Unknown

*/
