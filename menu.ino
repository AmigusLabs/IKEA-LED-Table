/* LedTable
 *
 * Written by: Klaas De Craemer, Ing. David Hrbaty
 * 
 * Menu system for the LED table
 */

#define MINSELECTION  1
#define MAXSELECTION  9
unsigned int curSelection = MINSELECTION;

#define TEXTSPEED  140

void mainLoop(void){
  char* curSelectionText;
  int curSelectionTextLength;
  unsigned long prevUpdateTime = 0;
  
  while(true){

    updateScreenIntensity();
    
    //Show menu system and wait for input
    clearTablePixels();
    switch (curSelection){
      case 1:
        curSelectionText = "1 Rainbow";
        curSelectionTextLength = 9;
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
        curSelectionText = "6 Tetris";
        curSelectionTextLength = 8;
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
    printTextToScreen(&(curSelectionText[2]));
    
    boolean selectionChanged = false;
    boolean runSelection = false;
    //Scroll current selection text from right to left;
    for (int x=FIELD_WIDTH; x>-(curSelectionTextLength*8); x--){
      printText(curSelectionText, curSelectionTextLength, x, (FIELD_HEIGHT-8)/2, YELLOW);
      //Read buttons
      unsigned long curTime;
      do{
        readInput();
        updateScreenIntensity();
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
    }
    
    //If we are here, it means a selection was changed or a game started, or user did nothing
    if (selectionChanged){
      //For now, do nothing
    } else if (runSelection){//Start selected game
      switch (curSelection){
        case 1:
          runRainbowPalette();
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
          runTetris();
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
    } else {
      //If we are here, no action was taken by the user, so we will move to the next selection automatically
      curSelection++;
      checkSelectionRange();
    }
  }
}

void checkSelectionRange(){
  if (curSelection>MAXSELECTION){
    curSelection = MINSELECTION;
  } else if (curSelection<MINSELECTION){
    curSelection = MAXSELECTION;
  }
}
