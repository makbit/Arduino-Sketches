    const int Trig = 8;
    const int Echo = 9;
    const int ledPin = 11;  // 13 – если будете использовать встроенный в Arduino светодиод

void setup()
{
    pinMode(Trig, OUTPUT);
    pinMode(Echo, INPUT);
    pinMode(ledPin, OUTPUT);
     
    Serial.begin(9600);
}

unsigned int time_us=0;
unsigned int distance_sm=0;

void loop()
{
    digitalWrite(Trig, HIGH); // Подаем сигнал на выход микроконтроллера
    delayMicroseconds(10); // Удерживаем 10 микросекунд
    digitalWrite(Trig, LOW); // Затем убираем
    time_us=pulseIn(Echo, HIGH); // Замеряем длину импульса
    distance_sm=time_us/58; // Пересчитываем в сантиметры
    Serial.println(distance_sm); // Выводим на порт

    if (distance_sm<50) // Если расстояние менее 50 сантиметром
    {
       digitalWrite(ledPin, 1); // Зажигаем светодиод
    }
    else
    {
       digitalWrite(ledPin, 0); // иначе тушим
    }
     
    delay(100);
}

