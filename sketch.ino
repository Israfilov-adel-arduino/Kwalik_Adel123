#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Инициализация LCD: адрес I²C, 20 столбцов, 4 строки
LiquidCrystal_I2C lcd(0x27, 20, 4); // Попробуйте 0x3F, если не работает

const int buttonPin = 7;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

// Итеративная функция для вычисления числа Фибоначчи (быстрая)
unsigned long fibonacci(int n) {
  if (n <= 1) return n;
  unsigned long a = 0, b = 1, c;
  for (int i = 2; i <= n; i++) {
    c = a + b;
    a = b;
    b = c;
  }
  return b;
}

void setup() {
  lcd.init();          // Инициализация дисплея
  lcd.backlight();     // Включение подсветки
  pinMode(buttonPin, INPUT_PULLUP);
  randomSeed(analogRead(0));

  // Приветственное сообщение
  lcd.setCursor(0, 0);
  lcd.print("Press button");
}

void loop() {
  int reading = digitalRead(buttonPin);

  // Улучшенная обработка дребезга контактов
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW) {
      // Генерация случайного числа от 5 до 30
      int randomNumber = random(5, 31);

      // Очищаем дисплей и выводим исходное число на первую строку
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Num: ");
      lcd.print(randomNumber);

      // Замер времени вычисления Фибоначчи
      unsigned long startTime = micros();
      unsigned long fibResult = fibonacci(randomNumber);
      unsigned long endTime = micros();

      // Расчёт времени в секундах (с точностью до 5 знаков)
      float durationSec = (float)(endTime - startTime) / 1000000.0;

      // Вывод времени на вторую строку
      lcd.setCursor(0, 1);
      lcd.print("Time: ");
      if (durationSec < 0.00001) {
        lcd.print("0.00000 sec");
      } else {
        lcd.print(durationSec, 5);
        lcd.print(" sec");
      }

      // Вывод числа Фибоначчи на третью строку
      lcd.setCursor(0, 2);
      lcd.print("Fib: ");

      char buffer[12];
      if (fibResult > 99999) {
        // Для больших чисел используем научную нотацию (например, 7.50e+04)
        dtostrf((float)fibResult, 8, 2, buffer);
        lcd.print(buffer);
      } else if (fibResult > 999) {
        // Для средних чисел — сокращаем до 4 цифр + "K" (например, 6.77K)
        float kValue = (float)fibResult / 1000.0;
        lcd.print(kValue, 2);  // 2 знака после запятой
        lcd.print("K");
      } else {
        // Нормальное число — выводим полностью
        sprintf(buffer, "%lu", fibResult);
        lcd.print(buffer);
      }
    }
  }
  lastButtonState = reading;
}
