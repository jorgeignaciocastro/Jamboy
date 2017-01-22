// Libraries

#include <Adafruit_GFX.h>    
#include <Adafruit_ST7735.h> 
#include <SPI.h>

#include "pitches.h";
#include "pins.h";
#include "TimerTask.h";
#include "global.h";

//BUTTONS
TimerTask btn_A;
boolean buttonAFlag = false;
int buttonAState = 0;

TimerTask btn_B;
boolean buttonBFlag = false;
int buttonBState = 0;

TimerTask btn_C;
boolean buttonCFlag = false;
int buttonCState = 0;

//Joystick values
TimerTask analog_X;
TimerTask analog_Y;

int analogXValue = 0;  // joystic X analog
int analogYValue = 0;  // joystic Y analog

//Background color
int background = 0x7BE0;

byte mapa1[8][10] = {
 
  {1,  1,  1,  1,  1,  1,  0,  1,  1,  0},
 
  {1,  0,  1,  1,  1,  1,  1,  1,  1,  1},
 
  {1,  0,  0,  0,  0,  0,  0,  0,  0,  1},

  {1,  0,  1,  1,  1,  1,  0,  1,  1,  1},

  {1,  0,  1,  1,  1,  1,  0,  0,  0,  1},
  
  {1,  0,  1,  0,  0,  0,  0,  1,  1,  1},

  {1,  0,  0,  0,  1,  1,  1,  1,  1,  1},

  {1,  1,  1,  1,  1,  1,  1,  1,  1,  1}

};



// player constants
struct PLAYER {
  int pSize = 16;
  int x = 16;
  int y = 16;

  int realX = 16;
  int realY = 16;
  
  int oldX = 16;
  int oldY = 16;
  
  boolean isMoving = false;
  int dirX = 0; 
  int dirY = 0; 
};

PLAYER jugador;

TimerTask Music;
int currentNote = 0;

// audio
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
 4, 8, 8, 4, 4, 4, 4, 4
};

int noteDuration = noteDurations[0];


int maxMelody = sizeof(melody);


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
  
  drawMenu();
   

    for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    //tone(audioPin, melody[thisNote], 1000);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(audioPin);
  }

  tft.fillScreen(background); 


  drawMap(mapa1);
  
}


void drawMenu() {

  tft.drawLine( 39, 45, 120, 45, ST7735_WHITE);

  tft.drawLine( 39, 62, 120, 62, ST7735_WHITE);

  tft.setCursor(40, 50);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextWrap(true);
  tft.println("La leyenda de");
  tft.setTextSize(2);
  tft.setCursor(45, 68);
  tft.println("Chelda");
  
  

  
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
  jugador.dirY = DIR_UP;
  if(jugador.isMoving == false) {
      jugador.oldY = jugador.realY;  
  }
  jugador.isMoving = true;
}

void Button_DOWN() {
  jugador.dirY = DIR_DOWN;
  if(jugador.isMoving == false) {
    jugador.oldY = jugador.realY;     
  }
  jugador.isMoving = true;  
}

void Button_LEFT() {
  jugador.dirX = DIR_RIGHT;
  if(jugador.isMoving == false) {
    jugador.oldX = jugador.realX;
  }
  jugador.isMoving = true; 
}

void Button_RIGHT() {
  jugador.dirX = DIR_LEFT;
  if(jugador.isMoving == false) {
    jugador.oldX = jugador.realX;
  }
  jugador.isMoving = true;
}

void checkAnalogs() {

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

  
    checkAnalogs();  
    
    

  
}



////////////////////////////////////// END BUTTONS CALLBACKS ////////////////////////////////////


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


/////////////////////////////// RENDERS ////////////////////////////////////

void drawMap (byte mapa[8][10]) {

  byte maxX = sizeof(mapa[0]);
  byte maxY = sizeof(mapa[1]);

  byte actual = 0;


  for (int x = 0; x < maxX; x ++) {
    for (int y = 0; y < maxY; y ++) {
      if( mapa[y][x] == 1) {
        drawTree(x * 16, y * 16 , background);
      }
    }
  }
  
}

