/*
    LED1x7Segments - lights a digit on a 7-segment LED display.

    This program drives a LED Display with common cathode.
    The LED Display has 10 PINS (7-Segments, dot-Segment, two common cathodes).
    The LED Display is connected to digital pins 3..10 of the Arduino UNO.

    The program will display digits 0..9 in infinite loop. Digits will cycle
    from 0 to 9 every second.
    Variable "nTicks" holds number of seconds since the Arduino was started.
    Created 20 March 2015 by K.M.
*/

int segA = 3; // top
int segB = 4; // right-top
int segC = 5; // right-bottom
int segD = 6; // bottom
int segE = 7; // left-bottom
int segF = 8; // left-top
int segG = 9; // miggle
int segH = 10;// dot


void setup()
{
    // Initialize PINs connected to 1-digit 7-Segments LED Display
    pinMode(segA, OUTPUT);
    pinMode(segB, OUTPUT);
    pinMode(segC, OUTPUT);
    pinMode(segD, OUTPUT);
    pinMode(segE, OUTPUT);
    pinMode(segF, OUTPUT);
    pinMode(segG, OUTPUT);
    pinMode(segH, OUTPUT);
}

//-----------------------------------------------------//
// Number to display
static int nTicks = 0;

void loop()
{
    // display seconds (0..9)
    delay(1000);
    drawDigit(nTicks % 10);
    nTicks++;
}


//--------------------------------------------------------//
// Draw digit on 7-Segment LED Display (slow, but simple) //
//--------------------------------------------------------//
void drawDigit(int n)
{
    // Turn OFF all segments
    digitalWrite(segA, LOW);
    digitalWrite(segB, LOW);
    digitalWrite(segC, LOW);
    digitalWrite(segD, LOW);
    digitalWrite(segE, LOW);
    digitalWrite(segF, LOW);
    digitalWrite(segG, LOW);
    digitalWrite(segH, LOW);

    // Turn ON required segments
    switch( n )
    {
      case 0:
        digitalWrite(segA, HIGH);
        digitalWrite(segB, HIGH);
        digitalWrite(segC, HIGH);
        digitalWrite(segD, HIGH);
        digitalWrite(segE, HIGH);
        digitalWrite(segF, HIGH);
        break;
      case 1:
        digitalWrite(segB, HIGH);
        digitalWrite(segC, HIGH);
        break;
      case 2:
        digitalWrite(segA, HIGH);
        digitalWrite(segB, HIGH);
        digitalWrite(segD, HIGH);
        digitalWrite(segE, HIGH);
        digitalWrite(segG, HIGH);
        break;
      case 3:
        digitalWrite(segA, HIGH);
        digitalWrite(segB, HIGH);
        digitalWrite(segC, HIGH);
        digitalWrite(segD, HIGH);
        digitalWrite(segG, HIGH);
        break;
      case 4:
        digitalWrite(segB, HIGH);
        digitalWrite(segC, HIGH);
        digitalWrite(segF, HIGH);
        digitalWrite(segG, HIGH);
        break;
      case 5:
        digitalWrite(segA, HIGH);
        digitalWrite(segC, HIGH);
        digitalWrite(segD, HIGH);
        digitalWrite(segF, HIGH);
        digitalWrite(segG, HIGH);
        break;
      case 6:
        digitalWrite(segA, HIGH);
        digitalWrite(segC, HIGH);
        digitalWrite(segD, HIGH);
        digitalWrite(segE, HIGH);
        digitalWrite(segF, HIGH);
        digitalWrite(segG, HIGH);
        break;
      case 7:
        digitalWrite(segA, HIGH);
        digitalWrite(segB, HIGH);
        digitalWrite(segC, HIGH);
        break;
      case 8:
        digitalWrite(segA, HIGH);
        digitalWrite(segB, HIGH);
        digitalWrite(segC, HIGH);
        digitalWrite(segD, HIGH);
        digitalWrite(segE, HIGH);
        digitalWrite(segF, HIGH);
        digitalWrite(segG, HIGH);
        break;
      case 9:
        digitalWrite(segA, HIGH);
        digitalWrite(segB, HIGH);
        digitalWrite(segC, HIGH);
        digitalWrite(segD, HIGH);
        digitalWrite(segF, HIGH);
        digitalWrite(segG, HIGH);
        break;
    }
}

//-----------------------------------------------------//
// Draw digit on 7-Segment LED Display (fast)          //
//-----------------------------------------------------//
void drawDigitFast(int n)
{
    const byte aPins[8] = {
        segA, segB, segC, segD, segE, segF, segG, segH 
    };
    const byte aSegments[10][8] = {
        //  A     B     C     D     E     F     G    H
        { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH,  LOW, LOW }, // 0
        {  LOW, HIGH, HIGH,  LOW,  LOW,  LOW,  LOW, LOW }, // 1
        { HIGH, HIGH,  LOW, HIGH, HIGH,  LOW, HIGH, LOW }, // 2
        { HIGH, HIGH, HIGH, HIGH,  LOW,  LOW, HIGH, LOW }, // 3
        {  LOW, HIGH, HIGH,  LOW,  LOW, HIGH, HIGH, LOW }, // 4
        { HIGH,  LOW, HIGH, HIGH,  LOW, HIGH, HIGH, LOW }, // 5
        { HIGH,  LOW, HIGH, HIGH, HIGH, HIGH, HIGH, LOW }, // 6
        { HIGH, HIGH, HIGH,  LOW,  LOW,  LOW, LOW,  LOW }, // 7
        { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW }, // 8
        { HIGH, HIGH, HIGH, HIGH,  LOW, HIGH, HIGH, LOW }, // 9
    };
     for( int i = 0; i < 8; i++ )
     {
        digitalWrite( aPins[i], aSegments[n][i] );
     }
}
