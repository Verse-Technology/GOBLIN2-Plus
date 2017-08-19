/*
  Using_the_SD_Card_with_GOBLIN2_PLUS.
  SD_PROGRAM1_READ_STATE.
  
  This program read the state of the MICRO SD of our "GOBLIN 2 PLUS"
  and print in the serial monitor.

  Check more documentation and examples in www.goblin2plus.io
 
  modified 14 August 2017.
  by Jorge Guillen, Antonio Reyes & Rafa Cortés.
  -GOBLIN 2 Plus TEAM. VERSE Technology.
*/

#include <SPI.h>
#include <SD.h>

Sd2Card card;
SdVolume volume;
SdFile root;

const int chipSelect = 4;

void setup() {
  SerialUSB.begin(9600);
   while (!SerialUSB){
   ;
 }
 
 SerialUSB.println("Initializing SD card...");

 if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    SerialUSB.println("initialization failed. Things to check:");
    SerialUSB.println("* is a card inserted?");
    SerialUSB.println("* is your wiring correct?");
    SerialUSB.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
    SerialUSB.println("Wiring is correct and a card is present.");
  }

  // print the type of card
  SerialUSB.print("\nCard type: ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      SerialUSB.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      SerialUSB.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      SerialUSB.println("SDHC");
      break;
    default:
      SerialUSB.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    SerialUSB.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }


  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  SerialUSB.print("\nVolume type is FAT");
  SerialUSB.println(volume.fatType(), DEC);
  SerialUSB.println();

  volumesize = volume.blocksPerCluster();                          // clusters are collections of blocks
  volumesize *= volume.clusterCount();                             // we'll have a lot of clusters
  volumesize *= 512;                                               // SD card blocks are always 512 bytes
  SerialUSB.print("Volume size (bytes): ");
  SerialUSB.println(volumesize);
  SerialUSB.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  SerialUSB.println(volumesize);
  SerialUSB.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  SerialUSB.println(volumesize);
  SerialUSB.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
 
}

void loop() { 
}

