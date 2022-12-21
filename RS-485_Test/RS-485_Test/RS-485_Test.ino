// Extended test
// writing 12 bytes to serial bus

#define TX_EN_PIN 6
#define SOM_BYTE 255 // 11111111
#define EOM_BYTE 85  // 01010101


// setup dummy data
typedef union {
  float val;
  byte b[4];
} f2b;


const int target_addr   = 1;
const int msg_type      = 1;
const byte padding      = 0;
f2b dummy_data;
byte msg[12] = {SOM_BYTE, byte(target_addr), byte(msg_type), 
                padding, padding, padding, padding,
                padding, padding, padding, padding, EOM_BYTE};


char data1[6]  = {'H','e','l','l','o',' '};
char data2[6]  = {'W','o','r','l','d','!'};

void setup() {
  
  dummy_data.val    = 3.14;
  for(int i=0; i<4; i++)
  {
        msg[i+7] = dummy_data.b[i];
  }

  // Pin defs
  pinMode(TX_EN_PIN, OUTPUT);
  // Set up serial
  Serial.begin(9600);
}

void loop() {
  // RS 485 Write Enable
  digitalWrite(TX_EN_PIN, HIGH);  
  // Print Serial

  for(int i=0; i<6; i++)
  {
    msg[i+3] = data1[i];
  }
  Serial.write(msg, 12);
  
  for(int i=0; i<6; i++)
  {
    msg[i+3] = data2[i];
  }
  Serial.write(msg, 12);
  
    
}
  

