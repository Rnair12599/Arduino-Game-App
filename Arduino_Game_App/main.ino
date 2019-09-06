// Cutie Hack Project 2017
// By: Rahul Nair and Suhas Jagannath
// November 11, 2017

#include <LiquidCrystal.h>
byte face1[8] = {               // sprite for character 1
   B00000,
   B00000,
   B00000,
   B01010,
   B00000,
   B10001,
   B01110,
   B00000,
};

byte face2[8] = {               // sprite for character 2
   B00000,
   B00000,
   B00000,
   B01110,
   B10101,
   B10101,
   B01110,
   B11011,
};

byte face3[8] = {               // sprite for character 3
   B00100,
   B01110,
   B11111,
   B01110,
   B10101,
   B10101,
   B01110,
   B11011,
};

byte face4[8] = {               // sprite for character 4
   B10001,
   B01110,
   B11111,
   B10101,
   B01110,
   B10001,
   B01110,
   B00100,
};

byte face5[8] = {               // sprite for character 5
   B10101,
   B01110,
   B11111,
   B10101,
   B01110,
   B11111,
   B01110,
   B11111,
};

byte block[8] = {               // sprite for blocks
   B11111,
   B11111,
   B11111,
   B11111,
   B11111,
   B11111,
   B11111,
   B11111,
};



int buttonState = 0;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;     // integers correspond to ports being used
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);      // main function for LCD program
int state = 0;                  // mode the button is on (HIGH or LOW)
int where = 0;                  // variable used to maintain positon of Kirby without holding down button
int pressed = 0;                // proxy variable used to maintain a certain state
int i = 18;                     // variable used to represent column space in first row
int j = 18;                     // variable used to represent columns space in second row
int randNum = 0;                // random number of spaces in between each block
int score = 0;                  // score recieved based on seconds lasted
bool gameEnd = false;           // says when game is over
int sprite = 0;                 // starting character sprite of game

void setup()
{
   randomSeed(analogRead(A0));
   lcd.begin(16, 2);
   pinMode(8, INPUT);
   lcd.createChar(0, face1);
   lcd.createChar(2, face2);
   lcd.createChar(3, face3);
   lcd.createChar(4, face4);
   lcd.createChar(5, face5);
   lcd.setCursor(1, 1);
   lcd.write(byte(sprite));
   lcd.createChar(1, block);
   randNum = random(4, 6);
   Serial.begin(9600);
}

void loop()
{
   if (lost(i, where)) {
      state = digitalRead(8);

      if (state == HIGH && pressed == LOW) {
         pressed = HIGH;
         jump(sprite);
      }
      pressed = state;


      static unsigned long interval = 500;      // ms 500 350 200 100
      static unsigned long lastRefreshTime = 0;

      if (millis() - lastRefreshTime >= interval) {
         lastRefreshTime += interval;
         if (i == j + randNum || i == j + randNum + 1
             || i + 1 == j + randNum || i == j + randNum + 2
             || i + 2 == j + randNum) {
            Serial.print(i);
            Serial.print("\n");
            Serial.print(j + randNum);
            i = 18;
            j = 18;

         }

         blocks(i, 1);
         blocks(j + randNum, 0);

         if (i == 0) {
            lcd.setCursor(0, 1);
            lcd.print(" ");

            i = 18;

         }

         if (j + randNum == 0) {

            lcd.setCursor(0, 0);
            lcd.print(" ");
            randNum = random(4, 6);

            j = 18;

         }
         --j;
         --i;


         score = millis() / 1000;

         if (score == 15) {

            interval = 400;
            sprite = 2;

         }
         if (score == 30) {

            interval = 300;
            sprite = 3;

         }
         if (score == 45) {

            interval = 200;
            sprite = 4;

         }
         if (score == 60) {

            interval = 100;
            sprite = 5;
         }
      }
   } else {

      lost(i, where);

   }
}

void blocks(int row, int column)
{

   lcd.setCursor(row, column);
   lcd.write(byte(1));
   lcd.setCursor(row + 1, column);
   lcd.print(" ");
}



void jump(int sprite)
{

   if (where == 0) {
      lcd.setCursor(1, 0);
      lcd.write(byte(sprite));
      lcd.setCursor(1, 1);
      lcd.print(" ");
      where = 1;
   } else {
      lcd.setCursor(1, 1);
      lcd.write(byte(sprite));
      lcd.setCursor(1, 0);
      lcd.print(" ");
      where = 0;
   }


}

bool lost(int kirby, int block)
{

   if (i == 0 && block == 0 || j + randNum == 0 && block == 1) {
      if (gameEnd == false) {

         gameEnd = true;
         lcd.clear();

      }
      lcd.setCursor(3, 0);
      lcd.print("GAME OVER");
      lcd.setCursor(7, 1);

      lcd.print(score);
      return false;

   }

   return true;

}
