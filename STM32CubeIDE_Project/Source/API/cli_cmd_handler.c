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
bool CLI_CMD_SetHandler (const sCommandParams_t *cmd_params) {
    if ((cmd_params->params == NULL) || (cmd_params->length == 0) || (cmd_params->response == NULL) || (cmd_params->response_size == 0)) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid input params\r");
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size, "Set command executed successfully.\r");
    return true;
}

bool CLI_CMD_ResetHandler (const sCommandParams_t *cmd_params) {
    if ((cmd_params->params == NULL) || (cmd_params->length == 0) || (cmd_params->response == NULL) || (cmd_params->response_size == 0)) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid input params\r");
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size, "Reset command executed successfully.\r");
    return true;
}

bool CLI_CMD_ToggleHandler (const sCommandParams_t *cmd_params) {
    if ((cmd_params->params == NULL) || (cmd_params->length == 0) || (cmd_params->response == NULL) || (cmd_params->response_size == 0)) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid input params\r");
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size, "Toggle command executed successfully.\r");
    return true;
}

bool CLI_CMD_BlinkHandler (const sCommandParams_t *cmd_params) {
    if ((cmd_params->params == NULL) || (cmd_params->length == 0) || (cmd_params->response == NULL) || (cmd_params->response_size == 0)) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid input params\r");
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size, "Blink command executed successfully.\r");
    return true;
}
