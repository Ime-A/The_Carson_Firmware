//#include "Stent_Sensor.h"
//#include "SEGGER_RTT.h"
//#define PWM0_PIN			GPIO_Pin_2
//#define BATTERY_VOLTAGE_CHANNEL		ADC_Input_AdcPin1


//NVIC_InitType NVIC_InitStructure;
//GPIO_InitType GPIO_InitStructure;
//ADC_InitType xADC_InitType;
//MFT_InitType timer_init;



//ErrorStatus ADC_Configuration(uint8_t input)
//{
//	if(ADC_SwCalibration() == SUCCESS){
//	
//		SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_ADC, ENABLE);

//		/* Configure ADC */
//		xADC_InitType.ADC_OSR = ADC_OSR_32;
//		/* batteryLevelADC */
//		xADC_InitType.ADC_Input = input;
//		xADC_InitType.ADC_ConversionMode = ADC_ConversionMode_Single;
//		xADC_InitType.ADC_ReferenceVoltage = ADC_ReferenceVoltage_0V6;
//		xADC_InitType.ADC_Attenuation = ADC_Attenuation_9dB54;
//		ADC_Init(&xADC_InitType);

//		/* Enable auto offset correction and calibration */
//		ADC_AutoOffsetUpdate(ENABLE);
//		ADC_Calibration(ENABLE);
//		
//		return SUCCESS;
//	}
//	else{
//		return ERROR;
//	}
//}


//void GPIO_Configuration(void)
//{
//	SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO, ENABLE);	
//	
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Input;
//  GPIO_InitStructure.GPIO_Pull = DISABLE;
//  GPIO_InitStructure.GPIO_HighPwr = DISABLE;
//  GPIO_Init(&GPIO_InitStructure);
//	
//  NVIC_InitStructure.NVIC_IRQChannel = GPIO_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LOW_PRIORITY;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//	
//	GPIO_EXTIConfigType GPIO_EXTIStructure;
//	GPIO_EXTIStructure.GPIO_Pin = GPIO_Pin_6;
//	GPIO_EXTIStructure.GPIO_IrqSense = GPIO_IrqSense_Edge;
//  GPIO_EXTIStructure.GPIO_Event = IRQ_ON_FALLING_EDGE;
//  GPIO_EXTIConfig(&GPIO_EXTIStructure);
//	
//	
//	GPIO_InitStructure.GPIO_Pin = PWM0_PIN;
//  GPIO_InitStructure.GPIO_Mode = Serial1_Mode;
//  GPIO_InitStructure.GPIO_Pull = DISABLE;
//  GPIO_InitStructure.GPIO_HighPwr = DISABLE;
//  GPIO_Init( &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Output;
//  GPIO_InitStructure.GPIO_Pull = DISABLE;
//  GPIO_InitStructure.GPIO_HighPwr = DISABLE;
//  GPIO_Init(&GPIO_InitStructure);
//	
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Output;
//  GPIO_InitStructure.GPIO_Pull = DISABLE;
//  GPIO_InitStructure.GPIO_HighPwr = DISABLE;
//  GPIO_Init(&GPIO_InitStructure);
//}


//void MFT_Configuration(void)
//{

//  
//  SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_MTFX1 | CLOCK_PERIPH_MTFX2, ENABLE);
//  
//  MFT_StructInit(&timer_init);
//  
//  timer_init.MFT_Mode = MFT_MODE_1;
//  
//	
//	timer_init.MFT_Prescaler = 160-1;   /* 5 us */   
// 
//	
//  /* MFT1 configuration */
//  timer_init.MFT_Clock1 = MFT_PRESCALED_CLK;
//  timer_init.MFT_Clock2 = MFT_NO_CLK;

//	timer_init.MFT_CRA = 400 - 1;       /* 2 ms positive duration */
//  timer_init.MFT_CRB = 4000 - 1;        /* 20 ms positive duration */


//  MFT_Init(MFT1, &timer_init);
//  

//    
//  /* Enable MFT2 Interrupt 1 */
//  NVIC_InitStructure.NVIC_IRQChannel = MFT1A_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LOW_PRIORITY;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//	

//	MFT_TnXEN(MFT1, MFT_TnA, ENABLE);
//	MFT_EnableIT(MFT1, MFT_IT_TNA, ENABLE);
//}


//uint16_t ADC_Raw_Value_TimeOut(uint8_t input)
//{

//	uint16_t raw_data;
//	//uint16_t count = 0;
//	//uint16_t timeout = 10000;
//	
//	//xADC_InitType.ADC_Input = input;
//	ADC_SelectInput(input);
//	ADC_Cmd(ENABLE);

