#include <LiquidCrystal_I2C.h>

int button_start;
int button1;
int button2;
int button3;
int button4;
int button5;
int lives = 3;
int matches;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // put your setup code here, to run once:
  button_start = 2;
  button1 = 2;
  button2 = 3;
  button3 = 4;
  button4 = 5;
  button5 = 6;
  pinMode(button_start, INPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);
  pinMode(button5, INPUT);
  lcd.init();
  lcd.backlight();
}

void generic_loop(int cell_start, int cell_end, int button){
  int time_limit = 4000;
  int cell = random(16);
  delay(2000);
  lcd.setCursor(cell, 1);
  lcd.print("π");
  int counter = 0;
  if(cell_start < cell < cell_end){
    while(digitalRead(button) == LOW){
      counter++;
      delay(1);
    }
    lcd.clear();
    if(counter > time_limit)
      lives--;
    else
      time_limit -= 200;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(button_start) == HIGH){
    generic_loop(0, 3, button1);
    generic_loop(4, 6, button2);
    generic_loop(7, 9, button3);
    generic_loop(10, 13, button4);
    generic_loop(14, 16, button5);
  }
}
