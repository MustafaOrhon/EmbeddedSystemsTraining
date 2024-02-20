/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdlib.h>
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_usart.h"
#include "ring_buffer.h"
#include "uart_driver.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define UART_RX_RING_BUFFER_SIZE 128
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
    IRQn_Type       irq_number;
    EnableClock_t   enable_clock;
} sUartStaticConfig_t;
typedef struct {
    sRingBuffer_t*  rx_ring_buffer;
} sUartDynamicConfig_t;
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
static const sUartStaticConfig_t g_uart_static_lut[eUartDriverPort_Last] = {
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
        .irq_number = USART1_IRQn,
        .enable_clock = LL_APB2_GRP1_EnableClock,
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
        .irq_number = USART2_IRQn,
        .enable_clock = LL_APB1_GRP1_EnableClock,
    }
};
/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static sUartDynamicConfig_t g_uart_dynamic_lut[eUartDriverPort_Last];
/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/
static void UniversalUsartIRQHandler(eUartPortEnum_t port);
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static void UniversalUsartIRQHandler(eUartPortEnum_t port) {
    if (g_uart_static_lut[port].port && g_uart_dynamic_lut[port].rx_ring_buffer) {
        if (LL_USART_IsActiveFlag_RXNE(g_uart_static_lut[port].port) && LL_USART_IsEnabledIT_RXNE(g_uart_static_lut[port].port)) {
            uint8_t data = LL_USART_ReceiveData8(g_uart_static_lut[port].port);
            Ring_Buffer_Write(g_uart_dynamic_lut[port].rx_ring_buffer, data);
        }
    }
}


void USART1_IRQHandler(void) {
    UniversalUsartIRQHandler(eUartDriverPort_Uart1);
}
void USART2_IRQHandler(void) {
    UniversalUsartIRQHandler(eUartDriverPort_Uart2);
}
/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool UART_Driver_Init (eUartPortEnum_t port, uint32_t baud_rate) {
    if ((port < eUartDriverPort_First) || (port >= eUartDriverPort_Last)) {
        return false;
    }
    g_uart_dynamic_lut[port].rx_ring_buffer = Ring_Buffer_Init(UART_RX_RING_BUFFER_SIZE);
    if (g_uart_dynamic_lut[port].rx_ring_buffer == NULL) {
        return false;
    }
    LL_USART_InitTypeDef usart_init_struct = {0};
    g_uart_static_lut[port].enable_clock(g_uart_static_lut[port].clock);
    usart_init_struct.BaudRate = (baud_rate == 0) ? g_uart_static_lut[port].baudrate : baud_rate;
    usart_init_struct.DataWidth = g_uart_static_lut[port].datawidth;
    usart_init_struct.StopBits = g_uart_static_lut[port].stopbits;
    usart_init_struct.Parity = g_uart_static_lut[port].parity;
    usart_init_struct.TransferDirection = g_uart_static_lut[port].transfer_direction;
    usart_init_struct.HardwareFlowControl = g_uart_static_lut[port].hardware_flow_control;
    usart_init_struct.OverSampling = g_uart_static_lut[port].oversampling;
    if (LL_USART_Init(g_uart_static_lut[port].port, &usart_init_struct) != SUCCESS) {
        return false;
    }
    LL_USART_ConfigAsyncMode(g_uart_static_lut[port].port);
    LL_USART_Enable(g_uart_static_lut[port].port);
    if (LL_USART_IsEnabled(g_uart_static_lut[port].port) == false) {
        return false;
    }
    LL_USART_EnableIT_RXNE(g_uart_static_lut[port].port);
    NVIC_SetPriority(g_uart_static_lut[port].irq_number, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0));
    NVIC_EnableIRQ(g_uart_static_lut[port].irq_number);
    return true;
}
bool UART_Driver_SendByte (eUartPortEnum_t port, uint8_t byte) {
    if ((port < eUartDriverPort_First) || (port >= eUartDriverPort_Last)) {
        return false;
    }
    while (LL_USART_IsActiveFlag_TXE(g_uart_static_lut[port].port) == 0) {};
    LL_USART_TransmitData8(g_uart_static_lut[port].port, byte);
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
bool UART_Driver_ReadByte (eUartPortEnum_t port, uint8_t *byte) {
    if (byte == NULL || port < eUartDriverPort_First || port >= eUartDriverPort_Last) {
        return false;
    }
    if (g_uart_dynamic_lut[port].rx_ring_buffer == NULL) {
        return false;
    }
    return Ring_Buffer_Read(g_uart_dynamic_lut[port].rx_ring_buffer, byte);
}

