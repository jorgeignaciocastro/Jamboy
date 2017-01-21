#include <Adafruit_GFX.h>    
#include <Adafruit_ST7735.h> 
#include <SPI.h>

#include "pitches.h";
#include "pins.h";

#include "global.h";



int buttonAState = 0;
int buttonBState = 0;
int buttonCState = 0;

int analogXValue = 0;  // joystic X analog
int analogYValue = 0;  // joystic Y analog

int background = 0x7BE0;

// player constants
struct PLAYER {
  int pSize = 16;
  int x = 0;
  int y = 0; 
};

PLAYER jugador;

byte mapa1[8][10] = {
 
  {0,  1,  1,  1,  1,  1,  0,  1,  1,  1},
 
  {0,  1,  1,  1,  1,  1,  1,  1,  1,  1},
 
  {0,  0,  0,  0,  0,  0,  0,  0,  0,  0},

  {1,  1,  1,  1,  1,  1,  0,  1,  1,  0},

  {1,  1,  1,  1,  1,  1,  0,  0,  0,  0},
  
  {1,  1,  1,  0,  0,  0,  0,  1,  1,  1},

  {1,  1,  1,  0,  1,  1,  1,  1,  1,  1},

  {1,  1,  1,  1,  1,  1,  1,  1,  1,  1}

};



// audio
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

#define drawPixel(a, b, c) tft.setAddrWindow(a, b, a, b); tft.pushColor(c)


void setup(void) {

  Serial.begin(9600);
  pinMode(buttonAPin, INPUT);
  pinMode(buttonBPin, INPUT);
  pinMode(buttonCPin, INPUT_PULLUP);

  tft.initR(INITR_BLACKTAB); 
  tft.fillScreen(background); 
  tft.setRotation(1);

  //tft print function!
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(0);
  tft.setCursor(0,80);
  //tft.println("Hello Global Game Jam");
   

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

  drawMap(mapa1);
  
}



//check inside screen

int checkMax(int pos, int maximum, bool invert) {
  
  if(invert == true) {
    if (pos < 0) {
        pos = 0;
        tone(audioPin, melody[0], 100);

    }
  } else {
    if (pos > maximum - jugador.pSize) {
        pos = maximum - jugador.pSize;
        tone(audioPin, melody[0], 100);
    }
  }

  return pos;
  
}


////////////////////////////////////// BUTTONS CALLBACKS ////////////////////////////////////////


void Button_A() {
    //jugador.color = ST7735_GREEN;
}

void Button_B() {
    //jugador.color = ST7735_RED;
}

void Button_C() {
    //jugador.color = ST7735_WHITE;
}


// Directionals 

void Button_UP() {
  jugador.y = jugador.y - 1;  
  jugador.y = checkMax(jugador.y, maxHeight, true);
}

void Button_DOWN() {
  jugador.y = jugador.y + 1;
  jugador.y = checkMax(jugador.y, maxHeight, false);      
}

void Button_LEFT() {
  jugador.x = jugador.x + 1;
  jugador.x = checkMax(jugador.x, maxWidth, false);            
}

void Button_RIGHT() {
  jugador.x = jugador.x - 1;  
  jugador.x = checkMax(jugador.x, maxWidth, true);            
}

////////////////////////////////////// END BUTTONS CALLBACKS ////////////////////////////////////


//check inputs
void checkJoystic() {
  
    //read Buttons
    buttonAState = digitalRead(buttonAPin);
    buttonBState = digitalRead(buttonBPin);
    buttonCState = digitalRead(buttonCPin);

    //read Analogs
    analogXValue = analogRead(analogX);
    analogYValue = analogRead(analogY);

    //Check BUTTONS
  
    if(buttonCState == HIGH) {
       
    } else {
      Button_C();
    }
    

    if(buttonAState == HIGH) {
      
    } else {
      Button_A();
    }

     if(buttonBState == HIGH) {
      
    } else {
      Button_B();
    }



    // check y axis
    if(analogYValue > DELTA_MAX) {
      Button_DOWN();
    } else if(analogYValue < DELTA_MIN) {
      Button_UP();
    }


   // check x axis    
   if(analogXValue > DELTA_MAX) {
      Button_LEFT();     
    } else if(analogXValue < DELTA_MIN) {
      Button_RIGHT();
    }
  
}


/////////////////////////////// RENDERS ////////////////////////////////////

