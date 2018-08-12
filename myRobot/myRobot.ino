//////////////////////////////////////////////////////////////////////////////////////
//                                                                                  //
//        myRobot - simple sketch to drive robocar witj IR control & sonar.         //
//                                                                                  //
//                                                                                  //
// Author: K.Max (c) 2015   Details: http://makbit.com/forum/                       //
//////////////////////////////////////////////////////////////////////////////////////
// Provide 8-bit PWM output with the analogWrite() function.
// AT MEGA – PWM: 2 to 13 and 44 to 46. 
// UNO – PWM: 3, 5, 6, 9, 10, and 11.
//////////////////////////////////////////////////////////////////////////////////////
// Multi-function sheild:
// 1) http://www.getmicros.net/arduino-multi-function-shield.php
// 2) http://forum.hobbycomponents.com/viewtopic.php?f=40&t=1618
// 3) http://www.sniff.org.uk/2015/04/arduino-multifunction-shield.html
//////////////////////////////////////////////////////////////////////////////////////
#include <IRremote.h>
#include <IRremoteInt.h>

//////////////////////////////////////////////////////////////////////////////////////
#define ON             LOW
#define OFF            HIGH

#define LED1            13
#define LED2            12
#define LED3            11
#define LED4            10

#define KEY1            A1
#define KEY2            A2
#define KEY3            A3
#define TEMPRT_PIN      A4
#define POT_PIN         A0
#define IRED_PIN        2
#define BEEP_PIN        3
#define LATCH_PIN       4
#define CLOCK_PIN       7
#define DATA_PIN        8

//////////////////////////////////////////////////////////////////////////////////////
#define BUT_POWER             0xe318261b
#define BUT_MODE              0x00511dbb
#define BUT_SOUND             0xee886d7f
#define BUT_PLAY              0x52a3d41f
#define BUT_PREV              0xd7e84b1b
#define BUT_NEXT              0x20fe4dbb
#define BUT_MINUS             0xa3c8eddb
#define BUT_PLUS              0xe5cfbd7f
#define BUT_EQ                0xf076c13b
#define BUT_0                 0xc101e57b
#define BUT_1                 0x9716be3f
#define BUT_2                 0x3d9ae3f7
#define BUT_3                 0x6182021b
#define BUT_4                 0x8c22657b
#define BUT_5                 0x488f3cbb
#define BUT_6                 0x0449e79f
#define BUT_7                 0x32c6fdf7
#define BUT_8                 0x1bc0157b
#define BUT_9                 0x3ec3fc1b
#define BUT_REWIND            0x97483bfb
#define BUT_USD               0xf0c41643

IRrecv IR(IRED_PIN);
decode_results results;
//////////////////////////////////////////////////////////////////////////////////////
// 7-Segment LED byte maps for numbers 0 to 9, 
// 'A', 'b', 'C', 'd', 'E', 'F', '.', 'H', 'J', 'L',
// 'P', 'q', 'U', 'o', '°', '_', '-', '=', '?', ' '
const byte SEGMENT_MAP[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,
                            0x77,0x7C,0x39,0x5E,0x79,0x71,0x80,0x76,0x0E,0x38,
                            0x73,0x67,0x3E,0x5C,0x63,0x08,0x40,0x48,0x53,0x00};
                            
// Byte maps to select digit 1 to 4
const byte SEGMENT_SELECT[] = {0xF1, 0xF2, 0xF4, 0xF8};

//////////////////////////////////////////////////////////////////////////////////////
// HC-SR04 Ultrasonic Sensor
#define ECHO_PIN           1
#define TRIG_PIN           0

//////////////////////////////////////////////////////////////////////////////////////
// ROBOT COMMAND & CONSTANTS
//////////////////////////////////////////////////////////////////////////////////////
#define CMD_STOP           0
#define CMD_RUN            1
#define CMD_LEFT           2
#define CMD_RIGHT          3
#define CMD_REVERSE        4
#define CMD_BEEP           5
#define CMD_FLASH          6

#define MOTOR_LEFT_FRWD    9        /* green */
#define MOTOR_LEFT_BKWD    A5       /* blue */
#define MOTOR_RIGHT_FRWD   5        /* green */
#define MOTOR_RIGHT_BKWD   6        /* violet */

//////////////////////////////////////////////////////////////////////////////////////
#define MAX_TIME           1500

static int nCommand = CMD_STOP;
static int nSpeed   = 0;
static int nDelay   = 0;

