#include <LiquidCrystal.h>

#include "joystick.h"

byte enemy[8] = {
  B00000,  
  B00000,
  B11111,
  B01001,
  B01001,
  B11111,
  B00100,
  B00000,
};

byte bullet[8] = {
  B00000,  
  B00000,
  B00000,
  B01110,
  B01110,
  B00000,
  B00000,
  B00000,
};

byte hearth[8] = {
  B00000,
  B00000,
  B01010,
  B10101,
  B10001,
  B01010,
  B00100,
  B00000,
};

byte playerC[8] = {
  B00000,
  B00100,
  B01010,
  B00100,
  B01110,
  B00100,
  B01010,
  B00000,
};

byte deffense[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte barrel[8] = {
  B11111,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111,
};

byte explosion[8] = {
  B00000,
  B00010,
  B11001,
  B00100,
  B00100,
  B01010,
  B10101,
  B00000,
};

byte finish[8] = {
  B01010,
  B01010,
  B01010,
  B01010,
  B01010,
  B01010,
  B01010,
  B01010,
};

struct Player{
  int x;
  int y;
  unsigned int life;
};

char maplvl1[16][2];

Joystick joystick(A0,A1,8);
LiquidCrystal lcd(1, 2, 4, 5, 6, 7);

int LPin0 = 9;
int LPin1 = 10;
int LPin2 = 11;

Player player;
Player prev;

int mapCounter = 0;

void builder(){
  lcd.clear();
  if(mapCounter < 2){
    for(unsigned int i = 0; i < 16; i++){
      for(unsigned int j = 0; j < 2; j++){
        maplvl1[i][j] = 0;
        if(i % 4 == 0 && j == 1 && i < 10){
          maplvl1[i][j] = 1;
          lcd.setCursor(i,j);
          lcd.write(byte(4));
        }else if(i % 4 == 2 && j == 0 && i < 10){
          maplvl1[i][j] = 1;
          lcd.setCursor(i,j);
          lcd.write(byte(4));
        }
      }
    }
    
    maplvl1[15][0] = 2;
    maplvl1[15][1] = 2;
  
    lcd.setCursor(15,0);
    lcd.write(byte(7));
    lcd.setCursor(15,1);
    lcd.write(byte(7));

    if(mapCounter == 1){
      maplvl1[14][0] = 3;
      maplvl1[14][1] = 3;
      
      lcd.setCursor(14,0);
      lcd.write(byte(3));
      lcd.setCursor(14,1);
      lcd.write(byte(3));
    }
  }
  player.x = 0;
  player.y = 0;

  mapCounter++;
}

void setup() { 
  lcd.begin(16,2);
  lcd.createChar(0, playerC);
  lcd.createChar(1, hearth);
  lcd.createChar(2, bullet);
  lcd.createChar(3, enemy);
  lcd.createChar(4, deffense);
  lcd.createChar(5, barrel);
  lcd.createChar(6, explosion);
  lcd.createChar(7, finish);

  player.life = 3;
  
  pinMode(LPin0, OUTPUT);
  pinMode(LPin1, OUTPUT);
  pinMode(LPin2, OUTPUT);
  
  digitalWrite(LPin0, HIGH);
  digitalWrite(LPin1, HIGH);
  digitalWrite(LPin2, HIGH);

  builder();
  
}

void loop() {
  if(player.life < 3){
    digitalWrite(LPin0, LOW);
  }
  if(player.life < 2){
    digitalWrite(LPin1, LOW);
  }
  if(player.life < 1){
    digitalWrite(LPin2, LOW);
  }
  prev.x = player.x;
  prev.y = player.y;
  if(joystick.getX() < 400){
    player.x++;
    if(player.x > 15){
      player.x = 0;
    }
  }else if(joystick.getX() > 700){
    player.x--;
    if(player.x < 0){
      player.x = 15;
    }
  }
  if(joystick.getY() > 700){
    player.y++;
    if(player.y > 2){
      player.y = 1;
    }
  }else if(joystick.getY() < 500){
    player.y--;
    if(player.y < 0){
      player.y = 0;
    }
  }
 for(unsigned int i = 0; i < 16; i++){
    for(unsigned int j = 0; j < 2; j++){
      lcd.setCursor(i,j);
      if(maplvl1[i][j] == 1){
        if(player.x == i && player.y == j){
          player.x = prev.x;
          player.y = prev.y;
          player.life--;
        }
      }else if(maplvl1[i][j] == 2){
        if(player.x == i && player.y == j){
          lcd.clear();

          digitalWrite(LPin0, LOW);
          digitalWrite(LPin1, LOW);
          digitalWrite(LPin2, LOW);
          
          if(mapCounter == 2){
            lcd.print("Nyertel");
            lcd.setCursor(0,1);
            lcd.print(player.life);
            lcd.print(" Eleted maradt");
            delay(10000);
          }else{
            lcd.print("Sikeres palya");
            lcd.setCursor(0,1);
            lcd.print(player.life);
            lcd.print(" Eleted maradt");

            delay(5000);
            
            builder();
          }
        }
      }
    }
  }
  if(player.life == 0){
    lcd.clear();
    lcd.print("Vesztettel");
    delay(10000);
  }else{
    lcd.setCursor(prev.x ,prev.y);
    lcd.print(" ");
    lcd.setCursor(player.x ,player.y);
    lcd.write(byte(0));
  }
 
 delay(500);
 //lcd.clear();
}
