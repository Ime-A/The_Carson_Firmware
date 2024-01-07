#include "Stent_Sensor_ADC.h"
//#include "SEGGER_RTT.h"


ADC_InitType xADC_InitType;

ErrorStatus ADC_Configuration(uint8_t input)
{
	if(ADC_SwCalibration() == SUCCESS){
	
		SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_ADC, ENABLE);

		/* Configure ADC */
		xADC_InitType.ADC_OSR = ADC_OSR_100;
		/* batteryLevelADC */
		xADC_InitType.ADC_Input = input;
		xADC_InitType.ADC_ConversionMode = ADC_ConversionMode_Single;
		xADC_InitType.ADC_ReferenceVoltage = ADC_ReferenceVoltage_0V6;
		xADC_InitType.ADC_Attenuation = ADC_Attenuation_9dB54;
		ADC_Init(&xADC_InitType);

		/* Enable auto offset correction and calibration */
		ADC_AutoOffsetUpdate(ENABLE);
		ADC_Calibration(ENABLE);
		
		return SUCCESS;
	}
	else{
		return ERROR;
	}
}


float ADC_Voltage_Value(uint8_t input)
{
	uint8_t counter = 0;
	uint16_t adc_value = 0;
	float voltage = 0.0f;
	uint8_t sample_threshold = 10;
	

	while(counter < sample_threshold){
		if ( ADC->CONF_b.CHSEL == input && ADC_GetFlagStatus(ADC_FLAG_EOC)){
			counter++;
			if(counter == sample_threshold)
			{
				adc_value = ADC_GetRawData();
			}
		}
		xADC_InitType.ADC_Input = input;
		/* Enable auto offset correction */
		ADC_SelectInput(input);
		ADC_Cmd(ENABLE);
	}

	
	voltage = ADC_ConvertSingleEndedVoltage(adc_value, input,  xADC_InitType.ADC_ReferenceVoltage, xADC_InitType.ADC_Attenuation);
	return voltage;
}


uint16_t ADC_Raw_Value(uint8_t input)
{
	uint8_t counter = 0;
	uint16_t adc_value = 0;
	uint8_t sample_threshold = 10;
	

	while(counter < sample_threshold){
		if ( ADC->CONF_b.CHSEL == input && ADC_GetFlagStatus(ADC_FLAG_EOC)){
			counter++;
			if(counter == sample_threshold)
			{
				adc_value = ADC_GetRawData();
			}
		}
		xADC_InitType.ADC_Input = input;
		/* Enable auto offset correction */
		ADC_SelectInput(input);
		ADC_Cmd(ENABLE);
	}

	return adc_value;
}


float ADC_Raw_to_Voltage(uint8_t input, uint16_t Raw_ADC_Value)
{
	return ADC_ConvertSingleEndedVoltage(Raw_ADC_Value, input,  xADC_InitType.ADC_ReferenceVoltage, xADC_InitType.ADC_Attenuation);
}
