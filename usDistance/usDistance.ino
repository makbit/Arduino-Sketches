const int Trig = 8;
const int Echo = 9;
const int ledPin = 13;

unsigned int time_us = 0;
unsigned int distance_cm = 0;

void setup()
{
    pinMode(Trig, OUTPUT);
    pinMode(Echo, INPUT);
    pinMode(ledPin, OUTPUT);
     
    Serial.begin(9600);
}


void loop()
{
    delayMicroseconds(10);         // Удерживаем 10 микросекунд
    digitalWrite(Trig, HIGH);      // Подаем сигнал на выход микроконтроллера
    delayMicroseconds(10);         // Удерживаем 10 микросекунд
    digitalWrite(Trig, LOW);       // Затем убираем

    time_us = pulseIn(Echo, HIGH); // Замеряем длину импульса
    distance_cm = time_us / 58;    // Пересчитываем в сантиметры
    if( time_us==38 )
    {
      Serial.println("---");   // Выводим на порт
    }
    else
    {
      Serial.println(distance_cm);   // Выводим на порт
    }

    if( distance_cm < 20 )         // Если расстояние менее 20 сантиметром
    {
       digitalWrite(ledPin, 1);
    }
    else
    {
       digitalWrite(ledPin, 0);
    }
    delay(200);
}

