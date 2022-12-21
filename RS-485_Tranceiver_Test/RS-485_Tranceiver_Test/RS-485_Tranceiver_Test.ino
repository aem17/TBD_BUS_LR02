// Licensed under
// D E E Z  N U T S


// TODO:
// - how to approach initialisation
// - define message structure
// - write bus write func
// - write bus read func

// MESSAGE STRUCT
// uint_8 Start of message byte
// uint_8 mesasge type id
// uint_8 adressee id
// data - what size? 64 bits for initial prove out
// uint_8 eom  

// DEFINES
// Set up controller id
#define CONTROLLER_ID 1

//message types
#define INFO_REQ 1
#define INFO_ANS 2
#define INFO_GEN 3
#define WARNING  4
#define FAULT    5
#define SHUTDOWN 0

// pins
#define TX_EN_PIN 6

// limits
// max data size, bytes
#define MAX_DATA_SIZE 8

void setup() {
  pinMode(TX_EN_PIN, HIGH);
      
}

void loop() {
  // put your main code here, to run repeatedly:

}


// RS-485 Write function
// adr_id - adressee id; message_type - int, type; data - data to be sent
int RS485_Write(uint8_t adr_id, uint8_t message_type, long data)
{
    // SAFETY CHECKS    
    // Data length check
    // Address exists check
    // Actual message type check
    // if all bueno then proceed
    // else return 0
    
    // DATA PACKAGING    
        
    // shift data left
    // add message_type
    // shift data left
    // add adr_id
    // shift data left
    // add end of line byte
    
    // WRITING TO BUS
    // tx enable
    // Serial write
    // tx disable / rx enable

    
}

// RS-485 Read function
// message_type - int pointer, type; data - pointer to 8 byte array
int RS485_Read(uint8_t message_type, long data)
{
    // SAFETY CHECKS?

    // READING BUS DATA
    // rx enable
    // read data to buffer?
    // bitwise & with adr_id/
    // is this relevant to me
    // if no  -> Clear buffer and return 0
    // if yes -> read and store data, message_type     
    //    
       
         
    // Serial write
    // tx disable / rx enable
}
