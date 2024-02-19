/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_usart.h"
#include "uart_driver.h"
#include "ring_buffer.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define UART_RX_RING_BUFFER_SIZE 128
static sGenericRingBuffer_t uart_rx_ring_buffer; // CAN BE IMPLEMENTED TO UART STRUCT??
// Should be separate buffer for every uart
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
const static sUartConfig_t static_uart_lut[eUartDriverPort_Last] = {
    [eUartDriverPort_Uart1] = {
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
    [eUartDriverPort_Uart2] = {
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
//Can use dynamic LUT in runtime
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
    if ((port < eUartDriverPort_First) || (port >= eUartDriverPort_Last)) {
        return false;
    }
    //NEEDS FIX
    if (GenericRingBuffer_Init(&uart_rx_ring_buffer, sizeof(uint8_t), UART_RX_RING_BUFFER_SIZE) == false) {
        return false;
    }
    LL_USART_InitTypeDef usart_init_struct = {0};
    static_uart_lut[port].enable_clock(static_uart_lut[port].clock);
    usart_init_struct.BaudRate = (baud_rate == 0) ? static_uart_lut[port].baudrate : baud_rate;
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
    if (LL_USART_IsEnabled(static_uart_lut[port].port) == 0) {
        return false;
    }
        //put variables to static
    //NEEDS FIX!!!!
    if (port == eUartDriverPort_Uart1) {
        LL_USART_EnableIT_RXNE(USART1);
        NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0)); //Can be higher than RTOS tick prio NEED CHECK
        NVIC_EnableIRQ(USART1_IRQn);
    } else if (port == eUartDriverPort_Uart2) {

        LL_USART_EnableIT_RXNE(USART2);
        NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0));
        NVIC_EnableIRQ(USART2_IRQn);
    }
    return true;
}
bool UART_Driver_SendByte (eUartPortEnum_t port, uint8_t byte) {
    if ((port < eUartDriverPort_First) || (port >= eUartDriverPort_Last)) {
        return false;
    }
    while (LL_USART_IsActiveFlag_TXE(static_uart_lut[port].port) == 0) {};
    LL_USART_TransmitData8(static_uart_lut[port].port, byte);
    return true;
}
bool UART_Driver_SendMultipleBytes (eUartPortEnum_t port, const uint8_t *bytes, size_t size) {
    if ((port < eUartDriverPort_First) || (port >= eUartDriverPort_Last) || (bytes == NULL) || (size == 0)) {
        return false;
    }
    for (size_t i = 0; i < size; i++) {
        if (UART_Driver_SendByte(port, bytes[i]) == false) {
            return false;
        }
    }
    return true;
}
//If code is repeating u can make another function or make it universal somehow between this IRQHandlers!!!
void USART1_IRQHandler (void) {
    if (LL_USART_IsActiveFlag_RXNE(USART1) && LL_USART_IsEnabledIT_RXNE(USART1)) {
        uint8_t data = LL_USART_ReceiveData8(USART1);
        GenericRingBuffer_Write(&uart_rx_ring_buffer, &data);
    }
    // Additional USART1 interrupt handling as needed
}
void USART2_IRQHandler (void) {
    if (LL_USART_IsActiveFlag_RXNE(USART2) && LL_USART_IsEnabledIT_RXNE(USART2)) {
        uint8_t data = LL_USART_ReceiveData8(USART2);
        GenericRingBuffer_Write(&uart_rx_ring_buffer, &data);
    }
}
//Needs to two input which uart and also where to put that byte
bool UART_Driver_ReadByte (uint8_t *byte) {
    return GenericRingBuffer_Read(&uart_rx_ring_buffer, byte);
}

