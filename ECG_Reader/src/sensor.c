/******************** (C) COPYRIGHT 2015 STMicroelectronics ********************
 * File Name          : sensor.c
 * Author             : AMS - VMA RF Application team
 * Version            : V1.0.0
 * Date               : 23-November-2015
 * Description        : Sensor init and sensor state machines
 ********************************************************************************
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 * AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
 * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
 * CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
 * INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "BlueNRG1_it.h"
#include "BlueNRG1_conf.h"
#include "ble_const.h" 
#include "bluenrg1_stack.h"
#include "gp_timer.h"
#include "SDK_EVAL_Config.h"
#include "gatt_db.h"
#include "Stent_Sensor_ADC.h"
#include "Stent_Sensor_Timer.h"
#include "Stent_Sensor_GPIO.h"

#ifndef SENSOR_EMULATION
#include "LPS25HB.h"
#include "lsm6ds3.h"
#include "LSM6DS3_hal.h"
#endif
#include "OTA_btl.h"   
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifndef DEBUG
#define DEBUG 0
#endif

#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#define UPDATE_CONN_PARAM 0 
#define  ADV_INTERVAL_MIN_MS  500
#define  ADV_INTERVAL_MAX_MS  600
#define LOW 0x1DD00000
#define DEFAULT_HIGH 0x04D00000
#define MID_POINT 0x15000000
#define READ_BIT(REG, BIT)    ((REG) & (BIT))


#define BLE_SENSOR_VERSION_STRING "1.1.0" 

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t set_connectable = 1;
uint16_t connection_handle = 0;
uint8_t connInfo[20];

BOOL sensor_board = FALSE; // It is True if sensor boad has been detected
volatile uint32_t High_Threshold = DEFAULT_HIGH;
volatile uint32_t Low_Threshold = LOW;
// volatile uint32_t RX_Threshold = 2000;

int connected = FALSE;


#define SENSOR_TIMER 1
static uint16_t acceleration_update_rate = 200;
static uint8_t sensorTimer_expired = FALSE;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




/*******************************************************************************
 * Function Name  : Sensor_DeviceInit.
 * Description    : Init the device sensors.
 * Input          : None.
 * Return         : Status.
 *******************************************************************************/
uint8_t Sensor_DeviceInit()
{
  uint8_t bdaddr[] = {0x12, 0x34, 0x00, 0xE1, 0x80, 0x02};
  uint8_t ret;
  uint16_t service_handle, dev_name_char_handle, appearance_char_handle;
  uint8_t device_name[] = {'E', 'C', 'G'};

  /* Set the device public address */
  ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN,
                                  bdaddr);  
  if(ret != BLE_STATUS_SUCCESS) {
   // printf("aci_hal_write_config_data() failed: 0x%02x\r\n", ret);
    return ret;
  }
  
  /* Set the TX power -2 dBm */
  aci_hal_set_tx_power_level(1, 4);
  
  /* GATT Init */
  ret = aci_gatt_init();
  if (ret != BLE_STATUS_SUCCESS) {
    //printf("aci_gatt_init() failed: 0x%02x\r\n", ret);
    return ret;
  }
  
  /* GAP Init */
  ret = aci_gap_init(GAP_PERIPHERAL_ROLE, 0, 0x07, &service_handle, &dev_name_char_handle, &appearance_char_handle);
  if (ret != BLE_STATUS_SUCCESS) {
   // printf("aci_gap_init() failed: 0x%02x\r\n", ret);
    return ret;
  }
 
  /* Update device name */
  ret = aci_gatt_update_char_value_ext(0, service_handle, dev_name_char_handle, 0,sizeof(device_name), 0, sizeof(device_name), device_name);
  if(ret != BLE_STATUS_SUCCESS) {
   // printf("aci_gatt_update_char_value_ext() failed: 0x%02x\r\n", ret);
    return ret;
  }
  
  ret = aci_gap_set_authentication_requirement(BONDING,
                                               MITM_PROTECTION_REQUIRED,
                                               SC_IS_NOT_SUPPORTED,
                                               KEYPRESS_IS_NOT_SUPPORTED,
                                               7, 
                                               16,
                                               USE_FIXED_PIN_FOR_PAIRING,
                                               123456,
                                               0x00);
  if(ret != BLE_STATUS_SUCCESS) {
   // printf("aci_gap_set_authentication_requirement()failed: 0x%02x\r\n", ret);
    return ret;
  } 
  
// printf("BLE Stack Initialized with SUCCESS\n");


  ret = Add_ADC_Service();
  if(ret == BLE_STATUS_SUCCESS) {
  // printf("Acceleration service added successfully.\n");
  } else {
   // printf("Error while adding Acceleration service: 0x%02x\r\n", ret);
    return ret;
  }



  
  /* Start the Sensor Timer */
  ret = HAL_VTimerStart_ms(SENSOR_TIMER, acceleration_update_rate);
  if (ret != BLE_STATUS_SUCCESS) {
   // printf("HAL_VTimerStart_ms() failed; 0x%02x\r\n", ret);
    return ret;
  } else {
    sensorTimer_expired = FALSE;
  }

  return BLE_STATUS_SUCCESS;
}

