/* LedTable
 *
 * Written by: Klaas De Craemer, Ing. David Hrbaty
 * 
 * 
 * Main file with common methods and defines, such as button reading from Bluetooth controller
 * or setting pixels on the LED area
 */
#include <SoftwareSerial.h>
//LED field size
#define  FIELD_WIDTH       10
#define  FIELD_HEIGHT      20
// #define  MIRROR_X 
// #define  MIRROR_Y 

#define USE_FAST_LED   // FAST_LED as library to control the LED strips


#define BLUETOOTH_SPEED 38400 
/*
 * Some defines used by the FAST_LED library
 */
#define FAST_LED_CHIPSET WS2811
#define FAST_LED_DATA_PIN  5


#if defined(USE_OCTOWS2811) && defined(USE_FAST_LED)
#error "Only one of USE_OCTOWS2811 and USE_FAST_LED can be defined"
#endif

#define  NUM_PIXELS    FIELD_WIDTH*FIELD_HEIGHT

/* *** LED color table *** */
#define  GREEN  0x00FF00
#define  RED    0xFF00FF
#define  BLUE   0x0000FF
#define  YELLOW 0xFFFF00
#define  LBLUE  0x00FFFF
#define  PURPLE 0xFF00FF
#define  WHITE  0XFFFFFF
unsigned int colorLib[3] = {YELLOW, BLUE, WHITE};
/* *** Game commonly used defines ** */
#define  DIR_UP    1
#define  DIR_DOWN  2
#define  DIR_LEFT  3
#define  DIR_RIGHT 4

/* *** Bluetooth controller button defines and input method *** */
#define  BTN_NONE  0
#define  BTN_UP    1
#define  BTN_DOWN  2
#define  BTN_LEFT  3
#define  BTN_RIGHT  4
#define  BTN_START  5
#define  BTN_EXIT  6

uint8_t curControl = BTN_NONE;
SoftwareSerial bluetooth(10, 11);

void readInput(){
  curControl = BTN_NONE;
  if (bluetooth.available() > 0) {
    // read the incoming byte:
    uint8_t incomingByte = bluetooth.read();
    Serial.println(incomingByte);
      switch(incomingByte){
        case 238:
          curControl = BTN_LEFT;
          break;
        case 239:
          curControl = BTN_RIGHT;
          break;
        case 236:
          curControl = BTN_UP;
          break;
        case 237:
          curControl = BTN_DOWN;
          break;
        case 224:
          curControl = BTN_START;
          break;
        case 225:
          curControl = BTN_EXIT;
          break;
      }
    } 
}

void testAllPixeles();

/*
 * FAST_LED implementation 
 */


#include "FastLED.h"

CRGB leds[NUM_PIXELS];

void initPixels(){

  FastLED.addLeds<FAST_LED_CHIPSET, FAST_LED_DATA_PIN, RGB>(leds, NUM_PIXELS);

}

void setPixel(int n, uint32_t color){
  leds[n] = CRGB(color);
}

void setPixelRGB(int n, int r,int g, int b){

  leds[n] = CRGB(r,g,b);
}

void setDelay(int duration) {
  FastLED.delay(duration);
}

int getPixel(int n){
  return (leds[n].r << 16) + (leds[n].g << 8) + leds[n].b;  
}

void showPixels(){
  FastLED.show();
}




void setTablePixel(int x, int y, uint32_t color){
#ifdef MIRROR_X
  x = (FIELD_WIDTH-1)-x;
#endif
#ifdef MIRROR_Y
  y = (FIELD_HEIGHT-1)-y;
#endif
  setPixel(y*FIELD_WIDTH + (y%2 ? ((FIELD_WIDTH-1)-x) : x),color);
  #ifdef USE_CONSOLE_OUTPUT
    setTablePixelConsole(x,y,color);
  #endif
}

void setTablePixelRGB(int x, int y, int r,int g, int b){
#ifdef MIRROR_X
  x = (FIELD_WIDTH-1)-x;
#endif
#ifdef MIRROR_Y
  y = (FIELD_HEIGHT-1)-y;
#endif
  setPixelRGB(y*FIELD_WIDTH + (y%2 ? ((FIELD_WIDTH-1)-x) : x),r,g,b);
  #ifdef USE_CONSOLE_OUTPUT
    setTablePixelConsole(x,y,color);
  #endif
}

