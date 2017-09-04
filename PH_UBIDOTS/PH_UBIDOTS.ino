/*
  PH_UBIDOTS.
  
  Receive the information of the pH sensor and then upload the data to the platform UBIDOTS.
   
  Check more documentation and examples in www.goblin2plus.io
 
  modified 31 August 2017.
  by Jorge Guillen, Antonio Reyes & Rafa Cortés.
  -GOBLIN 2 Plus TEAM. VERSE Technology.
*/
                          
char    BUFFER_USART;                                                        // BUFFER RX   
String  token ="xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";                             // TOKEN UBIDOTS                                      
String  id    ="xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";                             // ID VARIABLE UBIDOTS                                     

const int analogInPin = A5;                                                  // ADC SENSOR PH
int sensorValue = 0; 
unsigned long int avgValue; 

float b;
int buf[10],temp;

float pHVol;
float phValue;

void setup() { 
  SerialUSB.begin(115200);        // (SERIAL - SIM),  (SERIAL1 -D0-D1)
  Serial.begin(115200);           // Initialize hardware serial port, pins 0/1
  pinMode(5, OUTPUT);             // PIN ON SIM5320
  SIM5320_ON();                   // ON SIM5320
  delay(10000);
  SETUP_SIM5320();                // START SIM5320                             
}
  

void loop() {
  ph();
  WEB();                           // SEND JSON UBIDOTS
  delay(8000);                     // SAMPLE TIME
  }
  
void IMP(){

 if (Serial.available()) {   
      String toSend = ""; 
      while (Serial.available())  {   
      toSend += (char)Serial.read();
                                  }
      SerialUSB.print(toSend);
                                  } 
}

void WEB(){
  int LONG_JSON = 15;
  int SEND = 209;
    
  Serial.println("AT+CIPOPEN=0,\"TCP\",\"things.ubidots.com\",80");  
  delay(5000);
  IMP();
  
  Serial.print("AT+CIPSEND=0,");  
  delay(50);
  IMP();
  Serial.println(SEND);  
  delay(600);
  IMP();
  Serial.print("POST /api/v1.6/variables/");
  delay(50);
  IMP();
  Serial.print(id);
  delay(50);
  Serial.println("/values  HTTP/1.1");
  delay(50);
  IMP();
  Serial.println("Content-Type: application/json");
  delay(50);
  IMP();
  Serial.print("Content-Length:");
  delay(50);
  IMP();
  Serial.println(LONG_JSON);
  delay(50);
  IMP();
  Serial.print("X-Auth-Token: ");
  delay(50);
  IMP();
  Serial.println(token);
  delay(50);
  IMP();
  Serial.println("Host: things.ubidots.com");
  delay(50);
  IMP();
  Serial.println("");
  IMP();
  Serial.print("{\"value\":");
  delay(10);
  IMP();
  Serial.print(phValue);                             // 22.50 it's static value (you can change)  
  delay(50);
  IMP();
  Serial.println("}");
  delay(50);
  IMP();
  Serial.write(0x1A);
  delay(12000);
  IMP();
  Serial.println("AT+CIPCLOSE=0");
  delay(4000);
  IMP();

}

void SIM5320_ON()          
{
  digitalWrite(5, HIGH);
  delay(2000);                            
  digitalWrite(5, LOW);
  delay(2000);
}

void SETUP_SIM5320()
{
  Serial.println("AT+CFUN=1");                      // SIM5320 WORKING WITH ALL THE FEATURES
  delay(250);
  IMP();
  Serial.println("AT+CNMP=2");                      // AUTOMATIC MODE
  delay(250);
  IMP();

  Serial.println("AT+CNBP=0x0000000004000000");     // 3G 850MHZ TELCEL 0x0000000004000000 hspda     2G 1900MHZ TELCEL AT+CNBP=0x0000000000200000 edge
  delay(500);
  IMP();
  Serial.println("AT+CSQ");                         // SIGNAL LEVEL
  delay(250);
  IMP();
  Serial.println("AT+CREG?");          
  delay(250);
  IMP();
  Serial.println("AT+COPS?");
  delay(250);
  IMP();
  Serial.println("AT+CGSOCKCONT=1,\"IP\",\"internet.itelcel.com\"");
  delay(250);
  IMP();
  Serial.println("AT+CNSMOD?");                     // RED
  delay(250);
  IMP();
  Serial.println("AT+CSOCKSETPN=1");
  delay(250);
  IMP();
  Serial.println("AT+CIPMODE=0");
  delay(250);
  IMP();
  Serial.println("AT+CNBP?");
  delay(250);
  IMP();
  Serial.println("AT+NETOPEN");
  delay(8000);
  IMP();
  Serial.println("AT+IPADDR");                       // IP
  delay(500);
  IMP();
  Serial.println("AT+CGSN");                         // IMEI
  delay(500);
  IMP();
}

void ph() {
for(int i=0;i<10;i++) 
 { 
  buf[i]=analogRead(analogInPin);
  delay(10);
 }
 for(int i=0;i<9;i++)
 {
  for(int j=i+1;j<10;j++)
  {
   if(buf[i]>buf[j])
   {
    temp=buf[i];
    buf[i]=buf[j];
    buf[j]=temp;
   }
  }
 }
 avgValue=0;
 for(int i=2;i<8;i++)
 avgValue+=buf[i];
 
 pHVol=(float)avgValue*5.0/1024/6;
 phValue = -5.70 * pHVol + 30.34;

 
// Serial.print("sensor = ");
// Serial.println(phValue);
 delay(20);
}
