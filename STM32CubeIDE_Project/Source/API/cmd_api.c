/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <string.h>
#include "debug_api.h"
#include "memory_api.h"
#include "cmd_api.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
DEFINE_DEBUG_MODULE_TAG(CMD_API);
#define CLI_RESPONSE_BUFFER_SIZE 512
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
 static char g_cli_response_buffer[CLI_RESPONSE_BUFFER_SIZE];
/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
 void CMD_API_ProcessCommand (const char *command, size_t length, const sCommandEntry_t *command_table, size_t command_table_size) {
     if ((command == NULL) || (length == 0) || (command_table == NULL) || (command_table_size == 0)) {
         TRACE_ERROR("Invalid input parameters for command processing\r");
         return;
     }
     const char *delimiter_pos = strchr(command, ':');
     if (delimiter_pos == NULL) {
         TRACE_ERROR("Command format error (missing ':')\r");
         return;
     }
     size_t command_part_length = delimiter_pos - command;
     size_t params_length = length - (command_part_length + 1);
     for (size_t i = 0; i < command_table_size; ++i) {
         if ((command_part_length == command_table[i].command_size) && (strncmp(command, command_table[i].command, command_part_length) == 0)) {
             if (command_table[i].handler == NULL) {
                 TRACE_ERROR("Handler is NULL for command\r");
                 return;
             }
             if (command_table[i].handler(delimiter_pos + 1, params_length, g_cli_response_buffer, CLI_RESPONSE_BUFFER_SIZE) == false) {
                 TRACE_ERROR("%s", g_cli_response_buffer);
                 return;
             }
             TRACE_INFO("%s", g_cli_response_buffer);
             return;
         }
     }
     TRACE_ERROR("Invalid command received\r");
 }