void drawTree(int x, int y, int back) {
  drawPixel(x +0, y + 0,back); drawPixel(x +1, y + 0,back); drawPixel(x +2, y + 0,back); drawPixel(x +3, y + 0,back); drawPixel(x +4, y + 0,back); drawPixel(x +5, y + 0,back); drawPixel(x +6, y + 0,back); drawPixel(x +7, y + 0,back); drawPixel(x +8, y + 0,back); drawPixel(x +9, y + 0,back); drawPixel(x +10, y + 0,back); drawPixel(x +11, y + 0,back); drawPixel(x +12, y + 0,back); drawPixel(x +13, y + 0,back); drawPixel(x +14, y + 0,back); drawPixel(x +15, y + 0,back); drawPixel(x +0, y + 1,back); drawPixel(x +1, y + 1,back); drawPixel(x +2, y + 1,back); drawPixel(x +3, y + 1,back); drawPixel(x +4, y + 1,back); drawPixel(x +5, y + 1,ST7735_YELLOW); drawPixel(x +6, y + 1,ST7735_GREEN); drawPixel(x +7, y + 1,ST7735_GREEN); drawPixel(x +8, y + 1,ST7735_GREEN); drawPixel(x +9, y + 1,ST7735_GREEN); drawPixel(x +10, y + 1,ST7735_YELLOW); drawPixel(x +11, y + 1,back); drawPixel(x +12, y + 1,back); drawPixel(x +13, y + 1,back); drawPixel(x +14, y + 1,back); drawPixel(x +15, y + 1,back); drawPixel(x +0, y + 2,back); drawPixel(x +1, y + 2,back); drawPixel(x +2, y + 2,back); drawPixel(x +3, y + 2,back); drawPixel(x +4, y + 2,ST7735_GREEN); drawPixel(x +5, y + 2,ST7735_GREEN); drawPixel(x +6, y + 2,ST7735_GREEN); drawPixel(x +7, y + 2,ST7735_YELLOW); drawPixel(x +8, y + 2,ST7735_GREEN); drawPixel(x +9, y + 2,ST7735_GREEN); drawPixel(x +10, y + 2,ST7735_GREEN); drawPixel(x +11, y + 2,ST7735_GREEN); drawPixel(x +12, y + 2,back); drawPixel(x +13, y + 2,back); drawPixel(x +14, y + 2,back); drawPixel(x +15, y + 2,back); drawPixel(x +0, y + 3,back); drawPixel(x +1, y + 3,back); drawPixel(x +2, y + 3,back); drawPixel(x +3, y + 3,ST7735_GREEN); drawPixel(x +4, y + 3,ST7735_GREEN); drawPixel(x +5, y + 3,ST7735_GREEN); drawPixel(x +6, y + 3,ST7735_YELLOW); drawPixel(x +7, y + 3,ST7735_GREEN); drawPixel(x +8, y + 3,ST7735_YELLOW); drawPixel(x +9, y + 3,ST7735_GREEN); drawPixel(x +10, y + 3,ST7735_GREEN); drawPixel(x +11, y + 3,ST7735_GREEN); drawPixel(x +12, y + 3,ST7735_GREEN); drawPixel(x +13, y + 3,back); drawPixel(x +14, y + 3,back); drawPixel(x +15, y + 3,back); drawPixel(x +0, y + 4,back); drawPixel(x +1, y + 4,back); drawPixel(x +2, y + 4,ST7735_YELLOW); drawPixel(x +3, y + 4,ST7735_GREEN); drawPixel(x +4, y + 4,ST7735_GREEN); drawPixel(x +5, y + 4,ST7735_GREEN); drawPixel(x +6, y + 4,ST7735_GREEN); drawPixel(x +7, y + 4,ST7735_GREEN); drawPixel(x +8, y + 4,ST7735_GREEN); drawPixel(x +9, y + 4,ST7735_GREEN); drawPixel(x +10, y + 4,ST7735_YELLOW); drawPixel(x +11, y + 4,ST7735_GREEN); drawPixel(x +12, y + 4,ST7735_GREEN); drawPixel(x +13, y + 4,ST7735_YELLOW); drawPixel(x +14, y + 4,back); drawPixel(x +15, y + 4,back); drawPixel(x +0, y + 5,back); drawPixel(x +1, y + 5,back); drawPixel(x +2, y + 5,ST7735_GREEN); drawPixel(x +3, y + 5,ST7735_GREEN); drawPixel(x +4, y + 5,ST7735_YELLOW); drawPixel(x +5, y + 5,ST7735_GREEN); drawPixel(x +6, y + 5,ST7735_GREEN); drawPixel(x +7, y + 5,ST7735_YELLOW); drawPixel(x +8, y + 5,ST7735_GREEN); drawPixel(x +9, y + 5,ST7735_GREEN); drawPixel(x +10, y + 5,ST7735_GREEN); drawPixel(x +11, y + 5,ST7735_YELLOW); drawPixel(x +12, y + 5,ST7735_GREEN); drawPixel(x +13, y + 5,ST7735_GREEN); drawPixel(x +14, y + 5,back); drawPixel(x +15, y + 5,back); drawPixel(x +0, y + 6,back); drawPixel(x +1, y + 6,ST7735_YELLOW); drawPixel(x +2, y + 6,ST7735_GREEN); drawPixel(x +3, y + 6,ST7735_YELLOW); drawPixel(x +4, y + 6,ST7735_GREEN); drawPixel(x +5, y + 6,ST7735_GREEN); drawPixel(x +6, y + 6,ST7735_YELLOW); drawPixel(x +7, y + 6,ST7735_GREEN); drawPixel(x +8, y + 6,ST7735_YELLOW); drawPixel(x +9, y + 6,ST7735_GREEN); drawPixel(x +10, y + 6,ST7735_YELLOW); drawPixel(x +11, y + 6,ST7735_GREEN); drawPixel(x +12, y + 6,ST7735_GREEN); drawPixel(x +13, y + 6,ST7735_GREEN); drawPixel(x +14, y + 6,ST7735_YELLOW); drawPixel(x +15, y + 6,back); drawPixel(x +0, y + 7,back); drawPixel(x +1, y + 7,ST7735_YELLOW); drawPixel(x +2, y + 7,ST7735_GREEN); drawPixel(x +3, y + 7,ST7735_GREEN); drawPixel(x +4, y + 7,ST7735_YELLOW); drawPixel(x +5, y + 7,ST7735_GREEN); drawPixel(x +6, y + 7,ST7735_GREEN); drawPixel(x +7, y + 7,ST7735_GREEN); drawPixel(x +8, y + 7,ST7735_GREEN); drawPixel(x +9, y + 7,ST7735_GREEN); drawPixel(x +10, y + 7,ST7735_GREEN); drawPixel(x +11, y + 7,ST7735_GREEN); drawPixel(x +12, y + 7,ST7735_YELLOW); drawPixel(x +13, y + 7,ST7735_GREEN); drawPixel(x +14, y + 7,ST7735_GREEN); drawPixel(x +15, y + 7,back); drawPixel(x +0, y + 8,back); drawPixel(x +1, y + 8,ST7735_YELLOW); drawPixel(x +2, y + 8,ST7735_GREEN); drawPixel(x +3, y + 8,ST7735_GREEN); drawPixel(x +4, y + 8,ST7735_GREEN); drawPixel(x +5, y + 8,ST7735_GREEN); drawPixel(x +6, y + 8,ST7735_GREEN); drawPixel(x +7, y + 8,ST7735_YELLOW); drawPixel(x +8, y + 8,ST7735_YELLOW); drawPixel(x +9, y + 8,ST7735_GREEN); drawPixel(x +10, y + 8,ST7735_GREEN); drawPixel(x +11, y + 8,ST7735_GREEN); drawPixel(x +12, y + 8,ST7735_YELLOW); drawPixel(x +13, y + 8,ST7735_GREEN); drawPixel(x +14, y + 8,ST7735_GREEN); drawPixel(x +15, y + 8,back); drawPixel(x +0, y + 9,back); drawPixel(x +1, y + 9,back); drawPixel(x +2, y + 9,ST7735_GREEN); drawPixel(x +3, y + 9,ST7735_GREEN); drawPixel(x +4, y + 9,ST7735_GREEN); drawPixel(x +5, y + 9,ST7735_YELLOW); drawPixel(x +6, y + 9,ST7735_GREEN); drawPixel(x +7, y + 9,ST7735_GREEN); drawPixel(x +8, y + 9,ST7735_GREEN); drawPixel(x +9, y + 9,ST7735_GREEN); drawPixel(x +10, y + 9,ST7735_YELLOW); drawPixel(x +11, y + 9,ST7735_GREEN); drawPixel(x +12, y + 9,ST7735_GREEN); drawPixel(x +13, y + 9,ST7735_GREEN); drawPixel(x +14, y + 9,back); drawPixel(x +15, y + 9,back); drawPixel(x +0, y + 10,back); drawPixel(x +1, y + 10,back); drawPixel(x +2, y + 10,back); drawPixel(x +3, y + 10,ST7735_GREEN); drawPixel(x +4, y + 10,ST7735_GREEN); drawPixel(x +5, y + 10,ST7735_YELLOW); drawPixel(x +6, y + 10,ST7735_YELLOW); drawPixel(x +7, y + 10,ST7735_GREEN); drawPixel(x +8, y + 10,ST7735_GREEN); drawPixel(x +9, y + 10,ST7735_YELLOW); drawPixel(x +10, y + 10,ST7735_YELLOW); drawPixel(x +11, y + 10,ST7735_GREEN); drawPixel(x +12, y + 10,ST7735_GREEN); drawPixel(x +13, y + 10,back); drawPixel(x +14, y + 10,back); drawPixel(x +15, y + 10,back); drawPixel(x +0, y + 11,back); drawPixel(x +1, y + 11,back); drawPixel(x +2, y + 11,back); drawPixel(x +3, y + 11,back); drawPixel(x +4, y + 11,ST7735_YELLOW); drawPixel(x +5, y + 11,ST7735_GREEN); drawPixel(x +6, y + 11,ST7735_GREEN); drawPixel(x +7, y + 11,ST7735_GREEN); drawPixel(x +8, y + 11,ST7735_GREEN); drawPixel(x +9, y + 11,ST7735_GREEN); drawPixel(x +10, y + 11,ST7735_GREEN); drawPixel(x +11, y + 11,ST7735_YELLOW); drawPixel(x +12, y + 11,back); drawPixel(x +13, y + 11,back); drawPixel(x +14, y + 11,back); drawPixel(x +15, y + 11,back); drawPixel(x +0, y + 12,back); drawPixel(x +1, y + 12,back); drawPixel(x +2, y + 12,back); drawPixel(x +3, y + 12,back); drawPixel(x +4, y + 12,ST7735_BLUE); drawPixel(x +5, y + 12,ST7735_YELLOW); drawPixel(x +6, y + 12,ST7735_YELLOW); drawPixel(x +7, y + 12,ST7735_YELLOW); drawPixel(x +8, y + 12,ST7735_YELLOW); drawPixel(x +9, y + 12,ST7735_YELLOW); drawPixel(x +10, y + 12,ST7735_YELLOW); drawPixel(x +11, y + 12,ST7735_BLUE); drawPixel(x +12, y + 12,back); drawPixel(x +13, y + 12,back); drawPixel(x +14, y + 12,back); drawPixel(x +15, y + 12,back); drawPixel(x +0, y + 13,back); drawPixel(x +1, y + 13,back); drawPixel(x +2, y + 13,back); drawPixel(x +3, y + 13,back); drawPixel(x +4, y + 13,back); drawPixel(x +5, y + 13,ST7735_BLUE); drawPixel(x +6, y + 13,ST7735_CYAN); drawPixel(x +7, y + 13,ST7735_BLUE); drawPixel(x +8, y + 13,ST7735_BLUE); drawPixel(x +9, y + 13,ST7735_CYAN); drawPixel(x +10, y + 13,ST7735_BLUE); drawPixel(x +11, y + 13,back); drawPixel(x +12, y + 13,back); drawPixel(x +13, y + 13,back); drawPixel(x +14, y + 13,back); drawPixel(x +15, y + 13,back); drawPixel(x +0, y + 14,back); drawPixel(x +1, y + 14,back); drawPixel(x +2, y + 14,back); drawPixel(x +3, y + 14,back); drawPixel(x +4, y + 14,ST7735_BLUE); drawPixel(x +5, y + 14,ST7735_BLUE); drawPixel(x +6, y + 14,ST7735_BLUE); drawPixel(x +7, y + 14,ST7735_BLUE); drawPixel(x +8, y + 14,ST7735_CYAN); drawPixel(x +9, y + 14,ST7735_BLUE); drawPixel(x +10, y + 14,ST7735_BLUE); drawPixel(x +11, y + 14,ST7735_BLUE); drawPixel(x +12, y + 14,back); drawPixel(x +13, y + 14,back); drawPixel(x +14, y + 14,back); drawPixel(x +15, y + 14,back); drawPixel(x +0, y + 15,back); drawPixel(x +1, y + 15,back); drawPixel(x +2, y + 15,back); drawPixel(x +3, y + 15,back); drawPixel(x +4, y + 15,back); drawPixel(x +5, y + 15,back); drawPixel(x +6, y + 15,back); drawPixel(x +7, y + 15,back); drawPixel(x +8, y + 15,back); drawPixel(x +9, y + 15,back); drawPixel(x +10, y + 15,back); drawPixel(x +11, y + 15,back); drawPixel(x +12, y + 15,back); drawPixel(x +13, y + 15,back); drawPixel(x +14, y + 15,back); drawPixel(x +15, y + 15,back); 
}

