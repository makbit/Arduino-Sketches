// working example!


const int ocr2aval    = 3; 
const int freq2MHzPin = 11;   // OC2A output pin for ATmega328 boards


void init2MHzClock()
{
    // Set Timer 2 CTC mode with no prescaling.  OC2A toggles on compare match
    //
    // WGM22:0 = 010: CTC Mode, toggle OC 
    // WGM2 bits 1 and 0 are in TCCR2A,
    // WGM2 bit 2 is in TCCR2B
    // COM2A0 sets OC2A (arduino pin 11 on Uno or Duemilanove) to toggle on compare match
    //
    TCCR2A = ((1 << WGM21) | (1 << COM2A0));

    // Set Timer 2  No prescaling  (i.e. prescale division = 1)
    //
    // CS22:0 = 001: Use CPU clock with no prescaling
    // CS2 bits 2:0 are all in TCCR2B
    TCCR2B = (1 << CS20);

    // Make sure Compare-match register A interrupt for timer2 is disabled
    TIMSK2 = 0;
    // This value determines the output frequency
    OCR2A = ocr2aval;
}

/////////////////////////////////////////////////////////////////////
void setup()
{
    Serial.begin(19200);
    randomSeed(analogRead(4)+analogRead(5));

    pinMode(8, OUTPUT); // D0
    pinMode(9, OUTPUT); // D1
    pinMode(2, OUTPUT); // D2
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT); // D7
  
    pinMode(A2, OUTPUT);  // BC1
    pinMode(A3, OUTPUT);  // BDIR

    pinMode(freq2MHzPin, OUTPUT);
    init2MHzClock();
    resetAY();


    // Random notes
    // Mixer ABC tone
    ay_out( 0x07, ~0x07 );
    // Volume
    ay_out( 0x08, random(11, 15) );
    ay_out( 0x09, random(11, 15) );
    ay_out( 0x0A, random(11, 15) );
    for(int i = 0; i < 10; i++ )
    {
        // Frequency
        ay_out( 0, random(i*1, 255) );
        ay_out( 2, random(i*5, 255) );
        ay_out( 4, random(i*3, 100) );
        // Noise frequnecy
        ay_out( 6, random(0, 255) );
        // Envelope freq
        ay_out( 0x0C, random(0, 255) );
        // Envelope shape
        //ay_out( 0x0D, 8 );
        ay_out( 0x0D, random(0, 16) );
        delay( random(100, 500) );
    }
    resetAY();
}

void loop()
{
    byte buffer[2];
    if( Serial.available() >= 2 )
    {
        Serial.readBytes(buffer, sizeof(buffer));
        if( buffer[1]==0xFF && buffer[0]==0xFF )
        {
            resetAY();
        }
        else
        {
            switch( buffer[1] )
            {
                
                case 0xFF:
                    delayMicroseconds(20);
                    break;
                case 0xFE:
                    while( buffer[0]-- )
                        delayMicroseconds(80);
                    break;
                case 0xFD:
                    resetAY();
                    break;
                    
                default:
                    ay_out(buffer[1], buffer[0]);
                    break;
            }
        }
    }
}

void resetAY()
{
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
  for( int i=0; i < 16; i++)
    ay_out(i,0);
}


void ay_out(byte address, byte data)
{
  PORTC = PORTC & B11110011; // bc & dir = 00
  PORTD = (PORTD & 0x03) | (address & 0xFC);
  PORTB = (PORTB & 0xFC) | (address & 0x03);
  PORTC = PORTC | B00001100; // bc & dir = 11

  delayMicroseconds(1);

  PORTC = PORTC & B11110011; // bc & dir = 00
  PORTD = (PORTD & 0x03) | (data & 0xFC);
  PORTB = (PORTB & 0xFC) | (data & 0x03);
  PORTC = PORTC | B00001000; // bc, dir = 01

  delayMicroseconds(1);

  PORTC = PORTC & B11110011; // bc & dir = 00
}





/////////////////////////////////////////////////////////////////////

