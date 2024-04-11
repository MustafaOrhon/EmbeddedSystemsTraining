/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "memory_api.h"
#include "uart_api.h"
#include "led_app.h"
#include "led_api.h"
#include "record_sending.h"
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
    eHandlerCodeEnum_ParameterOutOfRange,
    eHandlerCodeEnum_TaskQueueFull,
    eHandlerCodeEnum_MissingParameter,
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
        case eHandlerCodeEnum_ParameterOutOfRange:
            snprintf(response_buffer, buffer_size, "Invalid input: parameter out of range.\r");
            break;
        case eHandlerCodeEnum_TaskQueueFull:
            snprintf(response_buffer, buffer_size, "Timeout: task queue is full.\r");
            break;
        case eHandlerCodeEnum_MissingParameter:
            snprintf(response_buffer, buffer_size, "Invalid input: missing parameter\r");
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
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_ParameterOutOfRange);
        return false;
    }
    sLedBasicCommandParams_t *led_params = (sLedBasicCommandParams_t *)Memory_API_Malloc(sizeof(sLedBasicCommandParams_t));
    if (led_params == NULL) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_Internal);
        return false;
    }
    led_params->led_number = led_number;
    sLedAppTask_t led_app_cmd = {.task = eLedAppTask_Set, .data = led_params};
    if (LED_APP_AddTask(&led_app_cmd) == false) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_TaskQueueFull);
        Memory_API_Free(led_params);
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size, "Turning on %s LED\r", LED_API_LedEnumToString(led_number));
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
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_ParameterOutOfRange);
        return false;
    }
    sLedBasicCommandParams_t *led_params = (sLedBasicCommandParams_t *)Memory_API_Malloc(sizeof(sLedBasicCommandParams_t));
    if (led_params == NULL) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_Internal);
        return false;
    }
    led_params->led_number = led_number;
    sLedAppTask_t led_app_cmd = {.task = eLedAppTask_Reset, .data = led_params};
    if (LED_APP_AddTask(&led_app_cmd) == false) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_TaskQueueFull);
        Memory_API_Free(led_params);
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size, "Turning off %s LED\r", LED_API_LedEnumToString(led_number));
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
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_ParameterOutOfRange);
        return false;
    }
    sLedBasicCommandParams_t *led_params = (sLedBasicCommandParams_t *)Memory_API_Malloc(sizeof(sLedBasicCommandParams_t));
    if (led_params == NULL) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_Internal);
        return false;
    }
    led_params->led_number = led_number;
    sLedAppTask_t led_app_cmd = {.task = eLedAppTask_Toggle, .data = led_params};
    if (LED_APP_AddTask(&led_app_cmd) == false) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_TaskQueueFull);
        Memory_API_Free(led_params);
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size, "Toggling %s LED\r", LED_API_LedEnumToString(led_number));
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
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_MissingParameter);
        return false;
    }
    char *end_ptr = NULL;
    uint32_t led_number = strtoul(token, &end_ptr, 10);
    if ((*end_ptr != '\0') || (LED_API_IsLEDValid(led_number) == false)) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_ParameterOutOfRange);
        return false;
    }
    token = strtok_r(NULL, led_command_delimiter, &saveptr);
    if (token == NULL) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_MissingParameter);
        return false;
    }
    uint32_t time = strtoul(token, &end_ptr, 10);
    if ((*end_ptr != '\0') || (LED_APP_IsTimeValid(time) == false)) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_InvalidInput);
        return false;
    }
    token = strtok_r(NULL, led_command_delimiter, &saveptr);
    if (token == NULL) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_MissingParameter);
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
    sLedAppTask_t led_app_cmd = {.task = eLedAppTask_Blink, .data = blink_params};
    if (LED_APP_AddTask(&led_app_cmd) == false) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_TaskQueueFull);
        Memory_API_Free(blink_params);
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size, "Blinking %s LED for %u s at %u Hz\r", LED_API_LedEnumToString(led_number), time, frequency);
    return true;
}

