/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <string.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "debug_api.h"
#include "modem_api.h"
#include "sms_api.h"
#include "uart_api.h"
#include "memory_api.h"
#include "cli_cmd_handler.h"
#include "cmd_api.h"
#include "math_utils.h"
#include "sms_app.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
DEFINE_DEBUG_MODULE_TAG(SMS_APP);
#define SMS_RESPONSE_BUFFER_SIZE 512
#define SMS_APP_INDEX_QUEUE_SIZE   50
#define SMS_APP_QUEUE_WAIT_TIME    200
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
    eSmsCmdEnum_StartTCP,
    eSmsCmdEnum_StopTCP,
    eSmsCmdEnum_GetLocation,
    eSmsCmdEnum_Last,
} eSmsCmdEnum_t;
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
static const osThreadAttr_t g_sms_api_thread_attr = {
    .name = "SMS_Thread",
    .stack_size = 4 * 250,
    .priority = osPriorityNormal,
};

static osThreadId_t g_sms_api_thread_id = NULL;
static osMessageQueueId_t g_sms_app_index_queue = NULL;
static char g_sms_response_buffer[SMS_RESPONSE_BUFFER_SIZE] = {0};
static const sCommand_t g_sms_command_table[eSmsCmdEnum_Last] = {
    DEFINE_CMD(get_location,CLI_CMD_GetLocationHandler,""),
    DEFINE_CMD(start_tcp, CLI_CMD_StartTCPHandler, ":"),
    DEFINE_CMD(stop_tcp, CLI_CMD_StopTCPHandler, ":")
};
/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static sSmsMessage_t g_sms_message = {0};
static uint32_t g_incoming_message_index = 0;
static sCmdParser_t g_sms_command_parser = {
    .command_table = g_sms_command_table,
    .command_table_size = DEFINE_ARRAY_LEN(g_sms_command_table),
    .response = g_sms_response_buffer,
    .response_size = SMS_RESPONSE_BUFFER_SIZE
};
/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/
static void SMS_APP_Thread (void *argument);
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static void SMS_APP_Thread (void *argument) {
    SMS_API_Init();
    while (1) {
        if (osMessageQueueGet(g_sms_app_index_queue, &g_incoming_message_index, NULL, osWaitForever) != osOK) {
            continue;
        }
        if (SMS_API_ReadMessageByIndex(g_incoming_message_index) == false) {
            continue;
        }
        if (SMS_API_ReceiveMessage(&g_sms_message) == false) {
            continue;
        }
        if (CMD_API_ProcessCommand(g_sms_message.message_content, g_sms_message.message_content_len, &g_sms_command_parser) == true) {
            TRACE_INFO(g_sms_command_parser.response);
        } else {
            TRACE_WARNING(g_sms_command_parser.response);
            SMS_API_SendSms("Wrong Command!");
        }
        if (SMS_API_DeleteMessage(g_incoming_message_index) == false) {
            continue;
        }
        Memory_API_Free(g_sms_message.message_content);
    }
}
/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool SMS_APP_Init(void){
    if (g_sms_app_index_queue == NULL) {
        g_sms_app_index_queue = osMessageQueueNew(SMS_APP_INDEX_QUEUE_SIZE,sizeof(uint32_t), NULL);
        if (g_sms_app_index_queue == NULL) {
            return false;
        }
    }
    if (g_sms_api_thread_id == NULL) {
        g_sms_api_thread_id = osThreadNew(SMS_APP_Thread, NULL, &g_sms_api_thread_attr);
        if (g_sms_api_thread_id == NULL) {
            return false;
        }
    }
    return true;
}

bool SMS_APP_AddIndex (uint32_t *index) {
    if (index == NULL) {
        return false;
    }
    osStatus_t status = osMessageQueuePut(g_sms_app_index_queue, index, 0, SMS_APP_QUEUE_WAIT_TIME);
    return (status == osOK);
}
