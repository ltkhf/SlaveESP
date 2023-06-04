#include <stdlib.h>
#include <MPU6050.h>
#include <I2Cdev.h>
#include <Dynamixel2Arduino.h>
#include "Wire.h"

#define DXL_SERIAL   Serial2
#define DEBUG_SERIAL Serial
const uint8_t DXL_DIR_PIN = 14;

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

MPU6050 mpu;
int16_t y,x,z;

const uint16_t mpu_addr = 59;
int32_t acc;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  while (!Serial){}

  DEBUG_SERIAL.begin(115200);
  dxl_port.begin(1000000);
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VER_1_0);
  dxl.setFirmwareVersion(1);
  dxl.setID(29);
  
  // Input the address(p1) and the variable(p2) to write/read the data as parameters.
  dxl.addControlItem(mpu_addr,acc);

  mpu.initialize();
  Serial.println(mpu.testConnection()? "Sukses":"Gagal");
  mpu.CalibrateAccel();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //   If there is a packet from the master, process it.
  if (dxl.processPacket() == false) {
    DEBUG_SERIAL.print("Last lib err code: ");
    DEBUG_SERIAL.print(dxl.getLastLibErrCode());
    DEBUG_SERIAL.print(", ");
    DEBUG_SERIAL.print("Last status packet err code: ");
    DEBUG_SERIAL.println(dxl.getLastStatusPacketError());
    DEBUG_SERIAL.println();
  } 
 
  mpu.getAcceleration(&x,&y,&z);
//  Serial.println(x);
//  Serial.println(y);
//  Serial.println(z);  

  acc = mpu.getAccelerationX();
  Serial.println (acc);

//  Serial.println(mpu.getFullScaleAccelRange());

//  DEBUG_SERIAL.print(x); DEBUG_SERIAL.print(" nilai x\t");
//  DEBUG_SERIAL.print(y); DEBUG_SERIAL.print(" nilai y \t");
//  DEBUG_SERIAL.print(z); DEBUG_SERIAL.print(" nilai z\t");


}
