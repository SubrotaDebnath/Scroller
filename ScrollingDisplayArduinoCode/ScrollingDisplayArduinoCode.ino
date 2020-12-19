/*Author: Subrota Debnath
 * +8801854397708
 * subrota.shuvro@gmail.com
 */

#include <SPI.h>        //Include all libraries
#include <DMD.h>        
#include <TimerOne.h>   //Timer
#include "SystemFont5x7.h"
#include "Arial_black_16.h"//front_liblary
#include <EEPROM.h> 

#define DISPLAYS_ACROSS 4  //Numbers of DMD-board(16x32) are using
#define DISPLAYS_DOWN 1   //DMD up or down

DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);//Display starting position

char r_char;               
byte index = 0;           
long int i; 
long int max_char=260;        
char greeting[] = "WELCOME TO ECRC-IST"; //Print message 
char reciveByte; 
int count = 0; 
char message[1024];
boolean hold = true;
boolean isClon=false;
char speed ;

void ScanDMD(){ 
     dmd.scanDisplayBySPI();  // Scan From  Bluetooth  
 }

void setup(void){
    Timer1.initialize(2000); 
    Timer1.attachInterrupt( ScanDMD ); // Bluetooth scanner function called
    dmd.clearScreen( true );//to clear RAM 
    Serial.begin(9600);//begin serial communication
//    strcpy(message,greeting);//  copying greeting to message
    //max_char = int(EEPROM.read(1010));
     

    max_char = pullMaxChar();
    //max_char = readIntFromEEPROM(1010);.......................................
    getFromEEPROMForStartup(max_char);
  }
  
void loop(void){
//  Serial.println(message);// Console  print
  if(Serial.available()){
    Timer1.stop();
    delay(5);       
      while(Serial.available() > 0){
        reciveByte = Serial.read();
        Serial.print(reciveByte);
        if(reciveByte=='<'){
          count=0;
          hold=false;
        }else if(reciveByte=='>'){
          hold=true;
        }
        if(!hold){
          if(reciveByte!='<'){
             if(reciveByte!='>'){
              if(reciveByte!=';' && !isClon){
                message[count]=reciveByte;
                count++;
                isClon=false;
              }else{
                isClon=true;
                EEPROM.write(1020, reciveByte);
                speed = reciveByte;
                //Serial.println(speed);
              }
            }
           }
        }else{
//          max_char = count;
          Serial.println("");
          Serial.print("Count: ");
          Serial.println(count);
          saveInEEPROM(count);
          count = 0;
          hold=true;
          isClon=false;
          Timer1.initialize(1000); 
          Timer1.attachInterrupt( ScanDMD ); // Bluetooth scanner function called
        }
    }
   }else if(hold){
     dmd.selectFont(Arial_Black_16);//Font 
    // Serial.println("");
     //Serial.print("Display Max_char: ");
     //Serial.print(max_char) ;
     dmd.drawMarquee(message ,max_char,(32*DISPLAYS_ACROSS)-1 ,0);// Matrix Size
     //dmd.goBabyGo(message ,max_char,(32*DISPLAYS_ACROSS)-1 ,0);
     
     long start=millis();
     long timer = start;
     boolean ret=false;
     while(!ret)
     {
      if ((timer+((speed-48)*10)) < millis()) {
          ret=dmd.stepMarquee(-1,0);
          timer=millis();
         }
     }
   }
}

void saveInEEPROM(int c){
  clearEEPROM();
  delay(5);
  //EEPROM.write(1010,c);
  insertMaxChar(c);
  //writeIntIntoEEPROM(1010, c);
  Serial.println("");
  Serial.println("Print EEPROM Write: ");
  for(int i=0;i<c;i++){
    EEPROM.write(i,message[i]);
    Serial.print(char(EEPROM.read(i)));
  }
  getFromEEPROMForStartup(c);
}

void getFromEEPROMForStartup(int c){
  Serial.println("");
  Serial.println("Print EEPROM Message: ");
   for (int i = 0 ; i < c ; i++) {
    message[i] = char(EEPROM.read(i));
    Serial.print(message[i]) ;
   }
   Serial.println("");
   Serial.print("Print Speed: ");
   Serial.print(char(EEPROM.read(1020))) ;

   speed = char(EEPROM.read(1020));
   Serial.println("");
   Serial.print("Print Speed: ");
   Serial.print(speed) ;
   max_char = pullMaxChar();
    //max_char = readIntFromEEPROM(1010);..................................
   Serial.println("");
   Serial.print("Max_char: ");
   Serial.print(max_char) ;
   //max_char = int(EEPROM.read(1010));
}

void clearEEPROM(){
   for (int i = 0 ; i < 900 ; i++) {
    EEPROM.write(i, 0);
  }
}

void insertMaxChar(int mc){
  int division = mc/255;
  Serial.println("");
   Serial.print("division: ");
   Serial.print(division) ;
  int reminder = mc%255;
  Serial.println("");
   Serial.print("reminder: ");
   Serial.print(reminder) ;

   EEPROM.write(1010, division);
   EEPROM.write(1015, reminder);
  
}

int pullMaxChar(){
  int division=int(EEPROM.read(1010));
  int reminder=int(EEPROM.read(1015));
  
  int mc = (division*255)+ reminder;
   Serial.println("");
   Serial.print("pullMaxChar: ");
   Serial.print(mc) ;

  return mc;
}

//void writeIntIntoEEPROM(int address, int number)
//{ 
//  EEPROM.write(address, number >> 8);
//  EEPROM.write(address + 1, number & 0xFF);
//}
//
//int readIntFromEEPROM(int address)
//{
//  byte byte1 = EEPROM.read(address);
//  byte byte2 = EEPROM.read(address + 1);
//  return (byte1 << 8) + byte2;
//}



//
//void getFromEEPROM(){
//  Serial.println("");
//  Serial.println("Print EEPROM Message: ");
//   for (int i = 0 ; i < count ; i++) {
//    Serial.print(char(EEPROM.read(i))) ;
//    message[i] = char(EEPROM.read(i));
//   }
//   Serial.println("");
//   Serial.print("Print Speed: ");
//   Serial.print(char(EEPROM.read(1020))) ;
//   max_char = int(EEPROM.read(1010));
//}
//
