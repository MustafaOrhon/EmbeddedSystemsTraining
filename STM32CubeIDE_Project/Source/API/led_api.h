#ifndef SOURCE_API_LED_API_H_
#define SOURCE_API_LED_API_H_
/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdbool.h>
/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/
typedef enum {
    eLedApi_First,
    eLedApi_GpsFix = eLedApi_First,
    eLedApi_Status,
    eLedApi_Last
} eLedApiNameEnum_t;
/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool LED_API_TurnOn(eLedApiNameEnum_t led);
bool LED_API_TurnOff(eLedApiNameEnum_t led);
bool LED_API_Toggle(eLedApiNameEnum_t led);
const char *LED_API_LedEnumToString(eLedApiNameEnum_t led);
#endif /* SOURCE_API_LED_API_H_ */