void drawPlayer(PLAYER j, int back) {
drawPixel( j.x +0, j.y + 0,background); drawPixel( j.x +1, j.y + 0,background); drawPixel( j.x +2, j.y + 0,background); drawPixel( j.x +3, j.y + 0,background); drawPixel( j.x +4, j.y + 0,background); drawPixel( j.x +5, j.y + 0,background); drawPixel( j.x +6, j.y + 0,background); drawPixel( j.x +7, j.y + 0,background); drawPixel( j.x +8, j.y + 0,background); drawPixel( j.x +9, j.y + 0,background); drawPixel( j.x +10, j.y + 0,background); drawPixel( j.x +11, j.y + 0,background); drawPixel( j.x +12, j.y + 0,background); drawPixel( j.x +13, j.y + 0,background); drawPixel( j.x +14, j.y + 0,background); drawPixel( j.x +15, j.y + 0,background); drawPixel( j.x +0, j.y + 1,background); drawPixel( j.x +1, j.y + 1,background); drawPixel( j.x +2, j.y + 1,background); drawPixel( j.x +3, j.y + 1,background); drawPixel( j.x +4, j.y + 1,background); drawPixel( j.x +5, j.y + 1,background); drawPixel( j.x +6, j.y + 1,background); drawPixel( j.x +7, j.y + 1,background); drawPixel( j.x +8, j.y + 1,background); drawPixel( j.x +9, j.y + 1,background); drawPixel( j.x +10, j.y + 1,background); drawPixel( j.x +11, j.y + 1,background); drawPixel( j.x +12, j.y + 1,background); drawPixel( j.x +13, j.y + 1,background); drawPixel( j.x +14, j.y + 1,background); drawPixel( j.x +15, j.y + 1,background); drawPixel( j.x +0, j.y + 2,background); drawPixel( j.x +1, j.y + 2,background); drawPixel( j.x +2, j.y + 2,background); drawPixel( j.x +3, j.y + 2,background); drawPixel( j.x +4, j.y + 2,background); drawPixel( j.x +5, j.y + 2,background); drawPixel( j.x +6, j.y + 2,background); drawPixel( j.x +7, j.y + 2,background); drawPixel( j.x +8, j.y + 2,background); drawPixel( j.x +9, j.y + 2,background); drawPixel( j.x +10, j.y + 2,background); drawPixel( j.x +11, j.y + 2,background); drawPixel( j.x +12, j.y + 2,background); drawPixel( j.x +13, j.y + 2,background); drawPixel( j.x +14, j.y + 2,background); drawPixel( j.x +15, j.y + 2,background); drawPixel( j.x +0, j.y + 3,background); drawPixel( j.x +1, j.y + 3,background); drawPixel( j.x +2, j.y + 3,background); drawPixel( j.x +3, j.y + 3,background); drawPixel( j.x +4, j.y + 3,background); drawPixel( j.x +5, j.y + 3,background); drawPixel( j.x +6, j.y + 3,background); drawPixel( j.x +7, j.y + 3,background); drawPixel( j.x +8, j.y + 3,background); drawPixel( j.x +9, j.y + 3,background); drawPixel( j.x +10, j.y + 3,background); drawPixel( j.x +11, j.y + 3,background); drawPixel( j.x +12, j.y + 3,background); drawPixel( j.x +13, j.y + 3,background); drawPixel( j.x +14, j.y + 3,background); drawPixel( j.x +15, j.y + 3,background); drawPixel( j.x +0, j.y + 4,background); drawPixel( j.x +1, j.y + 4,background); drawPixel( j.x +2, j.y + 4,background); drawPixel( j.x +3, j.y + 4,background); drawPixel( j.x +4, j.y + 4,background); drawPixel( j.x +5, j.y + 4,background); drawPixel( j.x +6, j.y + 4,background); drawPixel( j.x +7, j.y + 4,background); drawPixel( j.x +8, j.y + 4,background); drawPixel( j.x +9, j.y + 4,background); drawPixel( j.x +10, j.y + 4,background); drawPixel( j.x +11, j.y + 4,background); drawPixel( j.x +12, j.y + 4,background); drawPixel( j.x +13, j.y + 4,background); drawPixel( j.x +14, j.y + 4,background); drawPixel( j.x +15, j.y + 4,background); drawPixel( j.x +0, j.y + 5,background); drawPixel( j.x +1, j.y + 5,background); drawPixel( j.x +2, j.y + 5,background); drawPixel( j.x +3, j.y + 5,background); drawPixel( j.x +4, j.y + 5,background); drawPixel( j.x +5, j.y + 5,background); drawPixel( j.x +6, j.y + 5,background); drawPixel( j.x +7, j.y + 5,background); drawPixel( j.x +8, j.y + 5,background); drawPixel( j.x +9, j.y + 5,background); drawPixel( j.x +10, j.y + 5,background); drawPixel( j.x +11, j.y + 5,background); drawPixel( j.x +12, j.y + 5,background); drawPixel( j.x +13, j.y + 5,ST7735_BLUE); drawPixel( j.x +14, j.y + 5,ST7735_BLUE); drawPixel( j.x +15, j.y + 5,background); drawPixel( j.x +0, j.y + 6,background); drawPixel( j.x +1, j.y + 6,background); drawPixel( j.x +2, j.y + 6,background); drawPixel( j.x +3, j.y + 6,ST7735_BLUE); drawPixel( j.x +4, j.y + 6,ST7735_BLUE); drawPixel( j.x +5, j.y + 6,ST7735_BLUE); drawPixel( j.x +6, j.y + 6,ST7735_BLUE); drawPixel( j.x +7, j.y + 6,ST7735_BLUE); drawPixel( j.x +8, j.y + 6,ST7735_BLUE); drawPixel( j.x +9, j.y + 6,ST7735_BLUE); drawPixel( j.x +10, j.y + 6,ST7735_BLUE); drawPixel( j.x +11, j.y + 6,ST7735_BLUE); drawPixel( j.x +12, j.y + 6,ST7735_BLUE); drawPixel( j.x +13, j.y + 6,ST7735_BLUE); drawPixel( j.x +14, j.y + 6,ST7735_BLUE); drawPixel( j.x +15, j.y + 6,background); drawPixel( j.x +0, j.y + 7,background); drawPixel( j.x +1, j.y + 7,background); drawPixel( j.x +2, j.y + 7,background); drawPixel( j.x +3, j.y + 7,ST7735_BLUE); drawPixel( j.x +4, j.y + 7,ST7735_BLUE); drawPixel( j.x +5, j.y + 7,ST7735_BLUE); drawPixel( j.x +6, j.y + 7,ST7735_BLUE); drawPixel( j.x +7, j.y + 7,ST7735_BLUE); drawPixel( j.x +8, j.y + 7,ST7735_BLUE); drawPixel( j.x +9, j.y + 7,ST7735_BLUE); drawPixel( j.x +10, j.y + 7,ST7735_BLUE); drawPixel( j.x +11, j.y + 7,ST7735_BLUE); drawPixel( j.x +12, j.y + 7,ST7735_BLUE); drawPixel( j.x +13, j.y + 7,ST7735_BLUE); drawPixel( j.x +14, j.y + 7,background); drawPixel( j.x +15, j.y + 7,background); drawPixel( j.x +0, j.y + 8,background); drawPixel( j.x +1, j.y + 8,background); drawPixel( j.x +2, j.y + 8,background); drawPixel( j.x +3, j.y + 8,ST7735_BLUE); drawPixel( j.x +4, j.y + 8,ST7735_BLUE); drawPixel( j.x +5, j.y + 8,ST7735_BLUE); drawPixel( j.x +6, j.y + 8,ST7735_BLUE); drawPixel( j.x +7, j.y + 8,ST7735_BLUE); drawPixel( j.x +8, j.y + 8,ST7735_BLUE); drawPixel( j.x +9, j.y + 8,ST7735_BLUE); drawPixel( j.x +10, j.y + 8,ST7735_BLUE); drawPixel( j.x +11, j.y + 8,ST7735_BLUE); drawPixel( j.x +12, j.y + 8,ST7735_BLUE); drawPixel( j.x +13, j.y + 8,background); drawPixel( j.x +14, j.y + 8,background); drawPixel( j.x +15, j.y + 8,background); drawPixel( j.x +0, j.y + 9,background); drawPixel( j.x +1, j.y + 9,background); drawPixel( j.x +2, j.y + 9,background); drawPixel( j.x +3, j.y + 9,ST7735_BLUE); drawPixel( j.x +4, j.y + 9,ST7735_BLUE); drawPixel( j.x +5, j.y + 9,ST7735_BLUE); drawPixel( j.x +6, j.y + 9,ST7735_BLUE); drawPixel( j.x +7, j.y + 9,ST7735_BLUE); drawPixel( j.x +8, j.y + 9,ST7735_BLUE); drawPixel( j.x +9, j.y + 9,ST7735_BLUE); drawPixel( j.x +10, j.y + 9,ST7735_BLUE); drawPixel( j.x +11, j.y + 9,ST7735_BLUE); drawPixel( j.x +12, j.y + 9,ST7735_BLUE); drawPixel( j.x +13, j.y + 9,background); drawPixel( j.x +14, j.y + 9,background); drawPixel( j.x +15, j.y + 9,background); drawPixel( j.x +0, j.y + 10,background); drawPixel( j.x +1, j.y + 10,ST7735_BLUE); drawPixel( j.x +2, j.y + 10,ST7735_BLUE); drawPixel( j.x +3, j.y + 10,ST7735_BLUE); drawPixel( j.x +4, j.y + 10,ST7735_BLUE); drawPixel( j.x +5, j.y + 10,ST7735_YELLOW); drawPixel( j.x +6, j.y + 10,ST7735_YELLOW); drawPixel( j.x +7, j.y + 10,ST7735_BLUE); drawPixel( j.x +8, j.y + 10,ST7735_BLUE); drawPixel( j.x +9, j.y + 10,ST7735_YELLOW); drawPixel( j.x +10, j.y + 10,ST7735_YELLOW); drawPixel( j.x +11, j.y + 10,ST7735_BLUE); drawPixel( j.x +12, j.y + 10,ST7735_BLUE); drawPixel( j.x +13, j.y + 10,background); drawPixel( j.x +14, j.y + 10,background); drawPixel( j.x +15, j.y + 10,background); drawPixel( j.x +0, j.y + 11,background); drawPixel( j.x +1, j.y + 11,ST7735_BLUE); drawPixel( j.x +2, j.y + 11,ST7735_BLUE); drawPixel( j.x +3, j.y + 11,ST7735_BLUE); drawPixel( j.x +4, j.y + 11,ST7735_BLUE); drawPixel( j.x +5, j.y + 11,ST7735_YELLOW); drawPixel( j.x +6, j.y + 11,ST7735_YELLOW); drawPixel( j.x +7, j.y + 11,ST7735_BLUE); drawPixel( j.x +8, j.y + 11,ST7735_BLUE); drawPixel( j.x +9, j.y + 11,ST7735_YELLOW); drawPixel( j.x +10, j.y + 11,ST7735_YELLOW); drawPixel( j.x +11, j.y + 11,ST7735_BLUE); drawPixel( j.x +12, j.y + 11,ST7735_BLUE); drawPixel( j.x +13, j.y + 11,background); drawPixel( j.x +14, j.y + 11,background); drawPixel( j.x +15, j.y + 11,background); drawPixel( j.x +0, j.y + 12,background); drawPixel( j.x +1, j.y + 12,background); drawPixel( j.x +2, j.y + 12,background); drawPixel( j.x +3, j.y + 12,ST7735_BLUE); drawPixel( j.x +4, j.y + 12,ST7735_BLUE); drawPixel( j.x +5, j.y + 12,ST7735_BLUE); drawPixel( j.x +6, j.y + 12,ST7735_BLUE); drawPixel( j.x +7, j.y + 12,ST7735_BLUE); drawPixel( j.x +8, j.y + 12,ST7735_BLUE); drawPixel( j.x +9, j.y + 12,ST7735_BLUE); drawPixel( j.x +10, j.y + 12,ST7735_BLUE); drawPixel( j.x +11, j.y + 12,ST7735_BLUE); drawPixel( j.x +12, j.y + 12,ST7735_BLUE); drawPixel( j.x +13, j.y + 12,background); drawPixel( j.x +14, j.y + 12,background); drawPixel( j.x +15, j.y + 12,background); drawPixel( j.x +0, j.y + 13,background); drawPixel( j.x +1, j.y + 13,background); drawPixel( j.x +2, j.y + 13,background); drawPixel( j.x +3, j.y + 13,background); drawPixel( j.x +4, j.y + 13,ST7735_BLUE); drawPixel( j.x +5, j.y + 13,ST7735_BLUE); drawPixel( j.x +6, j.y + 13,background); drawPixel( j.x +7, j.y + 13,background); drawPixel( j.x +8, j.y + 13,background); drawPixel( j.x +9, j.y + 13,background); drawPixel( j.x +10, j.y + 13,ST7735_BLUE); drawPixel( j.x +11, j.y + 13,ST7735_BLUE); drawPixel( j.x +12, j.y + 13,background); drawPixel( j.x +13, j.y + 13,background); drawPixel( j.x +14, j.y + 13,background); drawPixel( j.x +15, j.y + 13,background); drawPixel( j.x +0, j.y + 14,background); drawPixel( j.x +1, j.y + 14,background); drawPixel( j.x +2, j.y + 14,background); drawPixel( j.x +3, j.y + 14,background); drawPixel( j.x +4, j.y + 14,ST7735_BLUE); drawPixel( j.x +5, j.y + 14,ST7735_BLUE); drawPixel( j.x +6, j.y + 14,background); drawPixel( j.x +7, j.y + 14,background); drawPixel( j.x +8, j.y + 14,background); drawPixel( j.x +9, j.y + 14,background); drawPixel( j.x +10, j.y + 14,ST7735_BLUE); drawPixel( j.x +11, j.y + 14,ST7735_BLUE); drawPixel( j.x +12, j.y + 14,background); drawPixel( j.x +13, j.y + 14,background); drawPixel( j.x +14, j.y + 14,background); drawPixel( j.x +15, j.y + 14,background); drawPixel( j.x +0, j.y + 15,background); drawPixel( j.x +1, j.y + 15,background); drawPixel( j.x +2, j.y + 15,background); drawPixel( j.x +3, j.y + 15,background); drawPixel( j.x +4, j.y + 15,background); drawPixel( j.x +5, j.y + 15,background); drawPixel( j.x +6, j.y + 15,background); drawPixel( j.x +7, j.y + 15,background); drawPixel( j.x +8, j.y + 15,background); drawPixel( j.x +9, j.y + 15,background); drawPixel( j.x +10, j.y + 15,background); drawPixel( j.x +11, j.y + 15,background); drawPixel( j.x +12, j.y + 15,background); drawPixel( j.x +13, j.y + 15,background); drawPixel( j.x +14, j.y + 15,background); drawPixel( j.x +15, j.y + 15,background); 
}