bool CLI_CMD_SendATHandler (const sCommandParams_t *cmd_params) {
    if (CLI_CMD_CheckCmdParams(cmd_params) == false) {
        return false;
    }
    char *new_command = (char *)Memory_API_Malloc(cmd_params->length + 3);
    if (new_command == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Internal Error.\r");
        return false;
    }
    memcpy(new_command, cmd_params->params, cmd_params->length);
    strcpy(new_command + cmd_params->length, "\r\n");
    if (UART_API_SendString(eUartApiPort_Modem, new_command, cmd_params->length + 2) == false) {
        snprintf(cmd_params->response, cmd_params->response_size, "AT Command Send Failed\r\n");
        Memory_API_Free(new_command);
        return false;
    }
    Memory_API_Free(new_command);
    snprintf(cmd_params->response, cmd_params->response_size, "AT Command Sent Successfully\r\n");
    return true;
}

bool CLI_CMD_StartTCPHandler(const sCommandParams_t *cmd_params) {
    if (CLI_CMD_CheckCmdParams(cmd_params) == false) {
        return false;
    }
    const char *ip_port = (const char *)cmd_params->params;
    char *ip_end = strchr(ip_port, ',');
    if (ip_end == NULL) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_MissingParameter);
        return false;
    }
    char ip[16] = {0};
    strncpy(ip, ip_port, ip_end - ip_port);
    ip[ip_end - ip_port] = '\0';
    uint16_t port = atoi(ip_end + 1);
    sRecordSendingAPPCommandParams_t *tcp_params = (sRecordSendingAPPCommandParams_t *)Memory_API_Malloc(sizeof(sRecordSendingAPPCommandParams_t));
    if (tcp_params == NULL) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_Internal);
        return false;
    }
    strncpy(tcp_params->ip, ip, sizeof(tcp_params->ip) - 1);
    tcp_params->ip[sizeof(tcp_params->ip) - 1] = '\0';
    tcp_params->port = port;
    sRecordSendingAPPTask_t tcp_task;
    tcp_task.task = eRecordSendingAPPTask_Start;
    tcp_task.params = tcp_params;
    if (RecordSending_APP_AddTask(&tcp_task) == false) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_TaskQueueFull);
        Memory_API_Free(tcp_params);
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size, "Starting TCP connection to %s:%u\r", ip, port);
    return true;
}

bool CLI_CMD_StopTCPHandler(const sCommandParams_t *cmd_params) {
    if (CLI_CMD_CheckCmdParams(cmd_params) == false) {
        return false;
    }
    const char *ip_port = (const char *)cmd_params->params;
    char *ip_end = strchr(ip_port, ',');
    if (ip_end == NULL) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_MissingParameter);
        return false;
    }
    char ip[16] = {0};
    strncpy(ip, ip_port, ip_end - ip_port);
    ip[ip_end - ip_port] = '\0';
    uint16_t port = atoi(ip_end + 1);
    sRecordSendingAPPCommandParams_t *tcp_params = (sRecordSendingAPPCommandParams_t *)Memory_API_Malloc(sizeof(sRecordSendingAPPCommandParams_t));
    if (tcp_params == NULL) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_Internal);
        return false;
    }
    strncpy(tcp_params->ip, ip, sizeof(tcp_params->ip) - 1);
    tcp_params->ip[sizeof(tcp_params->ip) - 1] = '\0';
    tcp_params->port = port;
    sRecordSendingAPPTask_t tcp_task;
    tcp_task.task = eRecordSendingAPPTask_Stop;
    tcp_task.params = tcp_params;
    if (RecordSending_APP_AddTask(&tcp_task) == false) {
        CLI_CMD_HandleResponse(cmd_params->response, cmd_params->response_size, eHandlerCodeEnum_TaskQueueFull);
        Memory_API_Free(tcp_params);
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size, "Stoping TCP connection to %s:%u\r", ip, port);
    return true;
}

