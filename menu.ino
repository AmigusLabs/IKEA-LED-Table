/* LedTable
 *
 * Written by: Klaas De Craemer, Ing. David Hrbaty
 * 
 * Menu system for the LED table
 */

 
#include <avr/pgmspace.h>

const uint8_t cover_01_tetris[] = {
B00000000, B00000100, B00000001, B10000000, B00100000, B10000010, B00110000, B10001000, B01100000, B00000011, B00100000, B11001000, B00000011, B00010001, B11110111, B11111001, B11000000
};

const uint8_t cover_empty[] = {
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000
};

const uint8_t cover_numbers[][5] = {
  { B11111100, B01101011, B01011010, B11000111, B11100000 /* 0 */ },
  { B11111100, B11110111, B10111101, B11000111, B11100000 /* 1 */ },
  { B11111100, B01111011, B00011011, B11000111, B11100000 /* 2 */ },
  { B11111100, B01111011, B10011110, B11000111, B11100000 /* 3 */ },
  { B11111101, B01101011, B00011110, B11110111, B11100000 /* 4 */ },
  { B11111100, B01101111, B00011110, B11000111, B11100000 /* 5 */ },
  { B11111100, B01101111, B00011010, B11000111, B11100000 /* 6 */ },
  { B11111100, B01111011, B11011110, B11110111, B11100000 /* 7 */ },
  { B11111100, B01101011, B00011010, B11000111, B11100000 /* 8 */ },
  { B11111100, B01101011, B00011110, B11110111, B11100000 /* 9 */ }
};

#define MINSELECTION  1
#define MAXSELECTION  9
unsigned int curSelection = MINSELECTION;

#define TEXTSPEED  140

void mainLoop(void){
  char* curSelectionText;
  int curSelectionTextLength;
  unsigned long prevUpdateTime = 0;
  unsigned long curTime = millis();
  unsigned long waitTime = millis();

  while(true){
    waitTime = millis();
    //Show menu system and wait for input
    clearTablePixels();
    switch (curSelection){
      case 1:
        curSelectionText = "1 Tetris";
        curSelectionTextLength = 8;
        break;
      case 2:
        curSelectionText = "2 Animation";
        curSelectionTextLength = 11;
        break;    
      case 3:
        curSelectionText = "3 Stars";
        curSelectionTextLength = 7;
        break;  
      case 4:
        curSelectionText = "4 Vu Meter";
        curSelectionTextLength = 10;
        break;    
      case 5:
        curSelectionText = "5 Dice"; 
        curSelectionTextLength = 6;
        break;
      case 6:
        curSelectionText = "6 Rainbow";
        curSelectionTextLength = 9;
        break;
      case 7:
        curSelectionText = "7 Snake";
        curSelectionTextLength = 7;
        break;
      case 8:
        curSelectionText = "8 Pong";
        curSelectionTextLength = 6;
        break;       
      case 9:
        curSelectionText = "9 Bricks";
        curSelectionTextLength = 8;
        break; 
    }

    
    if ((waitTime - curTime)> 20000){
      curSelection = 2;
      curSelectionText = "2 Animation";
      curSelectionTextLength = 11;
      printTextToScreen(&(curSelectionText[2]));
      runColorPalette();
      curTime = millis();

    }
    
    boolean selectionChanged = false;
    boolean runSelection = false;
    printTextToScreen(&(curSelectionText[2]));



    byte firstDigit = curSelection/10;
    byte secondDigit = curSelection%10;
    if (curSelection == 1) {
      printMenuGraphic(cover_numbers[firstDigit], cover_numbers[secondDigit], cover_01_tetris);
    } else {
      printMenuGraphic(cover_numbers[firstDigit], cover_numbers[secondDigit], cover_empty);
    }

    readInput();
        if (curControl != BTN_NONE){
          if (curControl == BTN_LEFT){
            curSelection--;
            selectionChanged = true;
          } else if (curControl == BTN_RIGHT){
            curSelection++;
            selectionChanged = true;
          } else if (curControl == BTN_START){
            runSelection = true;
          }
          
          checkSelectionRange();
        }

/*
    //Scroll current selection text from right to left;
    for (int x=FIELD_WIDTH; x>-(curSelectionTextLength*8); x--){
      printText(curSelectionText, curSelectionTextLength, x, (FIELD_HEIGHT-8)/2, YELLOW);
      //Read buttons
      unsigned long curTime;
      do{
        readInput();
        if (curControl != BTN_NONE){
          if (curControl == BTN_LEFT){
            curSelection--;
            selectionChanged = true;
          } else if (curControl == BTN_RIGHT){
            curSelection++;
            selectionChanged = true;
          } else if (curControl == BTN_START){
            runSelection = true;
          }
          
          checkSelectionRange();
        }
        curTime = millis();
      } while (((curTime - prevUpdateTime) < TEXTSPEED) && (curControl == BTN_NONE));//Once enough time  has passed, proceed
      prevUpdateTime = curTime;
      
      if (selectionChanged || runSelection)
        break;
    }*/

    //If we are here, it means a selection was changed or a game started, or user did nothing
    if (selectionChanged){
      //For now, do nothing
      curTime = millis();
    } else if (runSelection){//Start selected game
      switch (curSelection){
        case 1:
          runTetris();
          break;   
        case 2:
          runColorPalette();
          break;
        case 3:
          runStars();         
          break;
        case 4:
          runVUmeter();
          break;
        case 5:
          runDice();
          break;    
        case 6:
          runRainbowPalette();
          break;
        case 7:
          runSnake();
          break; 
        case 8:
          runPong();        
          break;  
        case 9:       
          runBricks();
          break;                      
      }
    }/* else {
      if (curSelection != 1) {
      //If we are here, no action was taken by the user, so we will move to the next selection automatically
      curSelection++;
      checkSelectionRange();
      }
    }*/
  }
}

void checkSelectionRange(){
  if (curSelection>MAXSELECTION){
    curSelection = MINSELECTION;
  } else if (curSelection<MINSELECTION){
    curSelection = MAXSELECTION;
  }
}
