#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int stopButton = 2;
const int buttonPin1 = 3; // Pin untuk tombol 1
const int buttonPin2 = 4; // Pin untuk tombol 2
const int buttonPin3 = 5;
const int buttonPin4 = 6;
const int pumpPin = 7; // Pin untuk kontrol motor DC
const int buzzPin = 8; 
int buttonState1 = HIGH;
int buttonState2 = HIGH;
int buttonState3 = HIGH;
int buttonState4 = HIGH;
int stopState = HIGH;
unsigned long ms_from_start = 0;
unsigned long current_ms = 0;
unsigned long interval1 = 100;
unsigned long pumpStartTime = 0;
unsigned long counter = 0;
bool pumpRunning = false;
unsigned long value = 0;

void pump_off()
{
  digitalWrite(pumpPin, LOW);
  digitalWrite(buzzPin, HIGH);
  pumpRunning = false;
  lcd.setCursor(1,0);
  lcd.print("Pompa Berhenti");
  lcd.setCursor(4, 1);
  lcd.print("00:00:00"); // Tampilkan waktu mundur ke 00:00:00
  delay(2000);
  digitalWrite(buzzPin, LOW);
}

void pump_on(unsigned long counter)
{
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Pompa Berputar");
  digitalWrite(pumpPin, HIGH);
  pumpStartTime = millis();
  pumpRunning = true;
  unsigned long startTime = millis();
  unsigned long remainingTime = 0;
  while (remainingTime <= counter)
  {
    unsigned long currentTime = millis();
    remainingTime = counter - (currentTime - startTime);
    if (remainingTime % 1000 == 0 || remainingTime == 0)
    {
      unsigned long seconds = remainingTime / 1000;
      unsigned long minutes = seconds / 60;
      unsigned long hours = minutes / 60;
      seconds = seconds % 60;
      minutes = minutes % 60;

      lcd.setCursor(4, 1);
      lcd.print(hours);
      lcd.print(":");
      if (minutes < 10)
        lcd.print("0");
      lcd.print(minutes);
      lcd.print(":");
      if (seconds < 10)
        lcd.print("0");
      lcd.print(seconds);
    }
    if (digitalRead(stopButton) == LOW)
    {
      pump_off();
      break; // Keluar dari loop jika tombol stop ditekan
    }
  }
}

void setup()
{
  // Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(buttonPin1, INPUT_PULLUP); // Mengatur pin tombol 1 sebagai input dengan pull-up resistor
  pinMode(buttonPin2, INPUT_PULLUP); // Mengatur pin tombol 2 sebagai input dengan pull-up resistor
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
  pinMode(stopButton, INPUT_PULLUP);
  pinMode(pumpPin, OUTPUT); // Mengatur pin motor sebagai output
  pinMode(buzzPin, OUTPUT);
  lcd.setCursor(1, 0);
  lcd.print("Filtrasi Minyak");
  lcd.setCursor(4,1);
  lcd.print("Otomatis");
  delay(2000);
  lcd.clear();
}

void loop()
{
  lcd.setCursor(1,0);
  lcd.print("Silahkan Pilih");
  lcd.setCursor(2,1);
  lcd.print("Tombol Timer");
  ms_from_start = millis();
  if (ms_from_start - current_ms > interval1)
  {
    buttonState1 = digitalRead(buttonPin1);
    buttonState2 = digitalRead(buttonPin2);
    buttonState3 = digitalRead(buttonPin3);
    buttonState4 = digitalRead(buttonPin4);
    stopState = digitalRead(stopButton); // Memeriksa status tombol stop di setiap iterasi loop
    current_ms = ms_from_start;
  }

  if (stopState == LOW)
  {
    pump_off(); // Mematikan pompa jika tombol stop ditekan
  }
  else if (buttonState1 == LOW && !pumpRunning)
  {
    counter = 1800000;
    pump_on(counter);
  }
  else if (buttonState2 == LOW && !pumpRunning)
  {
    counter = 2700000;
    pump_on(counter);
  }
  else if (buttonState3 == LOW && !pumpRunning)
  {
    counter = 3600000;
    pump_on(counter);
  }
  else if (buttonState4 == LOW && !pumpRunning)
  {
    counter = 4500000;
    pump_on(counter);
  }

  if (pumpRunning && (ms_from_start - pumpStartTime >= counter))
  {
    pump_off(); // Mematikan pompa setelah waktu yang ditentukan
  }
}
