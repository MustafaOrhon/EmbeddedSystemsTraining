/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "modem_api.h"
#include "memory_api.h"
#include "uart_api.h"
#include "sms_app.h"
#include "network_app.h"
#include "modem_cmd_handler.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define MODEM_CMD_QUEUE_WAIT_TIME     200
#define MODEM_INIT_STATUS_AT_READY    128
#define MODEM_INIT_STATUS_PHB_READY   2
#define MODEM_INIT_STATUS_SMS_READY   1
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
static eSmsStatusEnum_t MODEM_CMD_SMSStatusParse (const char *status_string);
static char *MODEM_CMD_CleanQuotes (char *token);
static uint64_t MODEM_CMD_ConvertDateTimeToUTC (char *date_string);
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static eSmsStatusEnum_t MODEM_CMD_SMSStatusParse (const char *status_string) {
    if (strcmp(status_string, "REC UNREAD") == 0) {
        return eSmsStatus_Unread;
    } else if (strcmp(status_string, "REC READ") == 0) {
        return eSmsStatus_Read;
    }
    return eSmsStatus_Unknown;
}

static char *MODEM_CMD_CleanQuotes (char *token) {
    if (token == NULL) {
        return token;
    }
    size_t len = strlen(token);
    if ((len > 2) && (token[0] == '"') && (token[len - 1] == '"')) {
        token[len - 1] = '\0';
        ++token;
    } else if ((len == 2) && (token[0] == '"') && (token[1] == '"')) {
        token[0] = '\0';
    }
    return token;
}

static uint64_t MODEM_CMD_ConvertDateTimeToUTC (char *date_string) {
    struct tm tm = {0};
    int hours_offset;
    char sign;
    char *saveptr = NULL;
    char *token = strtok_r(date_string, "/ :", &saveptr);
    if (token == NULL) {
        printf("Failed to parse date-time string.\n");
        return 0;
    }
    tm.tm_year = atoi(token) - 1900;
    token = strtok_r(NULL, "/ :", &saveptr);
    if (token == NULL) {
        printf("Failed to parse date-time string.\n");
        return 0;
    }
    tm.tm_mon = atoi(token) - 1;
    token = strtok_r(NULL, "/ :", &saveptr);
    if (token == NULL) {
        printf("Failed to parse date-time string.\n");
        return 0;
    }
    tm.tm_mday = atoi(token);
    token = strtok_r(NULL, "/ :", &saveptr);
    if (token == NULL) {
        printf("Failed to parse date-time string.\n");
        return 0;
    }
    tm.tm_hour = atoi(token);
    token = strtok_r(NULL, "/ :", &saveptr);
    if (token == NULL) {
        printf("Failed to parse date-time string.\n");
        return 0;
    }
    tm.tm_min = atoi(token);
    token = strtok_r(NULL, "/ :", &saveptr);
    if (token == NULL) {
        printf("Failed to parse date-time string.\n");
        return 0;
    }
    tm.tm_sec = atoi(token);
    token = strpbrk(token, "+-");
    if (token == NULL) {
        printf("Failed to find timezone offset.\n");
        return 0;
    }
    sign = *token;
    hours_offset = atoi(token + 1);
    time_t timestamp = mktime(&tm);
    if (timestamp == -1) {
        printf("Failed to convert to timestamp.\n");
        return 0;
    }
    timestamp += (sign == '-') ? -hours_offset * 3600 : hours_offset * 3600;
    return (uint64_t) timestamp;
}
/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool MODEM_CMD_OkHandler (const sCommandParams_t *cmd_params) {
    if (CMD_API_CheckCmdParams(cmd_params) == false) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid command parameters.\r");
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size, "MODEM RESPONDED AS <- %s\r", cmd_params->params);
    Modem_API_SetCommandResult(eModemStatus_Ok);
    return true;
}

