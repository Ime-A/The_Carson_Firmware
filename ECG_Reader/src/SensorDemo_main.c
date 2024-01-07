/******************** (C) COPYRIGHT 2018 STMicroelectronics ********************
* File Name          : SensorDemo_main.c
* Author             : AMS - RF Application team
* Version            : V1.2.0
* Date               : 03-December-2018
* Description        : BlueNRG-1-2 Sensor Demo main file 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/** @addtogroup BlueNRG1_demonstrations_applications
 * BlueNRG-1,2 SensorDemo \see SensorDemo_main.c for documentation.
 *
 *@{
 */

/** @} */
/** \cond DOXYGEN_SHOULD_SKIP_THIS
 */
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "BlueNRG1_it.h"
#include "BlueNRG1_conf.h"
#include "ble_const.h" 
#include "bluenrg1_stack.h"
#include "SDK_EVAL_Config.h"
#include "sleep.h"
#include "sensor.h"
#include "SensorDemo_config.h"
#include "OTA_btl.h"  
#include "gatt_db.h"
#include <stdio.h>
#include "Stent_Sensor_ADC.h"
#include "Stent_Sensor_Timer.h"
#include "Stent_Sensor_GPIO.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DECT_MODE 0
#define CONST_MODE 1
#define BLE_SENSOR_VERSION_STRING "1.0.0" 
uint32_t batt_voltage = 0;
uint8_t Mode = DECT_MODE;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int main(void) 
{
  uint8_t ret;

  /* System Init */
  SystemInit();
  
  /* Identify BlueNRG1 platform */
  SdkEvalIdentification();

  /* Configure I/O communication channel */
  SdkEvalComUartInit(UART_BAUDRATE);

  /* BlueNRG-1,2 stack init */
  ret = BlueNRG_Stack_Initialization(&BlueNRG_Stack_Init_params);
  if (ret != BLE_STATUS_SUCCESS) {

    while(1);
  }
  

	  /* Application demo Led Init */
  SdkEvalLedInit(LED2); //Activity led
  SdkEvalLedInit(LED3); //Error led
	SdkEvalLedInit(LED1);
  SdkEvalLedOff(LED2);
  SdkEvalLedOff(LED3);
	SdkEvalLedOff(LED1);
	

  

  SdkEvalLedOn(LED1);
  /* Sensor Device Init */
  ret = Sensor_DeviceInit();
  if (ret != BLE_STATUS_SUCCESS) {
    SdkEvalLedOn(LED3);
    while(1);
  }
	
	if(ADC_Configuration(ADC_Input_AdcPin2) != SUCCESS){
//		printf("Error Initializing ADC \n");
	}
		
//	GPIO_Interrupt_Configuration();
	MFT_Configuration();
	GPIO_LED_Status_Configuration();
//	printf("Initialized \n");
//	batt_voltage = (ADC_Voltage_Value(ADC_Input_AdcPin2)*1000);

	
		

  while(1)
  {
    /* BLE Stack Tick */
    BTLE_StackTick();

    /* Application Tick */
    APP_Tick();
    
    /* Power Save management */
    //BlueNRG_Sleep(SLEEPMODE_NOTIMER, 0, 0); 
    
  }/* while (1) */
}



/* Hardware Error event. 
   This event is used to notify the Host that a hardware failure has occurred in the Controller. 
   Hardware_Code Values:
   - 0x01: Radio state error
   - 0x02: Timer overrun error
   - 0x03: Internal queue overflow error
   After this event is recommended to force device reset. */

void hci_hardware_error_event(uint8_t Hardware_Code)
{
   NVIC_SystemReset();
}

/**
  * This event is generated to report firmware error informations.
  * FW_Error_Type possible values: 
  * Values:
  - 0x01: L2CAP recombination failure
  - 0x02: GATT unexpected response
  - 0x03: GATT unexpected request
    After this event with error type (0x01, 0x02, 0x3) it is recommended to disconnect. 
*/
void aci_hal_fw_error_event(uint8_t FW_Error_Type,
                            uint8_t Data_Length,
                            uint8_t Data[])
{
  if (FW_Error_Type <= 0x03)
  {
    uint16_t connHandle;
    
    /* Data field is the connection handle where error has occurred */
    connHandle = LE_TO_HOST_16(Data);
    
    aci_gap_terminate(connHandle, BLE_ERROR_TERMINATED_REMOTE_USER); 
  }
}

/****************** BlueNRG-1,2 Sleep Management Callback ********************************/

SleepModes App_SleepMode_Check(SleepModes sleepMode)
{
//  if(request_free_fall_notify || SdkEvalComIOTxFifoNotEmpty() || SdkEvalComUARTBusy())
//    return SLEEPMODE_RUNNING;
  
  return SLEEPMODE_RUNNING;
}

/***************************************************************************************/

/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
 //   printf("123");
    /* Infinite loop */
    while (1)
    {}
}


/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
/** \endcond
 */