void clearTablePixels(){
  for (int n=0; n<FIELD_WIDTH*FIELD_HEIGHT; n++){
    setPixel(n,0);
  }
}

/* *** text helper methods *** */
#include "font.h"
uint8_t charBuffer[8][8];

void printText(char* text, unsigned int textLength, int xoffset, int yoffset, uint32_t color){
  uint8_t curLetterWidth = 0;
  int curX = xoffset;
  clearTablePixels();
  
  //Loop over all the letters in the string
  for (int i=0; i<textLength; i++){
    //Determine width of current letter and load its pixels in a buffer
    curLetterWidth = loadCharInBuffer(text[i]);
    //Loop until width of letter is reached
    for (int lx=0; lx<curLetterWidth; lx++){
      //Now copy column per column to field (as long as within the field
      if (curX>=FIELD_WIDTH){//If we are to far to the right, stop loop entirely
        break;
      } else if (curX>=0){//Draw pixels as soon as we are "inside" the drawing area
        for (int ly=0; ly<8; ly++){//Finally copy column
          setTablePixel(curX,yoffset+ly,charBuffer[lx][ly]*color);
        }
      }
      curX++;
    }
  }
  
  showPixels();
}

//Load char in buffer and return width in pixels
uint8_t loadCharInBuffer(char letter){
  uint8_t* tmpCharPix;
  uint8_t tmpCharWidth;
  
  int letterIdx = (letter-32)*8;
  
  int x=0; int y=0;
  for (int idx=letterIdx; idx<letterIdx+8; idx++){
    for (int x=0; x<8; x++){
      charBuffer[x][y] = ((font[idx]) & (1<<(7-x)))>0;
    }
    y++;
  }
  
  tmpCharWidth = 8;
  return tmpCharWidth;
}


/* *********************************** */

void fadeOut(){
  //Select random fadeout animation
  int selection = random(3);
  
  switch(selection){
    case 0:
    case 1:
    {
      //Fade out by dimming all pixels
      for (int i=0; i<100; i++){
        dimLeds(0.97);
        showPixels();
        delay(10);
      }
      break;
    }
    case 2:
    {
      //Fade out by swiping from left to right with ruler
      const int ColumnDelay = 10;
      int curColumn = 0;
      for (int i=0; i<FIELD_WIDTH*ColumnDelay; i++){
        dimLeds(0.97);
        if (i%ColumnDelay==0){
          //Draw vertical line
          for (int y=0;y<FIELD_HEIGHT;y++){
            setTablePixel(curColumn, y, GREEN);
          }
          curColumn++;
        }
        showPixels();
        delay(5);
      }
      //Sweep complete, keep dimming leds for short time
      for (int i=0; i<100; i++){
        dimLeds(0.9);
        showPixels();
        delay(5);
      }
      break;
    }
  }
}

void dimLeds(float factor){
  //Reduce brightness of all LEDs, typical factor is 0.97
  for (int n=0; n<(FIELD_WIDTH*FIELD_HEIGHT); n++){
    int curColor = getPixel(n);
    //Derive the tree colors
    int r = ((curColor & 0xFF0000)>>16);
    int g = ((curColor & 0x00FF00)>>8);
    int b = (curColor & 0x0000FF);
    //Reduce brightness
    r = r*factor;
    g = g*factor;
    b = b*factor;
    //Pack into single variable again
    curColor = (r<<16) + (g<<8) + b;
    //Set led again
    setPixel(n,curColor);
  }
}

void testMatrix() {
    setTablePixel(0, 0, WHITE);
    showPixels();
    delay(2000);
    setTablePixel(0, 9, WHITE);
    showPixels();
    delay(2000);
    setTablePixel(9, 0, WHITE);
    showPixels();
    delay(2000);
    setTablePixel(9, 9, WHITE);
}
void setup(){
  Serial.begin(115200);
  //Wait for serial port to connect
  bluetooth.begin(BLUETOOTH_SPEED);
  //Initialise display
  initPixels();
  showPixels();


  //Init random number generator
  randomSeed(millis());

  mainLoop();
}

void loop(){
}

void testAllPixeles() {
     for (int x=0; x<FIELD_WIDTH; x++) {
    for (int y=0; y<FIELD_HEIGHT; y++) {
      setTablePixel(x, y, RED);
        showPixels();
      delay(50);
      setTablePixel(x, y, WHITE);
    }
  }
}