/////////////////////////////// RENDERS ////////////////////////////////////



void gameUpdate() {
  if(jugador.isMoving == true) {

  if(checkCollisionMap(mapa1)) {
    jugador.x = jugador.realX;
    jugador.y = jugador.realY;
    jugador.dirX = 0;
    jugador.dirY = 0;
    return;
  }
    
    if(jugador.dirX != 0) {
      jugador.x += jugador.dirX;    

      int dirX = jugador.oldX + jugador.pSize * jugador.dirX; 

      if(jugador.dirX == DIR_LEFT ) {
        if(jugador.x <= dirX) {
          jugador.dirX = 0;
          jugador.realX -= jugador.pSize;
          jugador.x = jugador.realX;
          jugador.oldX = jugador.realX;
          jugador.isMoving = false;
        }
      }

      
      if(jugador.dirX == DIR_RIGHT ) {
        if(jugador.x >= dirX) {
          jugador.dirX = 0;
          jugador.realX += jugador.pSize;
          jugador.x = jugador.realX;         
          jugador.oldX = jugador.realX;
          jugador.isMoving = false;
        }
      }
      
      } 
      
      if (jugador.dirY != 0) {
      jugador.y += jugador.dirY;    

      int dirY = jugador.oldY + jugador.pSize * jugador.dirY; 

      if(jugador.dirY == DIR_UP)  {
        if(jugador.y == dirY) {
          jugador.dirY = 0;
          jugador.realY -= jugador.pSize;
          jugador.y = jugador.realY;
          jugador.oldY = jugador.realY;
          jugador.isMoving = false;
        }
      } 
      
      if(jugador.dirY == DIR_DOWN ) {
        if(jugador.y == dirY) {
          jugador.dirY = 0;
          jugador.realY += jugador.pSize;
          jugador.y = jugador.realY;        
          jugador.oldY = jugador.realY;
          jugador.isMoving = false;
        }
      }  
    } 
  } else {
    jugador.dirY = 0;
    jugador.dirX = 0;

    jugador.y = jugador.realY;        
    jugador.oldY = jugador.realY;

    
    jugador.x = jugador.realX;        
    jugador.oldX = jugador.realX;
  }
}
  

bool checkCollisionMap (byte selected_map[8][10]) {

  int posY = (jugador.realY / jugador.pSize) + jugador.dirY;
  int posX = (jugador.realX / jugador.pSize) + jugador.dirX;
  
  if(selected_map[posY][posX] != 0) {
    return true; 
  } else {
    return false;
  }
 
  
}


// main loop
void loop() {

    //listen pins
    checkJoystic();
    gameUpdate();
    //render baby
    drawPlayer(jugador, background);

}


