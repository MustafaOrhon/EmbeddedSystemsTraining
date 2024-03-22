/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "memory_api.h"
#include "cmd_api.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
typedef enum {
    eCmdSeparatorStatus_NotFound = -1,
    eCmdSeparatorStatus_EmptyMessage = 0,
} eCmdSeparatorStatusEnum_t;
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/
static int CMD_API_FindSeparator (const char *data, size_t data_length, const char *separator, size_t separator_length);
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static int CMD_API_FindSeparator (const char *data, size_t data_length, const char *separator, size_t separator_length) {
    const char *delimiter_pos = strstr(data, separator);
    if (delimiter_pos == NULL) {
        return eCmdSeparatorStatus_NotFound;
    }
    if ((delimiter_pos == data) && (data_length == separator_length)) {
        return eCmdSeparatorStatus_EmptyMessage;
    }
    return delimiter_pos - data;
}
/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool CMD_API_ProcessCommand (const char *data, size_t length, const sCmdParser_t *command_context) {
    if ((data == NULL) || (length == 0) || (command_context == NULL)) {
        snprintf(command_context->response, command_context->response_size, "Invalid input parameters for command processing\r");
        return false;
    }
    for (size_t cmd = 0; cmd < command_context->command_table_size; ++cmd) {
        const sCommand_t *entry = &command_context->command_table[cmd];
        int separator_index = CMD_API_FindSeparator(data, length, entry->separator, entry->separator_length);
        if (separator_index == eCmdSeparatorStatus_NotFound) {
            snprintf(command_context->response, command_context->response_size, "Command format error (missing '%s')\r", entry->separator);
            return false;
        } else if (separator_index == eCmdSeparatorStatus_EmptyMessage) {
            snprintf(command_context->response, command_context->response_size, "Empty message\r");
            return false;
        }
        if (((size_t)separator_index == entry->command_size) && (strncmp(data, entry->command, entry->command_size) == 0)) {
            if (entry->handler == NULL) {
                snprintf(command_context->response, command_context->response_size, "Handler is NULL for command\r");
                return false;
            }
            sCommandParams_t cmd_params = {
                .params = data + separator_index + entry->separator_length,
                .length = length - (separator_index + entry->separator_length),
                .response = command_context->response,
                .response_size = command_context->response_size
            };
            return entry->handler(&cmd_params);
        }
    }
    snprintf(command_context->response, command_context->response_size, "Invalid command received\r");
    return false;
}
