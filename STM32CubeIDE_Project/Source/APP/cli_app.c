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

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
static const sCommandEntry_t g_command_table[eCliCmd_Last] = {
    [eCliCmd_Set] = {.command = "SET",
        .handler = CLI_CMD_Handler_Set},
    [eCliCmd_Reset] = {.command = "RESET",
        .handler = CLI_CMD_Handler_Reset},
    [eCliCmd_Toggle] = {.command = "TOGGLE",
        .handler = CLI_CMD_Handler_Toggle},
    [eCliCmd_Blink] = {.command = "BLINK",
        .handler = CLI_CMD_Handler_Blink},
};
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
        memset(&received_message, 0, sizeof(received_message));
        if (UART_API_ReceiveMessage(eUartApiPort_Uart1, &received_message,100) == false) {
            osThreadYield();
            continue;
        }
        if ((received_message.data == NULL) || (received_message.length == 0)) {
            continue;
        }
        CLI_APP_ProcessCommand(received_message.data, received_message.length, g_command_table, eCliCmd_Last);
        Memory_API_Free(received_message.data);
    }
}

static void CLI_APP_ProcessCommand (const char *command, size_t length, const sCommandEntry_t *command_table, size_t command_table_size) {
    if ((command == NULL) || (command_table == NULL) || (command_table_size == 0) ) {
        TRACE_Error("Invalid input parameters for command processing\r");
        return;
    }
    char *buffer = (char *)Memory_API_Calloc(length + 1, sizeof(char));
    if (buffer == NULL) {
        TRACE_Error("Memory allocation failed for command processing\r");
        return;
    }
    memcpy(buffer, command, length);
    buffer[length] = '\0';
    char *delimiter_pos = strchr(buffer, ':');
    if (delimiter_pos == NULL) {
        TRACE_Error("Command format error (missing ':'): %s\r", buffer);
        Memory_API_Free(buffer);
        return;
    }
    *delimiter_pos = '\0';
    const char *params = delimiter_pos + 1;
    for (size_t i = 0; i < command_table_size; ++i) {
        if (strcmp(buffer, command_table[i].command) == 0) {
            if (command_table[i].handler == NULL) {
                TRACE_Error("Handler is NULL for command: %s\r", buffer);
                Memory_API_Free(buffer);
                return;
            }
            command_table[i].handler(params);
            Memory_API_Free(buffer);
            return;
        }
    }
    TRACE_Error("Invalid command received: %s\r", buffer);
    Memory_API_Free(buffer);
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