static int nTemprt = 0;
static int nTick = 0;
static int nDist = 0;
static unsigned long ms = 0;
//////////////////////////////////////////////////////////////////////////////////////
class CRobot
{
    enum state_t { sStopped, sRunning, sReverse, sLeft, sRight, sBeep };
    // leftMotor;
    // rightMotor;
    public:
        CRobot();
        void forward();
        void backward();
        void left();
        void right();
        
        //int speed();
        //checkDistance
};

//////////////////////////////////////////////////////////////////////////////////////
void setup()
{
    // put your setup code here, to run once:
    //Serial.begin(9600);
    //analogReference(INTERNAL1V1);
    analogReference(INTERNAL);

    pinMode(KEY1,       INPUT);
    pinMode(KEY2,       INPUT);
    pinMode(KEY3,       INPUT);
    pinMode(POT_PIN,    INPUT);
    pinMode(TEMPRT_PIN, INPUT);

    pinMode(LATCH_PIN,  OUTPUT);
    pinMode(CLOCK_PIN,  OUTPUT);
    pinMode(DATA_PIN,   OUTPUT);
    pinMode(LED1,       OUTPUT);
    pinMode(LED2,       OUTPUT);
    pinMode(LED3,       OUTPUT);
    pinMode(LED4,       OUTPUT);
    pinMode(BEEP_PIN,   OUTPUT);

    digitalWrite(BEEP_PIN, OFF);
    digitalWrite(LED1,     OFF);
    digitalWrite(LED2,     OFF);
    digitalWrite(LED3,     OFF);
    digitalWrite(LED4,     OFF);

    pinMode(MOTOR_LEFT_FRWD, OUTPUT);
    pinMode(MOTOR_LEFT_BKWD, OUTPUT);
    pinMode(MOTOR_RIGHT_FRWD, OUTPUT);
    pinMode(MOTOR_RIGHT_BKWD, OUTPUT);

    pinMode(ECHO_PIN, INPUT);
    pinMode(TRIG_PIN, OUTPUT);
    
    IR.enableIRIn();
}