bool MODEM_CMD_ErrorHandler (const sCommandParams_t *cmd_params) {
    if (CMD_API_CheckCmdParams(cmd_params) == false) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid command parameters.\r");
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size, "MODEM RESPONDED AS <- %s\r", cmd_params->params);
    Modem_API_SetCommandResult(eModemStatus_Error);
    return true;
}

bool MODEM_CMD_SendSMSHandler (const sCommandParams_t *cmd_params) {
    snprintf(cmd_params->response, cmd_params->response_size, "MODEM RESPONDED AS <- %s\r", cmd_params->params);
    Modem_API_SetCommandResult(eModemStatus_ReadyToSendMessage);
    return true;
}

bool MODEM_CMD_CMTIHandler (const sCommandParams_t *cmd_params) {
    if (CMD_API_CheckCmdParams(cmd_params) == false) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid command parameters.\r");
        return false;
    }
    char *token = NULL;
    char *saveptr = NULL;
    token = strtok_r((char*) cmd_params->params, ",", &saveptr);
    token = strtok_r(NULL, ",", &saveptr);
    if (token == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Index missing in CMTI.\r");
        return false;
    }
    uint32_t index = strtoul(token, NULL, 10);
    if (index == -1) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid index.\r");
        return false;
    }
    if (SMS_APP_AddIndex(&index) == false) {
        snprintf(cmd_params->response, cmd_params->response_size, "Failed to put index into queue.\r");
        return false;
    }
    return true;
}

bool MODEM_CMD_CmglHandler (const sCommandParams_t *cmd_params) {
    if (CMD_API_CheckCmdParams(cmd_params) == false) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid command parameters.\r");
        return false;
    }
    const char *delimiter = ",";
    char *saveptr = NULL;
    sSmsMessage_t sms_message = {0};
    char *token = strtok_r((char*) cmd_params->params, delimiter, &saveptr);
    char *end_ptr = NULL;
    if (token == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Missing index parameter.\r");
        return false;
    }
    sms_message.index = strtoul(token, &end_ptr, 10);
    if (*end_ptr != '\0') {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid index format.\r");
        return false;
    }
    token = strtok_r(NULL, delimiter, &saveptr);
    if (token == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Missing status parameter.\r");
        return false;
    }
    sms_message.status = MODEM_CMD_SMSStatusParse(MODEM_CMD_CleanQuotes(token));
    token = strtok_r(NULL, delimiter, &saveptr);
    if (token == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Missing number parameter.\r");
        return false;
    }
    snprintf(sms_message.phone_number, sizeof(sms_message.phone_number), "%s", MODEM_CMD_CleanQuotes(token));
    token = strtok_r(NULL, delimiter, &saveptr);
    if (token == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Missing originator name parameter.\r");
        return false;
    }
    snprintf(sms_message.originator_name, sizeof(sms_message.originator_name), "%s", MODEM_CMD_CleanQuotes(token));
    token = strtok_r(NULL, delimiter, &saveptr);
    if (token == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Missing time stamp parameter.\r");
        return false;
    }
    sms_message.time_stamp = MODEM_CMD_ConvertDateTimeToUTC(MODEM_CMD_CleanQuotes(token));
    if (sms_message.time_stamp == 0) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid time stamp format.\r");
        return false;
    }
    sMessage_t message = {0};
    if (UART_API_ReceiveMessage(eUartApiPort_Modem, &message, MODEM_CMD_QUEUE_WAIT_TIME) == false) {
        snprintf(cmd_params->response, cmd_params->response_size, "Failed to receive SMS content.\r");
        return false;
    }
    sms_message.message_content = message.data;
    sms_message.message_content_len = message.length;
    if (MODEM_API_PutToQueue(eQueueType_SMS, &sms_message, MODEM_CMD_QUEUE_WAIT_TIME) == false) {
        snprintf(cmd_params->response, cmd_params->response_size, "Failed to enqueue SMS message.\r");
        Memory_API_Free(sms_message.message_content);
        return false;
    }
    if (SMS_APP_AddIndex(&sms_message.index) == false) {
        snprintf(cmd_params->response, cmd_params->response_size, "Failed to put index into queue.\r");
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size, "SMS message enqueued successfully.\r");
    return true;
}

