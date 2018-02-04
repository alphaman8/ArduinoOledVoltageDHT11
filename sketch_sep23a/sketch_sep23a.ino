/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Seeed_BME280.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define DHTPIN 10
#define DHTTYPE DHT11 

#define humidity_width 20
#define humidity_height 22
static unsigned char humidity_bits[] = {
   0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0xc0, 0x00, 0x00, 0xc0, 0x01, 0x00,
   0xc0, 0x01, 0x00, 0xe0, 0x03, 0x00, 0xf0, 0x03, 0x00, 0xf0, 0x07, 0x00,
   0xf8, 0x47, 0x00, 0xf8, 0x47, 0x00, 0xf8, 0xc5, 0x00, 0xf0, 0xe6, 0x00,
   0xf0, 0xf3, 0x01, 0xc0, 0xf1, 0x01, 0x00, 0xf8, 0x03, 0x00, 0xf8, 0x03,
   0x00, 0xfc, 0x07, 0x00, 0xfc, 0x06, 0x00, 0xf8, 0x06, 0x00, 0x78, 0x03,
   0x00, 0xf0, 0x01, 0x00, 0x00, 0x00 };

BME280 bme280;   

int sec = 0;
int min = 0;
int hour = 0;
float maxVoltage = 0;
float minVoltage = 3.3;
float volt = 0;
int margin_info2 = 45;
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000;   

void setup()   {                
  Serial.begin(9600);

  if(!bme280.init()){ 
    Serial.println("Device error!");
  }

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();

  // draw a bitmap icon and 'animate' movement
//  testdrawbitmap(humidity_bits, humidity_height, humidity_height);  
}


void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    calculateTime();
//    volt = readVcc();
//    volt = volt/1000;
//  
//    if (volt > maxVoltage) {
//      maxVoltage = volt;
//    }  

    display.clearDisplay();  
    printTempHumid();  
//    drawVoltage();  
    drawTime();      
  }    
//  delay(1000);
}

void calculateTime()
{
  sec++;

  if (sec == 60)
  {
    sec = 0;
    min++;
  }
  if (min == 60)
  {
    min = 0;
    hour++;
  }
}

void drawVoltage(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(margin_info2,10);
  display.print(volt);
  display.print("v");
  display.setCursor(margin_info2+35,10);
  display.print(maxVoltage);
  display.print("v"); 
  display.setCursor(margin_info2,20);
  float percent = 
    ((volt-minVoltage)*100)/(maxVoltage-minVoltage);
  display.print(percent);
  display.print("%"); 
  display.display();
}

void printTempHumid()
{
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = bme280.getHumidity();
    // Read temperature as Celsius (the default)
    float t = bme280.getTemperature();
  
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  
    // Compute heat index in Fahrenheit (the default)
//    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
//    float hic = dht.computeHeatIndex(t, h, false);

    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.setTextSize(2);
    display.print((int)t);
    display.setTextSize(1);
    display.print("C ");  
    display.setCursor(0,15);    
    display.setTextSize(2);
    display.print((int)h);
    display.setTextSize(1);    
    display.print("%");  
    display.display();
  }

void drawTime(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(margin_info2,0);
  display.print(hour);
  display.print(":");  
  display.print(min);
  display.print(":");
  display.print(sec);
  display.display();
}

//long readVcc() {
//    // Read 1.1V reference against AVcc
//    // set the reference to Vcc and the measurement to the internal 1.1V reference
//    #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
//      ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
//    #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
//      ADMUX = _BV(MUX5) | _BV(MUX0);
//    #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
//      ADMUX = _BV(MUX3) | _BV(MUX2);
//    #else
//      ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
//    #endif  
//  
//    delay(2); // Wait for Vref to settle
//    ADCSRA |= _BV(ADSC); // Start conversion
//    while (bit_is_set(ADCSRA,ADSC)); // measuring
//  
//    uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
//    uint8_t high = ADCH; // unlocks both
//  
//    long result = (high<<8) | low;
//  
//    result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
//    return result; // Vcc in millivolts
//}
