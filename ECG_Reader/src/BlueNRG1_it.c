/**
  ******************************************************************************
  * @file    BlueNRG1_it.c 
  * @author  VMA RF Application Team
  * @version V1.0.0
  * @date    September-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "BlueNRG1_it.h"
#include "BlueNRG1_conf.h"
#include "ble_const.h" 
#include "bluenrg1_stack.h"
#include "SDK_EVAL_Com.h"
#include "clock.h"
#include "stdio.h"
#include "Stent_Sensor_ADC.h"
#include "Stent_Sensor_Timer.h"
#include "Stent_Sensor_GPIO.h"

#ifndef SENSOR_EMULATION
#include "lsm6ds3.h"
#include "LSM6DS3_hal.h"
#include "gatt_db.h"
#endif

#define OFFSET 5
#define DECT_MODE 0
#define CONST_MODE 1
/** @addtogroup BlueNRG1_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_Examples
  * @{
  */ 

/** @addtogroup GPIO_IOToggle
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t Raw_ADC = 0;
uint32_t Voltage = 0;
volatile BOOL interrupt_flag = FALSE;
uint32_t adcHex[2];
volatile BOOL sendPower = TRUE;
uint16_t Threshold  = 250;
uint8_t Trigger  = 0;
extern uint16_t RX_Threshold;
extern uint8_t Mode;
uint8_t count = 0;
uint8_t Buzzer_Count = 0;
BOOL Buzzer_Flag = FALSE;
BOOL Flat_Flag = FALSE;
uint32_t first_voltage = 0;
uint32_t second_voltage = 0;
uint32_t differece = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}


/**
  * @brief  This function handles SVCall exception.
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  */
void SysTick_Handler(void)
{
  SysCount_Handler(); 
}

void MFT1A_Handler(void)
{
  if (MFT_StatusIT(MFT1,MFT_IT_TNA) != RESET)
  { 	
		 MFT_ClearIT(MFT1, MFT_IT_TNA);
		
//		if (Buzzer_Flag == FALSE)
//		{
//			count++;
//			count = count%6;
//		}
		if(Buzzer_Flag == TRUE)
		{
			//count = 0;
			Buzzer_Count++;
			if(Buzzer_Count >= 5)
			{
				Buzzer_Flag = FALSE;
				Buzzer_Count = 0;
				GPIO_ResetBits(SAMPLING_PIN);
			}
		}
		
  }
	

	if(MFT_StatusIT(MFT1,MFT_IT_TNB) != RESET)
  {	
		MFT_ClearIT(MFT1, MFT_IT_TNB);
		
		//GPIO_ResetBits(SAMPLING_PIN);
		Raw_ADC = ADC_Raw_Value(ADC_Input_AdcPin1);
		Voltage = ADC_Raw_to_Voltage(ADC_Input_AdcPin1, Raw_ADC)*(1000);
		if(Voltage > 320)
		{
			GPIO_SetBits(SAMPLING_PIN);
			Buzzer_Flag = TRUE;
			Buzzer_Count = 0;
		}
		
		if(Voltage < 100)
		{
			GPIO_SetBits(SAMPLING_PIN);
//			Flat_Flag = TRUE;
		}
		memcpy(&adcHex[0], &Raw_ADC, sizeof(uint32_t));
		memcpy(&adcHex[1], &Voltage, sizeof(uint32_t));
		Adc_Update(adcHex);
//		if(count == 0)
//		{
//			first_voltage = Voltage;
//		}
//		else if(count == 5)
//		{
//			second_voltage = Voltage;
//			differece = second_voltage - first_voltage;
//			if(differece > 200)
//			{
//				GPIO_SetBits(SAMPLING_PIN);
//				Buzzer_Flag = TRUE;
//				//count = 0;
//				
//			}
//		}
//		count++;
//		count = count%6;
		
		



		
	}  
}

void GPIO_Handler(void)
{


}
/******************************************************************************/
/*                 BlueNRG-1 Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (system_BlueNRG1.c).                                               */
/******************************************************************************/
/**
* @brief  This function handles UART interrupt request.
* @param  None
* @retval None
*/
void UART_Handler(void)
{  

}

void Blue_Handler(void)
{
   // Call RAL_Isr
   RAL_Isr();
}

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