bool MODEM_CMD_CmgrHandler (const sCommandParams_t *cmd_params) {
    if (CMD_API_CheckCmdParams(cmd_params) == false) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid command parameters.\r");
        return false;
    }
    const char *delimiter = ",";
    char *saveptr = NULL;
    sSmsMessage_t sms_message = {0};
    char *token = strtok_r((char*) cmd_params->params, delimiter, &saveptr);
    if (token == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Missing status parameter.\r");
        return false;
    }
    token++;
    sms_message.status = MODEM_CMD_SMSStatusParse(MODEM_CMD_CleanQuotes(token));
    token = strtok_r(NULL, delimiter, &saveptr);
    if (token == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Missing number parameter.\r");
        return false;
    }
    snprintf(sms_message.phone_number, sizeof(sms_message.phone_number), "%s", MODEM_CMD_CleanQuotes(token));
    token = strtok_r(NULL, delimiter, &saveptr);
    if (token != NULL) {
        snprintf(sms_message.originator_name, sizeof(sms_message.originator_name), "%s", MODEM_CMD_CleanQuotes(token));
    }
    token = strtok_r(NULL, delimiter, &saveptr);
    if (token == NULL) {
        snprintf(cmd_params->response, cmd_params->response_size, "Missing time stamp parameter.\r");
        return false;
    }
    sms_message.time_stamp = MODEM_CMD_ConvertDateTimeToUTC(MODEM_CMD_CleanQuotes(token));
    if (sms_message.time_stamp == 0) {
        snprintf(cmd_params->response, cmd_params->response_size, "Invalid time stamp format.\r");
        return false;
    }
    sMessage_t message = {0};
    if (UART_API_ReceiveMessage(eUartApiPort_Modem, &message, MODEM_CMD_QUEUE_WAIT_TIME) == false) {
        snprintf(cmd_params->response, cmd_params->response_size, "Failed to receive SMS content.\r");
        return false;
    }
    sms_message.message_content = message.data;
    sms_message.message_content_len = message.length;
    if (MODEM_API_PutToQueue(eQueueType_SMS, &sms_message, MODEM_CMD_QUEUE_WAIT_TIME) == false) {
        snprintf(cmd_params->response, cmd_params->response_size, "Failed to enqueue SMS message.\r");
        Memory_API_Free(sms_message.message_content);
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size, "SMS message enqueued successfully.\r");
    return true;
}

bool MODEM_CMD_ETLHandler (const sCommandParams_t *cmd_params) {
    if (CMD_API_CheckCmdParams(cmd_params) == false) {
        return false;
    }
    uint8_t new_socket_id = strtoul(cmd_params->params, NULL, 10);
    if (Network_APP_QueueSocketId(&new_socket_id) == false) {
        snprintf(cmd_params->response, cmd_params->response_size, "Failed to put index into queue.\r");
        return false;
    }
    snprintf(cmd_params->response, cmd_params->response_size, "Socket ID updated to %u.\r\n", new_socket_id);
    return true;
}

bool MODEM_CMD_EindHandler (const sCommandParams_t *cmd_params) {
    if (CMD_API_CheckCmdParams(cmd_params) == false) {
        return false;
    }
    uint8_t eind_number = strtoul(cmd_params->params, NULL, 10);
    switch (eind_number) {
        case MODEM_INIT_STATUS_AT_READY: {
            snprintf(cmd_params->response, cmd_params->response_size, "AT Ready!\r\n");
            break;
        }
        case MODEM_INIT_STATUS_PHB_READY: {
            snprintf(cmd_params->response, cmd_params->response_size, "PHB Ready!\r\n");
            break;
        }
        case MODEM_INIT_STATUS_SMS_READY: {
            Modem_API_Unlock();
            snprintf(cmd_params->response, cmd_params->response_size, "SMS_Ready!\r\n");
            break;
        }
    }
    return true;
}
