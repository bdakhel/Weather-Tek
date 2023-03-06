#include <DHT.h>  // include the DHT library
#include <DHT_U.h>
#include <Adafruit_LiquidCrystal.h>  // include the LCD library
#include <Encoder.h>
#define DHTPIN 4  // define the pin for the DHT sensor
#define DHTTYPE DHT22  // set the type of DHT sensor
Encoder myEnc(2, 3);  // create an Encoder object
DHT dht(DHTPIN, DHTTYPE);  // create a DHT object
Adafruit_LiquidCrystal lcd(12, 11, 9, 8, 7, 6);  // create an LCD object

// speed stuff
float tempo1;
float tempo2;
float oldPosition = -999;  // initialize the old position to a negative value
const float theta = 0.031415;  // set the constant for the conversion from encoder ticks to radians
const long conversion = 1000;  // set the constant for the conversion from milliseconds to seconds
float dt;
float dPos;
float omega;
float linearSpd;

void setup() {
  Serial.begin(9600);  // initialize the serial communication
  dht.begin();  // start the DHT sensor
  lcd.begin(16, 2);  // initialize the LCD screen
  lcd.setCursor(0,0);  // set the cursor to the first line, first position
  lcd.print("hmd:");  // print "hmd:" for humidity
  lcd.print("tmp:");  // print "tmp:" for temperature
  lcd.setCursor(0,1);  // set the cursor to the second line, first position
  lcd.print("wspd:");  // print "wspd:" for wind speed
}

void loop() {
  enc_reading();  // call the function to measure wind speed using the encoder
  dht_reading();  // call the function to measure temperature/humidity using the DHT sensor
}

void enc_reading() {
  long newPosition = myEnc.read();  // read the current position of the encoder
  if (newPosition != oldPosition) {  // if the position has changed
    tempo2 = millis();  // record the current time
    dPos = abs(newPosition - oldPosition);  // calculate the change in position (absolute value to handle counter-clockwise rotation)
    dt = float(tempo2 - tempo1) / conversion;  // calculate the time between readings, in seconds
    oldPosition = newPosition;  // store the new position as the old position for the next iteration
    tempo1 = tempo2;  // store the current time as the previous time for the next iteration
    omega = (float)(dPos * theta) / (float)(dt);  // calculate the angular speed (radians per second)
    linearSpd = 0.14 * omega;  // calculate the linear speed (meters per second) based on the radius of the anemometer (0.14 m)
    lcd.setCursor(0,0);  // set the cursor to the first line, first position
    lcd.print("WindSpd ");
