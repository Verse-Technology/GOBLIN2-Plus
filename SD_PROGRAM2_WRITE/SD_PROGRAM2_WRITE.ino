/*
  Using_the_SD_Card_with_GOBLIN2_PLUS.
  SD_PROGRAM2_WRITE.
  This program save one text file with the phrase "GOBLIN 2 PLUS".

  Check more documentation and examples in www.goblin2plus.io
 
  modified 14 August 2017.
  by Jorge Guillen, Antonio Reyes & Rafa Cortés.
  -GOBLIN 2 Plus TEAM. VERSE Technology.
*/
             
#include <SPI.h>
#include <SD.h>

const int pinCs = 4; 
File Archivo;

void setup(){ 
SerialUSB.begin(9600);
 while (!SerialUSB){
   ;
 }
   
SerialUSB.println("Example of communication with SD"); 
pinMode(pinCs, OUTPUT);            

if(!SD.begin(pinCs)){                                                     // If you can't communicate print error
SerialUSB.println("Error with the communication with SD"); 
return;
}

SerialUSB.println("Correct communication");                                                                          
Archivo = SD.open("ARCHIVO.txt", FILE_WRITE);                             // Select and open the file
                                                                         
if(!Archivo){
SerialUSB.println("Error opening file for writing");                      // If the file can't be opened
return;
}
                                                                         
Archivo.println("GOBLIN 2 PLUS");                                         // Write in the file
Archivo.close();                                                          // Close and save the data in the file 
Archivo = SD.open("Archivo.txt");                                         // Select and open the file
        
if(!Archivo){
SerialUSB.println("Error opening file for reading");                      // If the file can't be opened 
return;
}

while(Archivo.available()){ 
  SerialUSB.write(Archivo.read());
}
Archivo.close();
}

void loop()
{

}
