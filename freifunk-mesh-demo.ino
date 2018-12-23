#include <Adafruit_NeoPixel.h>

#define PIN 2
#define COUNT 100
Adafruit_NeoPixel strip = Adafruit_NeoPixel(COUNT, PIN, NEO_GRB + NEO_KHZ800);
boolean isPath[9]; boolean nextPath[9];
uint16_t pathMetric[] = {0,0,0,0,0,0,0,0,0};
boolean segmentReverse[] = {0,0,0,1,0,1,1,0,9};
uint8_t segmentStart[] = {0,8,24,32,46,60,68,84,92};
uint8_t segmentStop[] = {7,23,31,45,59,67,83,91,99};
uint8_t pkg_size = 0;
uint8_t pkg_length = 5;
uint8_t pkg_current = 0;
boolean pkg_running = false;

uint32_t pathColor, pkgColor, pkgColor2, npathColor;
void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.show();
  strip.setBrightness(50);
  pathColor = strip.Color(0, 0, 255);
   npathColor = strip.Color(0, 127, 127);
  pkgColor = strip.Color(0, 255, 0);
  pkgColor2 = strip.Color(0, 127, 0);
  initPath();
  Serial.begin(9600);
}
void printMetric() {
  Serial.print(" 0: ");
  Serial.print(pathMetric[0]);
  Serial.print(" 1: ");
  Serial.print(pathMetric[1]);  
  Serial.print(" 2: ");
  Serial.print(pathMetric[2]);
    Serial.print(" 3: ");
  Serial.print(pathMetric[3]);
    Serial.print(" 4: ");
  Serial.print(pathMetric[4]);
    Serial.print(" 5: ");
  Serial.print(pathMetric[5]);
    Serial.print(" 6: ");
  Serial.print(pathMetric[6]);
    Serial.print(" 7: ");
  Serial.print(pathMetric[7]);
    Serial.print(" 8: ");
  Serial.print(pathMetric[8]);
  Serial.println();
}
void calculatePkg() {
  pkg_size=0;
  for(int i=0;i<9;i++) {
    if(isPath[i]==true) {
      pkg_size+=segmentStop[i]-segmentStart[i]+1;
    }
  }
  pkg_running = true;
  pkg_current=0;
  Serial.println("Package led size: ");
  Serial.println(pkg_size);
}
void readPathMetric() {
  pathMetric[1]=analogRead(A0);
  pathMetric[2]=analogRead(A1);
  pathMetric[3]=analogRead(A2);
  pathMetric[8]=analogRead(A3);
  pathMetric[4]=analogRead(A5);
  pathMetric[5]=analogRead(A6);
  pathMetric[6]=analogRead(A7);
  pathMetric[7]=analogRead(A4);
  printMetric();
}
void initPath() {
  isPath[0]=true;
  isPath[1]=false;
  isPath[2]=false;
  isPath[3]=false;
  isPath[4]=false;
  isPath[5]=false;
  isPath[6]=false;
  isPath[7]=false;
  isPath[8]=false;
  readPathMetric();
  int minMetric = pathMetric[1]+pathMetric[2]+pathMetric[8];
  char path = 'a';
  if((pathMetric[3]+pathMetric[8])<minMetric) {
    minMetric=pathMetric[3]+pathMetric[8];
    path='b';
  }
  if((pathMetric[5]+pathMetric[6]+pathMetric[7])<minMetric) {
    minMetric=pathMetric[5]+pathMetric[6]+pathMetric[7];
    path='c';
  }
  if((pathMetric[4]+pathMetric[7])<minMetric) {
    minMetric=pathMetric[4]+pathMetric[7];
    path='d';
  }
  Serial.print("Choosen path ");
  Serial.print(path);
  Serial.print(" with metric ");
  Serial.println(minMetric);
  switch(path) {
    case 'a':
        isPath[1]=true; isPath[2]=true; isPath[8]=true;
    break;
    case 'b':
        isPath[3]=true; isPath[8]=true;
    break;
    case 'c':
        isPath[5]=true; isPath[6]=true; isPath[7]=true;
    break;
    case 'd': 
        isPath[4]=true; isPath[7]=true;
    break;
  }
}
void initNextPath() {
  nextPath[0]=true;
  nextPath[1]=false;
  nextPath[2]=false;
  nextPath[3]=false;
  nextPath[4]=false;
  nextPath[5]=false;
  nextPath[6]=false;
  nextPath[7]=false;
  nextPath[8]=false;
  readPathMetric();
  int minMetric = pathMetric[1]+pathMetric[2]+pathMetric[8];
  char path = 'a';
  if((pathMetric[3]+pathMetric[8])<minMetric) {
    minMetric=pathMetric[3]+pathMetric[8];
    path='b';
  }
  if((pathMetric[5]+pathMetric[6]+pathMetric[7])<minMetric) {
    minMetric=pathMetric[5]+pathMetric[6]+pathMetric[7];
    path='c';
  }
  if((pathMetric[4]+pathMetric[7])<minMetric) {
    minMetric=pathMetric[4]+pathMetric[7];
    path='d';
  }
  Serial.print("Choosen path ");
  Serial.print(path);
  Serial.print(" with metric ");
  Serial.println(minMetric);
  switch(path) {
    case 'a':
        nextPath[1]=true; nextPath[2]=true; nextPath[8]=true;
    break;
    case 'b':
        nextPath[3]=true; nextPath[8]=true;
    break;
    case 'c':
        nextPath[5]=true; nextPath[6]=true; nextPath[7]=true;
        Serial.print("5: ");
        Serial.print(pathMetric[5]);
        Serial.print(" 6: ");
        Serial.print(pathMetric[6]);
        Serial.print(" 7: ");
        Serial.print(pathMetric[7]);
        Serial.println();
                
    break;
    case 'd': 
        nextPath[4]=true; nextPath[7]=true;
    break;
  }
}
void displayPath() {
  for(int i=0;i<9;i++) {
    if(isPath[i]==true) {
      for(int l=segmentStart[i]; l<=segmentStop[i]; l++) {
        strip.setPixelColor(l,pathColor);
      }
    } else if (nextPath[i]==true) {
      for(int l=segmentStart[i]; l<=segmentStop[i]; l++) {
        strip.setPixelColor(l,npathColor);
      }
    } else {
      for(int l=segmentStart[i]; l<=segmentStop[i]; l++) {
        strip.setPixelColor(l,0);
      }
    }
  }
}
void setPathPix(int p) {
  if(p>32) {
    p=p-32+segmentStart[8]-1;
  }
  strip.setPixelColor(p,pkgColor);
}
void displayPkg() {
  if(pkg_running==true) {
    setPathPix(pkg_current);
    setPathPix(pkg_current-1);
    setPathPix(pkg_current-2);
    setPathPix(pkg_current-3);
    setPathPix(pkg_current-4);
    pkg_current++;
    if(pkg_current-5>pkg_size)  {
      pkg_running=false;
    }
  }
}
void loop() {
  if(pkg_running==false) {
    initPath();
    calculatePkg();
  }
  initNextPath();
  

  displayPath();
  
  //displayPkg();
  strip.show();
  delay(25);
}
