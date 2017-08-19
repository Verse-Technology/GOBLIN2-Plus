/*     BRIDGE_GOBLIN2PLUS.

  WITH THIS PROGRAM YOU CAN HAVE COMMUNICATION BETWEEN SIM5320A AND YOUR ARDUINO IDE.
  
  Check more documentation and examples in www.goblin2plus.io
 
  modified 4 August 2017.
  by Jorge Guillen, Antonio Reyes & Rafa Cortés.
  -GOBLIN 2 Plus TEAM. VERSE Technology.
*/

void setup() {
  SerialUSB.begin(115200);              // DEFAULT BAUDS 115200 of SIM5320A
  Serial.begin(115200);                 // SERIAL - SIM
  while (!SerialUSB);
}

void loop() {
  
  if (Serial.available()) {   
      String toSend = ""; 
      while (Serial.available())  {   
      toSend += (char)Serial.read();
                                      }
      SerialUSB.print(toSend);
                                    }  

  if (SerialUSB.available()) {   
      String toSend = ""; 
      while (SerialUSB.available())  {   
      toSend += (char)SerialUSB.read();
                                      }
      Serial.print(toSend);
  }
}
