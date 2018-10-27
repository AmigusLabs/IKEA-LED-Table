 
const int pinCS = 53;
const int numberOfHorizontalDisplays = 8;
const int numberOfVerticalDisplays = 1;
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

const int letterSpacer = 1;
const int letterWidth = 5 + letterSpacer; // The font width is 5 pixels
const int screenIntensity = 4; // Use a value between 0 and 15 for brightness
void setupScreen() {
  matrix.setIntensity(screenIntensity); 
  for( int i=0; i<numberOfHorizontalDisplays; i++) {
    matrix.setPosition(i, i, 0); // The first display is at <0, 0>
    matrix.setRotation(i, 1);    // Display is position upside down
  }

  printTextToScreen("amigus labs!");
  delay(2000);
}

int lettersPerLine = numberOfHorizontalDisplays * 8 / letterWidth;
void printTextToScreen(const char* text) {
  matrix.fillScreen(LOW);
  for (int i=0; i<lettersPerLine; i++) {
    if (strlen(text) > i) {
      matrix.drawChar(1+i*letterWidth, 0, text[i], HIGH, LOW, 1);
    }
  }
  matrix.write();
}


