/*
    LED4x7Segments - lights a number on a 4-digit 7-segment LED display.

    This program drives 4-digit LED Display with common cathode.
    It uses multiplexing - cycling through digits (1, 2, 3, 4).

    The LED Display is connected to digital pins 3..10 of the Arduino UNO.
    Analog PINs (A0, A1, A2, A3) are configured to digital mode and are used
    to emulate commone cathode (GND) for every digit (1..4).

    The program will display a counter updated every second.
    
    This file is free software; you can redistribute it or modify it.
    Created 25 March 2015 by K.M.
*/

int segA = 3; // top
int segB = 4; // right-top
int segC = 5; // right-bottom
int segD = 6; // bottom
int segE = 7; // left-bottom
int segF = 8; // left-top
int segG = 9; // middle

int digit1 = 14; // common cathode for digit1 (i.e. A0)
int digit2 = 15; // common cathode for digit2 (i.e. A1)
int digit3 = 16; // common cathode for digit3 (i.e. A2)
int digit4 = 17; // common cathode for digit4 (i.e. A3)

static int number = 0;
static int ms = 0;

//==============================================================//
void setup()
{
    pinMode(digit1, OUTPUT);
    pinMode(digit2, OUTPUT);
    pinMode(digit3, OUTPUT);
    pinMode(digit4, OUTPUT);

    pinMode(segA, OUTPUT);
    pinMode(segB, OUTPUT);
    pinMode(segC, OUTPUT);
    pinMode(segD, OUTPUT);
    pinMode(segE, OUTPUT);
    pinMode(segF, OUTPUT);
    pinMode(segG, OUTPUT);
}

//==============================================================//
void loop()
{
    if( millis() / 1000 > ms )
    {
        ms = millis() / 1000;
        if( ++number > 9999 ) number = 0;
    }

    digitalWrite(digit1, LOW);
        drawDigitFast( (number/1000) % 10 );
        delay(2);
        drawDigitFast( -1 );
    digitalWrite(digit1, HIGH);

  
    digitalWrite(digit2, LOW);
        drawDigitFast( (number/100) % 10 );
        delay(2);
        drawDigitFast( -1 );
    digitalWrite(digit2, HIGH);


    digitalWrite(digit3, LOW);
        drawDigitFast( (number/10) % 10 );
        delay(2);
        drawDigitFast( -1 );
    digitalWrite(digit3, HIGH);


    digitalWrite(digit4, LOW);
        drawDigitFast( number % 10 );
        delay(2);
        drawDigitFast( -1 );
    digitalWrite(digit4, HIGH);
}


//-----------------------------------------------------//
void drawDigitFast(int n)
{
    const byte aPins[8] = {
        segA, segB, segC, segD, segE, segF, segG
    };
    const byte aSegments[11][8] = {
        //  A     B     C     D     E     F     G
        { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH,  LOW }, // 0
        {  LOW, HIGH, HIGH,  LOW,  LOW,  LOW,  LOW }, // 1
        { HIGH, HIGH,  LOW, HIGH, HIGH,  LOW, HIGH }, // 2
        { HIGH, HIGH, HIGH, HIGH,  LOW,  LOW, HIGH }, // 3
        {  LOW, HIGH, HIGH,  LOW,  LOW, HIGH, HIGH }, // 4
        { HIGH,  LOW, HIGH, HIGH,  LOW, HIGH, HIGH }, // 5
        { HIGH,  LOW, HIGH, HIGH, HIGH, HIGH, HIGH }, // 6
        { HIGH, HIGH, HIGH,  LOW,  LOW,  LOW, LOW  }, // 7
        { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH }, // 8
        { HIGH, HIGH, HIGH, HIGH,  LOW, HIGH, HIGH }, // 9
        {  LOW,  LOW,  LOW,  LOW,  LOW,  LOW,  LOW }  // all off
    };

    if( n < 0 || n > 10 )
    {
        // all off
        n = 10;
    }

    for( int i = 0; i < 7; i++ )
    {
        digitalWrite( aPins[i], aSegments[n][i] );
    }
}
