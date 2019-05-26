#include <LiquidCrystal_I2C.h>

//Creo le variabili dei 5 bottoni + quello d'inizio, quella delle vite, delle partite, del tempo massimo per il quale il PiGreco può essere premuto, del counter per misurare il tempo che
//ci mette l'utente a premere il pulsante e una variabile d'appoggio "button" che a seconda della cella in cui verrà printato il PiGreco assumerà il valore del bottone corrispondente
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
int button;
int n_random;

LiquidCrystal_I2C lcd(0x27, 16, 2);

//Creo 3 byte personalizzati, rispettivamente il cuore per le vite e i bonus, il PiGreco e la X per il malus
byte Heart[8]   = { B00000, B01010, B11111, B11111, B11111, B01110, B00100, B00000 };
byte PiGreco[8] = { B00000, B11111, B01010, B01010, B01010, B01010, B00000, B00000 };
byte Malus[8]   = { B01110, B11111, B10101, B11111, B11111, B01010, B01110, B00000 };

void setup() {
  // put your setup code here, to run once:
  button_start  = 8;
  button1       = 6;
  button2       = 4;
  button3       = 12;
  button4       = 10;
  button5       = 2;
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
  lcd.createChar(2, Malus);
  start();
}

void generic_loop(int cell, int simbol){
  //Assegno alla variabile button il valore corrispondente
  if(cell == 0)
    button = button1;
  else if(cell == 4)
    button = button2;
  else if(cell == 8)
    button = button3;
  else if(cell == 12)
    button = button4;
  else if(cell == 16)
    button = button5;
  delay(2000);
  lcd.setCursor(cell, 1);
  lcd.write(simbol);
  counter = 0;
  while(digitalRead(button) == LOW && counter < time_limit){
    counter++;
    delay(1);
    //Se l'utente preme un bottone differente da quello assegnato sopra gli verrà tolta una vita
    if(button != button1 && digitalRead(button1) == HIGH || button != button2 && digitalRead(button2) == HIGH || button != button3 && digitalRead(button3) == HIGH || button != button4 && digitalRead(button4) == HIGH || button != button5 && digitalRead(button5) == HIGH){
      lives--;
      break;
    }
  }
  //Ciclo che mi cancella solo la seconda riga del display LCD
  for(int i = 0; i < 16; i++){
    lcd.setCursor(i, 1);
    lcd.print(" ");
  }
  //Se esce il malus e l'utente non preme alcun bottone gli verrà tolta una vita, tranne se il carattere è il malus
  if(counter == time_limit && simbol == byte(1))
    lives--;
  else{
    //Se il carattere è il bonus e l'utente riesce a prenderlo gli si aggiunge una vita, altrimenti sale semplicemente di livello e il tempo limite decrementa
    if(counter < time_limit && simbol == byte(0))
      lives++;
    else if(counter < time_limit && simbol == byte(2))
      lives--;
    time_limit -= 100;
    matches++;
  }
  lcd.setCursor(4, 0);
  lcd.print(lives);
  lcd.setCursor(14, 0);
  lcd.print(matches);
}

//Creo un ciclo d'inizio che verrà richiamato solamente nel setup() iniziale e quando l'utente non ha più vite
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
  lcd.setCursor(7, 0);
  lcd.print("GAME = " + (String)matches);
}

int positions(){
  int pos = random(5);
  if(pos == 1)
    return 0;
  else if(pos == 2)
    return 4;
  else if(pos == 3)
    return 8;
  else if(pos == 4)
    return 12;
  else if(pos == 5)
    return 16;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(lives > 0){
    //Estraggo 7 numeri random, se è un numero da 1 a 5 parte il ciclo con il PiGreco e la rispettiva posizione, se è 6 o 7 parte lo stesso ciclo con rispettivamente malus e bonus
    n_random = random(1, 8);
    if(n_random < 6)
      generic_loop(positions(), byte(1));
    else if(n_random == 6)
      generic_loop(positions(), byte(0));
    else if(n_random == 7)
      generic_loop(positions(), byte(2));
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
