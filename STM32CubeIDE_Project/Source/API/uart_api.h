#ifndef SOURCE_API_UART_API_H_
#define SOURCE_API_UART_API_H_
/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "message.h"
/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/
typedef enum {
    eUartApiPort_First = 0,
    eUartApiPort_Uart1 = eUartApiPort_First,
    eUartApiPort_Uart2,
    eUartApiPort_Last
} eUartApiPortEnum_t;
/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool UART_API_Init(eUartApiPortEnum_t port, uint32_t baud_rate, const char *delimiter, size_t delimiter_length);
bool UART_API_SendString(eUartApiPortEnum_t port, const char *str, size_t length);
bool UART_API_ReceiveMessage(eUartApiPortEnum_t port, sMessage_t *received_message, uint32_t queue_wait_time);
#endif /* SOURCE_API_UART_API_H_ */
