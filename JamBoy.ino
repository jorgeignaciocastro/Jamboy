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

// player constants
struct PLAYER {
  int pSize = 8;
  int x = 0;
  int y = 0; 
  int color = 1;
  byte dir = 0; 
};

PLAYER jugador;


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
    jugador.color = ST7735_GREEN;
}

void Button_B() {
    jugador.color = ST7735_RED;
}

void Button_C() {
    jugador.color = ST7735_WHITE;
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


// main loop

void loop() {

    //listen pins
    checkJoystic();

    //render baby
    tft.fillRoundRect(jugador.x, jugador.y, jugador.pSize, jugador.pSize, 4, jugador.color);

    //Debug
    Serial.println("DEBU");   

}


