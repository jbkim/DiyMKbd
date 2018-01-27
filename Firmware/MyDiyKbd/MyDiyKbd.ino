#include <HID-Project.h>
#include <Adafruit_NeoPixel.h>

#define NUM_BTN_COLS (4) // Number of switch columns (isolating diode anode)
#define NUM_BTN_ROWS (4) // Number of switch rows (isolating diode cathode)

// Debounce built-in to the code. This sets the number of button
// high or low senses that trigger a press or release
#define MAX_DEBOUNCE (3)
#define KEY_COMMAND KEY_LEFT_GUI

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            3

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      16

static const uint8_t btnRowPins[NUM_BTN_ROWS] = {A0,A1,A2,A3}; // Pins connected to switch rows (2)
static const uint8_t btnColPins[NUM_BTN_COLS] = {10,16,14,15};; // Pins connected to switch columns (1)
static int8_t debounce_count[NUM_BTN_COLS][NUM_BTN_ROWS]; // One debounce counter per switch

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void showPixels(uint8_t row, uint8_t col, int r, int g, int b)
{
   pixels.setPixelColor(col*4 + row, pixels.Color(r,g,b));
   pixels.show();
   // delay(100);
}

void setup()
{
  uint8_t i, j;

  setupSwitchPins();

  // Initialize the debounce counter array
  for (i = 0; i < NUM_BTN_COLS; i++)
  {
    for (j = 0; j < NUM_BTN_ROWS; j++)
    {
      debounce_count[i][j] = 0;
    }
  }

// Initialize USB keyboard
  Keyboard.begin();
  
// Initialize Neopixel
  pixels.begin();
  colorWipe(pixels.Color(127, 127, 127), 50); // White

// Off
  for(i = 0; i < 4 ; i++){
    for(j = 0; j < 4 ; j++){
      showPixels(i,j,0,0,0);
      delay(50);
    }
  }
}

void loop() 
{
  scan();
}

static void scan()
{
  static uint8_t currentRow = 0;
  uint8_t i, j; // for loop counters
  int key;

  // Select current row, and write all components on that row LOW.
  // That'll set the LED anode's LOW, and write the switch "2" pins LOW.
  // If diodes were added, "2' should be connected to the diode cathode
  digitalWrite(btnRowPins[currentRow], LOW);
  // digitalWrite(ledRowPins[currentRow], LOW);

  // Scan through switches on this row:
  for ( j = 0; j < NUM_BTN_COLS; j++)
  {
    // Read the button. If it's pressed, it should be LOW.
    if (digitalRead(btnColPins[j]) == LOW)
    {
      if ( debounce_count[currentRow][j] < MAX_DEBOUNCE)
      { // Increment a debounce counter
        debounce_count[currentRow][j]++;
        if ( debounce_count[currentRow][j] == MAX_DEBOUNCE )
        { // If debounce counter hits MAX_DEBOUNCE (default: 3)
          // Trigger key press -- Do anything here...
          // Serial.print("Key pressed "); 
          // Serial.println((currentRow * NUM_BTN_COLS) + j);

          showPixels(currentRow, j, 0, 255, 0);  // green
          key = (currentRow * NUM_BTN_COLS) + j;
          // key = key + '0';
          // Keyboard.press(key); // Will display 0123456789:;<=>>?
          run_cmd(key);
        }
      }
    }
    else // Otherwise, button is released
    {
      if ( debounce_count[currentRow][j] > 0)
      { 
        debounce_count[currentRow][j]--; // Decrement debounce counter
        if ( debounce_count[currentRow][j] == 0 )
        { // If debounce counter hits 0
          // Trigger key release -- Do anything here...
          showPixels(currentRow, j, 0,0,0);  
          Keyboard.releaseAll();
        }
      }
    }
  }

  // Once done scanning, de-select the switch and LED rows
  // by writing them HIGH.
  digitalWrite(btnRowPins[currentRow], HIGH);

  // Increment currentRow, so next time we scan the next row
  currentRow++;
  // if (currentRow >= NUM_LED_ROWS)
  if (currentRow >= NUM_BTN_ROWS)
  {
    currentRow = 0;
  }
}

static void setupSwitchPins()
{
  uint8_t i;

  // Button drive rows - written LOW when active, HIGH otherwise
  for (i = 0; i < NUM_BTN_ROWS; i++)
  {
    pinMode(btnRowPins[i], OUTPUT);

    // with nothing selected by default
    digitalWrite(btnRowPins[i], HIGH);
  }

  // Buttn select columns. Pulled high through resistor. Will be LOW when active
  for (i = 0; i < NUM_BTN_COLS; i++)
  {
    pinMode(btnColPins[i], INPUT_PULLUP);
  }
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, c);
    pixels.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) 
{
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j = 0; j < 256; j++) {
    for(i = 0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}


void run_cmd(uint8_t key)
{
  switch(key) {
    case 0:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('q');    
      break;    
    case 1:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('a');    
      break;
    case 2:
      break;
    case 3:
      Keyboard.press(KEY_LEFT_ARROW);
      break;
    case 4:
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('w');    
      break;
    case 5:
      break;
    case 6:
      Keyboard.press(KEY_UP_ARROW);
      break;
    case 7:
      Keyboard.press(KEY_DOWN_ARROW);
      break;
    case 8:
      break;
    case 9:
      break;
    case 10:
      break;
    case 11:
      Keyboard.press(KEY_RIGHT_ARROW);    
      break;
    case 12:
      Keyboard.press(KEY_ESC);
      break;
    case 13:
      Keyboard.press(KEY_LEFT_GUI);
      // Shift-Q logs out:
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press('q');      
      delay(100);
      Keyboard.releaseAll();
      // enter:
      // Keyboard.write(KEY_RETURN);      
      break;
    case 14:
      Keyboard.press(KEY_PAGE_UP);
      break;
    case 15:
      Keyboard.press(KEY_PAGE_DOWN);
      break;
  }
}
