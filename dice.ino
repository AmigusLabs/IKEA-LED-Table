/* LedTable
 *
 * Written by: Ing. David Hrbaty
 * 
 * 
 * Main code for Dice game
 */
 
#include "diceCommon.h"
#define MATH_ANIMATION 20
boolean diceRunning;
boolean diceMath;

               
void initDice() {
  diceRunning = true;
  diceMath = false;
  randomSeed(analogRead(A0));
  // Clear pixels
  for (int i = 0; i < FIELD_WIDTH * FIELD_HEIGHT; i++)
    setPixel(i, 0);
  showPixels();
}

void runDice() {
  initDice();
  unsigned long prevUpdateTime = 0;
  unsigned long curTime = 0;

   while(diceRunning) {
      
    if (diceMath) {
       // Clear pixels
      for (int i = 0; i < FIELD_WIDTH * FIELD_HEIGHT; i++)
        setPixel(i, 0);
      
      for(int i=0;i< MATH_ANIMATION;i++) {
        switch (random(6)) {
        case 0:
          printDice(diceOne);
          break;
        case 1:
          printDice(diceTwo);
          break;
        case 2:
          printDice(diceThree);
          break;
        case 3:
          printDice(diceFour);
          break;
        case 4:
          printDice(diceFive);
          break;
        case 5:
          printDice(diceSix);
          break;
        }
        delay(100);
      }
    }
    diceMath = false;
   
    do{
        readInput();
      if (curControl == BTN_EXIT){
        diceRunning = false;
        break;
      }
      else if (curControl == BTN_START) {
        diceMath = true;
      }
      curTime = millis();
      delay(10);
    }
    while ((curTime - prevUpdateTime) <20);//Once enough time  has passed, proceed. The lower this number, the faster the game is
    prevUpdateTime = curTime;
   }
}

void printDice(const boolean dice[][FIELD_WIDTH]){
  int x,y;
  int margin = (FIELD_HEIGHT - FIELD_WIDTH) / 2; 
  for (x=0;x<FIELD_WIDTH;x++){
    for (y=0;y<FIELD_WIDTH;y++){
      if (pgm_read_byte_near ( &dice[x][y]) == 1){
        setTablePixel(y, x+margin, YELLOW);
      } else {
        setTablePixel(y, x+margin, 0x000000);
      }
    }
  }
  showPixels();
}

