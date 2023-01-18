// DEFINES

// PINS
#define TX_EN_PIN 6
#define RX_EN_PIN 7
#define LED_PIN   13

// READ STATES
#define INIT_STATE 0
#define BASE_STATE 1
#define ROLL_STATE 2

// RS485 ENABLE
#define RS485_READ 0
#define RS485_WRITE 1

// RS485 DEVICE ID
#define RS485_ID 1

// SOM & EOM
#define SOM_BYTE 255 // 11111111
#define EOM_BYTE 85  // 01010101

// STRUCTS
struct RS485_Device
{
  uint8_t DEVICE_ID        = 0;             // RS485 Device Address
  byte rolling_buffer[12]  = {0,0,0,0,0,0,      
                             0,0,0,0,0,0};  // Rolling buffer
  uint8_t READ_STATE       = INIT_STATE;    // RS485 Reader state machine state
  byte read_msg_buffer[12] = {0,0,0,0,0,0,
                             0,0,0,0,0,0};  // Full message buffer
  uint8_t new_message_flag = 0;             // New message available flag
  uint8_t error_code       = 0;             // 
  uint8_t RS485_write_en   = RS485_READ;    // 
};

RS485_Device rs485_interface;

void setup() {
  // Pin Setup
  pinMode(TX_EN_PIN, OUTPUT);
  pinMode(RX_EN_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  // Set up serial
  Serial.begin(9600);
  // RS485 interface initialisation
  Serial.println("Calling Init");
  digitalWrite(LED_PIN, HIGH);
  init_rs485bus(&rs485_interface, RS485_ID, TX_EN_PIN, RX_EN_PIN);
  
}

void loop() {
  
  if(read_rs485bus(&rs485_interface) == 1)
  {
    Serial.println("RS485 Interface not initialisied");
  }
  // Successful Read - new_message_available and adressed to me
  if(rs485_interface.new_message_flag &&
     rs485_interface.read_msg_buffer[1] == RS485_ID)  
  {
    for(int i=3; i<11; i++)
    {
      Serial.print((char) rs485_interface.read_msg_buffer[i]);           
    }        
    Serial.println("");
  }  

}

//-------------------//
//---- FUNCTIONS ----// 
//-------------------//


/*  
 *read_rs485bus
 *
 *AEM
 *Monitors & Reads RS485 bus
 *
 **rs485_dev  - RS485 Device struct
 *returns 0 if successful, 1 if init error
 */
int read_rs485bus(RS485_Device *rs485_dev)
{
  switch(rs485_dev->READ_STATE)
  {
    // Reader not initialised
    case(INIT_STATE):
      return 1;          
    break;
    
    // Reader initialised, nothing read yet or just read new message
    case(BASE_STATE):
      rs485_dev -> new_message_flag = 0;  // reset message flag, previous message now read       
      if(Serial.available() <12)      
      {
        return 0; // not sufficient data for reading
      }
      
      for(int i=0; i<12; i++)
      {
        rs485_dev -> rolling_buffer[i] = Serial.read(); // read 12 bytes
      }
      rs485_dev -> READ_STATE = ROLL_STATE;
      return 0;        
    break;
    
  // Rolling buffer, reading 1 byte at a time     
    case(ROLL_STATE):
      // If message aligned
      if(rs485_dev -> rolling_buffer[0]  == SOM_BYTE &&
         rs485_dev -> rolling_buffer[11] == EOM_BYTE)          
      {
        for(int i=0; i<12; i++) 
        {
          rs485_dev -> read_msg_buffer[i] = rs485_dev -> rolling_buffer[i]; //copy rolling buff to msg buff
        }    
                   
        rs485_dev -> new_message_flag = 1;  // New message!
        
        for(int i=0; i<12; i++)  // Clear rolling buffer
        {
          rs485_dev -> rolling_buffer[i] = 0;            
        }        
        
        rs485_dev -> READ_STATE = BASE_STATE;
        return 0;                                                
      }      
      if(Serial.available())
      {
              
        // shift rolling bytes 
        for(int i=0; i<11; i++)
        {
          rs485_dev -> rolling_buffer[i] = rs485_dev -> rolling_buffer[i+1];
        }      
         
        // Add the latest read byte to the end 
        rs485_dev -> rolling_buffer[11] = Serial.read();
        return 0;
        
      }  
      return 0;    
    break;
  }
}


/*  
 *init_rs485bus
 *
 *AEM
 *initialises RS485 bus interaction, sets device ID,
 *activates bus to reading state
 * 
 **rs485_dev  - RS485 Device struct
 * dev_id -  RS485 bus id of this device
 * tx_en  - tx enable pin
 * rx_en  - rx enable pin
 *returns 1 if successful
 */
int init_rs485bus(RS485_Device *rs485_dev, int dev_id, int tx_en, int rx_en)
{
  rs485_dev -> READ_STATE = BASE_STATE;
  rs485_dev -> DEVICE_ID  = dev_id;
  digitalWrite(tx_en, LOW);
  digitalWrite(rx_en, LOW);

  Serial.println(rs485_dev -> READ_STATE);
  Serial.println(rs485_dev -> DEVICE_ID);
  Serial.println("Initialised");
 
}
