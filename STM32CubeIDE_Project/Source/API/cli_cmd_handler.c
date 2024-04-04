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
#include "led_api.h"
#include "cli_cmd_handler.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
typedef enum {
        eHandlerCodeEnum_First = 0,
        eHandlerCodeEnum_Internal = eHandlerCodeEnum_First,
        eHandlerCodeEnum_InvalidInput,
        eHandlerCodeEnum_OutOfRange,
        eHandlerCodeEnum_QueueFull,
        eHandlerCodeEnum_MissingLED,
        eHandlerCodeEnum_MissingTime,
        eHandlerCodeEnum_MissingFrequency,
        eHandlerCodeEnum_Success,
        eHandlerCodeEnum_Last
} eHandlerCodeEnum_t;
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
static void CLI_CMD_HandleResponse (char *response_buffer, size_t buffer_size, eHandlerCodeEnum_t error_code);
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

static void CLI_CMD_HandleResponse (char *response_buffer, size_t buffer_size, eHandlerCodeEnum_t error_code) {
    if ((response_buffer == NULL) || (buffer_size == 0) || (error_code < eHandlerCodeEnum_First) || (error_code >= eHandlerCodeEnum_Last)) {
        return;
    }
    switch (error_code) {
        case eHandlerCodeEnum_Internal:
            snprintf(response_buffer, buffer_size, "Internal Error\r");
            break;
        case eHandlerCodeEnum_InvalidInput:
            snprintf(response_buffer, buffer_size, "Invalid Input\r");
            break;
        case eHandlerCodeEnum_OutOfRange:
            snprintf(response_buffer, buffer_size, "Invalid Input: LED number out of range.\r");
            break;
        case eHandlerCodeEnum_QueueFull:
            snprintf(response_buffer, buffer_size, "Error: LED task queue is full.\r");
            break;
        case eHandlerCodeEnum_MissingLED:
            snprintf(response_buffer, buffer_size, "Missing LED number\r");
            break;
        case eHandlerCodeEnum_MissingTime:
            snprintf(response_buffer, buffer_size, "Missing time parameter.\r");
            break;
        case eHandlerCodeEnum_MissingFrequency:
            snprintf(response_buffer, buffer_size, "Missing frequency\r");
            break;
        case eHandlerCodeEnum_Success:
            snprintf(response_buffer, buffer_size, "Command successfully executed\r");
            break;
    }
}
/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool CLI_CMD_LedSetHandler (const sCommandParams_t *cmd_params) {
    if (CLI_CMD_CheckCmdParams(cmd_params) == false) {
        return false;
    }
    if (isdigit((unsigned char)cmd_params->params[0]) == 0) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_InvalidInput);
        return false;
    }
    char *end_ptr = NULL;
    uint32_t led_number = strtoul(cmd_params->params, &end_ptr, 10);
    if ((cmd_params->params == end_ptr) || (*end_ptr != '\0')) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_InvalidInput);
        return false;
    }
    if (LED_API_IsLEDValid(led_number) == false) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_OutOfRange);
        return false;
    }
    sLedBasicCommandParams_t *led_params = (sLedBasicCommandParams_t *)Memory_API_Malloc(sizeof(sLedBasicCommandParams_t));
    if (led_params == NULL) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_Internal);
        return false;
    }
    led_params->led_number = led_number;
    sLedAppCmd_t led_app_cmd = {.cmd = eLedAppCmd_Set, .data = led_params};
    if (LED_APP_AddTask(&led_app_cmd) == false) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_QueueFull);
        Memory_API_Free(led_params);
        return false;
    }
    CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_Success);
    return true;
}