//////////////////////////////////////////////////////////////////////////////////////
void loop()
{
    //n = analogRead(POT_PIN);
    if( millis() - ms >= 100 ) // 10 Hz
    {
        unsigned long n;
        digitalWrite(TRIG_PIN, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG_PIN, LOW);
        n = pulseIn(ECHO_PIN, HIGH, 7000) / 58;
        if( n > 0 && n < 1000 ) nDist = (n + 3*nDist) / 4;
        ms = millis();
    }

    if( digitalRead(KEY1)==ON )
    {
        digitalWrite(BEEP_PIN, ON);
        delay(500);
        digitalWrite(BEEP_PIN, OFF);
    }

    if( digitalRead(KEY2)==ON )
    {
        float t = (analogRead(TEMPRT_PIN) + analogRead(TEMPRT_PIN)) / 2;
        //nTemprt = (t * 5 * 100) / 1024;
        nTemprt = (t * 1.1 * 100) / 1024;
        drawDigit(0, 0x0C);   // Celsuis
        drawDigit(1, 0x0F+9); // Degree sign
        drawDigit(2, (nTemprt/10) % 10);
        drawDigit(3, (nTemprt/1)  % 10);
    }
    else
    {
        //drawDigit(0, 0x0D);   // Distance
        drawDigit(1, (nDist/100) % 10); // m
        drawDigit(2, (nDist/10 ) % 10); // dm
        drawDigit(3, (nDist/1  ) % 10); // cm
    }

/*    
    if( IR.decode(&results) )
    {
        Serial.print("irCode: ");
        Serial.print(results.value, HEX);
        Serial.print(",  bits: ");
        Serial.println(results.bits);
        IR.resume();
    }
*/    
    //-----------------------------------------------------------------//
    if( IR.decode(&results) )
    {
        switch( results.value )
        {
            case BUT_EQ:
                nCommand = CMD_FLASH;
                nDelay   = MAX_TIME;
                break;
            case BUT_SOUND:
                nCommand = CMD_BEEP;
                nDelay   = MAX_TIME;
                break;
            case BUT_POWER:
                nCommand = CMD_STOP;
                nSpeed   = 0;
                nDelay   = 0;
                break;
            case BUT_0://PLAY:
                nCommand = CMD_RUN;
                nSpeed   = (nSpeed < 5 ) ? nSpeed + 1 : 5;
                nDelay   = MAX_TIME;
                break;
            case BUT_REWIND:
                nCommand = CMD_REVERSE;
                nSpeed   = 1;
                nDelay   = MAX_TIME;
                break;
            case BUT_PREV:
                nCommand = CMD_LEFT;
                nSpeed   = 3;
                nDelay   = MAX_TIME;
                break;
            case BUT_NEXT:
                nCommand = CMD_RIGHT;
                nSpeed   = 3;
                nDelay   = MAX_TIME;
                break;
            case BUT_MINUS:
                nSpeed   = (nSpeed > 0 ) ? nSpeed - 1 : 0;
                nDelay   = MAX_TIME;
                break;
            case BUT_PLUS:
                nSpeed   = (nSpeed < 5 ) ? nSpeed + 1 : 5;
                nDelay   = MAX_TIME;
                break;
        }
        IR.resume();
    }

    //-----------------------------------------------------------------//
    switch( nCommand )
    {
        case CMD_FLASH:
            if( nDelay > 0 )
            {
                digitalWrite(LED1, ON);
                digitalWrite(LED2, ON);
                digitalWrite(LED3, ON);
                digitalWrite(LED4, ON);
                nDelay--;
            }
            else
            {
                digitalWrite(LED1, OFF);
                digitalWrite(LED2, OFF);
                digitalWrite(LED3, OFF);
                digitalWrite(LED4, OFF);
            }
            break;
        case CMD_BEEP:
            if( nDelay > 0 )
            {
                digitalWrite(BEEP_PIN, ON);
                nDelay--;
            }
            else
            {
                digitalWrite(BEEP_PIN, OFF);
            }
            break;
        case CMD_STOP:
            analogWrite(MOTOR_RIGHT_FRWD, 0);
            analogWrite(MOTOR_LEFT_FRWD, 0);
            digitalWrite(MOTOR_RIGHT_BKWD, LOW);
            digitalWrite(MOTOR_LEFT_BKWD, LOW);
            break;
        case CMD_RUN:
            if( nDelay > 0 )
            {
                digitalWrite(MOTOR_RIGHT_BKWD, LOW);
                digitalWrite(MOTOR_LEFT_BKWD, LOW);
                analogWrite(MOTOR_RIGHT_FRWD, nSpeed * 50);
                analogWrite(MOTOR_LEFT_FRWD,  nSpeed * 40);
                nDelay--;
            }
            else
            {
                nCommand = CMD_STOP;
                nSpeed   = 0;
            }
            break;
        case CMD_REVERSE:
            if( nDelay > 0 )
            {
                digitalWrite(MOTOR_RIGHT_BKWD, HIGH);
                digitalWrite(MOTOR_LEFT_BKWD, HIGH);
                analogWrite(MOTOR_RIGHT_FRWD, 250 - nSpeed * 50);
                analogWrite(MOTOR_LEFT_FRWD,  250 - nSpeed * 40);
                nDelay--;
            }
            else
            {
                nCommand = CMD_STOP;
                nSpeed   = 0;
            }
            break;
        case CMD_LEFT:
            if( nDelay > 0 )
            {
                digitalWrite(MOTOR_RIGHT_BKWD, LOW);
                digitalWrite(MOTOR_LEFT_BKWD, LOW);
                analogWrite(MOTOR_RIGHT_FRWD, nSpeed * 50);
                analogWrite(MOTOR_LEFT_FRWD,  0);
                nDelay--;
            }
            else
            {
                nCommand = CMD_STOP;
                nSpeed   = 0;
            }
            break;
        case CMD_RIGHT:
            if( nDelay > 0 )
            {
                digitalWrite(MOTOR_RIGHT_BKWD, LOW);
                digitalWrite(MOTOR_LEFT_BKWD, LOW);
                analogWrite(MOTOR_RIGHT_FRWD, 0);
                analogWrite(MOTOR_LEFT_FRWD,  nSpeed * 40);
                nDelay--;
            }
            else
            {
                nCommand = CMD_STOP;
                nSpeed   = 0;
            }
            break;
        default:
            break;        
    }
    //-----------------------------------------------------------//    
    if( nDist > 10 )
    {
    }
    else
    {
        nCommand = CMD_STOP;
        nSpeed   = 0;
    }
}


//////////////////////////////////////////////////////////////////////////////////////
void drawDigit(byte column, byte number)
{
    if( number >= sizeof(SEGMENT_MAP) )
    {
        number = sizeof(SEGMENT_MAP) - 1;
    }
    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, ~SEGMENT_MAP[number] );
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, SEGMENT_SELECT[column]);
    digitalWrite(LATCH_PIN, HIGH);
}
///////////////////////////////////////////////////////////////////////////////////////

