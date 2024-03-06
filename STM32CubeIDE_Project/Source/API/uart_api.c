/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <string.h>
#include "cmsis_os.h"
#include "memory_api.h"
#include "uart_driver.h"
#include "uart_api.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define MESSAGE_QUEUE_SIZE          5
#define UART1_MAX_MESSAGE_SIZE      32
#define UART2_MAX_MESSAGE_SIZE      1024
#define MSG_QUEUE_PUT_TMO                    100
#define MAX_DELIMITER_LENGTH                 10
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
typedef enum {
    eUartApiState_First,
    eUartApiState_Initialize = eUartApiState_First,
    eUartApiState_CollectData,
    eUartApiState_FlushData
} eUartApiState_t;

typedef struct {
    eUartApiPortEnum_t driver_port;
    char *queue_name;
    uint16_t max_message_size;
} sUartApiDesc_t;

typedef struct {
    osMessageQueueId_t message_queue_id;
    char *buffer;
    size_t index;
    eUartApiState_t state;
    bool is_initialized;
    char *delimiter;
    char delimiter_length;
} sUartApiWorkData_t;
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
static const sUartApiDesc_t g_uart_api_static_lut[eUartApiPort_Last] = {
    [eUartApiPort_Uart1] = {
        .driver_port = eUartDriverPort_Uart1,
        .queue_name = "Uart-1 Queue",
        .max_message_size = UART1_MAX_MESSAGE_SIZE},
    [eUartApiPort_Uart2] = {
        .driver_port = eUartDriverPort_Uart2,
        .queue_name = "Uart-2 Queue",
        .max_message_size = UART2_MAX_MESSAGE_SIZE}
};
static const osThreadAttr_t g_uart_api_thread_attr = {
    .name = "UART API Thread",
    .stack_size = 512,
    .priority = osPriorityNormal,
};
static const osMutexAttr_t g_uart_api_mutex_attr = {
    .name = "Uart API Mutex",
    .attr_bits = osMutexRecursive,
    NULL,
    0U
};
/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static sUartApiWorkData_t g_uart_api_dynamic_lut[eUartApiPort_Last] = {
    [eUartApiPort_Uart1] = {
        .buffer = NULL,
        .index = 0,
        .state = eUartApiState_Initialize,
        .is_initialized = false,
        .delimiter = NULL},
    [eUartApiPort_Uart2] = {
        .buffer = NULL,
        .index = 0,
        .state = eUartApiState_Initialize,
        .is_initialized = false,
        .delimiter = NULL},
};
static osThreadId_t g_uart_api_thread_id = NULL;
static osMutexId_t g_uart_api_mutex_id = NULL;
/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/
static void UART_API_Thread(void *argument);
static bool UART_API_CheckDelimiter(const sUartApiWorkData_t *work_data);
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static bool UART_API_CheckDelimiter(const sUartApiWorkData_t *work_data) {
    if ((work_data->index < work_data->delimiter_length) || (work_data->buffer == NULL) || (work_data == NULL)) {
        return false;
    }
    const char *start_index = work_data->buffer + work_data->index - work_data->delimiter_length;
    return strncmp(start_index, work_data->delimiter, work_data->delimiter_length) == 0;
}
/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool UART_API_Init (eUartApiPortEnum_t port, uint32_t baud_rate, const char *delimiter, size_t delimiter_length) {
    if ((port < eUartApiPort_First) || (port >= eUartApiPort_Last)) {
        return false;
    }
    if ((delimiter == NULL) || (delimiter_length == 0) || (delimiter_length >= MAX_DELIMITER_LENGTH)) {
        return false;
    }
    if (UART_Driver_Init(g_uart_api_static_lut[port].driver_port, baud_rate) == false) {
        return false;
    }
    g_uart_api_dynamic_lut[port].delimiter = (char *)Memory_API_Calloc(delimiter_length,sizeof(char));
    if (g_uart_api_dynamic_lut[port].delimiter == NULL) {
        return false;
    }
    memcpy(g_uart_api_dynamic_lut[port].delimiter, delimiter, delimiter_length);
    g_uart_api_dynamic_lut[port].delimiter_length = delimiter_length;
    osMessageQueueAttr_t queue_attr = {.name = g_uart_api_static_lut[port].queue_name};
    g_uart_api_dynamic_lut[port].message_queue_id = osMessageQueueNew(MESSAGE_QUEUE_SIZE, sizeof(sMessage_t), &queue_attr);
    if (g_uart_api_dynamic_lut[port].message_queue_id == NULL) {
        return false;
    }
    if (g_uart_api_mutex_id == NULL) {
        g_uart_api_mutex_id = osMutexNew(&g_uart_api_mutex_attr);
        if (g_uart_api_mutex_id == NULL) {
            return false;
        }
    }
    if (g_uart_api_thread_id == NULL) {
        g_uart_api_thread_id = osThreadNew(UART_API_Thread, NULL, &g_uart_api_thread_attr);
        if (g_uart_api_thread_id == NULL) {
            return false;
        }
    }
    g_uart_api_dynamic_lut[port].is_initialized = true;
    return true;
}

