/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <uart_driver.h>
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
typedef void (*EnableClock_t)(uint32_t periph);


typedef struct {
    USART_TypeDef   *port;
    uint32_t        baudrate;
    uint32_t        datawidth;
    uint32_t        stopbits;
    uint32_t        parity;
    uint32_t        transfer_direction;
    uint32_t        hardware_flow_control;
    uint32_t        oversampling;
    uint32_t        clock;
    EnableClock_t   enable_clock;
} sUartConfig_t;
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
const static sUartConfig_t static_uart_lut[] = {
    [eUart1] = {
        .port = USART1,
        .baudrate = 115200,
        .datawidth = LL_USART_DATAWIDTH_8B,
        .stopbits = LL_USART_STOPBITS_1,
        .parity = LL_USART_PARITY_NONE,
        .transfer_direction = LL_USART_DIRECTION_TX_RX,
        .hardware_flow_control = LL_USART_HWCONTROL_NONE,
        .oversampling = LL_USART_OVERSAMPLING_16,
        .clock = LL_APB2_GRP1_PERIPH_USART1,
        .enable_clock = LL_APB2_GRP1_EnableClock
    },
    [eUart2] = {
        .port = USART2,
        .baudrate = 115200,
        .datawidth = LL_USART_DATAWIDTH_8B,
        .stopbits = LL_USART_STOPBITS_1,
        .parity = LL_USART_PARITY_NONE,
        .transfer_direction = LL_USART_DIRECTION_TX_RX,
        .hardware_flow_control = LL_USART_HWCONTROL_NONE,
        .oversampling = LL_USART_OVERSAMPLING_16,
        .clock = LL_APB1_GRP1_PERIPH_USART2,
        .enable_clock = LL_APB1_GRP1_EnableClock
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
bool UART_Driver_Init(void){ //should take some port parameter and baud rate
  LL_USART_InitTypeDef usart_initstruct = {0};
  bool init_success = true;
  for(eUartPortEnum_t i = eUartDriverPort_First; i < eUartDriverPort_Last; i++){

      static_uart_lut[i].enable_clock(static_uart_lut[i].clock);

      usart_initstruct.BaudRate = static_uart_lut[i].baudrate;
      usart_initstruct.DataWidth = static_uart_lut[i].datawidth;
      usart_initstruct.StopBits = static_uart_lut[i].stopbits;
      usart_initstruct.Parity = static_uart_lut[i].parity;
      usart_initstruct.TransferDirection = static_uart_lut[i].transfer_direction;
      usart_initstruct.HardwareFlowControl = static_uart_lut[i].hardware_flow_control;
      usart_initstruct.OverSampling = static_uart_lut[i].oversampling;
      if (LL_USART_Init(static_uart_lut[i].port, &usart_initstruct) != SUCCESS) {
          init_success = false;
      }
      LL_USART_ConfigAsyncMode(static_uart_lut[i].port);
      LL_USART_Enable(static_uart_lut[i].port);
  }
  return init_success;
}

void UART_Driver_SendByte(eUartPortEnum_t port, uint8_t byte) {  //how to return bool ask
    while (!LL_USART_IsActiveFlag_TXE(static_uart_lut[port].port));
    LL_USART_TransmitData8(static_uart_lut[port].port, byte);
}

void UART_Driver_SendMultipleBytes(eUartPortEnum_t port, const uint8_t *bytes, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        UART_Driver_SendByte(port, bytes[i]);
    }
}

