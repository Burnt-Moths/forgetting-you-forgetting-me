#include <Adafruit_GFX.h> 
#include <Adafruit_ST7789.h> 
#include <SPI.h> 
#include <WiFi.h>
#include <time.h>

// i copied this off the docs because i made it a point to use the same pins after seeing the resolution issues
#define TFT_SCLK 0 
#define TFT_MOSI 1 
#define TFT_RST 2
#define TFT_DC 3
#define TFT_CS 4
#define TFT_BL 5

//ur not getting that info off me
const char* WIFI_SSID = "wifi name here";         
const char* WIFI_PASSWORD = "pasword here";     
const char* TIMEZONE = "IST-5:30";

//took me too fucking long to learn bool
bool alarmEnabled = true;
int alarmHour = 6;
int alarmMinute = 30;
bool alarmRinging = false;
int lastTriggeredDay = -1;

class MyST7789 : public Adafruit_ST7789 {
public:
  MyST7789(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst)
    : Adafruit_ST7789(cs, dc, mosi, sclk, rst) {}
  void setOffsets(uint8_t col, uint8_t row) {
    _colstart = _colstart2 = col;
    _rowstart = _rowstart2 = row;
  }
};

MyST7789 index(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup() {
  Serial.begin(115200); 

 
  pinMode(TFT_BL, OUTPUT); 
  analogWrite(TFT_BL, 0);

 
  index.init(76, 284); 
  index.setOffsets(82, 18); 
  index.invertDisplay(false); 
  index.setRotation(1); 
  
  index.fillScreen(ST77XX_BLACK);
  index.setTextColor(ST77XX_BLUE);
  index.setTextSize(3);
  
  index.setCursor(10, 10);
  index.print("Clear attempt");

 // my friend said this helps. im not sure how
  for (int brightness = 0; brightness <= 40; brightness++) {
    analogWrite(TFT_BL, brightness);
    delay(15);
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

unsigned long startAttempt = millis();

while (WiFi.status() != WL_CONNECTED &&
       millis() - startAttempt < 15000) {
  delay(500);
  Serial.println("Connecting");
}

if (WiFi.status() == WL_CONNECTED) {
  Serial.println("clear");
} else {
  Serial.println("MaayBacc");
}

  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  setenv("TZ", TIMEZONE, 1);
  tzset();

  index.fillScreen(ST77XX_BLACK);
}

void loop() {
  struct tm timeinfo;
 
  if (!getLocalTime(&timeinfo)) {
    index.setCursor(10, 10);
    index.print("ImhamBUrg");
    delay(1000);
    return;
  }

  char timeString[9];  
  char dateString[12]; 

  strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);
  strftime(dateString, sizeof(dateString), "%d/%m/%Y", &timeinfo);

 
  index.setTextSize(4);
  index.setTextColor(ST77XX_BLUE);
  
 // yeah i want the mild ghosting heh, it may be messy, if it is ill recode it
  index.setCursor(10, 15);
  index.print("********"); 
  
 
  
// ive just balled the curosrs for now, i may have to change that again when its in hand
  index.setCursor(15, 20);
  index.print(timeString);


  index.setTextSize(2);
  index.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  index.setCursor(10, 60);
  index.print(dateString);

  if (alarmEnabled && timeinfo.tm_hour == alarmHour && timeinfo.tm_min == alarmMinute) {
    if (lastTriggeredDay != timeinfo.tm_mday) {
      alarmRinging = true;
      lastTriggeredDay = timeinfo.tm_mday;
      Serial.println("ClearFail");
    }
  }

  if (alarmRinging) {
    index.setTextSize(2);
    index.setTextColor(ST77XX_BLUE);
    
 
    index.setCursor(10, 90);
    index.print("************");
    delay(50);
    
  
    index.setCursor(10, 90);
     index.print("UnkWaKeuPNowwwww");
  }
  //yeah that says unk wake up nowww, and yeah that shit is like that because of how the game jumbles letters when you fail to clash

  delay(900); 

  // i wanna figure out the buzzer after i have it in hand so im not makeing that rn, ill figure it out l8r
}
// i did use a lot of help form my friends and even consulted my maths sir for this as trashy as it is, kinda proud, and flattered that so many people helped me                               