bool UART_API_SendString (eUartApiPortEnum_t port, const char *str, size_t length) {
    if ((port < eUartApiPort_First) || (port >= eUartApiPort_Last) || (str == NULL) || (length == 0)) {
        return false;
    }
    if (g_uart_api_mutex_id == NULL) {
        return false;
    }
    osStatus_t status = osMutexAcquire(g_uart_api_mutex_id, osWaitForever);
    if (status != osOK) {
        return false;
    }
    bool result = UART_Driver_SendMultipleBytes(g_uart_api_static_lut[port].driver_port, (const uint8_t *)str, length);
    status = osMutexRelease(g_uart_api_mutex_id);
    if (status != osOK) {
        return false;
    }
    return result;
}

bool UART_API_ReceiveMessage (eUartApiPortEnum_t port, sMessage_t *received_message, uint32_t queue_wait_time) {
    if ((port < eUartApiPort_First) || (port >= eUartApiPort_Last) || (received_message == NULL)) {
        return false;
    }
    osStatus_t status = osMessageQueueGet(g_uart_api_dynamic_lut[port].message_queue_id, received_message, NULL, queue_wait_time);
    if (status == osOK) {
        return true;
    }
    return false;
}

static void UART_API_Thread (void *argument) {
    uint8_t byte;
    while (1) {
        for (size_t port = eUartApiPort_First; port < eUartApiPort_Last; port++) {
            if (g_uart_api_dynamic_lut[port].is_initialized == false) {
                continue;
            }
            switch (g_uart_api_dynamic_lut[port].state) {
                case eUartApiState_Initialize: {
                    g_uart_api_dynamic_lut[port].buffer = Memory_API_Calloc(g_uart_api_static_lut[port].max_message_size, sizeof(char));
                    g_uart_api_dynamic_lut[port].index = 0;
                    if (g_uart_api_dynamic_lut[port].buffer == NULL) {
                        continue;
                    }
                    g_uart_api_dynamic_lut[port].state = eUartApiState_CollectData;
                }
                case eUartApiState_CollectData: {
                    while (UART_Driver_ReadByte(g_uart_api_static_lut[port].driver_port, &byte) == true) {
                        g_uart_api_dynamic_lut[port].buffer[g_uart_api_dynamic_lut[port].index] = byte;
                        g_uart_api_dynamic_lut[port].index += 1;
                        if (g_uart_api_dynamic_lut[port].index >= g_uart_api_static_lut[port].max_message_size) {
                            g_uart_api_dynamic_lut[port].state = eUartApiState_FlushData;
                            break;
                        }
                        if (byte == g_uart_api_dynamic_lut[port].delimiter[g_uart_api_dynamic_lut[port].delimiter_length - 1]) {
                            if (UART_API_CheckDelimiter(&g_uart_api_dynamic_lut[port]) == true) {
                                g_uart_api_dynamic_lut[port].state = eUartApiState_FlushData;
                                break;
                            }
                        }
                    }
                    if (g_uart_api_dynamic_lut[port].state != eUartApiState_FlushData) {
                        break;
                    }
                }
                case eUartApiState_FlushData: {
                    if (g_uart_api_dynamic_lut[port].index > 0) {
                        sMessage_t message = {
                            .data = g_uart_api_dynamic_lut[port].buffer,
                            .length = g_uart_api_dynamic_lut[port].index
                        };
                        UART_API_SendString(eUartApiPort_Uart1, message.data, message.length);
                        if (osMessageQueuePut(g_uart_api_dynamic_lut[port].message_queue_id, &message, 0, MSG_QUEUE_PUT_TMO) != osOK) {
                            Memory_API_Free(message.data);
                        }
                        g_uart_api_dynamic_lut[port].buffer = NULL;
                        g_uart_api_dynamic_lut[port].index = 0;
                        g_uart_api_dynamic_lut[port].state = eUartApiState_Initialize;
                    }
                    break;
                }
            }
            osThreadYield();
        }
    }
}

