/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <string.h>
#include "cmsis_os.h"
#include "cli_cmd_handler.h"
#include "uart_api.h"
#include "debug_api.h"
#include "memory_api.h"
#include "cli_app.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define CLI_THREAD_WAIT_TIME 100

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
static const sCommandEntry_t g_command_table[eCliCmd_Last] = {
    [eCliCmd_Set] = {
        .command = "SET",
        .command_size = (sizeof("SET") - 1),
        .handler = CLI_CMD_Handler_Set},
    [eCliCmd_Reset] = {
        .command = "RESET",
        .command_size = (sizeof("RESET") - 1),
        .handler = CLI_CMD_Handler_Reset},
    [eCliCmd_Toggle] = {
        .command = "TOGGLE",
        .command_size = (sizeof("TOGGLE") - 1),
        .handler = CLI_CMD_Handler_Toggle},
    [eCliCmd_Blink] = {
        .command = "BLINK",
        .command_size = (sizeof("BLINK") - 1),
        .handler = CLI_CMD_Handler_Blink},
};

DEFINE_DEBUG_MODULE_TAG(CLI_APP);
/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
 static osThreadId_t g_cli_app_thread_id = NULL;

 static const osThreadAttr_t g_cli_app_thread_attr = {
     .name = "CLI Thread",
     .stack_size = 1024,
     .priority = osPriorityNormal,
 };
/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/
 static void CLI_APP_Thread(void *argument);
 static void CLI_APP_ProcessCommand (const char *command, size_t length,const sCommandEntry_t *command_table,size_t cmd_table_size);
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static void CLI_APP_Thread (void *argument) {
    sMessage_t received_message;
    while (1) {
        if (UART_API_ReceiveMessage(eUartApiPort_Debug, &received_message, CLI_THREAD_WAIT_TIME) == false) {
            osThreadYield();
            continue;
        }
        CLI_APP_ProcessCommand(received_message.data, received_message.length, g_command_table, eCliCmd_Last);
        Memory_API_Free(received_message.data);
    }
}

static void CLI_APP_ProcessCommand (const char *command, size_t length, const sCommandEntry_t *command_table, size_t command_table_size) {
    if ((command == NULL) || (length == 0) || (command_table == NULL) || (command_table_size == 0)) {
        TRACE_Error(module_tag,"Invalid input parameters for command processing\r");
        return;
    }
    const char *delimiter_pos = strchr(command, ':');
    if (delimiter_pos == NULL) {
        TRACE_Error(module_tag,"Command format error (missing ':')\r");
        return;
    }
    size_t command_part_length = delimiter_pos - command;
    size_t params_length = length - (command_part_length + 1);
    for (size_t i = 0; i < command_table_size; ++i) {
        if ((command_part_length == command_table[i].command_size) && (strncmp(command, command_table[i].command, command_part_length) == 0)) {
            if (command_table[i].handler == NULL) {
                TRACE_Error(module_tag,"Handler is NULL for command\r");
                return;
            }
            command_table[i].handler(delimiter_pos + 1, params_length);
            return;
        }
    }
    TRACE_Error(module_tag,"Invalid command received\r");
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




