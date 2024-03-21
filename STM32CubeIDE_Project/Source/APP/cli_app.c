/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <string.h>
#include "cmsis_os.h"
#include "cli_cmd_handler.h"
#include "uart_api.h"
#include "memory_api.h"
#include "cmd_api.h"
#include "debug_api.h"
#include "cli_app.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
DEFINE_DEBUG_MODULE_TAG(CLI_APP);
#define CLI_THREAD_WAIT_TIME 100
#define CLI_RESPONSE_BUFFER_SIZE 512
#define DEFINE_CMD(cmd) { \
    .command = #cmd, \
    .command_size = sizeof(#cmd) - 1, \
    .handler = CLI_CMD_Handler_##cmd \
}
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
static const sCommandEntry_t g_command_table[eCliCmd_Last] = {
    [eCliCmd_Set] = DEFINE_CMD(SET),
    [eCliCmd_Reset] = DEFINE_CMD(RESET),
    [eCliCmd_Toggle] = DEFINE_CMD(TOGGLE),
    [eCliCmd_Blink] = DEFINE_CMD(BLINK),
};

static const osThreadAttr_t g_cli_app_thread_attr = {
    .name = "CLI Thread",
    .stack_size = 1024,
    .priority = osPriorityNormal,
};
/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static osThreadId_t g_cli_app_thread_id = NULL;
static char g_cli_response_buffer[CLI_RESPONSE_BUFFER_SIZE];
/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/
static void CLI_APP_Thread (void *argument);
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static void CLI_APP_Thread (void *argument) {
    sMessage_t received_message;
    while (1) {
        if (UART_API_ReceiveMessage(eUartApiPort_Debug, &received_message, CLI_THREAD_WAIT_TIME) == false) {
            continue;
        }
        if (CMD_API_ProcessCommand(received_message.data, received_message.length, g_command_table, eCliCmd_Last, g_cli_response_buffer, CLI_RESPONSE_BUFFER_SIZE) == true) {
            TRACE_INFO("%s", g_cli_response_buffer);
        } else {
            TRACE_WARNING("%s", g_cli_response_buffer);
        }
        Memory_API_Free(received_message.data);
    }
}
/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool CLI_APP_Init (void) {
    if (g_cli_app_thread_id == NULL) {
        g_cli_app_thread_id = osThreadNew(CLI_APP_Thread, NULL, &g_cli_app_thread_attr);
    }
    if (g_cli_app_thread_id == NULL) {
        return false;
    }
    return true;
}
