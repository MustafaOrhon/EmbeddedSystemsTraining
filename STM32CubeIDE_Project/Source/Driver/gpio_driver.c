/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <gpio_driver.h> //explain how <> differs
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
const static sGpioConfig_t g_static_gpio_lut[eGpioDriverPin_Last] = { //give size
    [eGpioDriverPin_ModemPowerOffPin] = {
        .port = Modem_Power_Off_GPIO_Port,
        .pin_number = Modem_Power_Off_Pin,
        .pin_mode = LL_GPIO_MODE_OUTPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOE,
    },
    [eGpioDriverPin_GPSFixLedPin] = {
        .port = GPS_Fix_LED_GPIO_Port,
        .pin_number = GPS_Fix_LED_Pin,
        .pin_mode = LL_GPIO_MODE_OUTPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOB
    },
    [eGpioDriverPin_StatLedPin] = {
        .port = Stat_LED_GPIO_Port,
        .pin_number = Stat_LED_Pin,
        .pin_mode = LL_GPIO_MODE_OUTPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOB
    },
    [eGpioDriverPin_ModemUartDtrPin] = {
        .port = Modem_UART_DTR_GPIO_Port,
        .pin_number = Modem_UART_DTR_Pin,
        .pin_mode = LL_GPIO_MODE_OUTPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOD
    },
    [eGpioDriverPin_ModemUartRtsPin] = {
        .port = Modem_UART_RTS_GPIO_Port,
        .pin_number = Modem_UART_RTS_Pin,
        .pin_mode = LL_GPIO_MODE_OUTPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOD
    },
    [eGpioDriverPin_ModemOnPin] = {
        .port = Modem_ON_GPIO_Port,
        .pin_number = Modem_ON_Pin,
        .pin_mode = LL_GPIO_MODE_OUTPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOA
    },
    [eGpioDriverPin_ModemUartCtsPin] = {
        .port = Modem_UART_CTS_GPIO_Port,
        .pin_number = Modem_UART_CTS_Pin,
        .pin_mode = LL_GPIO_MODE_INPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOD
    },
    [eGpioDriverPin_GnssOnPin] = {
        .port = GNSS_On_GPIO_Port,
        .pin_number = GNSS_On_Pin,
        .pin_mode = LL_GPIO_MODE_OUTPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOG
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
void GPIO_Driver_Init(void){
  LL_GPIO_InitTypeDef gpio_init_struct = {0};

  for(eGpioDriverPin_t i = eGpioDriverPin_First; i < eGpioDriverPin_Last; i++){
      LL_AHB1_GRP1_EnableClock(g_static_gpio_lut[i].clock);
      LL_GPIO_ResetOutputPin(g_static_gpio_lut[i].port,g_static_gpio_lut[i].pin_number);
      gpio_init_struct.Mode = g_static_gpio_lut[i].pin_mode;
      gpio_init_struct.Pin = g_static_gpio_lut[i].pin_number;
      gpio_init_struct.OutputType = g_static_gpio_lut[i].pin_otype;
      gpio_init_struct.Pull = g_static_gpio_lut[i].pin_pupd_control;
      gpio_init_struct.Speed = g_static_gpio_lut[i].pin_speed;
      gpio_init_struct.Alternate = g_static_gpio_lut[i].pin_af_mode;

      LL_GPIO_Init(g_static_gpio_lut[i].port,&gpio_init_struct);
  }
}

bool GPIO_Driver_WritePin(eGpioDriverPin_t pin_name, bool pin_state) {
    if ((pin_name < eGpioDriverPin_First) || (pin_name >= eGpioDriverPin_Last)) {
        return false;
    }

    if (pin_state) {
        LL_GPIO_SetOutputPin(g_static_gpio_lut[pin_name].port, g_static_gpio_lut[pin_name].pin_number);
    } else {
        LL_GPIO_ResetOutputPin(g_static_gpio_lut[pin_name].port, g_static_gpio_lut[pin_name].pin_number);
    }

    return true;
}

bool GPIO_Driver_ReadPin(eGpioDriverPin_t pin_name, bool *is_pin_set) {
    if (((pin_name < eGpioDriverPin_First) || (pin_name >= eGpioDriverPin_Last)) || (is_pin_set == NULL)) {
        return false;
    }
    if(g_static_gpio_lut[pin_name].pin_mode == LL_GPIO_MODE_OUTPUT) {
        *is_pin_set = LL_GPIO_IsOutputPinSet(g_static_gpio_lut[pin_name].port, g_static_gpio_lut[pin_name].pin_number);
    }
    else {
        *is_pin_set = LL_GPIO_IsInputPinSet(g_static_gpio_lut[pin_name].port, g_static_gpio_lut[pin_name].pin_number);
    }
    return true;
}

bool GPIO_Driver_TogglePin(eGpioDriverPin_t pin_name) {
    if ((pin_name < eGpioDriverPin_First) || (pin_name >= eGpioDriverPin_Last)) {
        return false;
    }
    LL_GPIO_TogglePin(g_static_gpio_lut[pin_name].port, g_static_gpio_lut[pin_name].pin_number);
    return true;
}
