// Libraries for LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Declaring address of LCD
// lcd(Address, chars, lines)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Pins
int ledPinPlus = 8;
int ledPinMinus = 7;
int buttonPinPlus = 2;
int buttonPinMinus = 3;

// Global
int score = 0;
int gameTime = 0;
int toggleStatePlus;
int toggleStateMinus;
int lastButtonStatePlus = 1;
int lastButtonStateMinus = 1;
long unsigned int lastPress;
volatile int buttonFlag;
int debounceTime = 40;

void setup(){
    // Pin setup
    pinMode(ledPinPlus, OUTPUT);
    pinMode(ledPinMinus, OUTPUT);
    pinMode(buttonPinPlus, INPUT_PULLUP);
    pinMode(buttonPinMinus, INPUT_PULLUP);

    // Interrupt
    attachInterrupt(digitalPinToInterrupt (buttonPinPlus), ISR_button, LOW);
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt (buttonPinMinus), ISR_button, LOW);

    // LCD initial setup
    lcd.init();
    lcd.backlight();
    lcd.setCursor(4,0);
    lcd.print("Score: ");
    lcd.setCursor(11,0);
    lcd.print(score);

    lcd.setCursor(0,1);
    lcd.print("Time: ");
    lcd.print(gameTime);
}

void loop(){
  if ((millis() - lastPress) > debounceTime && buttonFlag){
    // Increment Side
    if (digitalRead(buttonPinPlus) == 0 && lastButtonStatePlus == 1){
      toggleStatePlus = 1;
      toggleStateMinus = 0;
      digitalWrite(ledPinPlus, toggleStatePlus);
      digitalWrite(ledPinMinus, toggleStateMinus);
      lastButtonStatePlus = 0;
      score++;
    }
    else if (digitalRead(buttonPinPlus) == 1 && lastButtonStatePlus == 0){
      lastButtonStatePlus = 1;
    }

    // Decrement Side
    else if (digitalRead(buttonPinMinus) == 0 && lastButtonStateMinus == 1){
      toggleStateMinus = 1;
      toggleStatePlus = 0;
      digitalWrite(ledPinPlus, toggleStatePlus);
      digitalWrite(ledPinMinus, toggleStateMinus);
      lastButtonStateMinus = 0;
      score--;
    }
    else if (digitalRead(buttonPinMinus) == 1 && lastButtonStateMinus == 0){
      lastButtonStateMinus = 1;
    }
    // Reset Flag
    buttonFlag = 0;

    // Update Timer
    gameTime++;
    
    // Update LED
    scoreChange();
  }
}

void scoreChange(){
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("Score: ");
  lcd.setCursor(11,0);
  lcd.print(score);
  
}

void ISR_button(){
  buttonFlag = 1;
}

// LCD Display
// The purpose of this function is to constantly update the LCD with the current values stored. In this case, we will be observing an incrementing or decrementing value from the buttons
//
// present lcd display and contents
//
// Time Elapsed: minutes:seconds
// Score: y
// Last Action: Increment / Decrement
