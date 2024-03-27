/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "debug_api.h"
#include "memory_api.h"
#include "led_app.h"
#include "cli_cmd_handler.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define HANDLER_API_MAX_LED_NUMBER    1
#define HANDLER_API_MIN_LED_NUMBER    0
#define HANDLER_API_MAX_TIME          60
#define HANDLER_API_MIN_TIME          1
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
    if (isdigit((unsigned char)cmd_params->params[0]) == 0) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid Input\r");
        return false;
    }
    char *end_ptr = NULL;
    uint32_t led_number = strtoul(cmd_params->params, &end_ptr, 10);
    if ((cmd_params->params == end_ptr) || (*end_ptr != '\0')) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid Input: LED number is incorrect format.\r");
        return false;
    }
    if ((led_number > HANDLER_API_MAX_LED_NUMBER) || (led_number < HANDLER_API_MIN_LED_NUMBER)) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid Input: LED number out of range.\r");
        return false;
    }
    sLedBasicCommandParams_t *led_params = (sLedBasicCommandParams_t *)Memory_API_Malloc(sizeof(sLedBasicCommandParams_t));
    if (led_params == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Internal Error\r");
        return false;
    }
    led_params->led_number = led_number;
    sLedAppCmd_t led_app_cmd = {.cmd = eLedAppCmd_Set, .data = led_params};
    if (LED_APP_AddTask(&led_app_cmd) == false) {
        snprintf(cmd_params->response, cmd_params->response_size, "Error: LED task queue is full.\r");
        Memory_API_Free(led_params);
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size, "LED task enqueued!\r");
    return true;
}

bool CLI_CMD_ResetHandler (const sCommandParams_t *cmd_params) {
    if (CLI_CMD_CheckCmdParams(cmd_params) == false) {
        return false;
    }
    if (isdigit((unsigned char)cmd_params->params[0]) == 0) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid Input\r");
        return false;
    }
    char *end_ptr = NULL;
    uint32_t led_number = strtoul(cmd_params->params, &end_ptr, 10);
    if ((cmd_params->params == end_ptr) || (*end_ptr != '\0')) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid Input: LED number is incorrect format.\r");
        return false;
    }
    if ((led_number > HANDLER_API_MAX_LED_NUMBER) || (led_number < HANDLER_API_MIN_LED_NUMBER)) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid Input: LED number out of range.\r");
        return false;
    }
    sLedBasicCommandParams_t *led_params = (sLedBasicCommandParams_t *)Memory_API_Malloc(sizeof(sLedBasicCommandParams_t));
    if (led_params == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Internal Error\r");
        return false;
    }
    led_params->led_number = led_number;
    sLedAppCmd_t led_app_cmd = {.cmd = eLedAppCmd_Reset, .data = led_params};
    if (LED_APP_AddTask(&led_app_cmd) == false) {
        snprintf(cmd_params->response, cmd_params->response_size, "Error: LED task queue is full.\r");
        Memory_API_Free(led_params);
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size, "LED task enqueued!\r");
    return true;
}

bool CLI_CMD_ToggleHandler (const sCommandParams_t *cmd_params) {
    if (CLI_CMD_CheckCmdParams(cmd_params) == false) {
        return false;
    }
    if (isdigit((unsigned char)cmd_params->params[0]) == 0) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid Input\r");
        return false;
    }
    char *end_ptr = NULL;
    uint32_t led_number = strtoul(cmd_params->params, &end_ptr, 10);
    if ((cmd_params->params == end_ptr) || (*end_ptr != '\0')) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid Input: LED number is incorrect format.\r");
        return false;
    }
    if ((led_number > HANDLER_API_MAX_LED_NUMBER) || (led_number < HANDLER_API_MIN_LED_NUMBER)) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid Input: LED number out of range.\r");
        return false;
    }
    sLedBasicCommandParams_t *led_params = (sLedBasicCommandParams_t *)Memory_API_Malloc(sizeof(sLedBasicCommandParams_t));
    if (led_params == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Internal Error\r");
        return false;
    }
    led_params->led_number = led_number;
    sLedAppCmd_t led_app_cmd = {.cmd = eLedAppCmd_Toggle, .data = led_params};
    if (LED_APP_AddTask(&led_app_cmd) == false) {
        snprintf(cmd_params->response, cmd_params->response_size, "Error: LED task queue is full.\r");
        Memory_API_Free(led_params);
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size, "LED task enqueued!\r");
    return true;
}

bool CLI_CMD_BlinkHandler (const sCommandParams_t *cmd_params) {
    if (CLI_CMD_CheckCmdParams(cmd_params) == false) {
        return false;
    }
    const char *led_command_delimiter = ",";
    char *saveptr = NULL;
    char *token = NULL;
    token = strtok_r((char *)cmd_params->params, led_command_delimiter, &saveptr);
    if (token == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Missing LED number\r");
        return false;
    }
    char *end_ptr = NULL;
    uint32_t led_number = strtoul(token, &end_ptr, 10);
    if ((*end_ptr != '\0') || (led_number > HANDLER_API_MAX_LED_NUMBER) || (led_number < HANDLER_API_MIN_LED_NUMBER)) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid LED number\r");
        return false;
    }
    token = strtok_r(NULL, led_command_delimiter, &saveptr);
    if (token == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Missing time parameter.\r");
        return false;
    }
    uint32_t time = strtoul(token, &end_ptr, 10);
    if ((*end_ptr != '\0') || (time > HANDLER_API_MAX_TIME) || (time < HANDLER_API_MIN_TIME)) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid time\r");
        return false;
    }
    token = strtok_r(NULL, led_command_delimiter, &saveptr);
    if (token == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid Input: Missing frequency\r");
        return false;
    }
    uint32_t frequency = strtoul(token, &end_ptr, 10);
    if ((*end_ptr != '\0') || (frequency > HANDLER_API_MAX_BLINK_FREQ) || (frequency < HANDLER_API_MIN_BLINK_FREQ)) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid frequency\r");
        return false;
    }
    sBlinkCommandParams_t *blink_params = (sBlinkCommandParams_t *)Memory_API_Malloc(sizeof(sBlinkCommandParams_t));
    if (blink_params == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Internal Error\r");
        return false;
    }
    blink_params->led_number = led_number;
    blink_params->time = time;
    blink_params->frequency = frequency;
    sLedAppCmd_t led_app_cmd = {.cmd = eLedAppCmd_Blink, .data = blink_params};
    if (LED_APP_AddTask(&led_app_cmd) == false) {
        snprintf(cmd_params->response, cmd_params->response_size, "LED task queue is full.\r");
        Memory_API_Free(blink_params);
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size, "LED task enqueued!\r");
    return true;
}
