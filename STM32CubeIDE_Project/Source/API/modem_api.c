/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <string.h>
#include "cmsis_os.h"
#include "uart_api.h"
#include "memory_api.h"
#include "debug_api.h"
#include "modem_cmd_handler.h"
#include "math_utils.h"
#include "gpio_driver.h"
#include "modem_api.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
DEFINE_DEBUG_MODULE_TAG(MODEM_API);
#define MODEM_MSG_QUEUE_SIZE           50
#define MODEM_RESPONSE_BUFFER_SIZE     512
#define MODEM_MUTEX_WAIT_TIME          10000
#define MODEM_UART_PORT                eUartApiPort_Modem
#define MODEM_BAUD_RATE                115200
#define MODEM_UART_DELIMITER           "\r\n"
#define MODEM_UART_DELIMITER_LENGTH    (sizeof(MODEM_UART_DELIMITER) - 1)
#define DEFINE_CMD(cmd, handler_func, sep) { \
    .command = #cmd, \
    .command_size = sizeof(#cmd) - 1, \
    .handler = &handler_func, \
    .separator = sep, \
    .separator_length = sizeof(sep) - 1 \
}
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
typedef enum {
    eModemCmdEnum_Ok,
    eModemCmdEnum_Error,
    eModemCmdEnum_CMGL,
    eModemCmdEnum_CMGR,
    eModemCmdEnum_CMTI,
    eModemCmdEnum_SendSMSContext,
    eModemCmdEnum_Etl,
    eModemCmdEnum_Eind,
    eModemCmd_Last,
} eModemCmdEnum_t;

typedef struct {
    osMessageQueueId_t message_queue_id;
    size_t message_size;
} sQueueProperties_t;
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
static const osThreadAttr_t g_modem_api_thread_attr = {
    .name = "MODEM API Thread",
    .stack_size = 4 * 200,
    .priority = osPriorityAboveNormal,
};

static const osMutexAttr_t g_modem_mutex_attr = {
    "Modem Mutex",
    osMutexRecursive,
    NULL,
    0U
};

static const sCommand_t g_command_table[eModemCmd_Last] = {
DEFINE_CMD(>,MODEM_CMD_SendSMSHandler,""),
DEFINE_CMD(OK,MODEM_CMD_OkHandler,""),
DEFINE_CMD(ERROR,MODEM_CMD_ErrorHandler,""),
DEFINE_CMD(+CMGL,MODEM_CMD_CmglHandler,":"),
DEFINE_CMD(+CMGR,MODEM_CMD_CmgrHandler,":"),
DEFINE_CMD(+CMTI,MODEM_CMD_CMTIHandler,":"),
DEFINE_CMD(+ETL,MODEM_CMD_ETLHandler,":"),
DEFINE_CMD(+EIND,MODEM_CMD_EindHandler,":")
};
/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static osThreadId_t g_modem_api_thread_id = NULL;
static osMutexId_t g_modem_mutex_id = NULL;
static osEventFlagsId_t g_modem_event_id = NULL;
static eModemStatusEnum_t g_last_command_status = eModemStatus_Error;
static char g_modem_response_buffer[MODEM_RESPONSE_BUFFER_SIZE] = {0};
sMessage_t g_received_message = {0};
static sCmdParser_t g_command_parser = {
    .command_table = g_command_table,
    .command_table_size = DEFINE_ARRAY_LEN(g_command_table),
    .response = g_modem_response_buffer,
    .response_size = MODEM_RESPONSE_BUFFER_SIZE
};

static sQueueProperties_t g_modem_queue_properties[eQueueType_Last] = {
    [eQueueType_General] = {
        .message_queue_id = NULL,
        .message_size = sizeof(sMessage_t)
    },
    [eQueueType_SMS] = {
        .message_queue_id = NULL,
        .message_size = sizeof(sSmsMessage_t)
    }
};
/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/
static void MODEM_API_Thread (void *argument);
static void MODEM_API_ModemTurnOn (void);
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static void MODEM_API_Thread (void *argument) {
    Modem_API_Lock();
    while (1) {
        if (UART_API_ReceiveMessage(MODEM_UART_PORT, &g_received_message, osWaitForever) == false) {
            continue;
        }
        if (CMD_API_ProcessCommand(g_received_message.data, g_received_message.length, &g_command_parser) == true) {
            TRACE_INFO(g_command_parser.response);
        } else {
            TRACE_WARNING("MODEM RESPONDED AS <- %s\r", g_received_message.data);  // change this for simetric app
        }
        Memory_API_Free(g_received_message.data);
    }
}

