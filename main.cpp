#include "mbed.h"
#include "SPI.h"

SPI spi(p11,p12,p13);
DigitalOut ss1(p14);
DigitalOut ss2(p15);
Timer timer;

/*
const int segConvert(int digit){
    switch(digit){
        case 0: return 0b11101110;
        case 1: return 0b00111110;
        case 2: return 0b10011100;
        //case 4: return 0b00011100;
    }
}
*/

const int letterArray[] = {0b11101110, 0b00111110, 0b10011100};

int inputConvert(int digit){
    int A = (digit >> 0) & 1;
    int B = (digit >> 1) & 1;
    int C = (digit >> 2) & 1;
    if (A == 1){
        return 0;
    }
    if (B == 1){
        return 1;
    }
    if (C == 1){
        return 2;
    }
    else {
        return -1;
    }

}

int main() {
  spi.frequency(100000);
  /* this block of code is used to troubleshoot the button inputs pls ignore
  while(1){
    spi.format(8,2);
    ss2 = 1;
    int yo = spi.write(0);
    ss2 = 0;

    spi.format(8,0);
    ss1 = 0;
    spi.write(letterArray[inputConvert(yo)]);
    ss1 = 1;
    wait(100);
  }*/
  
  while(1) {
      srand(timer.read_us());
      int number = rand() % 3;
      
      spi.format(8,0);
      ss1 = 0;
      spi.write(letterArray[number]);
      ss1 = 1;

      float timeLimit = 1.0;
      timer.start();
      while(timer.read() < timeLimit){
        int input = -1;
        int translatedInput = -1;
          
        spi.format(8,2);
        ss2 = 1;
        input = spi.write(0);
        ss2 = 0;
        translatedInput = inputConvert(input);

        

        if (input >= 0 && translatedInput >= 0){
          if(translatedInput != number){
            timer.stop();

            spi.format(8,0);
            ss1 = 0;
            spi.write(0b00011100);
            ss1 = 1;
            break;
          }
          else{
            spi.format(8,0);
            srand(timer.read_us());
            number = rand() % 3;
            ss1 = 0;
            spi.write(0);
            ss1 = 1;
            wait_ms(25);
            ss1 = 0;
            spi.write(letterArray[number]);
            ss1 = 1;

            wait_ms(100);

           
            timer.reset();
            
            timeLimit = 0.95 * timeLimit;
          }
        }
        
      }

    break;
  }
  timer.stop();

  spi.format(8,0);
  ss1 = 0;
  spi.write(0b00011100);
  ss1 = 1;
}