void drawMap (byte mapa[8][10]) {

  byte maxX = sizeof(mapa);
  byte maxY = sizeof(mapa[0]);

  byte actual = 0;


  for (int x = 0; x < 10; x ++) {
    for (int y = 0; y < 8; y ++) {
      if( mapa[y][x] == 1) {
        drawTree(x * 16, y * 16 , background);
      }
    }
  }
  
}


/////////////////////////////// RENDERS ////////////////////////////////////


void drawTree(int x, int y, int back) {
  drawPixel(x +0, y + 0,back); drawPixel(x +1, y + 0,back); drawPixel(x +2, y + 0,back); drawPixel(x +3, y + 0,back); drawPixel(x +4, y + 0,back); drawPixel(x +5, y + 0,back); drawPixel(x +6, y + 0,back); drawPixel(x +7, y + 0,back); drawPixel(x +8, y + 0,back); drawPixel(x +9, y + 0,back); drawPixel(x +10, y + 0,back); drawPixel(x +11, y + 0,back); drawPixel(x +12, y + 0,back); drawPixel(x +13, y + 0,back); drawPixel(x +14, y + 0,back); drawPixel(x +15, y + 0,back); drawPixel(x +0, y + 1,back); drawPixel(x +1, y + 1,back); drawPixel(x +2, y + 1,back); drawPixel(x +3, y + 1,back); drawPixel(x +4, y + 1,back); drawPixel(x +5, y + 1,ST7735_YELLOW); drawPixel(x +6, y + 1,ST7735_GREEN); drawPixel(x +7, y + 1,ST7735_GREEN); drawPixel(x +8, y + 1,ST7735_GREEN); drawPixel(x +9, y + 1,ST7735_GREEN); drawPixel(x +10, y + 1,ST7735_YELLOW); drawPixel(x +11, y + 1,back); drawPixel(x +12, y + 1,back); drawPixel(x +13, y + 1,back); drawPixel(x +14, y + 1,back); drawPixel(x +15, y + 1,back); drawPixel(x +0, y + 2,back); drawPixel(x +1, y + 2,back); drawPixel(x +2, y + 2,back); drawPixel(x +3, y + 2,back); drawPixel(x +4, y + 2,ST7735_GREEN); drawPixel(x +5, y + 2,ST7735_GREEN); drawPixel(x +6, y + 2,ST7735_GREEN); drawPixel(x +7, y + 2,ST7735_YELLOW); drawPixel(x +8, y + 2,ST7735_GREEN); drawPixel(x +9, y + 2,ST7735_GREEN); drawPixel(x +10, y + 2,ST7735_GREEN); drawPixel(x +11, y + 2,ST7735_GREEN); drawPixel(x +12, y + 2,back); drawPixel(x +13, y + 2,back); drawPixel(x +14, y + 2,back); drawPixel(x +15, y + 2,back); drawPixel(x +0, y + 3,back); drawPixel(x +1, y + 3,back); drawPixel(x +2, y + 3,back); drawPixel(x +3, y + 3,ST7735_GREEN); drawPixel(x +4, y + 3,ST7735_GREEN); drawPixel(x +5, y + 3,ST7735_GREEN); drawPixel(x +6, y + 3,ST7735_YELLOW); drawPixel(x +7, y + 3,ST7735_GREEN); drawPixel(x +8, y + 3,ST7735_YELLOW); drawPixel(x +9, y + 3,ST7735_GREEN); drawPixel(x +10, y + 3,ST7735_GREEN); drawPixel(x +11, y + 3,ST7735_GREEN); drawPixel(x +12, y + 3,ST7735_GREEN); drawPixel(x +13, y + 3,back); drawPixel(x +14, y + 3,back); drawPixel(x +15, y + 3,back); drawPixel(x +0, y + 4,back); drawPixel(x +1, y + 4,back); drawPixel(x +2, y + 4,ST7735_YELLOW); drawPixel(x +3, y + 4,ST7735_GREEN); drawPixel(x +4, y + 4,ST7735_GREEN); drawPixel(x +5, y + 4,ST7735_GREEN); drawPixel(x +6, y + 4,ST7735_GREEN); drawPixel(x +7, y + 4,ST7735_GREEN); drawPixel(x +8, y + 4,ST7735_GREEN); drawPixel(x +9, y + 4,ST7735_GREEN); drawPixel(x +10, y + 4,ST7735_YELLOW); drawPixel(x +11, y + 4,ST7735_GREEN); drawPixel(x +12, y + 4,ST7735_GREEN); drawPixel(x +13, y + 4,ST7735_YELLOW); drawPixel(x +14, y + 4,back); drawPixel(x +15, y + 4,back); drawPixel(x +0, y + 5,back); drawPixel(x +1, y + 5,back); drawPixel(x +2, y + 5,ST7735_GREEN); drawPixel(x +3, y + 5,ST7735_GREEN); drawPixel(x +4, y + 5,ST7735_YELLOW); drawPixel(x +5, y + 5,ST7735_GREEN); drawPixel(x +6, y + 5,ST7735_GREEN); drawPixel(x +7, y + 5,ST7735_YELLOW); drawPixel(x +8, y + 5,ST7735_GREEN); drawPixel(x +9, y + 5,ST7735_GREEN); drawPixel(x +10, y + 5,ST7735_GREEN); drawPixel(x +11, y + 5,ST7735_YELLOW); drawPixel(x +12, y + 5,ST7735_GREEN); drawPixel(x +13, y + 5,ST7735_GREEN); drawPixel(x +14, y + 5,back); drawPixel(x +15, y + 5,back); drawPixel(x +0, y + 6,back); drawPixel(x +1, y + 6,ST7735_YELLOW); drawPixel(x +2, y + 6,ST7735_GREEN); drawPixel(x +3, y + 6,ST7735_YELLOW); drawPixel(x +4, y + 6,ST7735_GREEN); drawPixel(x +5, y + 6,ST7735_GREEN); drawPixel(x +6, y + 6,ST7735_YELLOW); drawPixel(x +7, y + 6,ST7735_GREEN); drawPixel(x +8, y + 6,ST7735_YELLOW); drawPixel(x +9, y + 6,ST7735_GREEN); drawPixel(x +10, y + 6,ST7735_YELLOW); drawPixel(x +11, y + 6,ST7735_GREEN); drawPixel(x +12, y + 6,ST7735_GREEN); drawPixel(x +13, y + 6,ST7735_GREEN); drawPixel(x +14, y + 6,ST7735_YELLOW); drawPixel(x +15, y + 6,back); drawPixel(x +0, y + 7,back); drawPixel(x +1, y + 7,ST7735_YELLOW); drawPixel(x +2, y + 7,ST7735_GREEN); drawPixel(x +3, y + 7,ST7735_GREEN); drawPixel(x +4, y + 7,ST7735_YELLOW); drawPixel(x +5, y + 7,ST7735_GREEN); drawPixel(x +6, y + 7,ST7735_GREEN); drawPixel(x +7, y + 7,ST7735_GREEN); drawPixel(x +8, y + 7,ST7735_GREEN); drawPixel(x +9, y + 7,ST7735_GREEN); drawPixel(x +10, y + 7,ST7735_GREEN); drawPixel(x +11, y + 7,ST7735_GREEN); drawPixel(x +12, y + 7,ST7735_YELLOW); drawPixel(x +13, y + 7,ST7735_GREEN); drawPixel(x +14, y + 7,ST7735_GREEN); drawPixel(x +15, y + 7,back); drawPixel(x +0, y + 8,back); drawPixel(x +1, y + 8,ST7735_YELLOW); drawPixel(x +2, y + 8,ST7735_GREEN); drawPixel(x +3, y + 8,ST7735_GREEN); drawPixel(x +4, y + 8,ST7735_GREEN); drawPixel(x +5, y + 8,ST7735_GREEN); drawPixel(x +6, y + 8,ST7735_GREEN); drawPixel(x +7, y + 8,ST7735_YELLOW); drawPixel(x +8, y + 8,ST7735_YELLOW); drawPixel(x +9, y + 8,ST7735_GREEN); drawPixel(x +10, y + 8,ST7735_GREEN); drawPixel(x +11, y + 8,ST7735_GREEN); drawPixel(x +12, y + 8,ST7735_YELLOW); drawPixel(x +13, y + 8,ST7735_GREEN); drawPixel(x +14, y + 8,ST7735_GREEN); drawPixel(x +15, y + 8,back); drawPixel(x +0, y + 9,back); drawPixel(x +1, y + 9,back); drawPixel(x +2, y + 9,ST7735_GREEN); drawPixel(x +3, y + 9,ST7735_GREEN); drawPixel(x +4, y + 9,ST7735_GREEN); drawPixel(x +5, y + 9,ST7735_YELLOW); drawPixel(x +6, y + 9,ST7735_GREEN); drawPixel(x +7, y + 9,ST7735_GREEN); drawPixel(x +8, y + 9,ST7735_GREEN); drawPixel(x +9, y + 9,ST7735_GREEN); drawPixel(x +10, y + 9,ST7735_YELLOW); drawPixel(x +11, y + 9,ST7735_GREEN); drawPixel(x +12, y + 9,ST7735_GREEN); drawPixel(x +13, y + 9,ST7735_GREEN); drawPixel(x +14, y + 9,back); drawPixel(x +15, y + 9,back); drawPixel(x +0, y + 10,back); drawPixel(x +1, y + 10,back); drawPixel(x +2, y + 10,back); drawPixel(x +3, y + 10,ST7735_GREEN); drawPixel(x +4, y + 10,ST7735_GREEN); drawPixel(x +5, y + 10,ST7735_YELLOW); drawPixel(x +6, y + 10,ST7735_YELLOW); drawPixel(x +7, y + 10,ST7735_GREEN); drawPixel(x +8, y + 10,ST7735_GREEN); drawPixel(x +9, y + 10,ST7735_YELLOW); drawPixel(x +10, y + 10,ST7735_YELLOW); drawPixel(x +11, y + 10,ST7735_GREEN); drawPixel(x +12, y + 10,ST7735_GREEN); drawPixel(x +13, y + 10,back); drawPixel(x +14, y + 10,back); drawPixel(x +15, y + 10,back); drawPixel(x +0, y + 11,back); drawPixel(x +1, y + 11,back); drawPixel(x +2, y + 11,back); drawPixel(x +3, y + 11,back); drawPixel(x +4, y + 11,ST7735_YELLOW); drawPixel(x +5, y + 11,ST7735_GREEN); drawPixel(x +6, y + 11,ST7735_GREEN); drawPixel(x +7, y + 11,ST7735_GREEN); drawPixel(x +8, y + 11,ST7735_GREEN); drawPixel(x +9, y + 11,ST7735_GREEN); drawPixel(x +10, y + 11,ST7735_GREEN); drawPixel(x +11, y + 11,ST7735_YELLOW); drawPixel(x +12, y + 11,back); drawPixel(x +13, y + 11,back); drawPixel(x +14, y + 11,back); drawPixel(x +15, y + 11,back); drawPixel(x +0, y + 12,back); drawPixel(x +1, y + 12,back); drawPixel(x +2, y + 12,back); drawPixel(x +3, y + 12,back); drawPixel(x +4, y + 12,ST7735_BLUE); drawPixel(x +5, y + 12,ST7735_YELLOW); drawPixel(x +6, y + 12,ST7735_YELLOW); drawPixel(x +7, y + 12,ST7735_YELLOW); drawPixel(x +8, y + 12,ST7735_YELLOW); drawPixel(x +9, y + 12,ST7735_YELLOW); drawPixel(x +10, y + 12,ST7735_YELLOW); drawPixel(x +11, y + 12,ST7735_BLUE); drawPixel(x +12, y + 12,back); drawPixel(x +13, y + 12,back); drawPixel(x +14, y + 12,back); drawPixel(x +15, y + 12,back); drawPixel(x +0, y + 13,back); drawPixel(x +1, y + 13,back); drawPixel(x +2, y + 13,back); drawPixel(x +3, y + 13,back); drawPixel(x +4, y + 13,back); drawPixel(x +5, y + 13,ST7735_BLUE); drawPixel(x +6, y + 13,ST7735_CYAN); drawPixel(x +7, y + 13,ST7735_BLUE); drawPixel(x +8, y + 13,ST7735_BLUE); drawPixel(x +9, y + 13,ST7735_CYAN); drawPixel(x +10, y + 13,ST7735_BLUE); drawPixel(x +11, y + 13,back); drawPixel(x +12, y + 13,back); drawPixel(x +13, y + 13,back); drawPixel(x +14, y + 13,back); drawPixel(x +15, y + 13,back); drawPixel(x +0, y + 14,back); drawPixel(x +1, y + 14,back); drawPixel(x +2, y + 14,back); drawPixel(x +3, y + 14,back); drawPixel(x +4, y + 14,ST7735_BLUE); drawPixel(x +5, y + 14,ST7735_BLUE); drawPixel(x +6, y + 14,ST7735_BLUE); drawPixel(x +7, y + 14,ST7735_BLUE); drawPixel(x +8, y + 14,ST7735_CYAN); drawPixel(x +9, y + 14,ST7735_BLUE); drawPixel(x +10, y + 14,ST7735_BLUE); drawPixel(x +11, y + 14,ST7735_BLUE); drawPixel(x +12, y + 14,back); drawPixel(x +13, y + 14,back); drawPixel(x +14, y + 14,back); drawPixel(x +15, y + 14,back); drawPixel(x +0, y + 15,back); drawPixel(x +1, y + 15,back); drawPixel(x +2, y + 15,back); drawPixel(x +3, y + 15,back); drawPixel(x +4, y + 15,back); drawPixel(x +5, y + 15,back); drawPixel(x +6, y + 15,back); drawPixel(x +7, y + 15,back); drawPixel(x +8, y + 15,back); drawPixel(x +9, y + 15,back); drawPixel(x +10, y + 15,back); drawPixel(x +11, y + 15,back); drawPixel(x +12, y + 15,back); drawPixel(x +13, y + 15,back); drawPixel(x +14, y + 15,back); drawPixel(x +15, y + 15,back); 
}

