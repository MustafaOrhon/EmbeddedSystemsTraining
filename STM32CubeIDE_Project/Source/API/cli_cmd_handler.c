/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdio.h>
#include "memory_api.h"
#include "cli_cmd_handler.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/

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
 
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool CLI_CMD_Handler_SET (const char *params, size_t length, char *response, size_t response_size) {
    if ((params == NULL) || (length == 0) || (response == NULL) || (response_size == 0)) {
        snprintf(response, response_size, "Invalid input params\r");
        return false;
    }
    snprintf(response, response_size, "Set command executed successfully.\r\n");
    return true;
}

bool CLI_CMD_Handler_RESET (const char *params, size_t length, char *response, size_t response_size) {
    if ((params == NULL) || (length == 0) || (response == NULL) || (response_size == 0)) {
        snprintf(response, response_size, "Invalid input params\r");
        return false;
    }
    snprintf(response, response_size, "Reset command executed successfully.\r");
    return true;
}

bool CLI_CMD_Handler_TOGGLE (const char *params, size_t length, char *response, size_t response_size) {
    if ((params == NULL) || (length == 0) || (response == NULL) || (response_size == 0)) {
        snprintf(response, response_size, "Invalid input params\r");
        return false;
    }
    snprintf(response, response_size, "Toggle command executed successfully.\r");
    return true;
}

bool CLI_CMD_Handler_BLINK (const char *params, size_t length, char *response, size_t response_size) {
    if ((params == NULL) || (length == 0) || (response == NULL) || (response_size == 0)) {
        snprintf(response, response_size, "Invalid input params\r");
        return false;
    }
    snprintf(response, response_size, "Blink command executed successfully.\r");
    return true;
}
