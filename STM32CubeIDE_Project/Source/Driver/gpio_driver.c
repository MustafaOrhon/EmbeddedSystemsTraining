/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <gpio_driver.h>

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
const static sGpioConfig_t static_gpio_lut[] = {
    [eModemPowerOffPin] = {
        .port = Modem_Power_Off_GPIO_Port,
        .pin_number = Modem_Power_Off_Pin,
        .pin_mode = LL_GPIO_MODE_OUTPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOE,
    },
    [eGPSFixLedPin] = {
        .port = GPS_Fix_LED_GPIO_Port,
        .pin_number = GPS_Fix_LED_Pin,
        .pin_mode = LL_GPIO_MODE_OUTPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOB
    },
    [eStatLedPin] = {
        .port = Stat_LED_GPIO_Port,
        .pin_number = Stat_LED_Pin,
        .pin_mode = LL_GPIO_MODE_OUTPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOB
    },

    [eModemUartDtrPin] = {
        .port = Modem_UART_DTR_GPIO_Port,
        .pin_number = Modem_UART_DTR_Pin,
        .pin_mode = LL_GPIO_MODE_OUTPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = 0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOD
    },
    [eModemUartRtsPin] = {
        .port = Modem_UART_RTS_GPIO_Port,
        .pin_number = Modem_UART_RTS_Pin,
        .pin_mode = LL_GPIO_MODE_OUTPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOD
    },
    [eModemOnPin] = {
        .port = Modem_ON_GPIO_Port,
        .pin_number = Modem_ON_Pin,
        .pin_mode = LL_GPIO_MODE_OUTPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOA
    },
    [eModemUartCtsPin] = {
        .port = Modem_UART_CTS_GPIO_Port,
        .pin_number = Modem_UART_CTS_Pin,
        .pin_mode = LL_GPIO_MODE_INPUT,
        .pin_otype = LL_GPIO_OUTPUT_PUSHPULL,
        .pin_pupd_control = LL_GPIO_PULL_NO,
        .pin_speed = LL_GPIO_SPEED_FREQ_LOW,
        .pin_af_mode = LL_GPIO_AF_0,
        .clock = LL_AHB1_GRP1_PERIPH_GPIOD
    },
    [eGnssOnPin] = {
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
  LL_GPIO_InitTypeDef gpio_initstruct = {0};

  for(eGpioPinName_t i = eGpioDriverPinFirst; i <= eGpioPinLast; i++){
      LL_AHB1_GRP1_EnableClock(static_gpio_lut[i].clock);
      LL_GPIO_ResetOutputPin(static_gpio_lut[i].port,static_gpio_lut[i].pin_number);
      gpio_initstruct.Mode = static_gpio_lut[i].pin_mode;
      gpio_initstruct.Pin = static_gpio_lut[i].pin_number;
      gpio_initstruct.OutputType = static_gpio_lut[i].pin_otype;
      gpio_initstruct.Pull = static_gpio_lut[i].pin_pupd_control;
      gpio_initstruct.Speed = static_gpio_lut[i].pin_speed;
      gpio_initstruct.Alternate = static_gpio_lut[i].pin_af_mode;

      LL_GPIO_Init(static_gpio_lut[i].port,&gpio_initstruct);
  }
}

bool GPIO_Driver_WritePin(eGpioPinName_t pin_name, bool pin_state) {
    if ((pin_name < eGpioDriverPinFirst) || (pin_name >= eGpioPinLast)) {
        return false;
    }

    if (pin_state) {
        LL_GPIO_SetOutputPin(static_gpio_lut[pin_name].port, static_gpio_lut[pin_name].pin_number);
    } else {
        LL_GPIO_ResetOutputPin(static_gpio_lut[pin_name].port, static_gpio_lut[pin_name].pin_number);
    }

    return true;
}



bool GPIO_Driver_ReadPin(eGpioPinName_t pin_name, bool *is_pin_set) {
    if (((pin_name < eGpioDriverPinFirst) || (pin_name >= eGpioPinLast)) || (is_pin_set == NULL)) {
        return false; // Error due to invalid pin_name or due to NULL pointer to is_pin_set
    }
    if(static_gpio_lut[pin_name].pin_mode == LL_GPIO_MODE_OUTPUT) {
        *is_pin_set = LL_GPIO_IsOutputPinSet(static_gpio_lut[pin_name].port, static_gpio_lut[pin_name].pin_number);
    }
    else {
        *is_pin_set = LL_GPIO_IsInputPinSet(static_gpio_lut[pin_name].port, static_gpio_lut[pin_name].pin_number);
    }
    return true;
}


bool GPIO_Driver_TogglePin(eGpioPinName_t pin_name) {
    if ((pin_name < eGpioDriverPinFirst) || (pin_name >= eGpioPinLast)) {
        return false;  /*Error due to invalid pin_name*/
    }
    LL_GPIO_TogglePin(static_gpio_lut[pin_name].port, static_gpio_lut[pin_name].pin_number);
    return true;
}

