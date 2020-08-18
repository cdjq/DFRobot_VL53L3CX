     /*!
 * @file DFRobot_VL53L3CX.h
 * @brief Define the infrastructure of the DFRobot_AS7341 class
 * @n 可以驱动IC为AS731的光谱传感器，
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2020-07-16
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_AS7341
 */

#ifndef DFROBOT_VL53L3CX_H
#define DFROBOT_VL53L3CX_H
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Wire.h>
//#define ENABLE_DBG

#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif

#include "include/vl53lx_api.h"
#include "include/53L3A2.h"




class DFRobot_VL53L3CX 
{
public:
#define ERR_OK             0      //OK
#define ERR_DATA_BUS      -1      //Data Bus error
#define ERR_IC_VERSION    -2      //Chip version mismatch 
  DFRobot_VL53L3CX(TwoWire *pWire = &Wire);
  /**
   * @brief init function
   * @return Return 0 if the initialization succeeds, otherwise return non-zero and error code.
   */
  int begin();
  
  /**
   * @brief 读取传感器的ID
   * @return 读取到的传感器ID,一个字节数据.
   */
  uint8_t readID();
  
  uint8_t ranging();
  
private:

  uint8_t readReg16(uint16_t reg, void* pBuf, size_t size);
  uint8_t writeReg16();
  TwoWire *_pWire;
  uint8_t _address;
    VL53LX_Dev_t                   dev;
    VL53LX_DEV                     Dev = &dev;
};

#endif