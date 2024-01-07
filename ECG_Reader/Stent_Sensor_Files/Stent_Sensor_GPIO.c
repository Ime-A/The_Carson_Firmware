#include "Stent_Sensor_GPIO.h"

//#define INTERRUPT_PIN GPIO_Pin_6
//#define BLUETOOTH_PIN GPIO_Pin_11
//#define SAMPLING_PIN GPIO_Pin_0


void GPIO_Interrupt_Configuration(void)
{
	
	GPIO_InitType GPIO_InitStructure;
	NVIC_InitType NVIC_InitStructure;
	GPIO_EXTIConfigType GPIO_EXTIStructure;
	
	SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO, ENABLE);	

	
	//Falling Edge Interrupt
	/*----------------------------------------*/
	GPIO_InitStructure.GPIO_Pin = INTERRUPT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Input;
  GPIO_InitStructure.GPIO_Pull = DISABLE;
  GPIO_InitStructure.GPIO_HighPwr = DISABLE;
  GPIO_Init(&GPIO_InitStructure);
	
  NVIC_InitStructure.NVIC_IRQChannel = GPIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LOW_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	
	GPIO_EXTIStructure.GPIO_Pin = INTERRUPT_PIN;
	GPIO_EXTIStructure.GPIO_IrqSense = GPIO_IrqSense_Edge;
  GPIO_EXTIStructure.GPIO_Event = IRQ_ON_FALLING_EDGE;
	
  GPIO_EXTIConfig(&GPIO_EXTIStructure);
	GPIO_ClearITPendingBit(INTERRUPT_PIN);
	/*----------------------------------------*/
	
	
	
//	// PWM Pin
//	/*----------------------------------------*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//  GPIO_InitStructure.GPIO_Mode = Serial1_Mode;
//  GPIO_InitStructure.GPIO_Pull = DISABLE;
//  GPIO_InitStructure.GPIO_HighPwr = DISABLE;
//  GPIO_Init( &GPIO_InitStructure);
//	/*----------------------------------------*/
	
	
	
//	//LED Status Pins
//	/*----------------------------------------*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Output;
//  GPIO_InitStructure.GPIO_Pull = DISABLE;
//  GPIO_InitStructure.GPIO_HighPwr = DISABLE;
//  GPIO_Init(&GPIO_InitStructure);
//	/*----------------------------------------*/	
	
}

void GPIO_LED_Status_Configuration(void)
{
		
	GPIO_InitType GPIO_InitStructure;
	
	SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO, ENABLE);	
	

	//Connection Status
	/*----------------------------------------*/
//	GPIO_InitStructure.GPIO_Pin = BLUETOOTH_PIN;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Output;
//  GPIO_InitStructure.GPIO_Pull = DISABLE;
//  GPIO_InitStructure.GPIO_HighPwr = DISABLE;
//  GPIO_Init(&GPIO_InitStructure);
	/*----------------------------------------*/

	//Sampling Status
	/*----------------------------------------*/
	GPIO_InitStructure.GPIO_Pin = SAMPLING_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Output;
  GPIO_InitStructure.GPIO_Pull = DISABLE;
  GPIO_InitStructure.GPIO_HighPwr = DISABLE;
  GPIO_Init(&GPIO_InitStructure);
	/*----------------------------------------*/
}

void GPIO_Enable_Interrupt(void)
{
	GPIO_EXTICmd(INTERRUPT_PIN, ENABLE);
}


void GPIO_Disable_Interrupt(void)
{
	GPIO_EXTICmd(INTERRUPT_PIN, DISABLE);
}
