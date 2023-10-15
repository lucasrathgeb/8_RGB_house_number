// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
// Replace with your network credentials
const char* ssid     = "#8";
const char* password = "8robtest";

// Set web server port number to 80
WiFiServer server(80);

// Decode HTTP GET valu
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
#define NUMPIXELS 20
#define DELAYVAL 5

// Variable to store the HTTP req  uest
String header;
//LED TYPE IS SK6812 (similar to WS2812?)
const int ledPin = 14;    // 14 corresponds to GPIO14
Adafruit_NeoPixel strip(NUMPIXELS, ledPin, NEO_GRBW + NEO_KHZ800);
// Setting PWM bit resolution
//const int resolution = 256;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting up ");
  Serial.println(ssid);
  
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.begin();
  strip.begin();
  strip.show();
  pinMode(ledPin, OUTPUT);
  strip.setBrightness(255);
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients
  strip.clear();
  if (client) {                           
    // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {            // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
                   
            // Display the HTML web page
            client.println("<!DOCTYPE html> <html><head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <link rel=\"icon\" href=\"data:,\"> <link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\"> <script src=\"https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js\"> </script> </head> <body onload=\"getColour()\"> <div class=\"container\"> <div class=\"row\"> <h1>ESP Color Picker</h1> </div> <p>Color Options <select name=\"Color Options\" id=\"color_type\"> <option value=\"sing\">Single Color</option> <option value=\"dual\">Dual Color</option> <option value=\"tric\">Tri Color</option> <option value=\"whit\">White</option> </select> </p> <p>Fucntion Options <select name=\"Function Options\" id=\"func_type\"> <option value=\"fill\">Fill</option> <option value=\"anim\">Animate (Not Ready)</option> <option value=\"puls\">Pulse (Not Ready)</option> <option value=\"rain\">Rainbow</option> </select> </p> <table> <thead> <tr> <th>Column 1</th> <th>Column 2</th> </tr> </thead> <tbody id=\"tableBody\"> </tbody> </table> </div> <div class=\"container\"> <a class=\"btn btn-primary btn-lg\" href=\"#\" id=\"updatebutton\" role=\"button\" onclick=\"update()\">GO</a> <div> <script> function updateTable() { const num_colors = document.getElementById('color_type').value; var rowCount = 0; if(num_colors === \"sing\"){ rowCount = 1; } else if (num_colors === \"dual\"){ rowCount = 2; } else if (num_colors === \"tric\"){ rowCount = 3; } else if (num_colors === \"whit\"){ rowCount = 1; } else{ rowCount = 2; } tableBody = document.getElementById('tableBody'); tableBody.innerHTML = ''; for (let i = 0; i < rowCount; i++) { row = document.createElement('tr'); cell1 = document.createElement('td'); cell2 = document.createElement('td'); cell1.textContent = \"Color \" + String(i+1); colorInput = document.createElement('input'); colorInput.setAttribute('class', 'jscolor'); colorInput.setAttribute('id', 'rgb' + String(i)); cell2.appendChild(colorInput); row.appendChild(cell1); row.appendChild(cell2); tableBody.appendChild(row); } jscolor.installByClassName('jscolor'); } document.getElementById('color_type').addEventListener('change', updateTable); updateTable(); </script> <script> function getPickerColour(picker){ console.log(Math.round(picker.rgb[0]) + ', ' + Math.round(picker.rgb[1]) + ', ' + Math.round(picker.rgb[2])); return Math.round(picker.rgb[0]) + ', ' + Math.round(picker.rgb[1]) + ', ' + Math.round(picker.rgb[2]); } </script> <script> function getColourCodeURLFromPicker(picker){ return Math.round(picker.rgb[0]) + \"g\" + Math.round(picker.rgb[1]) + \"b\" + Math.round(picker.rgb[2]) + \"&\"; } </script> <script> function update() { picker0 = document.getElementById('rgb0').jscolor; picker0.innerHTML = getPickerColour(picker0); rgb0_url = \"?LED0r\" + getColourCodeURLFromPicker(picker0) ; if(document.getElementById('rgb1') != null){ picker1 = document.getElementById('rgb1').jscolor; picker1.innerHTML = getPickerColour(picker1); rgb1_url = \"LED1r\" + getColourCodeURLFromPicker(picker1); } else{ rgb1_url = \"LED1r0b0g0&\" } if(document.getElementById('rgb2') != null){ picker2 = document.getElementById('rgb2').jscolor; picker2.innerHTML = getPickerColour(picker2); rgb2_url = \"LED2r\" + getColourCodeURLFromPicker(picker2); } else{ rgb2_url = \"LED2r0b0g0&\"; } colour_type = \"c\" + document.getElementById('color_type').value; func_type = \"+\" + document.getElementById('func_type').value; new_url = rgb0_url + rgb1_url + rgb2_url + colour_type + func_type; document.getElementById(\"updatebutton\").href = new_url; } </script> <script> function getRGBFromURL(led_num){ console.log(led_num); pos1 = url.indexOf(\"LED\" + String(led_num))+1; console.log(\"LED\" + String(led_num)); pos2 = url.indexOf(\"&\",pos1); current_colour = url.substring(pos1,pos2); posr = current_colour.indexOf(\"r\"); posg = current_colour.indexOf(\"g\"); posb = current_colour.indexOf(\"b\"); redval = current_colour.substring(posr+1,posg); blueval = current_colour.substring(posg+1,posb); greenval = current_colour.substring(posb+1); rgb_string = \"rgb(\" + redval + \", \" + blueval + \",\" + greenval + \")\"; return rgb_string; } </script> <script> function getColourType(url){ if(url.indexOf(\"&c\")>=0){ pos1 = url.indexOf(\"&c\") + 2; pos2 = pos1 + 4; color_func_type = url.substring(pos1,pos2); console.log(color_func_type); } else{ color_func_type = \"null\" } return color_func_type; } </script> <script> function getFuncType(url){ if(url.indexOf(\"+\")>=0){ pos1 = url.indexOf(\"+\") + 1; pos2 = pos1 + 4; func_type = url.substring(pos1,pos2); console.log(func_type); } else{ func_type = \"null\" } return func_type; } </script> <script> function getColour(){ url = window.location.href; document.getElementById('color_type').value = getColourType(url); document.getElementById('func_type').value = getFuncType(url); updateTable(); if(url.indexOf(\"?LED0\")>=0){ document.getElementById('rgb0').jscolor.fromString(getRGBFromURL(0)); if(document.getElementById('rgb1') != null){ document.getElementById('rgb1').jscolor.fromString(getRGBFromURL(1)); } if(document.getElementById('rgb2') != null){ document.getElementById('rgb2').jscolor.fromString(getRGBFromURL(2)); } } } </script> </body> </html>");
            client.println();
            Serial.println("HTML has been served");
            // Request sample: /?r201g32b255&
            // Red = 201 | Green = 32 | Blue = 255
            // NEW FORMAT: /?LED0#r255g255b255&LED1#r0b0g0&LED2#r0b0g0&csing+anim
            if(header.indexOf("GET /?LED0") >= 0) {
              // Get LED RGB values from the weburl
              // Serial.println(header.substring(header.indexOf("/?LED0")));
//              int[] rgbVals = getRGBvals(header);
              
              String data = header.substring(header.indexOf("?LED0"),header.indexOf("+")+5);
              Serial.println(data);
              Serial1.println(data);
              delay(1100);

            }
            
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
