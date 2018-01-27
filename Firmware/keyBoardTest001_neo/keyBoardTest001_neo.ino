//#include <HID-Settings.h>
#include <HID-Project.h>

// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            3
#define GAP           20

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      16
#define FN1 0x01
#define FN2 0x02
#define FN3 0x03

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int fn;

void showPixels(uint8_t row, uint8_t col,int r, int g, int b){
   pixels.setPixelColor(col*4+row,pixels.Color(r,g,b));
   pixels.show();
//   delay(100);
}

/*  4x4 macro keyboard 
 *  Copyright 2016 by Pedro Nariyoshi <pedro.nariyoshi@gmail.com>
 *  Licensed under GNU General Public License 3.0 or later.
 *  @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 *  
 *  Code derived from Button/LED Matrix Scanning Example - 3x3 Keypad
 *  by Jimbo @ SparkFun Electronics
 *    https://learn.sparkfun.com/tutorials/cherry-mx-switch-breakout-hookup-guide
*/

//// Hardware definitions
#define NUM_COLS 4 // Number of switch columns 
#define NUM_ROWS 4 // Number of switch rows

//// Software defined debounce
#define MAX_DEBOUNCE 3

//// Apple keyboard macros
#define KEY_COMMAND KEY_LEFT_GUI
#define KEY_CTRL KEY_LEFT_CTRL
#define KEY_OPTION KEY_LEFT_ALT
#define KEY_EJECT HID_CONSUMER_EJECT

//// Keyboard matrix pin assignments
// Pins connected to rows
static const uint8_t RowPins[NUM_ROWS] = {A0,A1,A2,A3};
// Pins connected to columns 
static const uint8_t ColPins[NUM_COLS] = {10,16,14,15};

//// Global variables
// Counter for how long a button has been pressed
static uint8_t debounce_count[NUM_COLS][NUM_ROWS];

void setup()
{
  // Counter to set up pins
  uint8_t i;
  fn = FN1;
  // Set row scan pins to output and then to HIGH (not active)
  for (i = 0; i < NUM_ROWS; i++)
  {
    pinMode(RowPins[i], OUTPUT);
    digitalWrite(RowPins[i], HIGH);
  }
  
  // Set column pins to input with pull-up resistors (no need for external pull-up resistors)
  for (i = 0; i < NUM_COLS; i++)
  {
    pinMode(ColPins[i], INPUT_PULLUP);
  }
  
  // Initialize USB keyboard
  Keyboard.begin();
  pixels.begin();
  // Initialize the debounce counter array
  memset(debounce_count,0,sizeof(debounce_count));
  //  for (uint8_t i = 0; i < NUM_COLS; i++)
  //  {
  //    for (uint8_t j = 0; j < NUM_ROWS; j++)
  //    {
  //      debounce_count[i][j] = 0;
  //    }
//  }
  for(int row = 0 ; row < 4 ; row++){
    for(int col = 0 ; col < 4 ; col++){
      showPixels(row,col,random(0,255),random(0,255),random(0,255));
      delay(50);
    }
  }
  for(int row = 0 ; row < 4 ; row++){
    for(int col = 0 ; col < 4 ; col++){
      showPixels(row,col,0,0,0);
      delay(50);
    }
  }

showPixels(0,0,random(0,255),random(0,255),random(0,255));


}

void loop() 
{
  // Each run through the scan function operates on a single row
  // of the matrix, kept track of using the currentRow variable.
  static uint8_t currentRow = 0;
  static uint8_t currentCol; // for loop counters

  // Select current row
  digitalWrite(RowPins[currentRow], LOW);

  // Scan through switches on this row:
  for (currentCol = 0; currentCol < NUM_COLS; currentCol++)
  {
    // Read the button. If it's pressed, it should be LOW.
    if (digitalRead(ColPins[currentCol]) == LOW)
    {
      if (debounce_count[currentRow][currentCol] < MAX_DEBOUNCE)
      {
        // Increment a debounce counter
        debounce_count[currentRow][currentCol]++;
        // If debounce counter hits MAX_DEBOUNCE
        // Trigger key press
        if ( debounce_count[currentRow][currentCol] == MAX_DEBOUNCE )
        { 
          pressMacro(currentRow,currentCol); 
        }
      }
    }
    else // Otherwise, button is released
    {
      if ( debounce_count[currentRow][currentCol] > 0)
      { 
        // Decrement debounce counter
        debounce_count[currentRow][currentCol]--;
        if ( debounce_count[currentRow][currentCol] == 0 )
        { // If debounce counter hits 0
          releaseMacro(currentRow,currentCol);
        }
      }
    }
  }

  // Once done scanning, de-select the row pin
  digitalWrite(RowPins[currentRow], HIGH);

  // Increment currentRow, so next time we scan the next row
  currentRow = (currentRow > NUM_ROWS-2) ? 0 : currentRow+1;
//  currentRow++;
//  if (currentRow >= NUM_ROWS)
//  {
//    currentRow = 0;
//  }
}

