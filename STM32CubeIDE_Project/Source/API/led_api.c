/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "cmsis_os.h"
#include "gpio_driver.h"
#include "led_api.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
typedef struct {
    eLedApiNameEnum_t pin;
    bool is_inverted;
} sLedProperties_t;
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
static const sLedProperties_t g_led_api_mapping[eLedApi_Last] = {
    [eLedApi_GpsFix] = {.pin = eGpioDriverPin_GPSFixLedPin, .is_inverted = true},
    [eLedApi_Status] = {.pin = eGpioDriverPin_StatLedPin, .is_inverted = false}
};
/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool LED_API_TurnOn (eLedApiNameEnum_t led) {
    if ((led < eLedApi_First) || (led >= eLedApi_Last)) {
        return false;
    }
    return GPIO_Driver_WritePin(g_led_api_mapping[led].pin, !(g_led_api_mapping[led].is_inverted));
}

bool LED_API_TurnOff (eLedApiNameEnum_t led) {
    if ((led < eLedApi_First) || (led >= eLedApi_Last)) {
        return false;
    }
    return GPIO_Driver_WritePin(g_led_api_mapping[led].pin, g_led_api_mapping[led].is_inverted);
}

bool LED_API_Toggle (eLedApiNameEnum_t led) {
    if ((led < eLedApi_First) || (led >= eLedApi_Last)) {
        return false;
    }
    return GPIO_Driver_TogglePin(g_led_api_mapping[led].pin);
}

const char *LED_API_LedEnumToString (eLedApiNameEnum_t led) {
    switch (led) {
        case eLedApi_GpsFix:
            return "GNSS Fix";
        case eLedApi_Status:
            return "Status";
        default:
            return "Unknown LED";
    }
}
