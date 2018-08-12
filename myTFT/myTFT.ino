#include "TFTLCD.h"
//#include "TouchScreen.h"

/*
#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM  7  // can be a digital pin
#define XP  6  // can be a digital pin
*/
#define LCD_CS    A3
#define LCD_CD    A2
#define LCD_WR    A1
#define LCD_RD    A0 
#define LCD_RESET A4

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0 
#define WHITE           0xFFFF

/*
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940
*/
// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
//TouchScreen ts(XP, YP, XM, YM, 300);
TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);


static uint16_t id = 0;
void setup() {
    // put your setup code here, to run once:
    //Serial.begin(9600);
    //id = tft.readRegister( 0 );
    //Serial.println( id );

    tft.reset();
    tft.initDisplay();
    tft.fillScreen(WHITE);
    tft.setRotation(1);

}

static char* str1 = "A problem has been detected and Windows has been shut down to prevent damage to your computer.";
static char* str2 = "";
static char* str3 = "UNMOUNTABLE_BOOT_VOLUME";
static char* str4 = "";
static char* str5 = "If this is the first time you've seen this stop error screen, restart your computer.";
static char* str6 = "If this screen appears again, follow these steps:";
static char* str7 = "";
static char* str8 = "Check to make sure any new hardware or software is properly installed.";
static char* str9 = "If this is a new installation, ask your software manufacturer for any Windows updates you might need.";
static char* str10= "";
static char* str11= "Technical Information:";
static char* str12= "STOP: 0x000000ED (0x00000002, 0x00000001, 0x00000002, 0x00000000)";


void loop() {
  // put your main code here, to run repeatedly:
/*
    delay(500);
    tft.fillScreen(BLACK);
    delay(500);
    tft.fillScreen(RED);
    delay(500);
    tft.fillScreen(GREEN);
    delay(500);
    tft.fillScreen(BLUE);
    delay(500);
    tft.fillScreen(CYAN);
    delay(500);
    tft.fillScreen(MAGENTA);
    delay(500);
    tft.fillScreen(YELLOW);
*/
    delay(500);
    tft.fillScreen(BLACK);
    tft.drawString(30, 30, "Boot from SD Card", WHITE, 2);
    tft.drawString(30, 60, "Please wait...", WHITE, 2);
    for( int i = 0; i < 40; i++ )
    {
        tft.drawString(30+i*5, 90, ".", WHITE, 2);
        delay(100);
    }
    
    delay(1000);
    tft.fillScreen(BLUE);
    for( int x = 0; x < 320; x++ )
    {
        //tft.drawRect(x, x, 320-x, 240-x, BLUE);
    }
    tft.drawString(5, 10, str1,  WHITE);
    tft.drawString(5, 20, str2,  WHITE);
    tft.drawString(5, 30, str3,  WHITE);
    tft.drawString(5, 40, str4,  WHITE);
    tft.drawString(5, 50, str5,  WHITE);
    tft.drawString(5, 60, str6,  WHITE);
    tft.drawString(5, 70, str7,  WHITE);
    tft.drawString(5, 80, str8,  WHITE);
    tft.drawString(5, 90, str9,  WHITE);
    tft.drawString(5,100, str10, WHITE);
    tft.drawString(5,110, str11, WHITE);
    tft.drawString(5,120, str12, WHITE);
    delay(10000);
}