bool CLI_CMD_LedResetHandler (const sCommandParams_t *cmd_params) {
    if (CLI_CMD_CheckCmdParams(cmd_params) == false) {
        return false;
    }
    if (isdigit((unsigned char)cmd_params->params[0]) == 0) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_InvalidInput);
        return false;
    }
    char *end_ptr = NULL;
    uint32_t led_number = strtoul(cmd_params->params, &end_ptr, 10);
    if ((cmd_params->params == end_ptr) || (*end_ptr != '\0')) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_InvalidInput);
        return false;
    }
    if (LED_API_IsLEDValid(led_number) == false) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_OutOfRange);
        return false;
    }
    sLedBasicCommandParams_t *led_params = (sLedBasicCommandParams_t *)Memory_API_Malloc(sizeof(sLedBasicCommandParams_t));
    if (led_params == NULL) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_Internal);
        return false;
    }
    led_params->led_number = led_number;
    sLedAppCmd_t led_app_cmd = {.cmd = eLedAppCmd_Reset, .data = led_params};
    if (LED_APP_AddTask(&led_app_cmd) == false) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_QueueFull);
        Memory_API_Free(led_params);
        return false;
    }
    CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_Success);
    return true;
}

bool CLI_CMD_LedToggleHandler (const sCommandParams_t *cmd_params) {
    if (CLI_CMD_CheckCmdParams(cmd_params) == false) {
        return false;
    }
    if (isdigit((unsigned char)cmd_params->params[0]) == 0) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_InvalidInput);
        return false;
    }
    char *end_ptr = NULL;
    uint32_t led_number = strtoul(cmd_params->params, &end_ptr, 10);
    if ((cmd_params->params == end_ptr) || (*end_ptr != '\0')) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_InvalidInput);
        return false;
    }
    if (LED_API_IsLEDValid(led_number) == false) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_OutOfRange);
        return false;
    }
    sLedBasicCommandParams_t *led_params = (sLedBasicCommandParams_t *)Memory_API_Malloc(sizeof(sLedBasicCommandParams_t));
    if (led_params == NULL) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_Internal);
        return false;
    }
    led_params->led_number = led_number;
    sLedAppCmd_t led_app_cmd = {.cmd = eLedAppCmd_Toggle, .data = led_params};
    if (LED_APP_AddTask(&led_app_cmd) == false) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_QueueFull);
        Memory_API_Free(led_params);
        return false;
    }
    CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_Success);
    return true;
}

bool CLI_CMD_LedBlinkHandler (const sCommandParams_t *cmd_params) {
    if (CLI_CMD_CheckCmdParams(cmd_params) == false) {
        return false;
    }
    const char *led_command_delimiter = ",";
    char *saveptr = NULL;
    char *token = NULL;
    token = strtok_r((char *)cmd_params->params, led_command_delimiter, &saveptr);
    if (token == NULL) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_MissingLED);
        return false;
    }
    char *end_ptr = NULL;
    uint32_t led_number = strtoul(token, &end_ptr, 10);
    if ((*end_ptr != '\0') || (LED_API_IsLEDValid(led_number) == false)) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_OutOfRange);
        return false;
    }
    token = strtok_r(NULL, led_command_delimiter, &saveptr);
    if (token == NULL) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_MissingTime);
        return false;
    }
    uint32_t time = strtoul(token, &end_ptr, 10);
    if ((*end_ptr != '\0') || (LED_APP_IsTimeValid(time) == false)) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_InvalidInput);
        return false;
    }
    token = strtok_r(NULL, led_command_delimiter, &saveptr);
    if (token == NULL) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_MissingFrequency);
        return false;
    }
    uint32_t frequency = strtoul(token, &end_ptr, 10);
    if ((*end_ptr != '\0') || (LED_APP_IsFrequencyValid(frequency) == false)) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_InvalidInput);
        return false;
    }
    sBlinkCommandParams_t *blink_params = (sBlinkCommandParams_t *)Memory_API_Malloc(sizeof(sBlinkCommandParams_t));
    if (blink_params == NULL) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_Internal);
        return false;
    }
    blink_params->led_number = led_number;
    blink_params->time = time;
    blink_params->frequency = frequency;
    sLedAppCmd_t led_app_cmd = {.cmd = eLedAppCmd_Blink, .data = blink_params};
    if (LED_APP_AddTask(&led_app_cmd) == false) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_QueueFull);
        Memory_API_Free(blink_params);
        return false;
    }
    CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_Success);
    return true;
}
