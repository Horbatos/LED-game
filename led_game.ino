#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
#define PIN 10	 // input pin Neopixel is attached to
#define NUMPIXELS      48 // number of neopixels in strip

LiquidCrystal lcd(9, 8, A0, A1, A2, A3);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval;
int activeStripe, score, highscore;
bool gameActive, sensorActive;
float duration, distance; 
int sensor1 = 4, sensor2 = 5, sensor3 = 6;
bool sensor1_pressed, sensor2_pressed, sensor3_pressed;

//sets up the game
void setup() 
{
  Serial.begin(9600);
  pixels.begin();
  pixels.clear();
  highscore = 0;
  lcd.begin(16, 2);
  gameActive = false;
}

// the main loop
void loop() 
{
  delay(10);
  if(gameActive == false){
    lcd.clear();
    lcd.print("GAME READY");
    lcd.setCursor(0, 1);
    lcd.print("HIGH SCORE: ");
    lcd.print(highscore);
    if(SonarSensor(sensor1) || SonarSensor(sensor2) || SonarSensor(sensor3)){
      gameStart();
    }
  }
  
  if(gameActive == true){
  	fallingLights();
  }
}

//Falling lights effect on the neopixel stripe
void fallingLights()
{
  if (activeStripe == 1){
  	 for (int i=0; i < (NUMPIXELS/3); i++) {
       pixels.clear();
       pixels.setPixelColor(i, pixels.Color(140, 60, 255));
       pixels.show(); 
       if(SonarSensor(sensor1)){
         scoreUp();
         break;
       }
       else if(SonarSensor(sensor2)){
         gameOver();
         break;
       }
       else if(SonarSensor(sensor3)){
         gameOver();
         break;
       }
       delay(delayval);
       if(i == (NUMPIXELS/3)-1){
       	 gameOver();
         break;
       }
  	}
  }
  else if (activeStripe == 2){
     for (int i=(NUMPIXELS/3); i < (NUMPIXELS/3*2); i++) {
       pixels.clear();
       pixels.setPixelColor(i, pixels.Color(140, 60, 255));
       pixels.show(); 
       if(SonarSensor(sensor2)){
         scoreUp();
         break;
       }
       else if(SonarSensor(sensor1)){
         gameOver();
         break;
       }
       else if(SonarSensor(sensor3)){
         gameOver();
         break;
       }
       delay(delayval);
       
       if(i == (NUMPIXELS/3*2)-1){
       	 gameOver();
         break;
       }
  	}
  }
  else if (activeStripe == 3){
     for (int i=(NUMPIXELS/3*2); i < NUMPIXELS; i++) {
       pixels.clear();
       pixels.setPixelColor(i, pixels.Color(140, 60, 255));
       pixels.show(); 
       if(SonarSensor(sensor3)){
         scoreUp();
         break;
       }
       else if(SonarSensor(sensor1)){
         gameOver();
         break;
       }
       else if(SonarSensor(sensor2)){
         gameOver();
         break;
       }
       delay(delayval);
       
       if(i == NUMPIXELS-1){
       	 gameOver();
         break;
       }
  	}
  }
}

//function for calculating distance
bool SonarSensor(int sensor)
{
  pinMode(sensor, OUTPUT);
  digitalWrite(sensor, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor, LOW);
  pinMode(sensor, INPUT);
  duration = pulseIn(sensor, HIGH);
  distance = (duration/2) / 29;
  if (distance < 100){return true;}
  else {return false;}
}


//function for choosing a new random stripe
void randomStripe(){
  activeStripe = rand() % 3 + 1;
}

//function to start the game
void gameStart(){
  lcd.clear();
  lcd.print("GAME STARTING!");
  score = 0;
  delayval = 500;
  gameActive = true;
  randomStripe();
  pixels.clear();
  for (int i=0; i < NUMPIXELS; i++) {
	pixels.setPixelColor(i, pixels.Color(0, 255, 0));
  }
  pixels.show();
  delay(3000);
  pixels.clear();
  lcd.clear();
  lcd.print("GO!!!");
}

//if the wrong sensor is pressed or if it is not pressed on time the game is over
void gameOver(){
  lcd.clear();
  lcd.print("GAME OVER");
  lcd.setCursor(0, 1);
  lcd.print("YOUR SCORE: ");
  lcd.print(score);
  if (score > highscore){highscore = score;}
  gameActive = false;
  pixels.clear();
  for (int i=0; i < NUMPIXELS; i++) {
	pixels.setPixelColor(i, pixels.Color(255, 0, 0));
  }
  pixels.show();
  delay(5000);
  pixels.clear();
  pixels.show();
}

//if the right sensor is pressed the score goes up
void scoreUp(){
  pixels.clear();
  score = score + 10;
  lcd.clear();
  if(activeStripe == 1) {lcd.print("GOOD JOB!");}
  else if(activeStripe == 1) {lcd.print("KEEP GOING!");}
  else {lcd.print("NICE!");}
  lcd.setCursor(0, 1);
  lcd.print("SCORE: ");
  lcd.print(score);
  randomStripe();
  if(score % 50 == 0){
    delayval = delayval - 10;
  }
  if (delayval < 10){delayval = 10;}
  delay(400);
}
