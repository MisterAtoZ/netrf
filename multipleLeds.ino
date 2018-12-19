#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "DriePlusVeertien";
const char* password = "achttien";

ESP8266WebServer server(80);

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN1 0 //D3
#define PIN2 2 //D4

int j=0;
int i=0;
int q=0;
int k=0;
boolean theateroff = false;
int led1 = 2;
int led2 = 2;

long sinTab[256];
//uint8_t rgbWave[] = {0, 0, 0};
byte rgb1[] = {50, 0, 0};
byte rgb2[] = {0, 50, 0};
int t,w,start;
long s;
int i2=0;

// Parameter 1 = number of pixels in strip11
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW product
//eerste ledstrip1 (met 5 kabels)
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(72, PIN1, NEO_GRB + NEO_KHZ800);
//tweede ledstrip1 (drie kabels)
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(72, PIN2, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

int getArgValue(String name)
{
  for (uint8_t i = 0; i < server.args(); i++)
    if(server.argName(i) == name)
      return server.arg(i).toInt();
  return -1;
}

String getArgValueString(String name)
{
  for (uint8_t i = 0; i < server.args(); i++)
    if(server.argName(i) == name)
      Serial.println("de getargvalue van de kleur is: "+ (String)server.arg(i));
      Serial.println(server.arg(i));
      return server.arg(i);
  Serial.println("getargs is zwart(geen waarde)");
  return "#000000";
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void handleRoot() {
  String message ="<!DOCTYPE html> <html> <head> <style> body{ width: 100%; margin: auto; min-width: 600px; max-width: 1600px; } #header { background-color: #D94F20; height: 100px; line-height: 100px; } #header h1 { color: white; text-decoration: none; text-transform: uppercase; letter-spacing: 0.1em; } #title { display: block; float: left; font-size: 20px; font-weight: bold; } .content { font-family: sans-serif; font-size: 12px; } .container { width: 80%; margin: 0 auto; max-width: 1300px; min-width: 600px; } .button{ width: 100px; } .button:hover{ background-color: #aaaaaa; } #ledstrip1{ display: block; float:left; } #ledstrip2{ display: block; float:left; margin-top: 0; margin-left: 100px; } </style> </head> <body> <div id='header'> <div class='container'> <h1 id='title'>Ledstrip aansturen</h1> </div><!--container--> </div><!--header--> <div class='content'> <div class='container'><br> <h2>Click on one of the buttons:</h2> <div class='Ledstrip' id='ledstrip1'> <h3 id = title1>Ledstrip 1</h3> <p>Stel de kleur in:</p> <input type='color' id='setColor1' value='#000000'> <p>kies een functie:</p> <a href='/rainbow?s=1'><button class='button' type='button' name='rainbow'>Rainbow</button></a><br> <a class= 'colorDepending1' href='/theaterchase?s=1&c=000000' ><button class='button' type='button' name='theaterchase'>theaterchase</button></a> <br> <a href='/ledsoff?s=1'><button class='button' type='button' name='off'>off</button></a><br> <a class= 'colorDepending1' href='/setcolor?s=1&c=000000'><button class='button' type='button' name='setleds'>set color</button></a><br> </div><!--Ledstrip--> <div class='Ledstrip' id='ledstrip2'> <h3 id=title2>Ledstrip 2</h3> <p>Stel de kleur in:</p> <input type='color' id='setColor2' value='#000000'> <p>kies een functie:</p> <a href='/rainbow?s=2'><button class='button' type='button' name='rainbow'>Rainbow</button></a><br> <a class= colorDepending2 href='/theaterchase?s=2&c=000000' ><button class='button' type='button' name='theaterchase'>theaterchase</button></a> <br> <a href='/ledsoff?s=2'><button class='button' type='button' name='off'>off</button></a><br> <a class= colorDepending2 href='/setcolor?s=2&c=000000'><button class='button' type='button' name='setleds'>set color</button></a><br> </div><!--Ledstrip--> </div><!--container--> </div><!--content--> <script type='text/javascript'> var setColor1; links1 = document.getElementsByClassName('colorDepending1'); setColor1 = document.querySelector('#setColor1'); setColor1.addEventListener('input', updateFirst, false); function updateFirst(event){ var h3 = document.querySelector('#title1'); if (h3){ h3.style.color = event.target.value; for (var i=0; i<links1.length; i++){ var link1 = links1[i].getAttribute('href'); var splitLink1 = link1.split(link1.slice(link1.indexOf('c=')+2, link1.indexOf('c=')+10)); var knopFeedback1 = event.target.value.replace('#',''); var kleur1 = knopFeedback1; links1[i].setAttribute('href', splitLink1[0] + kleur1 + splitLink1[1]); } } } </script><!--setColor1--> <script type='text/javascript'> var setColor2; links2 = document.getElementsByClassName('colorDepending2'); setColor2 = document.querySelector('#setColor2'); setColor2.addEventListener('input', updateSecond, false); function updateSecond(event){ var h3 = document.querySelector('#title2'); if (h3){ h3.style.color = event.target.value; for (var i=0; i<links2.length; i++){ var link2 = links2[i].getAttribute('href'); var splitLink2 = link2.split(link2.slice(link2.indexOf('c=')+2, link2.indexOf('c=')+10)); var knopFeedback2 = event.target.value.replace('#',''); var kleur2 = knopFeedback2; links2[i].setAttribute('href', splitLink2[0] + kleur2 + splitLink2[1]); } } } </script><!--setColor1--> </body> </html>";
  server.send(200, "text/html", message);
}

void setRGB(int strip, String color) {
  Serial.println("de strip in setRGB is: "+(String) strip); 
  Serial.println("de kleur bij setRGB is: "+color);
  color = "#"+color; 
  int number = (int) strtol(&color[1], NULL, 16);
  int r = number >>16;
  Serial.print("de waarde van r is: "+(String)r);
  int g = number >> 8 & 0xFF;
  Serial.print("de waarde van g is: "+(String)g);
  int b = number & 0xFF;
  
  if(strip==1) {
    rgb1[0] = r;
    rgb1[1] = g;
    rgb1[2] = b;
  }
  if(strip==2) {
    rgb2[0] = r;
    Serial.print("dit is de waarde van rgb2[0]");
    Serial.println(rgb2[0]);
    rgb2[1] = g;
    Serial.print("dit is de waarde van rgb2[1]");
    Serial.println(rgb2[1]);
    rgb2[2] = b;
    Serial.print("dit is de waarde van rgb2[2]");
    Serial.println(rgb2[2]);
  }
}
void setRainbow(int strip) {
  if(strip==1) {led1=0;}
  else if(strip==2) {led2=0;}
  else {}
}

void setTheater(int strip) {
  if(strip==1) {led1=1;}
  else if(strip==2) {led2=1;}
  else {}
}

void setLedsoff(int strip) {
  if(strip==1) {led1=2;}
  else if(strip==2) {led2=2;}
  else {}
}

void setledsColor(int strip) {
  if(strip==1) {led1=3;}
  else if(strip==2) {led2=3;}
  else {}
}

void setup() {
  //initialise WIFI
   Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  //Setup for the loop
  for(i=0;i<strip1.numPixels();i++){
    sinTab[i] = sin(3.1415 / 128 * i) * 0x7fff + 0x8000;
  }
  strip1.begin();
  strip2.begin();
  strip1.show(); // Initialize all pixels to 'off'
  
  strip2.show(); // Initialize all pixels to 'off'  
  //setup for the wifi
  server.on("/", handleRoot);

  server.on("/rainbow", [](){  
    //checkFadeAndSetLedFunction(new RainbowFunction());
    //int fade = getArgValue("fade");
    int stripRainbow = getArgValue("s");
    Serial.println("de strip in de rainbow is: "+(String) stripRainbow);
    setRainbow(stripRainbow);
    //server.send(200, "text/html", handleroot);
  });

  server.on("/theaterchase", [](){
    int stripTheater = getArgValue("s");
    Serial.println("de strip in de server is: "+(String) stripTheater);
    String color = getArgValueString("c");
    Serial.println("de kleur in de server is: "+(String)color);
    setRGB(stripTheater, color);
    setTheater(stripTheater);
    handleRoot();
    //server.send terug op de originele site zetten
  });

  server.on("/ledsoff", [](){
    int stripledsoff = getArgValue("s");
    setLedsoff(stripledsoff);
    //server.send terug op de originele site zetten
  });

  server.on("/setcolor", [](){
    String color = getArgValueString("c");
    int stripSet = getArgValue("s");
    setRGB(stripSet, color);
    setledsColor(stripSet);
    //server.send terug op de originele site zetten
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  //ifs voor de rainbow
  if(j >= 256*5) {j=0; t = ((millis() - start) / 63) & 255; w = ((millis() - start) / 71) & 255; start = millis();}
  if(q >= 3) {q=0;}
  if(i2>= 5){i2=0; long newValue = sinTab[0]; memcpy(sinTab, &sinTab[1], sizeof(sinTab) );sinTab[71] = newValue;}//- sizeof(int)); sinTab[sizeof(sinTab)+1] = newValue;}
  if(i >= strip1.numPixels()) {
    i=0;
    if(led1!=1 and led2!=1){
      strip1.show();
      strip2.show();
      delay(50); //rainbowdelay
    } 
    j++;
    i2++;
  }

  //ifs voor de theaterchase
  
  if(k >= strip1.numPixels()) {
    k=0;
    if(led1==1 || led2==1){
      strip1.show();
      strip2.show();
      if(theateroff==true){q++;} 
      else{delay(50);}
      theateroff = !theateroff;
    }
  }
  
  //welke functie moet er worden uitgevoerd voor de 1e strip?
  switch(led1) {
    case 0: //rainbow
      //Kleuren uit functie halen en de kleur van de leds setten
      strip1.setPixelColor(i, rainbow(j,i));
     break;
    case 1: //theaterchase
      if(theateroff == false) {
        strip1.setPixelColor(k+q,rgb1[0],rgb1[1],rgb1[2]);
      }
      else {strip1.setPixelColor(k+q,0,0,0);}
    break;
    case 2: //ledsoff
      strip1.setPixelColor(i,0,0,0);
    break;
    case 3: //setcolor
      strip1.setPixelColor(i,rgb1[0],rgb1[1],rgb1[2]);
    break;
    /*case 4:
      //sinTab[i] = sin(3.1415 / 128 * i) * 0x7fff + 0x8000;
      //s = (sinTab[(i*3 + t) & 255] >> 8) * (sinTab[-(i*4 + w) & 255] >> 8);
      strip1.setPixelColor(i, sinTab[i]);
      //strip1.setPixelColor(i, (rgbWave[0] * s) >> 16, (rgbWave[1] * s) >> 16, (rgbWave[2] * s) >> 16);
    break;*/
    default:
      strip1.setPixelColor(i,0,0,0);
    break;
  }
  //=======================================================
  //welke functie moet er worden uitgevoerd voor de 2e strip?
  switch(led2) {
    case 0://rainbow
      //Kleuren uit functie halen en de kleur van de leds setten
      strip2.setPixelColor(i, rainbow(j,i));
     break;
    case 1://theaterchase
      if(theateroff == false) {
        strip2.setPixelColor(k+q,rgb2[0],rgb2[1],rgb2[2]);
      }
      else {strip2.setPixelColor(k+q,0,0,0);}
    break;
    case 2: //ledsoff
      strip2.setPixelColor(i,0,0,0);
    break;
    case 3: //SetColor
      strip2.setPixelColor(i,rgb2[0],rgb2[1],rgb2[2]);
    break;
    /*case 4:
      sinTab[i+i2] = sin(3.1415 / 128 * i) * 0x7fff + 0x8000;
      
      strip2.setPixelColor(i, sinTab[i]);
    break;*/
    default:
      strip2.setPixelColor(i,0,0,0);
    break;
  }
  //=======================================================
  k=k+3;
  i++;
}

int rainbow(int j, int i) {
  //uint16_t i, j;

  //for(j=0; j<256; j++) {
    //for(i=0; i<strip1.numPixels(); i++) {
    //return Wheel((i+j) & 255);
    return Wheel(((i * 256 / strip1.numPixels()) + j) & 255);
      //rainbowLijst = {i, Wheel((i+j) & 255)};
    //}
    //strip1.show();
    //delay(wait);
  //}
}


void loopledje() {
  uint16_t start = 300;
  for(uint16_t j=0; j<strip1.numPixels(); j++) {
    double k=0.0001;
    for(uint16_t i=start; i>j; i--) {
      //k=k+0.25;
      strip1.setPixelColor(i,125,0,125);
      strip1.show();
      delay(k);
      strip1.setPixelColor(i,0,0,0);
    }
    strip1.setPixelColor(j,0,125,125); 
    strip1.setPixelColor(start,125,125,0); 
    start = start-1;
    k=k+0.5;
  }
  strip1.Color(0,0,0);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip1.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip1.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip1.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
