/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdio.h>
#include "debug_api.h"
#include "memory_api.h"
#include "cli_cmd_handler.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
typedef struct {
    unsigned int led_number;
} sLedBasicCommandParams_t;

typedef struct {
    unsigned int led_number;
    unsigned int time;
    unsigned int frequency;
} sBlinkCommandParams_t;

typedef struct {
    eCliCmd_t command_type;
    void *command_params;
} sCommand_t;
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
void CLI_CMD_Handler_Set (const char *params) {
    if (params == NULL) {
        TRACE_Error("NULL parameters provided\r");
        return;
    }
    sLedBasicCommandParams_t *cmd_params = (sLedBasicCommandParams_t *)Memory_API_Malloc(sizeof(sLedBasicCommandParams_t));
    if (cmd_params == NULL) {
        TRACE_Error("Memory allocation failed for SET command\r");
        return;
    }
    if (sscanf(params, "%u", &cmd_params->led_number) != 1) {
        TRACE_Error("Invalid parameters provided\r");
        Memory_API_Free(cmd_params);
        return;
    }
    if (cmd_params->led_number > HANDLER_API_MAX_LED_NUMBER) {
        TRACE_Error("Invalid LED number (%u)\r", cmd_params->led_number);
        Memory_API_Free(cmd_params);
        return;
    }
    TRACE_Info("SET command prepared for LED %u\r", cmd_params->led_number);
    Memory_API_Free(cmd_params);
}

void CLI_CMD_Handler_Reset (const char *params) {
    if (params == NULL) {
        TRACE_Error("NULL parameters provided\r");
        return;
    }
    sLedBasicCommandParams_t *cmd_params = (sLedBasicCommandParams_t *)Memory_API_Malloc(sizeof(sLedBasicCommandParams_t));
    if (cmd_params == NULL) {
        TRACE_Error("Memory allocation failed for RESET command\r");
        return;
    }
    if (sscanf(params, "%u", &cmd_params->led_number) != 1) {
        TRACE_Error("Invalid parameters provided\r");
        Memory_API_Free(cmd_params);
        return;
    }
    if (cmd_params->led_number > HANDLER_API_MAX_LED_NUMBER) {
        TRACE_Error("Invalid LED number (%u)\r", cmd_params->led_number);
        Memory_API_Free(cmd_params);
        return;
    }
    TRACE_Info("RESET command prepared for LED %u\r", cmd_params->led_number);
    Memory_API_Free(cmd_params);
}

void CLI_CMD_Handler_Toggle (const char *params) {
    if (params == NULL) {
        TRACE_Error("NULL parameters provided\r");
        return;
    }
    sLedBasicCommandParams_t *cmd_params = (sLedBasicCommandParams_t *)Memory_API_Malloc(sizeof(sLedBasicCommandParams_t));
    if (cmd_params == NULL) {
        TRACE_Error("Memory allocation failed for TOGGLE command\r");
        return;
    }
    if (sscanf(params, "%u", &cmd_params->led_number) != 1) {
        TRACE_Error("Invalid parameters provided\r");
        Memory_API_Free(cmd_params);
        return;
    }
    if (cmd_params->led_number > HANDLER_API_MAX_LED_NUMBER) {
        TRACE_Error("Invalid LED number (%u)\r", cmd_params->led_number);
        Memory_API_Free(cmd_params);
        return;
    }
    TRACE_Info("TOGGLE command prepared for LED %u\r", cmd_params->led_number);
    Memory_API_Free(cmd_params);
}

void CLI_CMD_Handler_Blink (const char *params) {
    if (params == NULL) {
        TRACE_Error("NULL parameters provided\r");
        return;
    }
    sBlinkCommandParams_t *cmd_params = (sBlinkCommandParams_t *)Memory_API_Malloc(sizeof(sBlinkCommandParams_t));
    if (cmd_params == NULL) {
        TRACE_Error("Memory allocation failed for BLINK command\r");
        return;
    }
    if (sscanf(params, "%u,%u,%u", &cmd_params->led_number, &cmd_params->time, &cmd_params->frequency) != 3) {
        TRACE_Error("Invalid parameters provided\r");
        Memory_API_Free(cmd_params);
        return;
    }
    if ((cmd_params->led_number > HANDLER_API_MAX_LED_NUMBER) || (cmd_params->time > HANDLER_API_MAX_TIME) || (cmd_params->frequency < HANDLER_API_MIN_BLINK_FREQ) || (cmd_params->frequency > HANDLER_API_MAX_BLINK_FREQ)) {
        TRACE_Error("Invalid parameters: LED %u, time %u, frequency %u\r", cmd_params->led_number, cmd_params->time, cmd_params->frequency);
        Memory_API_Free(cmd_params);
        return;
    }
    TRACE_Info("BLINK command prepared for LED %u with time %u and frequency %u\r", cmd_params->led_number, cmd_params->time, cmd_params->frequency);
    Memory_API_Free(cmd_params);
}