/*******************************************************************************
 * Function Name  : Set_DeviceConnectable.
 * Description    : Puts the device in connectable mode.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void Set_DeviceConnectable(void)
{  
  uint8_t ret;
  uint8_t local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'E','C','G'}; 


  hci_le_set_scan_response_data(0,NULL);
 // PRINTF("Set General Discoverable Mode.\n");
  
  ret = aci_gap_set_discoverable(ADV_IND,
                                (ADV_INTERVAL_MIN_MS*1000)/625,(ADV_INTERVAL_MAX_MS*1000)/625,
                                 STATIC_RANDOM_ADDR, NO_WHITE_LIST_USE,
                                 sizeof(local_name), local_name, 0, NULL, 0, 0); 
  if(ret != BLE_STATUS_SUCCESS)
  {
 //   PRINTF("aci_gap_set_discoverable() failed: 0x%02x\r\n",ret);
    SdkEvalLedOn(LED3);  
  }
 // else
//    printf("aci_gap_set_discoverable() --> SUCCESS\r\n");
}

/*******************************************************************************
 * Function Name  : APP_Tick.
 * Description    : Sensor Demo state machine.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void APP_Tick(void)
{
  /* Make the device discoverable */
  if(set_connectable) {
    Set_DeviceConnectable();
    set_connectable = FALSE;

  }
	
}

/* ***************** BlueNRG-1 Stack Callbacks ********************************/

/*******************************************************************************
 * Function Name  : hci_le_connection_complete_event.
 * Description    : This event indicates that a new connection has been created.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void hci_le_connection_complete_event(uint8_t Status,
                                      uint16_t Connection_Handle,
                                      uint8_t Role,
                                      uint8_t Peer_Address_Type,
                                      uint8_t Peer_Address[6],
                                      uint16_t Conn_Interval,
                                      uint16_t Conn_Latency,
                                      uint16_t Supervision_Timeout,
                                      uint8_t Master_Clock_Accuracy)
{
  
  connected = TRUE;
  connection_handle = Connection_Handle;
	SdkEvalLedOn(LED2);//activity led 
	//GPIO_SetBits(BLUETOOTH_PIN);
	Start_Timer();
	//GPIO_Enable_Interrupt();
		
	//GPIO_EXTICmd(GPIO_Pin_0, ENABLE);
	//printf("HEllo PC\n");
  //GPIO_EXTICmd(GPIO_Pin_0, ENABLE);
	//sent_battery = 1;
    
}/* end hci_le_connection_complete_event() */

/*******************************************************************************
 * Function Name  : hci_disconnection_complete_event.
 * Description    : This event occurs when a connection is terminated.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void hci_disconnection_complete_event(uint8_t Status,
                                      uint16_t Connection_Handle,
                                      uint8_t Reason)
{
  connected = FALSE;
	//printf("%x \n",Reason);
  /* Make the device connectable again. */
  set_connectable = TRUE;
  connection_handle = 0;
  SdkEvalLedOff(LED2);//activity led
	GPIO_ResetBits(SAMPLING_PIN);	
	//GPIO_Disable_Interrupt();
	Stop_Timer();
	
//	MFT_Cmd(MFT1, DISABLE);
//	GPIO_EXTICmd(GPIO_Pin_0, DISABLE);

}/* end hci_disconnection_complete_event() */


/*******************************************************************************
 * Function Name  : aci_gatt_read_permit_req_event.
 * Description    : This event is given when a read request is received
 *                  by the server from the client.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void aci_gatt_read_permit_req_event(uint16_t Connection_Handle,
                                    uint16_t Attribute_Handle,
                                    uint16_t Offset)
{
//	printf("%d,%d\r\n",Connection_Handle,Attribute_Handle);
  Read_Request_CB(Attribute_Handle);    
}

/*******************************************************************************
 * Function Name  : HAL_VTimerTimeoutCallback.
 * Description    : This function will be called on the expiry of 
 *                  a one-shot virtual timer.
 * Input          : See file bluenrg1_stack.h
 * Output         : See file bluenrg1_stack.h
 * Return         : See file bluenrg1_stack.h
 *******************************************************************************/
void HAL_VTimerTimeoutCallback(uint8_t timerNum)
{
  if (timerNum == SENSOR_TIMER) {
    sensorTimer_expired = TRUE;
  }

}

/*******************************************************************************
 * Function Name  : aci_gatt_attribute_modified_event.
 * Description    : This event occurs when an attribute is modified.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void aci_gatt_attribute_modified_event(uint16_t Connection_Handle,
                                       uint16_t Attr_Handle,
                                       uint16_t Offset,
                                       uint16_t Attr_Data_Length,
                                       uint8_t Attr_Data[])
{
	Attribute_Modified_CB(Attr_Handle, Attr_Data_Length, Attr_Data); 
}


void aci_hal_end_of_radio_activity_event(uint8_t Last_State,
                                         uint8_t Next_State,
                                         uint32_t Next_State_SysTime)
{

}
