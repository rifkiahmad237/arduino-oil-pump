//Program by: Alim Mulyadi
//tgl: 21/02/2021
//Program: Countdown Timer Sederhana

#include <LiquidCrystal_I2C.h>
#include "Countimer.h"

Countimer tdown;
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define pb_set    A0  //Set
#define pb_up     A1  //Up dan Start
#define pb_down   A2  //Down, Pause, dan Reset

int relay = 4;
int buzzer = 9;

int set_detik = 0;
int set_menit = 0;
int set_jam = 0;
int last_set_detik = 0;
int last_set_menit = 0;
int last_set_jam = 0;
int set = 0;
bool kondisi_set = 0;
bool kondisi_relay = 0;
bool kondisi_reset = 0;

unsigned long lastmillis;

void setup() {
  Serial.begin (9600);

  pinMode(pb_set,   INPUT_PULLUP);
  pinMode(pb_up,    INPUT_PULLUP);
  pinMode(pb_down,  INPUT_PULLUP);

  pinMode(relay, OUTPUT);
  pinMode(buzzer, OUTPUT);

  lcd.begin();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Countdown  Timer");
  lcd.setCursor(0, 1);
  lcd.print("By: Alim Mulyadi");
  tdown.setInterval(print_time, 1000);
  delay(2000);
  lcd.clear();
}

void print_time() {
  set_detik = set_detik - 1;
  if (set_detik < 0) {
    set_detik = 59;
    set_menit = set_menit - 1;
  }
  if (set_menit < 0) {
    set_menit = 59;
    set_jam = set_jam - 1;
  }
}

void loop() {
  tdown.run();

  if (digitalRead (pb_set) == 0) {
    if (kondisi_set == 0 && kondisi_relay == 0) {
      kondisi_set = 1;
      set = set + 1;
      if (set > 3) {
        set = 0;
      }
      delay(100);
    }
  } else {
    kondisi_set = 0;
  }

  if (digitalRead (pb_up) == 0) {
    if (set == 0) {
      tdown.start();
      kondisi_relay = 1;
    }
    if (set == 1) {
      set_detik++;
      last_set_detik = set_detik;
    }
    if (set == 2) {
      set_menit++;
      last_set_menit = set_menit;
    }
    if (set == 3) {
      set_jam++;
      last_set_jam = set_jam;
    }
    if (set_detik > 59) {
      set_detik = 0;
    }
    if (set_menit > 59) {
      set_menit = 0;
    }
    if (set_jam > 99) {
      set_jam = 0;
    }
    delay(200);
  }

  if (digitalRead (pb_down) == 0) {
    //delay(500);
    lastmillis = millis();
    kondisi_reset = 0;
    while (digitalRead (pb_down) == 0 && set == 0) {
      if (millis() - lastmillis > 500) {
        kondisi_reset = 1;
        kondisi_relay = 0;
        tdown.stop();
        set_detik = last_set_detik;
        set_menit = last_set_menit;
        set_jam = last_set_jam;

        lcd.setCursor(4, 1);
        if (set_jam <= 9) {
          lcd.print("0");
        }
        lcd.print(set_jam);
        lcd.print(":");
        if (set_menit <= 9) {
          lcd.print("0");
        }
        lcd.print(set_menit);
        lcd.print(":");
        if (set_detik <= 9) {
          lcd.print("0");
        }
        lcd.print(set_detik);
        lcd.print("   ");
        delay(100);
      }
    }

    if (kondisi_reset == 0) {
      if (set == 0) {
        tdown.stop();
        kondisi_relay = 0;
      }
      if (set == 1) {
        set_detik--;
        last_set_detik = set_detik;
      }
      if (set == 2) {
        set_menit--;
        last_set_menit = set_menit;
      }
      if (set == 3) {
        set_jam--;
        last_set_jam = set_jam;
      }
      if (set_detik < 0) {
        set_detik = 59;
      }
      if (set_menit < 0) {
        set_menit = 59;
      }
      if (set_jam < 0) {
        set_jam = 99;
      }
      delay(200);
    }
  }

  lcd.setCursor(0, 0);
  if (set == 0) {
    lcd.print("      Timer     ");
  }
  if (set == 1) {
    lcd.print("  Set Timer SS  ");
  }
  if (set == 2) {
    lcd.print("  Set Timer MM  ");
  }
  if (set == 3) {
    lcd.print("  Set Timer HH  ");
  }

  lcd.setCursor(4, 1);
  if (set_jam <= 9) {
    lcd.print("0");
  }
  lcd.print(set_jam);
  lcd.print(":");
  if (set_menit <= 9) {
    lcd.print("0");
  }
  lcd.print(set_menit);
  lcd.print(":");
  if (set_detik <= 9) {
    lcd.print("0");
  }
  lcd.print(set_detik);
  lcd.print("   ");

  if (set_detik == 0 && set_menit == 0 && set_jam == 0 && kondisi_relay == 1) {
    kondisi_relay = 0;
    tdown.stop();
    digitalWrite(relay, LOW);
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(buzzer, LOW);
  }

  if (kondisi_relay == 1) {
    digitalWrite(relay, HIGH);
  }
  else {
    digitalWrite(relay, LOW);
  }

  delay(1);
}