//		while(1){
//			
//		if(ADC->CONF_b.CHSEL == input && ADC_GetFlagStatus(ADC_FLAG_EOC))
//			{
//				raw_data = ADC_GetRawData();
//				return raw_data;	
//			}
//			
//			//count++;
//			
//	}
//	//ADC_Cmd(DISABLE);	
//	

//	return 0;
//}

//float Raw_to_Voltage(uint8_t input, uint16_t adc_value)
//{
//	
//	float voltage = 0;
//	 
//	voltage = ADC_ConvertSingleEndedVoltage(adc_value, input,  xADC_InitType.ADC_ReferenceVoltage, xADC_InitType.ADC_Attenuation);
//	
//	return voltage;
//}


//float ADC_Voltage_Value(uint8_t input)
//{
//	uint8_t counter = 0;
//	uint16_t adc_value = 0;
//	float voltage = 0.0f;
//	uint8_t sample_threshold = 12;
//	

//	while(counter < sample_threshold){
//		if ( ADC->CONF_b.CHSEL == input && ADC_GetFlagStatus(ADC_FLAG_EOC)){
//			counter++;
//			if(counter == sample_threshold - 1)
//			{
//				adc_value = ADC_GetRawData();
//			}
//		}
//		xADC_InitType.ADC_Input = input;
//		/* Enable auto offset correction */
//		ADC_SelectInput(input);
//		ADC_Cmd(ENABLE);
//	}

//	
//	voltage = ADC_ConvertSingleEndedVoltage(adc_value, input,  xADC_InitType.ADC_ReferenceVoltage, xADC_InitType.ADC_Attenuation);
//	return voltage;
//}


//uint16_t ADC_Raw_Value(uint8_t input)
//{
//	uint8_t counter = 0;
//	uint16_t adc_value = 0;
//	uint16_t raw_data_list[16];
//	printf("-----------\n");
//	while(counter<sizeof(raw_data_list)/sizeof(uint16_t)){
//		if ( ADC->CONF_b.CHSEL == input && ADC_GetFlagStatus(ADC_FLAG_EOC)){
//			/* Read converted data */
//			uint16_t raw_data = ADC_GetRawData();
//			printf("%x \n",raw_data);
//			raw_data_list[counter] = raw_data;
//			//ADC_ConvertSingleEndedVoltage(raw_data , input,  xADC_InitType.ADC_ReferenceVoltage, xADC_InitType.ADC_Attenuation);
//			counter++;
//		}
//		xADC_InitType.ADC_Input = input;
//		/* Enable auto offset correction */
//		ADC_SelectInput(input);
//		ADC_Cmd(ENABLE);
//	}
//	printf("-----------\n");
//	for (uint8_t i = 8;i<sizeof(raw_data_list)/sizeof(uint16_t);i++){
//		
//		adc_value += raw_data_list[i];
//	}
//	return adc_value / 8;
//}


//float ADC_SampleBatteryVoltage(void)
//{
//	float adcReading = 0.0f;
//	uint8_t counter = 0;
//	/* Change the input channel */
//	ADC_SelectInput(BATTERY_VOLTAGE_CHANNEL);
//	ADC_Cmd(ENABLE);
//	while(counter < 10){
//		if(ADC->CONF_b.CHSEL == BATTERY_VOLTAGE_CHANNEL && ADC_GetFlagStatus(ADC_FLAG_EOC)){
//			counter++;
//			if(counter == 10){
//				adcReading = ADC_CompensateOutputValue(ADC_GetConvertedData(xADC_InitType.ADC_Input,
//						xADC_InitType.ADC_ReferenceVoltage));
//			}
//		}
//		ADC_Cmd(ENABLE);
//	}
//	return adcReading;
//}

//float ADC_Switch_Reading(uint8_t input)
//{
//	float adcReading = 0.0f;
//	uint8_t counter = 0;
//	/* Change the input channel */
//	ADC_SelectInput(input);
//	ADC_Cmd(ENABLE);
//	while(counter < 10){
//		if(ADC->CONF_b.CHSEL == input && ADC_GetFlagStatus(ADC_FLAG_EOC)){
//			counter++;
//			if(counter == 10){
//				adcReading = ADC_CompensateOutputValue(ADC_GetConvertedData(xADC_InitType.ADC_Input,
//						xADC_InitType.ADC_ReferenceVoltage));
//			}
//		}
//		ADC_Cmd(ENABLE);
//	}
//	return adcReading;
//}


//uint16_t flip_endianness_16bit(uint16_t value)
//{
//    // Extract the low byte (LSB) and the high byte (MSB)
//    uint8_t low_byte = value & 0xFF;
//    uint8_t high_byte = (value >> 8) & 0xFF;

//    // Swap the high and low bytes
//    uint16_t flipped_value = (low_byte << 8) | high_byte;

//    return flipped_value;
//}

