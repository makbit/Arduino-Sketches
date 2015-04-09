/*
    LED4x7SegmentsISR - draw a number on a 4-digit 7-segment LED display.

    This program drives 4-digit LED Display with common cathode.
    It uses multiplexing - cycling through digits (1, 2, 3, 4).

    The LED Display is connected to digital pins 3..10 of the Arduino UNO.
    Analog PINs (A0, A1, A2, A3) are configured to digital mode and are used
    to emulate commone cathode (GND) for every digit (1..4).

    The program will display a counter updated every second.
    
    The code uses Timer #1 interrupt, thus the main loop can
    be used for any useful task.
    
    This file is free software; you can redistribute it or modify it.
    Created 27 March 2015 by K.M.
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
static int nDelay = 0; // delay counter
static int nDigit = 1; // active digit on LED
static int aDigitPins[4] = { digit1, digit2, digit3, digit4 };

//==============================================================//
ISR(TIMER1_COMPA_vect)
{
    if( nDelay > 0 )
    {
        // do some delay
        nDelay--;
        if( nDelay==0 )
        {
            // Delay is over -> turn off the Digit
            digitalWrite(aDigitPins[nDigit-1], HIGH);
            // Turn OFF all segments
            drawDigitFast( -1 );
            // Cycle to next Digit (1..4)
            nDigit++;
            if( nDigit > 4 ) nDigit = 1;
        }
    }
    else
    {
        // Turn ON digits and let them light 'nDelay' ticks
        switch( nDigit )
        {
            case 1:
                digitalWrite(digit1, LOW);
                drawDigitFast( (number/1000) % 10 );
                nDelay = 4;
                break;
            case 2:
                digitalWrite(digit2, LOW);
                drawDigitFast( (number/100) % 10 );
                nDelay = 4;
                break;
            case 3:
                digitalWrite(digit3, LOW);
                drawDigitFast( (number/10) % 10 );
                nDelay = 4;
                break;
            case 4:
                digitalWrite(digit4, LOW);
                drawDigitFast( number % 10 );
                nDelay = 4;
                break;
        }
    }
}

//==============================================================//
void setup()
{
    // Configure PINs
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

    //======================
    // PIN for LM35 Sensor, 0-1.1V 
    analogReference(INTERNAL);
    //======================
    // initialize Timer1
    //======================
    noInterrupts();
    // Reset config registers
    TCCR1A = 0;
    TCCR1B = 0;
    // Turn on CTC mode - Clear Timer on Compare match (counter in OCR1A).
    TCCR1B |= (1 << WGM12);
    // Set 1024 prescaler
    TCCR1B |= (1 << CS12) | (1 << CS10); 
    // Setup tick 1000Hz (for 16MHz CPU, 1024 prescaler)
    OCR1A = (16000000 / 1024 - 1) / 1000;
    // Enable Clear Timer on Compare match (CTC) interrupt (channel A)
    TIMSK1 |= (1 << OCIE1A);
    interrupts();
}

//==============================================================//
void loop()
{
    // Do something useful
    delay(1000);
    if( ++number > 9999 ) number = 0;
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
        n = 10;
    }
    for( int i = 0; i < 7; i++ )
    {
        digitalWrite( aPins[i], aSegments[n][i] );
    }
}
