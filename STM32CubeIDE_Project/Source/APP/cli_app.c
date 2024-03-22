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
 
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
static const sCommand_t g_command_table[eCliCmd_Last] = {
    [eCliCmd_SetLed] = DEFINE_CMD(led_set, CLI_CMD_SetHandler, ":"),
    [eCliCmd_ResetLed] = DEFINE_CMD(led_reset, CLI_CMD_ResetHandler, ":"),
    [eCliCmd_ToggleLed] = DEFINE_CMD(led_toggle, CLI_CMD_ToggleHandler, ":"),
    [eCliCmd_BlinkLed] = DEFINE_CMD(led_blink, CLI_CMD_BlinkHandler, ":"),
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
static sMessage_t g_received_message = {0};
static sCmdParser_t g_command_parser = {
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
        if (UART_API_ReceiveMessage(eUartApiPort_Debug, &g_received_message, osWaitForever) == false) {
            continue;
        }
        if (CMD_API_ProcessCommand(g_received_message.data, g_received_message.length, &g_command_parser) == true) {
            TRACE_INFO(g_command_parser.response);
        } else {
            TRACE_WARNING(g_command_parser.response);
        }
        Memory_API_Free(g_received_message.data);
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
