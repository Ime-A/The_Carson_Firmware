
#ifndef _GATT_DB_H_
#define _GATT_DB_H_
tBleStatus Add_ADC_Service(void);
tBleStatus Adc_Update(uint32_t* adcHexValue);
tBleStatus Trigger_Update(uint8_t Trigger);

//uint16_t ADC_Raw_Value_TimeOut(uint8_t input);
//uint32_t ADC_Raw_Value(uint8_t input);
//float Raw_to_Voltage(uint8_t input, uint16_t adc_value);

void Read_Request_CB(uint16_t handle);
tBleStatus Batt_Update(void);
void Attribute_Modified_CB(uint16_t handle, uint16_t data_length, uint8_t *att_data);
uint16_t flip_endianness_16bit(uint16_t value);
//extern uint16_t ADCServiceHandle, BattServiceHandle,VoltageCharHandle, ThresholdCharHandle;
#endif /* _GATT_DB_H_ */
