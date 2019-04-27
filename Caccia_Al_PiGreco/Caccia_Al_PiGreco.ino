#include <LiquidCrystal_I2C.h>

int button_start;
int button1;
int button2;
int button3;
int button4;
int button5;
int lives;
int time_limit;
int matches;
int counter;
int support;

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte Heart[8] = { B00000, B01010, B11111, B11111, B11111, B01110, B00100, B00000 };
byte PiGreco[8] = { B00000, B11111, B01010, B01010, B01010, B01010, B00000, B00000 };

void setup() {
  // put your setup code here, to run once:
  button_start = 2;
  button1      = 4;
  button2      = 6;
  button3      = 8;
  button4      = 10;
  button5      = 12;
  pinMode(button_start, INPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);
  pinMode(button5, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, Heart);
  lcd.createChar(1, PiGreco);
  start();
}

void generic_loop(int cell, int button){
  delay(2000);
  lcd.setCursor(cell, 1);
  lcd.write(byte(1));
  counter = 0;
  support = 1;
  while(digitalRead(button) == LOW && counter < time_limit){
    counter++;
    delay(1);
    if((button != button1 && digitalRead(button1) == HIGH) || (button != button2 && digitalRead(button2) == HIGH) || (button != button3 && digitalRead(button3) == HIGH) || (button != button4 && digitalRead(button4) == HIGH) || (button != button5 && digitalRead(button5) == HIGH)){
      while(support < 2){
        lives--;
        support++;
      }
      counter++;
      lcd.setCursor(4, 0);
      lcd.print(lives);
    }
  }
  
  clean();
  if(counter == time_limit)
    lives--;
  else if(counter < 80){
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("NON IMBROGLIARE!");
    lcd.setCursor(4, 1);
    lcd.print("- 1 VITA");
    delay(3000);
    lives--;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(byte(0));
    lcd.print(" = ");
    lcd.setCursor(8, 0);
    lcd.print("WON = ");
  }
  else{
    if(time_limit > 300)
      time_limit -= 150;
    else
      time_limit -= 50;
    matches++;
  }
  lcd.setCursor(4, 0);
  lcd.print(lives);
  lcd.setCursor(14, 0);
  lcd.print(matches);
}

void clean(){
  for(int i = 0; i < 16; i++){
    lcd.setCursor(i, 1);
    lcd.print(" ");
  }
}

void start(){
  lcd.clear();
  time_limit = 4050;
  lives      = 3;
  matches    = 0;
  while(digitalRead(button_start) == LOW){
    lcd.setCursor(0, 0);
    lcd.print("INIZIA A GIOCARE");
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.print(" = "+ (String)lives);
  lcd.setCursor(8, 0);
  lcd.print("WON = " + (String)matches);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(lives > 0 && time_limit > 0){
    int n_random = random(1, 5);
    if(n_random == 1)
      generic_loop(0, button1);
    else if(n_random == 2)
      generic_loop(4, button2);
    else if(n_random == 3)
      generic_loop(8, button3);
    else if(n_random == 4)
      generic_loop(12, button4);
    else if(n_random == 5)
      generic_loop(16, button5);
  }
  else{
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("PUNTI TOTALI:");
    lcd.setCursor(7, 1);
    lcd.print(matches);
    delay(3000);
    start();
  } 
}
