#include <SoftwareSerial.h>

#define ARDUINO_INTERRUPT_0 2
#define ARDUINO_INTERRUPT_1 3

#define SIGFOX_RX 5
#define SIGFOX_TX

#define LED_PIN 13

SoftwareSerial sigfox(SIGFOX_RX,SIGFOX_RX);

long bufferVal = 0;
int bitsRead = 0;
String tagID;


void setup()
{
     Serial.begin(9600);
     sigfox.begin(9600);
     
     pinMode(LED_PIN, OUTPUT);
     digitalWrite(LED_PIN, LOW);
     
    initInterrupts();
}

void loop() {
     if(bitsRead>=26){
       
              digitalWrite(LED_PIN, HIGH);
          int serialNumber=(bufferVal >> 1) & 0x3fff;
          int siteCode= (bufferVal >> 17) & 0x3ff;

          Serial.print(" Reader 1:");
          Serial.print(bufferVal,HEX);
          Serial.print(" ");
          Serial.print("Reader 1 converted to DEC:");
          Serial.print(bufferVal& 0xfffffff);
          Serial.print(" ");
          Serial.print("siteCode:");
          Serial.print(siteCode);
          Serial.print(" ");
          Serial.print("serial number:");
          Serial.println(serialNumber);
          
          
        sendTag(String(bufferVal, HEX));
          
          delay(500);
         digitalWrite(LED_PIN, LOW);
          
         resetBuffers();
     }
}
void initInterrupts(){
   //Weigand pins - Arduino interrups
     attachInterrupt(0, readWZero, RISING);
     attachInterrupt(1, readWOne, RISING);
     
     pinMode(ARDUINO_INTERRUPT_0, INPUT);
     pinMode(ARDUINO_INTERRUPT_1, INPUT);
     
     digitalWrite(ARDUINO_INTERRUPT_0, HIGH);
     digitalWrite(ARDUINO_INTERRUPT_1, HIGH);

}
void sendTag(String tag){
  if (tag.length() % 2 == 1){
    tag = "0"+tag;
  }
  Serial.print(tag);
  Serial.print(" | ");
  Serial.println(tag.length()); 
  sigfox.write("AT$SF=");
  sigfox.print(tag);
  sigfox.write("\n"); 
}
void resetBuffers(){
   bufferVal = 0;
   bitsRead = 0;
}
void readWOne(void) {
     bitsRead++;
     bufferVal = bufferVal << 1;
     bufferVal |= 1;
}

void readWZero(void) {
     bitsRead++;
     bufferVal = bufferVal << 1;
}

