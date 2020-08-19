#include "DFRobot_VL53L3CX.h"

 
DFRobot_VL53L3CX::DFRobot_VL53L3CX(TwoWire *pWire)
{
  
  _pWire = pWire;
  _address = 0x29;
}

int DFRobot_VL53L3CX::begin()
{
  uint8_t buffer[2];
  _pWire->begin();
  _pWire->beginTransmission(_address);
  if(_pWire->endTransmission() != 0){
    DBG("");
    DBG("bus data access error"); DBG("");
    return ERR_DATA_BUS;
  }
  
  XNUCLEO53L3A2_Init();
  /* Allow to select the sensor to be used, multi-sensor is not managed in this example.
    Only when use use the Left ToF in interrupt mode,  solder the U7 on the X-Nucleo-53L3A2 board 
    Only when use the Right ToF in interrupt mode, solder the U7 on the X-Nucleo-53L3A2 board
    See "Solder drop configurations" in the X-Nucleo-53L3A2 User Manual for more details */
  uint8_t ToFSensor = 1; // Select ToFSensor: 0=Left, 1=Center, 2=Right
  XNUCLEO53L3A2_ResetId(ToFSensor, 0); // Reset ToF sensor
  delay(2);
  XNUCLEO53L3A2_ResetId(ToFSensor, 1); // Reset ToF sensor
  delay(2);
  
  return ERR_OK;
}


uint8_t DFRobot_VL53L3CX::readID(){
  uint8_t id;
  readReg16(0x010F,&id,1);
  
  return id;
}

uint8_t DFRobot_VL53L3CX::ranging()
{
    int status;
    VL53LX_MultiRangingData_t MultiRangingData;
    VL53LX_MultiRangingData_t *pMultiRangingData = &MultiRangingData;
    uint8_t NewDataReady=0;
    int no_of_object_found=0,j;
    SerialUSB.println("Ranging loop starts\n");

    status = VL53LX_WaitDeviceBooted(Dev);

    status = VL53LX_DataInit(Dev);
    status = VL53LX_StartMeasurement(Dev);

    if(status){
        SerialUSB.println("VL53LX_StartMeasurement failed: error = %d \n");
        while(1);
    }

    do{ // polling mode

        status = VL53LX_GetMeasurementDataReady(Dev, &NewDataReady); 
        //SerialUSB.println(NewDataReady);
        delay(1); // 1 ms polling period, could be longer.
        if((!status)&&(NewDataReady!=0)){
            status = VL53LX_GetMultiRangingData(Dev, pMultiRangingData);
            no_of_object_found=pMultiRangingData->NumberOfObjectsFound;
            //printf("Count=%5d, ", pMultiRangingData->StreamCount);
            //printf("#Objs=%1d ", no_of_object_found);
			SerialUSB.print("Count= ");
			SerialUSB.println(pMultiRangingData->StreamCount);
			SerialUSB.print("#Objs=%1d ");
			SerialUSB.println(no_of_object_found);
            for(j=0;j<no_of_object_found;j++){
                if(j!=0)printf("\n                     ");

			SerialUSB.print("status= ");
			SerialUSB.println(pMultiRangingData->RangeData[j].RangeStatus);

			SerialUSB.print("D= ");
			SerialUSB.println(pMultiRangingData->RangeData[j].RangeMilliMeter);


			SerialUSB.print("Signal= ");
			SerialUSB.println(pMultiRangingData->RangeData[j].SignalRateRtnMegaCps/65536.0);

			SerialUSB.print("Ambient= ");
			SerialUSB.println(pMultiRangingData->RangeData[j].AmbientRateRtnMegaCps/65536.0);

            SerialUSB.println();
            SerialUSB.println();
/*
                printf("status=%d, D=%5dmm, Signal=%2.2f Mcps, Ambient=%2.2f Mcps",
                pMultiRangingData->RangeData[j].RangeStatus,
                pMultiRangingData->RangeData[j].RangeMilliMeter,
                pMultiRangingData->RangeData[j].SignalRateRtnMegaCps/65536.0,
                pMultiRangingData->RangeData[j].AmbientRateRtnMegaCps/65536.0);
  */
  }
            printf ("\n");
            if (status==0){
                status = VL53LX_ClearInterruptAndStartMeasurement(Dev);
            }
        }
    }
    while (1);
   

}


uint8_t DFRobot_VL53L3CX::readReg16(uint16_t reg, void* pBuf, size_t size)
{
  if (pBuf == NULL) {
    //DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  Wire.beginTransmission(_address);
  Wire.write(reg>>8);
  Wire.write(reg);
  ;
  if ( Wire.endTransmission() != 0) {
    return 0;
  }
  delay(10);
  Wire.requestFrom(_address, size);
  for (uint16_t i = 0; i < size; i++) {
    _pBuf[i] = Wire.read();
  }
  Wire.endTransmission();
  return size;


}