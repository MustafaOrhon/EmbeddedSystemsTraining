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
#define CLI_RESPONSE_BUFFER_SIZE 512
#define DEFINE_CMD(cmd, handler_func) { \
    .command = #cmd, \
    .command_size = sizeof(#cmd) - 1, \
    .handler = &handler_func, \
    .separator = ":", \
    .separator_length = sizeof(":") - 1 \
}
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
static const sCommand_t g_command_table[eCliCmd_Last] = {
    [eCliCmd_Set] = DEFINE_CMD(led_set, CLI_CMD_SetHandler),
    [eCliCmd_Reset] = DEFINE_CMD(led_reset, CLI_CMD_ResetHandler),
    [eCliCmd_Toggle] = DEFINE_CMD(led_toggle, CLI_CMD_ToggleHandler),
    [eCliCmd_Blink] = DEFINE_CMD(led_blink, CLI_CMD_BlinkHandler),
};
static const osThreadAttr_t g_cli_app_thread_attr = {
    .name = "CLI Thread",
    .stack_size = 512,
    .priority = osPriorityNormal,
};
/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static osThreadId_t g_cli_app_thread_id = NULL;
static char g_cli_response_buffer[CLI_RESPONSE_BUFFER_SIZE] = {0};
static sMessage_t received_message = {0};
static sCmdParser_t command_parser = {
    .command_table = g_command_table,
    .command_table_size = eCliCmd_Last,
    .response = g_cli_response_buffer,
    .response_size = CLI_RESPONSE_BUFFER_SIZE
};
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
    while (1) {
        if (UART_API_ReceiveMessage(eUartApiPort_Debug, &received_message, osWaitForever) == false) {
            continue;
        }
        if (CMD_API_ProcessCommand(received_message.data, received_message.length, &command_parser)) {
            TRACE_INFO(command_parser.response);
        } else {
            TRACE_WARNING(command_parser.response);
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
