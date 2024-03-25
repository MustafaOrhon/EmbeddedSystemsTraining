/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdio.h>
#include "debug_api.h"
#include "memory_api.h"
#include "led_app.h"
#include "cli_cmd_handler.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define HANDLER_API_MAX_LED_NUMBER    1
#define HANDLER_API_MAX_TIME          60
#define HANDLER_API_MIN_BLINK_FREQ    1
#define HANDLER_API_MAX_BLINK_FREQ    100
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
 static bool CLI_CMD_CheckCmdParams (const sCommandParams_t *cmd_params);
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static bool CLI_CMD_CheckCmdParams (const sCommandParams_t *cmd_params) {
    if (cmd_params == NULL) {
        return false;
    }
    if ((cmd_params->params == NULL) || (cmd_params->response == NULL) || (cmd_params->response_size == 0)) {
        snprintf(cmd_params->response, cmd_params->response_size, "Internal error\r");
        return false;
    }
    return true;
}
/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool CLI_CMD_SetHandler (const sCommandParams_t *cmd_params) {
    if (CLI_CMD_CheckCmdParams(cmd_params) == false) {
        return false;
    }
    sLedBasicCommandParams_t *led_params = (sLedBasicCommandParams_t *)Memory_API_Malloc(sizeof(sLedBasicCommandParams_t));
    if (led_params == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Memory allocation failed.\r");
        return false;
    }
    if (sscanf(cmd_params->params, "%u", &led_params->led_number) != 1) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid parameters provided\r");
        Memory_API_Free(led_params);
        return false;
    }
    if (led_params->led_number > HANDLER_API_MAX_LED_NUMBER) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid LED (%u)\r", led_params->led_number);
        Memory_API_Free(led_params);
        return false;
    }
    sLedAppCmd_t led_app_cmd = {.cmd = eLedAppCmd_Set, .data = led_params};
    if (LED_APP_SendCmd(&led_app_cmd) == false) {
        snprintf(cmd_params->response, cmd_params->response_size, "LED queue is full.\r");
        Memory_API_Free(led_params);
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size,"Set Handler Executed!\r");
    return true;
}

bool CLI_CMD_ResetHandler (const sCommandParams_t *cmd_params) {
    if (CLI_CMD_CheckCmdParams(cmd_params) == false) {
        return false;
    }
    sLedBasicCommandParams_t *led_params = (sLedBasicCommandParams_t *)Memory_API_Malloc(sizeof(sLedBasicCommandParams_t));
    if (led_params == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Memory allocation failed.\r");
        return false;
    }
    if (sscanf(cmd_params->params, "%u", &led_params->led_number) != 1) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid parameters provided\r");
        Memory_API_Free(led_params);
        return false;
    }
    if (led_params->led_number > HANDLER_API_MAX_LED_NUMBER) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid LED (%u)\r", led_params->led_number);
        Memory_API_Free(led_params);
        return false;
    }
    sLedAppCmd_t led_app_cmd = {.cmd = eLedAppCmd_Reset, .data = led_params};
    if (LED_APP_SendCmd(&led_app_cmd) == false) {
        snprintf(cmd_params->response, cmd_params->response_size, "LED queue is full.\r");
        Memory_API_Free(led_params);
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size,"Reset Handler Executed!\r");
    return true;
}

bool CLI_CMD_ToggleHandler (const sCommandParams_t *cmd_params) {
    if (CLI_CMD_CheckCmdParams(cmd_params) == false) {
        return false;
    }
    sLedBasicCommandParams_t *led_params = (sLedBasicCommandParams_t *)Memory_API_Malloc(sizeof(sLedBasicCommandParams_t));
    if (led_params == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Memory allocation failed.\r");
        return false;
    }
    if (sscanf(cmd_params->params, "%u", &led_params->led_number) != 1) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid parameters provided\r");
        Memory_API_Free(led_params);
        return false;
    }
    if (led_params->led_number > HANDLER_API_MAX_LED_NUMBER) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid LED (%u)\r", led_params->led_number);
        Memory_API_Free(led_params);
        return false;
    }
    sLedAppCmd_t led_app_cmd = {.cmd = eLedAppCmd_Toggle, .data = led_params};
    if (LED_APP_SendCmd(&led_app_cmd) == false) {
        snprintf(cmd_params->response, cmd_params->response_size, "LED queue is full.\r");
        Memory_API_Free(led_params);
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size,"Toggle Handler Executed!\r");
    return true;
}

bool CLI_CMD_BlinkHandler (const sCommandParams_t *cmd_params) {
    if (CLI_CMD_CheckCmdParams(cmd_params) == false) {
        return false;
    }
    sBlinkCommandParams_t *blink_params = (sBlinkCommandParams_t *)Memory_API_Malloc(sizeof(sBlinkCommandParams_t));
    if (blink_params == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Memory allocation failed.\r");
        return false;
    }
    if (sscanf(cmd_params->params, "%u,%u,%u", &blink_params->led_number, &blink_params->time, &blink_params->frequency) != 3) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid parameters provided\r");
        Memory_API_Free(blink_params);
        return false;
    }
    if ((blink_params->led_number > HANDLER_API_MAX_LED_NUMBER) || (blink_params->time > HANDLER_API_MAX_TIME)
        || (blink_params->frequency < HANDLER_API_MIN_BLINK_FREQ) || (blink_params->frequency > HANDLER_API_MAX_BLINK_FREQ)) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid parameters: LED %u, time %u, frequency %u\r", blink_params->led_number, blink_params->time, blink_params->frequency);
        Memory_API_Free(blink_params);
        return false;
    }
    sLedAppCmd_t led_app_cmd = {.cmd = eLedAppCmd_Blink, .data = blink_params};
    if (LED_APP_SendCmd(&led_app_cmd) == false) {
        snprintf(cmd_params->response, cmd_params->response_size, "LED queue is full.\r");
        Memory_API_Free(blink_params);
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size,"Blink Handler Executed!\r");
    return true;
}

