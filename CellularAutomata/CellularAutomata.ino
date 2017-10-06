/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2016 RuntimeProjects.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

//set to 1 if the previous screen was the same as the current one
int sameScreen;

//Current grid
uint8_t grid[20][12];

//The new grid is used so that 'grid' is not compromised
uint8_t newgrid[20][12];


void setup()   {                

  // Serial.begin(9600); //use only for debugging purposes

  //Setting up the display pay attention to the address. Yours might be different
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
 
  sameScreen=0;
}

void loop() {
        
    //Displaying a simple splash screen    

	display.clearDisplay();
	display.setTextSize(2);
	display.setTextColor(WHITE);
	display.setCursor(25,5);
	display.println(F("Arduino"));
	display.setCursor(20,25);
	display.println(F("Cellular"));        
	display.setCursor(20,45);
	display.println(F("Automata"));
	display.display();
	delay(2500);

	//Initialize CA
	display.setTextSize(2);
	display.clearDisplay();
	initGrid();
	drawGrid();
	delay(2000);
	
	//Compute 100 generations (or cycles) or less if screen remains static
	for (int gen=0;gen<100;gen++)
	{

	   computeCA();
		if (sameScreen==1){
		  gen=100;
		  sameScreen=0;
		}
	   drawGrid();
	   delay(250);
	}
   
}

//Draws the grid on the display
void drawGrid(void) {

  for (int16_t x=1; x<19;x++) {
    // alternate colors
    
    for (int16_t y=1; y<11;y++) {
      // alternate colors
        uint8_t color = grid[x][y];
        display.fillRect(6*x, 6*y, 6, 6, color);
       // Serial.print(color);
    }
    //Serial.println('\n');
  }
  display.display();
}

//Initialize Grid
void initGrid(void){
  for (int16_t x=0; x<20;x++) {
    // alternate colors
    
    for (int16_t y=0; y<12;y++) {
      // alternate colors
        uint8_t color = random(2);
        if (x==0 || x==19 || y==0 || y==11){
          grid[x][y]=0;
        }
        else{
          if (color==1)
            grid[x][y]=1;
          else 
            grid[x][y]=0;
        }
    }
    
  }
}

//Compute the CA. Basically everything related to CA starts here
void computeCA(){
  for (int16_t x=1; x<19;x++) {
    // alternate colors
    
    for (int16_t y=1; y<11;y++) {
      // alternate colors
        int neighbors = getNumberOfNeighbors(x,y);
        if (grid[x][y]==1 && (neighbors == 2 || neighbors == 3 ))
        {
          newgrid[x][y]=1;
        }
        else if (grid[x][y]==1)  newgrid[x][y]=0;
        if (grid[x][y]==0 && (neighbors==3))
        {
          newgrid[x][y]=1;
        }
        else if (grid[x][y]==0) newgrid[x][y]=0;
    }
    
  }

  if (isSameScreen()) sameScreen=1;
  
  for (int16_t x=1; x<19;x++) {
    // alternate colors
    
    for (int16_t y=1; y<11;y++) {
      grid[x][y]=newgrid[x][y];
    }
  }
}

//check the Moore neighborhood
int getNumberOfNeighbors(int x, int y){
 // Serial.println(grid[x-1][y]+grid[x-1][y-1]+grid[x][y-1]+grid[x+1][y-1]+grid[x+1][y]+grid[x+1][y+1]+grid[x][y+1]+grid[x-1][y+1]);
  return grid[x-1][y]+grid[x-1][y-1]+grid[x][y-1]+grid[x+1][y-1]+grid[x+1][y]+grid[x+1][y+1]+grid[x][y+1]+grid[x-1][y+1];
}

//Checks if the new screen is exactly like the previous one
bool isSameScreen(){
  for (int16_t x=1; x<20;x++) {
    // alternate colors
    
    for (int16_t y=1; y<11;y++) {
      if(grid[x][y]!=newgrid[x][y]) return false;
    }
  }
  return true;
}


