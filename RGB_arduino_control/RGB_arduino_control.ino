#include <Adafruit_NeoPixel.h>

// Decode HTTP GET value
String redString = "0";
String greenString = "0";
String blueString = "0";
String testString = "0";
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;
int pos5 = 0;
int pos6 = 0;
String header = "";
String data = "";
#define NUMPIXELS 20
#define DELAYVAL 5

const int ledPin = 10;    
Adafruit_NeoPixel strip(NUMPIXELS, ledPin, NEO_GRBW + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  strip.begin();
  strip.show();
  pinMode(ledPin, OUTPUT);
  strip.setBrightness(255);
}

void loop() {
  // put your main code here, to run repeatedly:
  strip.clear();
  if(Serial.available()){
    delay(1000);
    data = Serial.readString();
    Serial.println("new data received: " + data);
    delay(1000);
  }
  

  if(header != data && data != ""){
    header = data;
    Serial.println("data updated to: " + header);
  }
  else{
    Serial.println("continuing with prev command: " + header);
  }

  

  int rgbVals[3][3];
  int pos1 = header.indexOf("?LED0") + 1;
  int pos2 = header.indexOf("&");

  //other index vars for parsing
  int rpos;
  int gpos;
  int bpos;
  int endpos;
  //array to hold each LED string
  String rgbStrings[3];
    
  //extract each LEDs RGB substring
  for(int i=0; i<3;i++){         
    //extract LED string
    rgbStrings[i] = header.substring(pos1,pos2);
    //continue parsing through header by shifting to next LED block
    pos1 = pos2 + 1;
    pos2 = header.indexOf("&",pos1);
    Serial.println(rgbStrings[i]);
  }
    //loop through each LED RGB string and extract RGB values
  for(int i=0; i<3;i++){
    rpos = rgbStrings[i].indexOf("r");
    gpos = rgbStrings[i].indexOf("g");
    bpos = rgbStrings[i].indexOf("b");
    endpos = rgbStrings[i].indexOf("&");
    //Store RGB values in array
    rgbVals[i][0] = rgbStrings[i].substring(rpos+1,gpos).toInt();
    rgbVals[i][1] = rgbStrings[i].substring(gpos+1,bpos).toInt();
    rgbVals[i][2] = rgbStrings[i].substring(bpos+1).toInt();      
  }
//              int led7 = header.indexOf("LED6");
  // String lastLED = header.substring(header.indexOf("LED6"));
  // endpos = lastLED.indexOf("$");
  String color_type = header.substring(header.indexOf("&c")+2,header.indexOf("&c")+6);
  String command = header.substring(header.indexOf("+")+1,header.indexOf("+")+5);
  uint32_t color1 = strip.Color(rgbVals[0][0], rgbVals[0][1], rgbVals[0][2]);
  uint32_t color2 = strip.Color(rgbVals[1][0], rgbVals[1][1], rgbVals[1][2]);
  uint32_t color3 = strip.Color(rgbVals[2][0], rgbVals[2][1], rgbVals[2][2]);
  Serial.println(color_type);
  Serial.println(command);

  // for (int i=0;i<3;i++){
  //   Serial.println("LED " + String(i) + ": (" + String(rgbVals[i][0]) + ", " + String(rgbVals[i][1]) + ", " + String(rgbVals[i][2]) + ")");
  // }
  if(color_type == "whit"){
    strip.fill(strip.Color(0,0,0,255));
    strip.show();
  }
  else if(command == "fill"){
    Serial.println("Fill command seleceted");
    handleFill(color_type, color1, color2, color3);
  }
  else if(command == "rain"){
    rainbow(7);
  }
  else if(command == "anim"){
    colorWipe(color1, 50);
  }
  else if(command == "puls"){
    handlepulse(color_type, rgbVals);
  }
  else{
    Serial.println("No valid command found");
  }

}



void handleFill(String color_type, uint32_t color1, uint32_t color2, uint32_t color3){

  if(color_type == "sing"){
    strip.fill(color1);
    strip.show();
  }
  else if(color_type == "dual"){
    // strip.setPixelColor(1, color1);
    for(int i=0; i<20; i +=4 ){
      strip.setPixelColor(i, color1);
      strip.setPixelColor(i+1, color1);
      strip.setPixelColor(i+2, color2);
      strip.setPixelColor(i+3, color2);
    }
    strip.show();
  }
  else if(color_type == "tric"){
    for(int i=0; i<20; i += 3){
      strip.setPixelColor(i, color1);
      strip.setPixelColor(i+1, color2);
      strip.setPixelColor(i+2, color3);
    }
    strip.show();
  }
  else{
    Serial.println("Could not identify color type command for the fill function");
    for(int i=0; i<5; i++){
      strip.fill(strip.Color(255,0,0));
      strip.show();
      delay(1000);
      strip.clear();
    }
    
  }

}
void handlepulse(String color_type, int rgbVals[3][3]){
  if(color_type == "sing"){
    int red = rgbVals[0][0];
    int green = rgbVals[0][1];
    int blue = rgbVals[0][2];
    float r = 0;
    float g = 0;
    float b = 0;
    for (int i=0; i<256; i++){
      if(r<red){r += red/255;}
      if(g<green){g += green/255;}
      if(b<blue){b += blue/255;}
      strip.fill(strip.Color(r,g,b));
      strip.show();
      delay(5);
    }
    delay(10);
    for (int i=256; i>0; i--){
      if(r>0){r -= red/255;}
      if(g>0){g -= green/255;}
      if(b>0){b -= blue/255;}
      strip.fill(strip.Color(r,g,b));
      strip.show();
      delay(5);
    }
  }
  else if(color_type == "dual"){
    
  }
  else if(color_type == "tric"){
    
  }
  else{
    Serial.println("Could not identify color type command for the fill function");
    for(int i=0; i<5; i++){
      strip.fill(strip.Color(255,0,0));
      strip.show();
      delay(1000);
      strip.clear();
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    strip.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}