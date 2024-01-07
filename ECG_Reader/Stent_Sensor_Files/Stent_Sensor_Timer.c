#include "Stent_Sensor_Timer.h"




void MFT_Configuration(void)
{

  MFT_InitType timer_init;
	NVIC_InitType NVIC_InitStructure;
	GPIO_InitType GPIO_InitStructure;
	
  SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_MTFX1 | CLOCK_PERIPH_MTFX2, ENABLE);
  
  MFT_StructInit(&timer_init);
  
  timer_init.MFT_Mode = MFT_MODE_1;
  
	
	timer_init.MFT_Prescaler = 160-1;   /* 5 us */   
 
	
  /* MFT1 configuration */
  timer_init.MFT_Clock1 = MFT_PRESCALED_CLK;
  timer_init.MFT_Clock2 = MFT_NO_CLK;

	timer_init.MFT_CRA = 400 - 1;       /* 2 ms positive duration */
  timer_init.MFT_CRB = 4000 - 1;        /* 5 ms positive duration */


  MFT_Init(MFT1, &timer_init);
  

    
  /* Enable MFT2 Interrupt 1 */
  NVIC_InitStructure.NVIC_IRQChannel = MFT1A_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LOW_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = Serial1_Mode;
  GPIO_InitStructure.GPIO_Pull = DISABLE;
  GPIO_InitStructure.GPIO_HighPwr = DISABLE;
  GPIO_Init( &GPIO_InitStructure);

	MFT_TnXEN(MFT1, MFT_TnA, ENABLE);
	MFT_EnableIT(MFT1, MFT_IT_TNA, ENABLE);
}

void Start_Timer(void)
{
	MFT_Cmd(MFT1, ENABLE);
}

void Stop_Timer(void)
{
	MFT_Cmd(MFT1, DISABLE);
}
