/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_bus.h"
#include "gpio_driver.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
typedef struct {
    GPIO_TypeDef *port ;
    uint32_t pin_number;
    uint32_t pin_mode;
    uint32_t pin_otype;
    uint32_t pin_pupd_control;
    uint32_t pin_speed;
    uint32_t pin_af_mode;
    uint32_t clock;
} sGpioConfig_t;
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
const static sGpioConfig_t g_static_gpio_lut[eGpioDriverPin_Last] = {
    [eGpioDriverPin_ModemPowerOffPin] = {
        .port = GPIOE,
        .pin_number = LL_GPIO_PIN_1,
        .pin_mode = LL_GPIO_MODE_OUTPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOE,
    },
    [eGpioDriverPin_GPSFixLedPin] = {
        .port = GPIOB,
        .pin_number = LL_GPIO_PIN_4,
        .pin_mode = LL_GPIO_MODE_OUTPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOB
    },
    [eGpioDriverPin_StatLedPin] = {
        .port = GPIOB,
        .pin_number = LL_GPIO_PIN_5,
        .pin_mode = LL_GPIO_MODE_OUTPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOB
    },
    [eGpioDriverPin_ModemUartDtrPin] = {
        .port = GPIOD,
        .pin_number = LL_GPIO_PIN_7,
        .pin_mode = LL_GPIO_MODE_OUTPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOD
    },
    [eGpioDriverPin_ModemUartRtsPin] = {
        .port = GPIOD,
        .pin_number = LL_GPIO_PIN_3,
        .pin_mode = LL_GPIO_MODE_OUTPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOD
    },
    [eGpioDriverPin_ModemOnPin] = {
        .port = GPIOA,
        .pin_number = LL_GPIO_PIN_15,
        .pin_mode = LL_GPIO_MODE_OUTPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOA
    },
    [eGpioDriverPin_ModemUartCtsPin] = {
        .port = GPIOD,
        .pin_number = LL_GPIO_PIN_4,
        .pin_mode = LL_GPIO_MODE_INPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOD
    },
    [eGpioDriverPin_GnssOnPin] = {
        .port = GPIOG,
        .pin_number = LL_GPIO_PIN_7,
        .pin_mode = LL_GPIO_MODE_OUTPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOG
    },
    [eDebugTxPin] = {
        .port = Debug_TX_GPIO_Port,
        .pin_number = Debug_TX_Pin,
        .pin_mode = LL_GPIO_MODE_ALTERNATE,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
        .pin_af_mode = LL_GPIO_AF_7,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOB
    },
    [eDebugRxPin] = {
        .port = Debug_RX_GPIO_Port,
        .pin_number = Debug_RX_Pin,
        .pin_mode = LL_GPIO_MODE_ALTERNATE,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
        .pin_af_mode = LL_GPIO_AF_7,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOB
    },
    [eModemUartRxPin] = {
        .port = Modem_UART_RX_GPIO_Port,
        .pin_number = Modem_UART_RX_Pin,
        .pin_mode = LL_GPIO_MODE_ALTERNATE,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
        .pin_af_mode = LL_GPIO_AF_7,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOD
    },
    [eModemUartTxPin] = {
        .port = Modem_UART_TX_GPIO_Port,
        .pin_number = Modem_UART_TX_Pin,
        .pin_mode = LL_GPIO_MODE_ALTERNATE,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
        .pin_af_mode = LL_GPIO_AF_7,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOD
    }

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
bool GPIO_Driver_Init (void) {
    LL_GPIO_InitTypeDef gpio_init_struct = {0};
    bool init_success = true;
    for (eGpioDriverPin_t gpio = eGpioDriverPin_First; gpio < eGpioDriverPin_Last; gpio++) {
        LL_AHB1_GRP1_EnableClock(g_static_gpio_lut[gpio].clock);
        LL_GPIO_ResetOutputPin(g_static_gpio_lut[gpio].port, g_static_gpio_lut[gpio].pin_number);
        gpio_init_struct.Mode = g_static_gpio_lut[gpio].pin_mode;
        gpio_init_struct.Pin = g_static_gpio_lut[gpio].pin_number;
        gpio_init_struct.OutputType = g_static_gpio_lut[gpio].pin_otype;
        gpio_init_struct.Pull = g_static_gpio_lut[gpio].pin_pupd_control;
        gpio_init_struct.Speed = g_static_gpio_lut[gpio].pin_speed;
        gpio_init_struct.Alternate = g_static_gpio_lut[gpio].pin_af_mode;
        if (LL_GPIO_Init(g_static_gpio_lut[gpio].port, &gpio_init_struct) != SUCCESS) {
            init_success = false;
        }
    }
    return init_success;
}

bool GPIO_Driver_WritePin (eGpioDriverPin_t pin_name, bool pin_state) {
    if ((pin_name < eGpioDriverPin_First) || (pin_name >= eGpioDriverPin_Last)) {
        return false;
    }
    if (g_static_gpio_lut[pin_name].pin_mode != LL_GPIO_MODE_OUTPUT) {
        return false;
    }
    if (pin_state) {
        LL_GPIO_SetOutputPin(g_static_gpio_lut[pin_name].port, g_static_gpio_lut[pin_name].pin_number);
    } else {
        LL_GPIO_ResetOutputPin(g_static_gpio_lut[pin_name].port, g_static_gpio_lut[pin_name].pin_number);
    }
    return true;
}

bool GPIO_Driver_ReadPin (eGpioDriverPin_t pin_name, bool *is_pin_set) {
    if ((pin_name < eGpioDriverPin_First) || (pin_name >= eGpioDriverPin_Last) || (is_pin_set == NULL)) {
        return false;
    }
    if (g_static_gpio_lut[pin_name].pin_mode == LL_GPIO_MODE_OUTPUT) {
        *is_pin_set = LL_GPIO_IsOutputPinSet(g_static_gpio_lut[pin_name].port, g_static_gpio_lut[pin_name].pin_number);
    }
    else {
        *is_pin_set = LL_GPIO_IsInputPinSet(g_static_gpio_lut[pin_name].port, g_static_gpio_lut[pin_name].pin_number);
    }
    return true;
}

bool GPIO_Driver_TogglePin (eGpioDriverPin_t pin_name) {
    if ((pin_name < eGpioDriverPin_First) || (pin_name >= eGpioDriverPin_Last)) {
        return false;
    }
    if (g_static_gpio_lut[pin_name].pin_mode != LL_GPIO_MODE_OUTPUT) {
        return false;
    }
    LL_GPIO_TogglePin(g_static_gpio_lut[pin_name].port, g_static_gpio_lut[pin_name].pin_number);
    return true;
}
