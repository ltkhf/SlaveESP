
#include <Dynamixel2Arduino.h>

  #include <SoftwareSerial.h>
  SoftwareSerial soft_serial(16, 17); // DYNAMIXELShield UART RX/TX

  #define DXL_SERIAL   Serial2
  #define DEBUG_SERIAL Serial
  const int DXL_DIR_PIN = 14; // DYNAMIXEL Shield DIR PIN


// Create a port object for DYNAMIXEL communication.
// The list of available classes is as follows.
// 1) DYNAMIXEL::SerialPortHandler     (HardwareSerial only)
//     -note: If you do not want to use half duplex communication, do not enter the second parameter.
// 2) DYNAMIXEL::USBSerialPortHandler  (Only USB port on each board)
DYNAMIXEL::SerialPortHandler dxl_port(DXL_SERIAL, DXL_DIR_PIN);

// Create a Slave object to communicate with the master.
const float DXL_PROTOCOL_VER_1_0 = 1.0;
const float DXL_PROTOCOL_VER_2_0 = 2.0;
const uint16_t DXL_MODEL_NUM = 0x0190; // Modify it to what you want.
DYNAMIXEL::Slave dxl(dxl_port, DXL_MODEL_NUM);

// Declare the address of the Slave control item you want 
// to register and the variable (size is also important)
// to store its data.
const uint16_t ADDR_CONTROL_ITEM_LED = 10;
const uint16_t ADDR_CONTROL_ITEM_ANALOG = 20;
// note: 'int' is not supported because its size varies by system architecture.
uint8_t control_item_led;
int16_t control_item_analog;

//This namespace is required to use Control table item names
using namespace ControlTableItem;

void setup() {
  // put your setup code here, to run once:
  DEBUG_SERIAL.begin(115200);

  // Speed setting for communication (not necessary for USB)
  dxl_port.begin(1000000);

  dxl.setPortProtocolVersion(DXL_PROTOCOL_VER_1_0);
  dxl.setFirmwareVersion(1);
  dxl.setID(20);

  // Add control items.
  // Input the address(p1) and the variable(p2) to write/read the data as parameters.
  dxl.addControlItem(ADDR_CONTROL_ITEM_LED, control_item_led);
  dxl.addControlItem(ADDR_CONTROL_ITEM_ANALOG, control_item_analog);
}

void loop() {
  // put your main code here, to run repeatedly:

  // If there is a packet from the master, process it.
  if(dxl.processPacket() == false){
    DEBUG_SERIAL.print("Last lib err code: ");
    DEBUG_SERIAL.print(dxl.getLastLibErrCode());
    DEBUG_SERIAL.print(", ");
    DEBUG_SERIAL.print("Last status packet err code: ");
    DEBUG_SERIAL.println(dxl.getLastStatusPacketError());
    DEBUG_SERIAL.println();
  }

  // Update or use variables for each control item.
  digitalWrite(13, control_item_led);
  control_item_analog = analogRead(A0);
}
