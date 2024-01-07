#include <stdio.h>
#include "BlueNRG1_conf.h"
#include "SDK_EVAL_Config.h"

#define INTERRUPT_PIN GPIO_Pin_6
#define BLUETOOTH_PIN GPIO_Pin_11
#define SAMPLING_PIN GPIO_Pin_0


void GPIO_Interrupt_Configuration(void);
void GPIO_LED_Status_Configuration(void);
void GPIO_Enable_Interrupt(void);
void GPIO_Disable_Interrupt(void);
