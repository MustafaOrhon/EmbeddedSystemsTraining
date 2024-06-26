/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "cmsis_os.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_tim.h"
#include "gpio_driver.h"
#include "led_app.h"
#include "uart_api.h"
#include "memory_api.h"
#include "cli_app.h"
#include "debug_api.h"
#include "modem_api.h"
#include "sms_app.h"
#include "sms_api.h"
#include "record_sending.h"
#include "gnss_api.h"
#include "network_app.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define DEBUG_UART_DELIMITER          ("\r")
#define DEBUG_UART_DELIMITER_LENGTH   (sizeof(DEBUG_UART_DELIMITER) - 1)
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
volatile unsigned long ulHighFrequencyTimerTicks;
/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/
static void SystemClock_Config (void);
static void TIM13_Init(void);
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static void SystemClock_Config (void) {
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
    while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_3) {
    }
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
    LL_RCC_HSI_SetCalibTrimming(16);
    LL_RCC_HSI_Enable();
    while (LL_RCC_HSI_IsReady() != 1) {
    }
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_8, 100, LL_RCC_PLLP_DIV_2);
    LL_RCC_PLL_Enable();
    while (LL_RCC_PLL_IsReady() != 1) {
    }
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {
    }
    LL_SetSystemCoreClock(100000000);
    if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK) {
        __disable_irq();
        while (1) {
        }
    }
    LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}
static void TIM13_Init(void){
    LL_TIM_InitTypeDef LL_TIM_InitStruct = {0};

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM13);

    NVIC_SetPriority(TIM8_UP_TIM13_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0));
    NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);

    LL_TIM_InitStruct.Prescaler   = 960-LL_TIM_IC_FILTER_FDIV1_N2;
    LL_TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
    LL_TIM_InitStruct.Autoreload  = 65535;
    LL_TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;

    LL_TIM_Init(TIM13,&LL_TIM_InitStruct);
    LL_TIM_DisableARRPreload(TIM13);
}
/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
int main (void) {
    HAL_Init();
    SystemClock_Config();
    TIM13_Init();
    if (osKernelInitialize() != osOK) {
        while (1);
    }
    if (Memory_API_Init() == false) {
        while(1);
    }
    if (GPIO_Driver_Init() == false) {
        while(1);
    }
    if (DEBUG_API_Init() == false) {
        while(1);
    }
    if (UART_API_Init(eUartApiPort_Debug, 115200, DEBUG_UART_DELIMITER, DEBUG_UART_DELIMITER_LENGTH) == false) {
        while(1);
    }
    if (CLI_APP_Init() == false) {
        while(1);
    }
    if (LED_APP_Init()== false) {
        while(1);
    }
    if (MODEM_API_Init() == false) {
        return false;
    }
    if (SMS_APP_Init() == false) {
        while(1);
    }
    if (RecordSending_APP_Init() == false) {
        while(1);
    }
    if (Network_APP_Init() == false) {
        while(1);
    }
    if (GNSS_API_Init() == false) {
        while(1);
    }
    if (osKernelStart() != osOK) {
        while (1);
    }
    while (1) {

    }
}
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        HAL_IncTick();
    }
}

void configureTimerForRunTimeStats(void){
    ulHighFrequencyTimerTicks = 0;
    LL_TIM_EnableIT_UPDATE(TIM13);
    LL_TIM_EnableCounter(TIM13);
}
unsigned long getRunTimeCounterValue(void){
    return ulHighFrequencyTimerTicks;
}

void TIM8_UP_TIM13_IRQHandler(void){
    if(LL_TIM_IsActiveFlag_UPDATE(TIM13)){
        ulHighFrequencyTimerTicks++;
        LL_TIM_ClearFlag_UPDATE(TIM13);
    }
}
