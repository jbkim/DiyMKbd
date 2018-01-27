#include <NeoPixelAnimator.h>
#include <NeoPixelBrightnessBus.h>
#include <NeoPixelBus.h>

// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            3
#define GAP           20
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      16

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define HSB
int delayval = 10; // delay for half a second
uint8_t c[3];

void hsb2rgb(uint16_t index, uint8_t sat, uint8_t bright, uint8_t color[3])
{
  uint16_t r_temp, g_temp, b_temp;
  uint8_t index_mod;
  uint8_t inverse_sat = (sat ^ 255);

  index = index % 768;
  index_mod = index % 256;

  if (index < 256)
  {
    r_temp = index_mod ^ 255;
    g_temp = index_mod;
    b_temp = 0;
  }

  else if (index < 512)
  {
    r_temp = 0;
    g_temp = index_mod ^ 255;
    b_temp = index_mod;
  }

  else if ( index < 768)
  {
    r_temp = index_mod;
    g_temp = 0;
    b_temp = index_mod ^ 255;
  }

  else
  {
    r_temp = 0;
    g_temp = 0;
    b_temp = 0;
  }

  r_temp = ((r_temp * sat) / 255) + inverse_sat;
  g_temp = ((g_temp * sat) / 255) + inverse_sat;
  b_temp = ((b_temp * sat) / 255) + inverse_sat;

  r_temp = (r_temp * bright) / 255;
  g_temp = (g_temp * bright) / 255;
  b_temp = (b_temp * bright) / 255;

  color[0]  = (uint8_t)r_temp;
  color[1]  = (uint8_t)g_temp;
  color[2] = (uint8_t)b_temp;
}



void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
  Serial.begin(115200);
  pixels.begin(); // This initializes the NeoPixel library.
  pinMode(10,INPUT);
}

void loop() {
#if 0
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  int dr = digitalRead(10);
  Serial.print("Read Data:");
  Serial.println(dr);
  if(dr){
   pixels.setPixelColor(0,pixels.Color(0,255,0));
   pixels.show();
   delay(100);
    
  }else{
   pixels.setPixelColor(0,pixels.Color(255,0,0));
   pixels.show();
   delay(100);
    
  }
#endif

    for(int j = 0; j < 767 ; j+=5){
       
       for(int i=0;i<NUMPIXELS;i++){
        hsb2rgb(j+GAP*0,255,255,c);
         pixels.setPixelColor(0,pixels.Color(c[0],c[1],c[2])); // Moderately bright green color.
         hsb2rgb((j+GAP*1)%767,255,255,c);
        pixels.setPixelColor(1,pixels.Color(c[0],c[1],c[2])); // Moderately bright green color.
        pixels.setPixelColor(4,pixels.Color(c[0],c[1],c[2])); // Moderately bright green color.
         hsb2rgb((j+GAP*2)%767,255,255,c);
         pixels.setPixelColor(2,pixels.Color(c[0],c[1],c[2])); // Moderately bright green color.
        pixels.setPixelColor(5,pixels.Color(c[0],c[1],c[2])); // Moderately bright green color.
        pixels.setPixelColor(8,pixels.Color(c[0],c[1],c[2])); // Moderately bright green color.
        hsb2rgb((j+GAP*3)%767,255,255,c);
         pixels.setPixelColor(3,pixels.Color(c[0],c[1],c[2])); // Moderately bright green color.
        pixels.setPixelColor(6,pixels.Color(c[0],c[1],c[2])); // Moderately bright green color.
        pixels.setPixelColor(9,pixels.Color(c[0],c[1],c[2])); // Moderately bright green color.
        pixels.setPixelColor(12,pixels.Color(c[0],c[1],c[2])); // Moderately bright green color.
        hsb2rgb((j+GAP*4)%767,255,255,c);
         pixels.setPixelColor(7,pixels.Color(c[0],c[1],c[2])); // Moderately bright green color.
        pixels.setPixelColor(10,pixels.Color(c[0],c[1],c[2])); // Moderately bright green color.
        pixels.setPixelColor(13,pixels.Color(c[0],c[1],c[2])); // Moderately bright green color.
        hsb2rgb((j+GAP*5)%767,255,255,c);
         pixels.setPixelColor(11,pixels.Color(c[0],c[1],c[2])); // Moderately bright green color.
        pixels.setPixelColor(14,pixels.Color(c[0],c[1],c[2])); // Moderately bright green color.
        hsb2rgb((j+GAP*6)%767,255,255,c);
         pixels.setPixelColor(15,pixels.Color(c[0],c[1],c[2])); // Moderately bright green color.
        
       }
       pixels.show(); // This sends the updated pixel color to the hardware.
      
       delay(delayval); // Delay for a period of time (in milliseconds).
    }
    


}
