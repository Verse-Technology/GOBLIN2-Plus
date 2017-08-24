/*
  CAMERA_GOBLIN2PLUS_V1
  This program is an example of motion detection with a camera using the Adafruit VC0706 library.
  When starting up, the GOBLIN 2 PLUS will find the camera, the SD card and if there is any movement, 
  if the camera, SD, and if the motion is detected the camera takes a picture and saves it to the SD card.  

  Check more documentation and examples in www.goblin2plus.io
 
 - Written by Limor Fried/Ladyada for Adafruit Industries. 
  modified 21 August 2017.
  by Jorge Guillen, Antonio Reyes & Rafa Cortés.
  -GOBLIN 2 Plus TEAM. VERSE Technology. 
*/

#include <Adafruit_VC0706.h>
#include <SPI.h>
#include <SD.h>
#define chipSelect 4

Adafruit_VC0706 cam = Adafruit_VC0706(&Serial1);                      //RX - D0 OF GOBLIN 2 PLUS WITH TX OF THE CAMERA ,TX - D1 OF GOBLIN 2 PLUS WITH RX

void setup() {
  SerialUSB.begin(9600);
  Serial1.begin(9600);  

  while (!SerialUSB){
  delay(10); 
   }

  pinMode(4, OUTPUT);                                                 // CS define how OUTPUT
  SerialUSB.println("VC0706 Camera test");
                                                     
  if (!SD.begin(chipSelect)) {                                       // See if the card is present and can be initialized:
    SerialUSB.println("Card failed, or not present");                // Don't do anything more:
    return;
  }  
   
  if (cam.begin(38400)) {                                            // Try to locate the camera
    SerialUSB.println("Camera Found:");
  } else {
    SerialUSB.println("No camera found?");
    return;
  }

  char *reply = cam.getVersion();                                    // Print out the camera version information (optional)
  if (reply == 0) {
    SerialUSB.print("Failed to get version");
  } else {
    SerialUSB.println("-----------------");
    SerialUSB.print(reply);
    SerialUSB.println("-----------------");
  }

  // Set the picture size - you can choose one of 640x480, 320x240 or 160x120 
  // Remember that bigger pictures take longer to transmit!
  
   cam.setImageSize(VC0706_640x480);                                 // biggest
  //cam.setImageSize(VC0706_320x240);                                // medium
  //cam.setImageSize(VC0706_160x120);                                // small

                                                           
  uint8_t imgsize = cam.getImageSize();                               // You can read the size back from the camera (optional, but maybe useful?)
  SerialUSB.print("Image size: ");
  if (imgsize == VC0706_640x480) SerialUSB.println("640x480");
  if (imgsize == VC0706_320x240) SerialUSB.println("320x240");
  if (imgsize == VC0706_160x120) SerialUSB.println("160x120");


  //  Motion detection system can alert you when the camera 'sees' motion!
  cam.setMotionDetect(true);           // turn it on
  //cam.setMotionDetect(false);        // turn it off   (default)

  // You can also verify whether motion detection is active!
  SerialUSB.print("Motion detection is ");
  if (cam.getMotionDetect()) 
    SerialUSB.println("ON");
  else 
    SerialUSB.println("OFF");
}

void loop() {
  if (cam.motionDetected()) {
   SerialUSB.println("Motion!");   
   cam.setMotionDetect(false);
   
  if (! cam.takePicture()) 
    SerialUSB.println("Failed to snap!");
  else 
    SerialUSB.println("Picture taken!");
  
  char filename[13];
  strcpy(filename, "IMAGE00.JPG");
  for (int i = 0; i < 100; i++) {
    filename[5] = '0' + i/10;
    filename[6] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }
  
  File imgFile = SD.open(filename, FILE_WRITE);
  
  uint16_t jpglen = cam.frameLength();
  SerialUSB.print(jpglen, DEC);
  SerialUSB.println(" byte image");
 
  SerialUSB.print("Writing image to "); 
  SerialUSB.print(filename);
  
  while (jpglen > 0) {
    // read 32 bytes at a time;
    uint8_t *buffer;
    uint8_t bytesToRead = min(32, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
    buffer = cam.readPicture(bytesToRead);
    imgFile.write(buffer, bytesToRead);

    //Serial.print("Read ");  Serial.print(bytesToRead, DEC); Serial.println(" bytes");

    jpglen -= bytesToRead;
  }
  imgFile.close();
  SerialUSB.println("...Done!");
  cam.resumeVideo();
  cam.setMotionDetect(true);
 }
}

