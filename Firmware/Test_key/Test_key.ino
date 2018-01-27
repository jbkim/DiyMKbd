#include <HID-Project.h>
#include <Adafruit_NeoPixel.h>

#define NUM_BTN_COLS (4) // Number of switch columns (isolating diode anode)
#define NUM_BTN_ROWS (4) // Number of switch rows (isolating diode cathode)

// Debounce built-in to the code. This sets the number of button
// high or low senses that trigger a press or release
#define MAX_DEBOUNCE (3)


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
  setupSwitchPins();

  // Initialize the debounce counter array
  for (uint8_t i = 0; i < NUM_BTN_COLS; i++)
  {
    for (uint8_t j = 0; j < NUM_BTN_ROWS; j++)
    {
      debounce_count[i][j] = 0;
    }
  }

// Initialize USB keyboard
  Keyboard.begin();
  
// Initialize Neopixel
  pixels.begin();
  pixels.show(); // Initialize all pixels to 'off'
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
          showPixels(currentRow, j, 255,0,0);  // red
          key = (currentRow * NUM_BTN_COLS) + j;
          key = key + '0';
          Keyboard.press(key); // Will display 0123456789:;<=>>?
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