static void MODEM_API_ModemTurnOn (void) {
    GPIO_Driver_WritePin(eGpioDriverPin_ModemUartCtsPin, false);
    GPIO_Driver_WritePin(eGpioDriverPin_ModemUartDtrPin, false);
    GPIO_Driver_WritePin(eGpioDriverPin_ModemUartRtsPin, false);
    GPIO_Driver_WritePin(eGpioDriverPin_ModemPowerOffPin, true);
    GPIO_Driver_WritePin(eGpioDriverPin_ModemOnPin, true);
}
/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool MODEM_API_Init (void) {
    if (UART_API_Init(eUartApiPort_Modem, 115200, MODEM_UART_DELIMITER, MODEM_UART_DELIMITER_LENGTH) == false) {
        return false;
    }
    if (g_modem_event_id == NULL) {
        g_modem_event_id = osEventFlagsNew(NULL);
        if (g_modem_event_id == NULL) {
            return false;
        }
    }
    if (g_modem_mutex_id == NULL) {
        g_modem_mutex_id = osMutexNew(&g_modem_mutex_attr);
        if (g_modem_mutex_id == NULL) {
            return false;
        }
    }
    for (eQueueTypeEnum_t queue_type = eQueueType_First; queue_type < eQueueType_Last; ++queue_type) {
        if (g_modem_queue_properties[queue_type].message_queue_id == NULL) {
            g_modem_queue_properties[queue_type].message_queue_id = osMessageQueueNew(MODEM_MSG_QUEUE_SIZE, g_modem_queue_properties[queue_type].message_size, NULL);
            if (g_modem_queue_properties[queue_type].message_queue_id == NULL) {
                return false;
            }
        }
    }
    if (g_modem_api_thread_id == NULL) {
        g_modem_api_thread_id = osThreadNew(MODEM_API_Thread, NULL, &g_modem_api_thread_attr);
        if (g_modem_api_thread_id == NULL) {
            return false;
        }
    }
    MODEM_API_ModemTurnOn();
    return true;
}

eModemStatusEnum_t MODEM_API_SendAndWait (const char *cmd, uint32_t wait_time) {
    if ((cmd == NULL) || (wait_time == 0)) {
        return eModemStatus_Error;
    }
    TRACE_INFO("CMD SEND TO MODEM -> %s",cmd);
    UART_API_SendString(MODEM_UART_PORT, cmd, strlen(cmd));
    uint32_t flags = osEventFlagsWait(g_modem_event_id, 0x01, osFlagsWaitAny, wait_time);
    eModemStatusEnum_t result = (flags & 0x01) ? g_last_command_status : eModemStatus_Timeout;
    return result;
}

eModemStatusEnum_t MODEM_API_SendAndWaitRepeat(const char *cmd, uint32_t wait_time, uint32_t repeat, uint32_t delay) {
    if ((cmd == NULL) || (wait_time == 0) || (repeat == 0)) {
        return eModemStatus_Error;
    }
    eModemStatusEnum_t result;
    while (repeat > 0) {
        result = MODEM_API_SendAndWait(cmd, wait_time);
        if (result != eModemStatus_Error) {
            return result;
        }
        repeat--;
        if (repeat > 0) {
            osDelay(delay);
        }
    }
    return result;
}

bool Modem_API_Lock (void) {
    if (g_modem_mutex_id == NULL) {
        return false;
    }
    if (osMutexAcquire(g_modem_mutex_id, MODEM_MUTEX_WAIT_TIME) == osOK) {
        return true;
    } else {
        return false;
    }
}

bool Modem_API_Unlock (void) {
    if (g_modem_mutex_id == NULL) {
        return false;
    }
    if (osMutexRelease(g_modem_mutex_id) == osOK) {
        return true;
    } else {
        return false;
    }
}

void Modem_API_SetCommandResult (eModemStatusEnum_t status) {
    g_last_command_status = status;
    osEventFlagsSet(g_modem_event_id, 0x01);
}

bool MODEM_API_ReceiveFromQueue (eQueueTypeEnum_t queue, void *message, uint32_t queue_wait_time) {
    if ((queue < eQueueType_First) || (queue >= eQueueType_Last) || (message == NULL)) {
        return false;
    }
    osStatus_t status = osMessageQueueGet(g_modem_queue_properties[queue].message_queue_id, message, NULL, queue_wait_time);
    return status == osOK;
}

bool MODEM_API_PutToQueue (eQueueTypeEnum_t queue, const void *message, uint32_t queue_wait_time) {
    if ((queue < eQueueType_First) || (queue >= eQueueType_Last) || (message == NULL)) {
        return false;
    }
    osStatus_t status = osMessageQueuePut(g_modem_queue_properties[queue].message_queue_id, message, 0, queue_wait_time);
    return status == osOK;
}
