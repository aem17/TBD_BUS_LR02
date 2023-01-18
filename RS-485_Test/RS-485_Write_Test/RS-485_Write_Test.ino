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
  uint8_t RS485_write_en   = RS485_READ;    // Read or Write Mode

  // READ STUFF
  byte rolling_buffer[12]  = {0,0,0,0,0,0,      
                             0,0,0,0,0,0};  // Rolling buffer
  uint8_t READ_STATE       = INIT_STATE;    // RS485 Reader state machine state
  byte read_msg_buffer[12] = {0,0,0,0,0,0,
                             0,0,0,0,0,0};  // Full message buffer
  uint8_t new_message_flag = 0;             // New message available flag
  uint8_t error_code       = 0;             // 

  // WRITE STUFF
  byte write_msg_buffer[12]= {SOM_BYTE,
                              0,0,0,0,0,      
                              0,0,0,0,0,
                              EOM_BYTE};    // Write buffer
  
};

// Data to be sent
int data1[8]  = {'D','e','e','z',' ',' ',' ',' '};
int data2[8]  = {'N','u','t','s',' ','8','=','>'};
  
// Message "config"
const int target_addr   = 1;
const int msg_type      = 1;


// Initialise RS485 interface
RS485_Device rs485_interface;


void setup() {
  
  // Pin defs
  pinMode(TX_EN_PIN, OUTPUT);
  
  // Set up serial
  Serial.begin(9600);
  // Set up rs485bus
  init_rs485bus(&rs485_interface, RS485_ID, TX_EN_PIN, RX_EN_PIN);
}


void loop() {
  write_rs485bus(&rs485_interface, target_addr, msg_type, data1);
  write_rs485bus(&rs485_interface, target_addr, msg_type, data2);
}


//-------------------//
//---- FUNCTIONS ----// 
//-------------------//


/*  
 *write_rs485bus
 *
 *AEM
 *Writes packet to RS485 bus,
 * 
 **rs485_dev  - RS485 Device struct
 * address - id of device message is addressed to
 * msg_type - type of message being sent
 * data  - 8 byte array of data to be sent
 * returns 1 if successful
 */
int write_rs485bus(RS485_Device *rs485_dev, int address, int msg_type, int data[8])
{
  // Populate write buffer
  rs485_dev -> write_msg_buffer[1] = byte(address);
  rs485_dev -> write_msg_buffer[2] = byte(msg_type);
  for(int i=0; i<8; i++)
  {
    rs485_dev -> write_msg_buffer[i+3] = data[i];
  }
  // Write innit
  digitalWrite(TX_EN_PIN, HIGH);
  Serial.write(rs485_dev -> write_msg_buffer, 12);
  digitalWrite(TX_EN_PIN, LOW);

  return 1;
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
