#define TX_EN_PIN 6
#define RX_EN_PIN 7
#define LED_PIN   13

byte incomingData[12];
const byte SOM_BYTE = 255;
const byte EOM_BYTE = B01010101;

void setup() {
  // Pin defs
  pinMode(TX_EN_PIN, OUTPUT);
  pinMode(RX_EN_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  // Set up serial
  Serial.begin(9600);
}

void loop() {
  // RS 485 Read Enable
  digitalWrite(TX_EN_PIN, LOW);
  digitalWrite(RX_EN_PIN, LOW);
  // Serial read
  if(Serial.available() >= 12)
  {
    Serial.readBytes(incomingData, 12);
    for(int i=0; i<12; i++)
    {
    Serial.print(incomingData[i], DEC); 
    Serial.print(" ");
    } 
    Serial.println("");
  }
  // Safe implementation
  // 
//  if(Serial.available())
//  {
//    byte read_byte;
//    read_byte = Serial.read();
//    if(read_byte == SOM_BYTE)
//    {
//      incomingData[0] = read_byte;     
//      if(Serial.available())
//      {
//        read_byte = Serial.read();
//        while(read_byte != EOM_BYTE)
//        {
//          
//        }                
//      }      
//    }
//  }


  
}