/*
unsigned int cb = 0;

byte rawData[] = {
    0xFF, 0x00, 0x8E, 0x02, 0x38, 0x03, 0x02, 0x04, 0x0E, 0x05, 0x02, 0x07, 
    0x1A, 0x08, 0x0F, 0x09, 0x10, 0x0A, 0x0E, 0x0B, 0x47, 0x0D, 0x0E, 0xFF, 
    0x00, 0x77, 0x04, 0x8E, 0x05, 0x03, 0x07, 0x3A, 0x08, 0x0E, 0x0A, 0x0D, 
    0xFF, 0x00, 0x5E, 0x04, 0x0E, 0x05, 0x05, 0x0A, 0x0C, 0xFF, 0x04, 0x8E, 
    0x05, 0x06, 0x07, 0x32, 0x08, 0x00, 0x0A, 0x0A, 0xFF, 0x05, 0x08, 0x0A, 
    0x07, 0xFF, 0x04, 0x0E, 0x05, 0x0A, 0x0A, 0x04, 0xFF, 0x00, 0x8E, 0x04, 
    0x8E, 0x05, 0x00, 0x07, 0x1E, 0x08, 0x0F, 0x0A, 0x0B, 0x0D, 0x0E, 0xFF, 
    0x00, 0x77, 0x08, 0x0E, 0x0A, 0x06, 0xFF, 0x00, 0x5E, 0x07, 0x3E, 0x0A, 
    0x00, 0xFF, 0x07, 0x36, 0x08, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x8E, 0x07, 
    0x33, 0x08, 0x0B, 0x0A, 0x0F, 0x0D, 0x0E, 0xFF, 0x04, 0x77, 0x08, 0x06, 
    0x0A, 0x0E, 0xFF, 0x04, 0x5E, 0x07, 0x3B, 0x08, 0x00, 0xFF, 0x07, 0x1B, 
    0x0A, 0x00, 0xFF, 0xFF, 0xFF, 0x02, 0x1C, 0x03, 0x01, 0x04, 0x8E, 0x07, 
    0x33, 0x08, 0x0B, 0x0A, 0x0B, 0x0B, 0x23, 0x0D, 0x0E, 0xFF, 0x04, 0x77, 
    0x08, 0x06, 0x0A, 0x0A, 0xFF, 0x04, 0x5E, 0x07, 0x3B, 0x08, 0x00, 0x0A, 
    0x09, 0xFF, 0x07, 0x1B, 0x0A, 0x00, 0xFF, 0xFF, 0xFF, 0x02, 0x8E, 0x03, 
    0x00, 0x04, 0x0E, 0x05, 0x01, 0x07, 0x18, 0x08, 0x0F, 0x09, 0x0B, 0x0A, 
    0x0E, 0xFF, 0x00, 0x77, 0x02, 0x77, 0x04, 0x8E, 0x06, 0x01, 0x08, 0x0E, 
    0x09, 0x0A, 0x0A, 0x0D, 0xFF, 0x00, 0x5E, 0x02, 0x5E, 0x04, 0x0E, 0x05, 
    0x02, 0x06, 0x02, 0x09, 0x09, 0x0A, 0x0C, 0xFF, 0x02, 0x8E, 0x04, 0x8E, 
    0x07, 0x30, 0x08, 0x00, 0x09, 0x08, 0x0A, 0x0A, 0xFF, 0x02, 0x77, 0xFF,
    0xFF
};

void pseudoInterrupt(){
  while(rawData[cb]<0xFF)
  {
   Serial.print("AY[");
   Serial.print(rawData[cb],DEC);
   Serial.print("] = ");
   Serial.print(rawData[cb+1],HEX);
   Serial.println();
   ay_out(rawData[cb],rawData[cb+1]);
   cb++;
   cb++;
 }
 if(rawData[cb]==0xff) cb++;
 
 if (cb>20*12)
 {
   Serial.println("==================================================== ZERO ==================================================");
   cb=0;
 }
}

void loop() {
  delay(20);
  pseudoInterrupt();
}
*/
//==========================================================================//
/*
void fillBuffer(){
  int fillSz = 0;
  int freeSz = bufSize;
  if (fillPos>playPos) {
    fillSz = fillPos-playPos;
    freeSz = bufSize - fillSz;
  }
  if (playPos>fillPos) {
    freeSz = playPos - fillPos;
    fillSz = bufSize - freeSz;
  }
  
  freeSz--; // do not reach playPos
  while (freeSz>0){
    byte b = 0xFD;
    if (fp.available()){
      b = fp.read();
    }
    playBuf[fillPos] = b;
    fillPos++;
    if (fillPos==bufSize) fillPos=0;
    freeSz--;
  }
}


int skipCnt = 0;

ISR(TIMER1_COMPA_vect){
  if (skipCnt>0){
    skipCnt--;
  } else {
    int fillSz = 0;
    int freeSz = bufSize;
    if (fillPos>playPos) {
      fillSz = fillPos-playPos;
      freeSz = bufSize - fillSz;
    }
    if (playPos>fillPos) {
      freeSz = playPos - fillPos;
      fillSz = bufSize - freeSz;
    }

    boolean ok = false;
    int p = playPos;
    while (fillSz>0){
      byte b = playBuf[p];
      p++; if (p==bufSize) p=0;
      fillSz--;
      
      if (b==0xFF){ ok = true; break; }
      if (b==0xFD){ 
        ok = true; 
        playFinished = true;
        for (int i=0;i<16;i++) ay_out(i,0);
        break; 
      }
      if (b==0xFE){ 
        if (fillSz>0){
          skipCnt = playBuf[p];
          p++; if (p==bufSize) p=0;
          fillSz--;
          
          skipCnt = 4*skipCnt;
          ok = true; 
          break; 
        } 
      }
      if (b<=252){
        if (fillSz>0){
          byte v = playBuf[p];
          p++; if (p==bufSize) p=0;
          fillSz--;
          
          if (b<16) ay_out(b,v);
        } 
      }
    } // while (fillSz>0)
  
    if (ok){
      playPos = p;
    }
  } // else skipCnt 
}
*/

/*
Структура PSG-формата
Offset Number of byte Description
+0 3 Identifier 'PSG'
+3 1 Marker “End of Text” (1Ah)
+4 1 Version number
+5 1 Player frequency (for versions 10+)
+6 10 Data

Data — последовательности пар байтов записи в регистр.
Первый байт — номер регистра (от 0 до 0x0F), второй — значение.
Вместо номера регистра могут быть специальные маркеры: 0xFF, 0xFE или 0xFD
0xFD — конец композиции.
0xFF — ожидание 20 мс.
0xFE — следующий байт показывает сколько раз выждать по 80 мс.
*/


