#ifndef SOURCE_DRIVER_GPIO_DRIVER_H_
#define SOURCE_DRIVER_GPIO_DRIVER_H_
/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdbool.h>
/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/
typedef enum {
    eGpioDriverPin_First = 0,
    eGpioDriverPin_ModemPowerOffPin = eGpioDriverPin_First,
    eGpioDriverPin_GPSFixLedPin,
    eGpioDriverPin_StatLedPin,
    eGpioDriverPin_ModemUartDtrPin,
    eGpioDriverPin_ModemUartRtsPin,
    eGpioDriverPin_ModemOnPin,
    eGpioDriverPin_ModemUartCtsPin,
    eGpioDriverPin_GnssOnPin,
    eGpioDriverPin_DebugTxPin,
    eGpioDriverPin_DebugRxPin,
    eGpioDriverPin_ModemUartRxPin,
    eGpioDriverPin_ModemUartTxPin,
    eGpioDriverPin_GnssUartTxPin,
    eGpioDriverPin_GnssUartRxPin,
    eGpioDriverPin_Last,
} eGpioDriverPin_t;
/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool GPIO_Driver_Init(void);
bool GPIO_Driver_WritePin(eGpioDriverPin_t pin_name, bool pin_state);
bool GPIO_Driver_ReadPin(eGpioDriverPin_t pin_name, bool *pin_state);
bool GPIO_Driver_TogglePin(eGpioDriverPin_t pin_name);
#endif /* SOURCE_DRIVER_GPIO_DRIVER_H_ */
