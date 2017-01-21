#include <Adafruit_GFX.h>    
#include <Adafruit_ST7735.h> 
#include <SPI.h>

#include "pitches.h";


#define TFT_CS     10
#define TFT_RST    9  
#define TFT_DC     8

#define TFT_SCLK 13   
#define TFT_MOSI 11   

const int maxWidth = 128;
const int maxHeight =  160;

const int buttonAPin = 3;
const int buttonBPin = 4;
const int buttonCPin = 5;

const int audioPin = 2;


const int DELTA_MAX = 1000;
const int DELTA_MIN = 50;

int buttonAState = 0;
int buttonBState = 0;
int buttonCState = 0;



// player constants
const int pSize = 8;

int x = 0;
int y = 0;



int color = 1;

int analogY = A1;
int analogX = A0;

int analogXValue = 0;  // joystic X analog
int analogYValue = 0;  // joystic Y analog


// audio
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

void setup(void) {

  Serial.begin(9600);
  
  pinMode(buttonAPin, INPUT);
  pinMode(buttonBPin, INPUT);
  pinMode(buttonCPin, INPUT_PULLUP);

  tft.initR(INITR_BLACKTAB); 
  tft.fillScreen(ST7735_BLACK); 
 

  //tft print function!
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(0);
  tft.setCursor(0,80);
  tft.println("Hello Global Game Jam");
  tft.setCursor(0,0);  


    for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(audioPin, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(audioPin);
  }
  
}


//check inside screen

int checkMax(int pos, int maximum, bool invert) {
  
  if(invert == true) {
    if (pos < 1) {
        pos = 1;
    }
  } else {
    if (pos > maximum - pSize) {
        pos = maximum - pSize;
    }
  }

  return pos;
  
}


//check inputs
void checkJoystic() {
  
    //read Buttons
    buttonAState = digitalRead(buttonAPin);
    buttonBState = digitalRead(buttonBPin);
    buttonCState = digitalRead(buttonCPin);

    //read Analogs
    analogXValue = analogRead(analogX);
    analogYValue = analogRead(analogY);
  
}


// main loop

void loop() {


    //listen pins
    checkJoystic();

    if(buttonCState == HIGH) {
       
    } else {
      color = ST7735_BLACK;
    }
    

    if(buttonAState == HIGH) {
      
    } else {
     color = ST7735_GREEN;
    }

     if(buttonBState == HIGH) {
      
    } else {
     color = ST7735_RED;
    }



    // check y axis
    if(analogYValue > DELTA_MAX) {
      y = y + 1;
      y = checkMax(y, maxHeight, false);      
    }

    if(analogYValue < DELTA_MIN) {
      y = y - 1;  
      y = checkMax(y, maxHeight, true);
    }


   // check x axis    
   if(analogXValue > DELTA_MAX) {
      x = x + 1;
      x = checkMax(x, maxWidth, false);      
    }

    if(analogXValue < DELTA_MIN) {
      x = x - 1;  
      x = checkMax(x, maxWidth, true);
    }

    //render baby
    tft.fillRoundRect(x, y, pSize, pSize, 0, color);

    //Debug
    Serial.println("DEBU");   

}