void drawPlayer(PLAYER j, int back) {
  drawPixel( j.x +0, j.y + 0,back); drawPixel( j.x +1, j.y + 0,back); drawPixel( j.x +2, j.y + 0,back); drawPixel( j.x +3, j.y + 0,back); drawPixel( j.x +4, j.y + 0,back); drawPixel( j.x +5, j.y + 0,back); drawPixel( j.x +6, j.y + 0,back); drawPixel( j.x +7, j.y + 0,back); drawPixel( j.x +8, j.y + 0,back); drawPixel( j.x +9, j.y + 0,back); drawPixel( j.x +10, j.y + 0,back); drawPixel( j.x +11, j.y + 0,back); drawPixel( j.x +12, j.y + 0,back); drawPixel( j.x +13, j.y + 0,back); drawPixel( j.x +14, j.y + 0,back); drawPixel( j.x +15, j.y + 0,back); drawPixel( j.x +0, j.y + 1,back); drawPixel( j.x +1, j.y + 1,back); drawPixel( j.x +2, j.y + 1,back); drawPixel( j.x +3, j.y + 1,back); drawPixel( j.x +4, j.y + 1,back); drawPixel( j.x +5, j.y + 1,back); drawPixel( j.x +6, j.y + 1,back); drawPixel( j.x +7, j.y + 1,back); drawPixel( j.x +8, j.y + 1,back); drawPixel( j.x +9, j.y + 1,back); drawPixel( j.x +10, j.y + 1,back); drawPixel( j.x +11, j.y + 1,back); drawPixel( j.x +12, j.y + 1,back); drawPixel( j.x +13, j.y + 1,back); drawPixel( j.x +14, j.y + 1,back); drawPixel( j.x +15, j.y + 1,back); drawPixel( j.x +0, j.y + 2,back); drawPixel( j.x +1, j.y + 2,back); drawPixel( j.x +2, j.y + 2,back); drawPixel( j.x +3, j.y + 2,back); drawPixel( j.x +4, j.y + 2,back); drawPixel( j.x +5, j.y + 2,back); drawPixel( j.x +6, j.y + 2,back); drawPixel( j.x +7, j.y + 2,back); drawPixel( j.x +8, j.y + 2,back); drawPixel( j.x +9, j.y + 2,back); drawPixel( j.x +10, j.y + 2,back); drawPixel( j.x +11, j.y + 2,back); drawPixel( j.x +12, j.y + 2,back); drawPixel( j.x +13, j.y + 2,back); drawPixel( j.x +14, j.y + 2,back); drawPixel( j.x +15, j.y + 2,back); drawPixel( j.x +0, j.y + 3,back); drawPixel( j.x +1, j.y + 3,back); drawPixel( j.x +2, j.y + 3,back); drawPixel( j.x +3, j.y + 3,ST7735_BLUE); drawPixel( j.x +4, j.y + 3,ST7735_BLUE); drawPixel( j.x +5, j.y + 3,ST7735_BLUE); drawPixel( j.x +6, j.y + 3,ST7735_BLUE); drawPixel( j.x +7, j.y + 3,ST7735_BLUE); drawPixel( j.x +8, j.y + 3,ST7735_BLUE); drawPixel( j.x +9, j.y + 3,ST7735_BLUE); drawPixel( j.x +10, j.y + 3,ST7735_BLUE); drawPixel( j.x +11, j.y + 3,ST7735_BLUE); drawPixel( j.x +12, j.y + 3,ST7735_BLUE); drawPixel( j.x +13, j.y + 3,back); drawPixel( j.x +14, j.y + 3,back); drawPixel( j.x +15, j.y + 3,back); drawPixel( j.x +0, j.y + 4,back); drawPixel( j.x +1, j.y + 4,back); drawPixel( j.x +2, j.y + 4,back); drawPixel( j.x +3, j.y + 4,ST7735_BLUE); drawPixel( j.x +4, j.y + 4,ST7735_BLUE); drawPixel( j.x +5, j.y + 4,ST7735_BLUE); drawPixel( j.x +6, j.y + 4,ST7735_BLUE); drawPixel( j.x +7, j.y + 4,ST7735_BLUE); drawPixel( j.x +8, j.y + 4,ST7735_BLUE); drawPixel( j.x +9, j.y + 4,ST7735_BLUE); drawPixel( j.x +10, j.y + 4,ST7735_BLUE); drawPixel( j.x +11, j.y + 4,ST7735_BLUE); drawPixel( j.x +12, j.y + 4,ST7735_BLUE); drawPixel( j.x +13, j.y + 4,back); drawPixel( j.x +14, j.y + 4,back); drawPixel( j.x +15, j.y + 4,back); drawPixel( j.x +0, j.y + 5,back); drawPixel( j.x +1, j.y + 5,back); drawPixel( j.x +2, j.y + 5,back); drawPixel( j.x +3, j.y + 5,ST7735_BLUE); drawPixel( j.x +4, j.y + 5,ST7735_BLUE); drawPixel( j.x +5, j.y + 5,ST7735_BLUE); drawPixel( j.x +6, j.y + 5,ST7735_BLUE); drawPixel( j.x +7, j.y + 5,ST7735_BLUE); drawPixel( j.x +8, j.y + 5,ST7735_BLUE); drawPixel( j.x +9, j.y + 5,ST7735_BLUE); drawPixel( j.x +10, j.y + 5,ST7735_BLUE); drawPixel( j.x +11, j.y + 5,ST7735_BLUE); drawPixel( j.x +12, j.y + 5,ST7735_BLUE); drawPixel( j.x +13, j.y + 5,back); drawPixel( j.x +14, j.y + 5,back); drawPixel( j.x +15, j.y + 5,back); drawPixel( j.x +0, j.y + 6,back); drawPixel( j.x +1, j.y + 6,back); drawPixel( j.x +2, j.y + 6,back); drawPixel( j.x +3, j.y + 6,ST7735_BLUE); drawPixel( j.x +4, j.y + 6,ST7735_BLUE); drawPixel( j.x +5, j.y + 6,back); drawPixel( j.x +6, j.y + 6,back); drawPixel( j.x +7, j.y + 6,ST7735_BLUE); drawPixel( j.x +8, j.y + 6,ST7735_BLUE); drawPixel( j.x +9, j.y + 6,back); drawPixel( j.x +10, j.y + 6,back); drawPixel( j.x +11, j.y + 6,ST7735_BLUE); drawPixel( j.x +12, j.y + 6,ST7735_BLUE); drawPixel( j.x +13, j.y + 6,back); drawPixel( j.x +14, j.y + 6,back); drawPixel( j.x +15, j.y + 6,back); drawPixel( j.x +0, j.y + 7,back); drawPixel( j.x +1, j.y + 7,ST7735_BLUE); drawPixel( j.x +2, j.y + 7,ST7735_BLUE); drawPixel( j.x +3, j.y + 7,ST7735_BLUE); drawPixel( j.x +4, j.y + 7,ST7735_BLUE); drawPixel( j.x +5, j.y + 7,back); drawPixel( j.x +6, j.y + 7,back); drawPixel( j.x +7, j.y + 7,ST7735_BLUE); drawPixel( j.x +8, j.y + 7,ST7735_BLUE); drawPixel( j.x +9, j.y + 7,back); drawPixel( j.x +10, j.y + 7,back); drawPixel( j.x +11, j.y + 7,ST7735_BLUE); drawPixel( j.x +12, j.y + 7,ST7735_BLUE); drawPixel( j.x +13, j.y + 7,ST7735_BLUE); drawPixel( j.x +14, j.y + 7,ST7735_BLUE); drawPixel( j.x +15, j.y + 7,back); drawPixel( j.x +0, j.y + 8,back); drawPixel( j.x +1, j.y + 8,ST7735_BLUE); drawPixel( j.x +2, j.y + 8,ST7735_BLUE); drawPixel( j.x +3, j.y + 8,ST7735_BLUE); drawPixel( j.x +4, j.y + 8,ST7735_BLUE); drawPixel( j.x +5, j.y + 8,ST7735_BLUE); drawPixel( j.x +6, j.y + 8,ST7735_BLUE); drawPixel( j.x +7, j.y + 8,ST7735_BLUE); drawPixel( j.x +8, j.y + 8,ST7735_BLUE); drawPixel( j.x +9, j.y + 8,ST7735_BLUE); drawPixel( j.x +10, j.y + 8,ST7735_BLUE); drawPixel( j.x +11, j.y + 8,ST7735_BLUE); drawPixel( j.x +12, j.y + 8,ST7735_BLUE); drawPixel( j.x +13, j.y + 8,ST7735_BLUE); drawPixel( j.x +14, j.y + 8,ST7735_BLUE); drawPixel( j.x +15, j.y + 8,back); drawPixel( j.x +0, j.y + 9,back); drawPixel( j.x +1, j.y + 9,back); drawPixel( j.x +2, j.y + 9,back); drawPixel( j.x +3, j.y + 9,ST7735_BLUE); drawPixel( j.x +4, j.y + 9,ST7735_BLUE); drawPixel( j.x +5, j.y + 9,ST7735_BLUE); drawPixel( j.x +6, j.y + 9,ST7735_BLUE); drawPixel( j.x +7, j.y + 9,ST7735_BLUE); drawPixel( j.x +8, j.y + 9,ST7735_BLUE); drawPixel( j.x +9, j.y + 9,ST7735_BLUE); drawPixel( j.x +10, j.y + 9,ST7735_BLUE); drawPixel( j.x +11, j.y + 9,ST7735_BLUE); drawPixel( j.x +12, j.y + 9,ST7735_BLUE); drawPixel( j.x +13, j.y + 9,back); drawPixel( j.x +14, j.y + 9,back); drawPixel( j.x +15, j.y + 9,back); drawPixel( j.x +0, j.y + 10,back); drawPixel( j.x +1, j.y + 10,back); drawPixel( j.x +2, j.y + 10,back); drawPixel( j.x +3, j.y + 10,ST7735_BLUE); drawPixel( j.x +4, j.y + 10,ST7735_BLUE); drawPixel( j.x +5, j.y + 10,ST7735_BLUE); drawPixel( j.x +6, j.y + 10,ST7735_BLUE); drawPixel( j.x +7, j.y + 10,ST7735_BLUE); drawPixel( j.x +8, j.y + 10,ST7735_BLUE); drawPixel( j.x +9, j.y + 10,ST7735_BLUE); drawPixel( j.x +10, j.y + 10,ST7735_BLUE); drawPixel( j.x +11, j.y + 10,ST7735_BLUE); drawPixel( j.x +12, j.y + 10,ST7735_BLUE); drawPixel( j.x +13, j.y + 10,back); drawPixel( j.x +14, j.y + 10,back); drawPixel( j.x +15, j.y + 10,back); drawPixel( j.x +0, j.y + 11,back); drawPixel( j.x +1, j.y + 11,back); drawPixel( j.x +2, j.y + 11,back); drawPixel( j.x +3, j.y + 11,ST7735_BLUE); drawPixel( j.x +4, j.y + 11,ST7735_BLUE); drawPixel( j.x +5, j.y + 11,ST7735_BLUE); drawPixel( j.x +6, j.y + 11,ST7735_BLUE); drawPixel( j.x +7, j.y + 11,ST7735_BLUE); drawPixel( j.x +8, j.y + 11,ST7735_BLUE); drawPixel( j.x +9, j.y + 11,ST7735_BLUE); drawPixel( j.x +10, j.y + 11,ST7735_BLUE); drawPixel( j.x +11, j.y + 11,ST7735_BLUE); drawPixel( j.x +12, j.y + 11,ST7735_BLUE); drawPixel( j.x +13, j.y + 11,back); drawPixel( j.x +14, j.y + 11,back); drawPixel( j.x +15, j.y + 11,back); drawPixel( j.x +0, j.y + 12,back); drawPixel( j.x +1, j.y + 12,back); drawPixel( j.x +2, j.y + 12,back); drawPixel( j.x +3, j.y + 12,back); drawPixel( j.x +4, j.y + 12,ST7735_BLUE); drawPixel( j.x +5, j.y + 12,ST7735_BLUE); drawPixel( j.x +6, j.y + 12,back); drawPixel( j.x +7, j.y + 12,back); drawPixel( j.x +8, j.y + 12,back); drawPixel( j.x +9, j.y + 12,back); drawPixel( j.x +10, j.y + 12,ST7735_BLUE); drawPixel( j.x +11, j.y + 12,ST7735_BLUE); drawPixel( j.x +12, j.y + 12,back); drawPixel( j.x +13, j.y + 12,back); drawPixel( j.x +14, j.y + 12,back); drawPixel( j.x +15, j.y + 12,back); drawPixel( j.x +0, j.y + 13,back); drawPixel( j.x +1, j.y + 13,back); drawPixel( j.x +2, j.y + 13,back); drawPixel( j.x +3, j.y + 13,back); drawPixel( j.x +4, j.y + 13,ST7735_BLUE); drawPixel( j.x +5, j.y + 13,ST7735_BLUE); drawPixel( j.x +6, j.y + 13,back); drawPixel( j.x +7, j.y + 13,back); drawPixel( j.x +8, j.y + 13,back); drawPixel( j.x +9, j.y + 13,back); drawPixel( j.x +10, j.y + 13,ST7735_BLUE); drawPixel( j.x +11, j.y + 13,ST7735_BLUE); drawPixel( j.x +12, j.y + 13,back); drawPixel( j.x +13, j.y + 13,back); drawPixel( j.x +14, j.y + 13,back); drawPixel( j.x +15, j.y + 13,back); drawPixel( j.x +0, j.y + 14,back); drawPixel( j.x +1, j.y + 14,back); drawPixel( j.x +2, j.y + 14,back); drawPixel( j.x +3, j.y + 14,back); drawPixel( j.x +4, j.y + 14,ST7735_BLUE); drawPixel( j.x +5, j.y + 14,ST7735_BLUE); drawPixel( j.x +6, j.y + 14,back); drawPixel( j.x +7, j.y + 14,back); drawPixel( j.x +8, j.y + 14,back); drawPixel( j.x +9, j.y + 14,back); drawPixel( j.x +10, j.y + 14,ST7735_BLUE); drawPixel( j.x +11, j.y + 14,ST7735_BLUE); drawPixel( j.x +12, j.y + 14,back); drawPixel( j.x +13, j.y + 14,back); drawPixel( j.x +14, j.y + 14,back); drawPixel( j.x +15, j.y + 14,back); drawPixel( j.x +0, j.y + 15,back); drawPixel( j.x +1, j.y + 15,back); drawPixel( j.x +2, j.y + 15,back); drawPixel( j.x +3, j.y + 15,back); drawPixel( j.x +4, j.y + 15,back); drawPixel( j.x +5, j.y + 15,back); drawPixel( j.x +6, j.y + 15,back); drawPixel( j.x +7, j.y + 15,back); drawPixel( j.x +8, j.y + 15,back); drawPixel( j.x +9, j.y + 15,back); drawPixel( j.x +10, j.y + 15,back); drawPixel( j.x +11, j.y + 15,back); drawPixel( j.x +12, j.y + 15,back); drawPixel( j.x +13, j.y + 15,back); drawPixel( j.x +14, j.y + 15,back); drawPixel( j.x +15, j.y + 15,back); 
}

// main loop

void loop() {

    //listen pins
    checkJoystic();

    //render baby
    drawPlayer(jugador, background);
    //drawPixel(jugador.x, jugador.y, jugador.color);
    //tft.fillRoundRect(jugador.x, jugador.y, jugador.pSize, jugador.pSize, 4, jugador.color);

    //Debug
    //Serial.println("DEBdfffffffffffffffffU");   

}


