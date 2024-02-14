#ifndef SOURCE_DRIVER_GPIO_DRIVER_H_
#define SOURCE_DRIVER_GPIO_DRIVER_H_
/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <main.h>
#include <stm32f4xx_ll_gpio.h>
#include <stdbool.h>

/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/
typedef enum {
    eGpioDriverPinFirst = 0, /* Placeholder to indicate the first element*/
    eModemPowerOffPin = eGpioDriverPinFirst,
    eGPSFixLedPin,
    eStatLedPin,
    eModemUartDtrPin,
    eModemUartRtsPin,
    eModemOnPin,
    eModemUartCtsPin,
    eGnssOnPin,
    eDebugTxPin,
    eDebugRxPin,
    eModemUartRxPin,
    eModemUartTxPin,
    eGpioPinLast, /* Placeholder to indicate the last element*/
} eGpioPinName_t;

/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
void GPIO_Driver_Init(void);
bool GPIO_Driver_WritePin(eGpioPinName_t pin_name, bool pin_state);
bool GPIO_Driver_ReadPin(eGpioPinName_t pin_name, bool *pin_state);
bool GPIO_Driver_TogglePin(eGpioPinName_t pin_name);
#endif /* SOURCE_DRIVER_GPIO_DRIVER_H_ */
