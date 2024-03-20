/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <string.h>
#include "cmsis_os.h"
#include "cli_cmd_handler.h"
#include "uart_api.h"
#include "memory_api.h"
#include "cmd_api.h"
#include "cli_app.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define CLI_THREAD_WAIT_TIME 100
#define CLI_RESPONSE_BUFFER_SIZE 512
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
        CMD_API_ProcessCommand(received_message.data, received_message.length, g_command_table, eCliCmd_Last,g_cli_response_buffer,CLI_RESPONSE_BUFFER_SIZE);
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
