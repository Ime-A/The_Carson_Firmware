/******************** (C) COPYRIGHT 2015 STMicroelectronics ********************
* File Name          : gatt_db.c
* Author             : BlueNRG-1 main file for Chat demo
* Version            : V1.0.0
* Date               : 16-September-2015
* Description        : Functions to build GATT DB and handle GATT events.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "BlueNRG1_it.h"
#include "BlueNRG1_conf.h"
#include "ble_const.h" 
#include "bluenrg1_stack.h"
#include "clock.h"
#include "gp_timer.h"
#include "gatt_db.h"
#include "osal.h"
#include "SDK_EVAL_Config.h"
#include "Stent_Sensor_ADC.h"
#include "Stent_Sensor_GPIO.h"
#include "Stent_Sensor_Timer.h"

#ifndef DEBUG
#define DEBUG 0
#endif

#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#define DECT_MODE 0
#define CONST_MODE 1



uint16_t ADCServiceHandle;
//uint16_t BattServiceHandle;


uint16_t VoltageCharHandle;
//uint16_t ThresholdCharHandle;
//uint16_t BatteryCharHandle;
//uint16_t ModeCharHandle;
//uint16_t TriggerCharHandle;

extern uint16_t connection_handle;
extern uint32_t batt_voltage;
extern uint8_t Mode;
volatile uint32_t RX_Threshold = 250;

/* UUIDs */
Service_UUID_t service_uuid;
Char_UUID_t char_uuid;
Char_Desc_Uuid_t char_desc_uuid;
/*******************************************************************************
* Function Name  : Add_ADC_Service
* Description    : Add the ADC service.
* Input          : None
* Return         : Status.
*******************************************************************************/
uint8_t Add_ADC_Service(void)
{
  uint8_t ret;

  /*
  UUIDs:
  D973F2E0-B19E-11E2-9E96-0800200C9A66
  D973F2E1-B19E-11E2-9E96-0800200C9A66
  D973F2E2-B19E-11E2-9E96-0800200C9A66
  */

//  const uint8_t uuid[16] = {0x66,0x9a,0x0c,0x20,0x00,0x08,0x96,0x9e,0xe2,0x11,0x9e,0xb1,0xe0,0xf2,0x73,0xd9};
//  const uint8_t voltagecharUuid[16] = {0x66,0x9a,0x0c,0x20,0x00,0x08,0x96,0x9e,0xe2,0x11,0x9e,0xb1,0xe1,0xf2,0x73,0xd9};
  

	uint16_t value_serviceUUID = 0x1516;
  Osal_MemCpy(&service_uuid.Service_UUID_128, &value_serviceUUID, 2);
  ret = aci_gatt_add_service(UUID_TYPE_128, &service_uuid, PRIMARY_SERVICE, 15, &ADCServiceHandle); 
  if (ret != BLE_STATUS_SUCCESS) goto fail;    

	
	uint16_t voltagecharUUID = 0x2A01;
  Osal_MemCpy(&char_uuid.Char_UUID_128, &voltagecharUUID , 2);
  ret =  aci_gatt_add_char(ADCServiceHandle, UUID_TYPE_128, &char_uuid, 8, CHAR_PROP_NOTIFY, ATTR_PERMISSION_NONE, 0,
                16, 1, &VoltageCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;




//	uint16_t thresholdcharUUID = 0x2B01;
//	Osal_MemCpy(&char_uuid.Char_UUID_128, &thresholdcharUUID, 2);
//  ret =  aci_gatt_add_char(ADCServiceHandle, UUID_TYPE_128, &char_uuid, 4, CHAR_PROP_WRITE|CHAR_PROP_WRITE_WITHOUT_RESP, ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE,
//                16, 0, &ThresholdCharHandle);
//  if (ret != BLE_STATUS_SUCCESS) goto fail;

	
	


//	uint16_t batterycharUUID = 0x2C01;
//	Osal_MemCpy(&char_uuid.Char_UUID_128, &batterycharUUID, 2);
//  ret =  aci_gatt_add_char(ADCServiceHandle, UUID_TYPE_128, &char_uuid, 4, CHAR_PROP_READ|CHAR_PROP_NOTIFY, ATTR_PERMISSION_NONE,
//								GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
//                16, 0, &BatteryCharHandle);
//  if (ret != BLE_STATUS_SUCCESS) goto fail;
//	
	
//	uint16_t modecharUUID = 0x2D01;
//	Osal_MemCpy(&char_uuid.Char_UUID_128, &modecharUUID, 2);
//  ret =  aci_gatt_add_char(ADCServiceHandle, UUID_TYPE_128, &char_uuid, 2, CHAR_PROP_WRITE|CHAR_PROP_WRITE_WITHOUT_RESP, ATTR_PERMISSION_NONE,
//								GATT_NOTIFY_ATTRIBUTE_WRITE,
//                16, 0, &ModeCharHandle);
//  if (ret != BLE_STATUS_SUCCESS) goto fail;
	


//	uint16_t triggcharUUID = 0x2E01;
//	Osal_MemCpy(&char_uuid.Char_UUID_128, &triggcharUUID, 2);
//  ret =  aci_gatt_add_char(ADCServiceHandle, UUID_TYPE_128, &char_uuid, 1,  CHAR_PROP_NOTIFY, ATTR_PERMISSION_NONE,
//								0,
//                16, 1, &TriggerCharHandle);
//  if (ret != BLE_STATUS_SUCCESS) goto fail;
	
	
  PRINTF("Service added and Characteristic added.\n");
  return BLE_STATUS_SUCCESS; 

fail:
//  printf("Error while adding Serial Port  service.\n");
  return BLE_STATUS_ERROR ;
}


tBleStatus Adc_Update(uint32_t* adcHexValue)
{  

  tBleStatus ret;

	
	
  ret = aci_gatt_update_char_value_ext(connection_handle, ADCServiceHandle, VoltageCharHandle, 1,8,0, 0x08, (uint8_t*)adcHexValue);
  if (ret != BLE_STATUS_SUCCESS){
//    printf("Error while updating characteristic: 0x%02X\n",ret) ;
    return BLE_STATUS_ERROR ;
  }
	
	//printf("Updated characteristic: 0x%02X\n",ret) ;
  return BLE_STATUS_SUCCESS;
    
}

/*
tBleStatus Batt_Update()
{
	tBleStatus ret;
	

	
	 ret = aci_gatt_update_char_value_ext(connection_handle, ADCServiceHandle, BatteryCharHandle, 1,4,0, 0x04, (uint8_t*)&batt_voltage);
  if (ret != BLE_STATUS_SUCCESS){
    return BLE_STATUS_ERROR ;
  }

	
	return ret;	
}

*/

/*
tBleStatus Trigger_Update(uint8_t Tigger)
{  

  tBleStatus ret;

	
	
  ret = aci_gatt_update_char_value_ext(connection_handle, ADCServiceHandle, TriggerCharHandle, 1,1,0, 0x01, (uint8_t*)&Tigger);
  if (ret != BLE_STATUS_SUCCESS){
    return BLE_STATUS_ERROR ;
  }
	
  return BLE_STATUS_SUCCESS;
    
}
*/
void Read_Request_CB(uint16_t handle)
{

	tBleStatus ret;
	
	if(connection_handle !=0)
  {
    ret = aci_gatt_allow_read(connection_handle);
    if (ret != BLE_STATUS_SUCCESS)
    {
    }
  }
	
//	if(handle ==  BatteryCharHandle + 1)
//	{
//		
//		Batt_Update();
//	}
	
	
	


}



void Attribute_Modified_CB(uint16_t handle, uint16_t data_length, uint8_t *att_data)
{

//	if(handle == ThresholdCharHandle + 1)
//	{

//			uint16_t RX;
//			memcpy(&RX, att_data, sizeof(uint16_t));
//			RX_Threshold = flip_endianness_16bit(RX);
//	}
//	
//		if(handle == ModeCharHandle +1)
//	{
//			uint8_t Mode_Recieved;
//			memcpy(&Mode_Recieved, att_data, sizeof(uint8_t));
//		
//			if(Mode_Recieved == DECT_MODE)
//				{
//					Mode = DECT_MODE;
//					GPIO_ResetBits(SAMPLING_PIN);
//				}
//				
//				if(Mode_Recieved == CONST_MODE)
//				{
//					Mode = CONST_MODE;
//				}
//	}
	
}


uint16_t flip_endianness_16bit(uint16_t value)
{
    // Extract the low byte (LSB) and the high byte (MSB)
    uint8_t low_byte = value & 0xFF;
    uint8_t high_byte = (value >> 8) & 0xFF;

    // Swap the high and low bytes
    uint16_t flipped_value = (low_byte << 8) | high_byte;

    return flipped_value;
}


