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

//separate .h for ring buffer struct!!!
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

const static sUartConfig_t static_uart_lut[eUartDriverPort_Last] = {
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
//can create another lut maybe
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/

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

bool UART_Driver_Init (eUartPortEnum_t port, uint32_t baud_rate) {
    if (port < eUartDriverPort_First || port >= eUartDriverPort_Last) {
        return false;
    }
    LL_USART_InitTypeDef usart_init_struct = {0};
    static_uart_lut[port].enable_clock(static_uart_lut[port].clock);

    usart_init_struct.BaudRate = baud_rate;
    usart_init_struct.DataWidth = static_uart_lut[port].datawidth;
    usart_init_struct.StopBits = static_uart_lut[port].stopbits;
    usart_init_struct.Parity = static_uart_lut[port].parity;
    usart_init_struct.TransferDirection = static_uart_lut[port].transfer_direction;
    usart_init_struct.HardwareFlowControl = static_uart_lut[port].hardware_flow_control;
    usart_init_struct.OverSampling = static_uart_lut[port].oversampling;
    if (LL_USART_Init(static_uart_lut[port].port, &usart_init_struct) != SUCCESS) {
        return false;
    }
    LL_USART_ConfigAsyncMode(static_uart_lut[port].port);
    LL_USART_Enable(static_uart_lut[port].port);
    return true;
}

void UART_Driver_SendByte(eUartPortEnum_t port, uint8_t byte) {  //how to return bool ask
    while (!LL_USART_IsActiveFlag_TXE(static_uart_lut[port].port)); //check port numbers
    LL_USART_TransmitData8(static_uart_lut[port].port, byte);
}

void UART_Driver_SendMultipleBytes(eUartPortEnum_t port, const uint8_t *bytes, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) { //check port numbers,bytes null,size check
        UART_Driver_SendByte(port, bytes[i]);
    }
}

