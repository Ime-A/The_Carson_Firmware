#include <stdio.h>
#include "BlueNRG1_conf.h"
#include "SDK_EVAL_Config.h"

ErrorStatus ADC_Configuration(uint8_t input);
float ADC_Voltage_Value(uint8_t input);
uint16_t ADC_Raw_Value(uint8_t input);
float ADC_Raw_to_Voltage(uint8_t input, uint16_t Raw_ADC_Value);