void pressMacro(uint8_t row, uint8_t col)
{
  
  showPixels(row,col,0,0,0);
  switch(row)
  {
    case 0:
      switch(col)
      {
        case 0:
          showPixels(1,0,0,0,0);
          showPixels(2,0,0,0,0);
          break;
        case 1:
          switch(fn){
            case FN1:
              Keyboard.press(KEY_CTRL);
              Keyboard.press(KEY_UP);
              
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          
          break;
        case 2:
          switch(fn){
            case FN1:
              Keyboard.press(KEY_COMMAND);
              Keyboard.press('~');
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          break;
        case 3:
          switch(fn){
            case FN1:
              Keyboard.press(KEY_COMMAND);
              Keyboard.press('s');
              break;
            case FN2:
              break;
            case FN3:
              Keyboard.press(KEY_COMMAND);
              Keyboard.press(KEY_OPTION);
              Keyboard.press(KEY_EJECT);
              break;
          }
          
          break;
      }
      break;
    case 1:
      switch(col)
      {
        case 0:
          showPixels(0,0,0,0,0);
          showPixels(2,0,0,0,0);
          break;
        case 1:
          switch(fn){
            case FN1:
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          
          break;
        case 2:
          switch(fn){
            case FN1:
            Keyboard.press(KEY_COMMAND);
            Keyboard.press(KEY_LEFT);
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          
          break;
        case 3:
          switch(fn){
            case FN1:
              Keyboard.press(KEY_LEFT);

              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          break;
      }
      break;
    case 2:
          switch(col)
      {
        case 0:
          showPixels(0,0,0,0,0);
          showPixels(1,0,0,0,0);
          break;
        case 1:
          switch(fn){
            case FN1:
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          break;
        case 2:
          switch(fn){
            case FN1:
              Keyboard.press(KEY_UP);
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          break;
          
        case 3:
          switch(fn){
            case FN1:
              Keyboard.press(KEY_DOWN);
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          break;
          
      }
      break;
    case 3:
          switch(col)
      {
        case 0:
          switch(fn){
            case FN1:
              Keyboard.press(KEY_PAGE_UP);
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          break;
          
        case 1:
          switch(fn){
            case FN1:
              Keyboard.press(KEY_PAGE_DOWN);
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          break;
        case 2:
          switch(fn){
            case FN1:
              Keyboard.press(KEY_COMMAND);
              Keyboard.press(KEY_RIGHT);
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          break;

        case 3:
          switch(fn){
            case FN1:
              Keyboard.press(KEY_RIGHT);
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          break;
      }
      break;
  }
}
void releaseMacro(uint8_t row, uint8_t col)
{
  
  showPixels(row,col,0,0,0);
  switch(row)
  {
    case 0:
      switch(col)
      {
        case 0:
          showPixels(row,col,random(0,255),random(0,255),random(0,255));
          fn = FN1;
          break;
        case 1:
          switch(fn){
            case FN1:
              Keyboard.release(KEY_CTRL);
              Keyboard.release(KEY_UP);
              
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          
          break;
        case 2:
          switch(fn){
            case FN1:
              Keyboard.release(KEY_COMMAND);
              Keyboard.release('~');
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          break;
        case 3:
          switch(fn){
            case FN1:
              Keyboard.release(KEY_COMMAND);
              Keyboard.release('s');
              break;
            case FN2:
              break;
            case FN3:
              Keyboard.release(KEY_COMMAND);
              Keyboard.release(KEY_OPTION);
              Keyboard.release(KEY_EJECT);
              break;
          }
          
          break;
      }
      break;
    case 1:
      switch(col)
      {
        case 0:
          showPixels(row,col,random(0,255),random(0,255),random(0,255));
          fn = FN2;
          break;
        case 1:
          switch(fn){
            case FN1:
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          
          break;
        case 2:
          switch(fn){
            case FN1:
            Keyboard.release(KEY_COMMAND);
            Keyboard.release(KEY_LEFT);
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          
          break;
        case 3:
          switch(fn){
            case FN1:
              Keyboard.release(KEY_LEFT);

              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          break;
      }
      break;
    case 2:
          switch(col)
      {
        case 0:
          showPixels(row,col,random(0,255),random(0,255),random(0,255));
          fn=FN3;
          break;
        case 1:
          switch(fn){
            case FN1:
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          break;
        case 2:
          switch(fn){
            case FN1:
              Keyboard.release(KEY_UP);
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          break;
          
        case 3:
          switch(fn){
            case FN1:
              Keyboard.release(KEY_DOWN);
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          break;
          
      }
      break;
    case 3:
          switch(col)
      {
        case 0:
          switch(fn){
            case FN1:
              Keyboard.release(KEY_PAGE_UP);
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          break;
          
        case 1:
          switch(fn){
            case FN1:
              Keyboard.release(KEY_PAGE_DOWN);
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          break;
        case 2:
          switch(fn){
            case FN1:
              Keyboard.release(KEY_COMMAND);
              Keyboard.release(KEY_RIGHT);
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          break;

        case 3:
          switch(fn){
            case FN1:
              Keyboard.release(KEY_RIGHT);
              break;
            case FN2:
              break;
            case FN3:
              break;
          }
          break;
      }
      break;
  }
}
