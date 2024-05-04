/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include "cmsis_os.h"
#include "uart_api.h"
#include "memory_api.h"
#include "sms_api.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define SMS_API_WAIT_TIME 300
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static char g_cmd_buffer[64];
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
bool SMS_API_Init (void) {
    if (Modem_API_Lock() == false) {
        return false;
    }
    if (MODEM_API_SendAndWait("ATE0\r\n", SMS_API_WAIT_TIME) != eModemStatus_Ok) {
        return false;
    }
    if (MODEM_API_SendAndWait("AT+CMGF=1\r\n", SMS_API_WAIT_TIME) != eModemStatus_Ok) {
        return false;
    }
    if (MODEM_API_SendAndWait("AT+CNMI=2,1,0,0,0\r\n", SMS_API_WAIT_TIME) != eModemStatus_Ok) {
        return false;
    }
    Modem_API_Unlock();
    return true;
}

bool SMS_API_SetTextMode (void) {
    if (Modem_API_Lock() == false) {
        return false;
    }
    bool status = (MODEM_API_SendAndWait("AT+CMGF=1\r\n", 5000) == eModemStatus_Ok);
    Modem_API_Unlock();
    return (status == eModemStatus_Ok);
}

bool SMS_API_SendSms (const char *message_content) {
    if (Modem_API_Lock() == false) {
        return false;
    }
    char *at_command_send_number = "AT+CMGS=\"37069354293\"\r\n";
    eModemStatusEnum_t status = MODEM_API_SendAndWaitRepeat(at_command_send_number, 10000, 3, 5000);
    if (status != eModemStatus_ReadyToSendMessage) {
        Modem_API_Unlock();
        return false;
    }
    size_t buffer_size = strlen(message_content) + strlen("\x1A\n") + 1;
    char *text = Memory_API_Calloc(1, buffer_size);
    if (text == NULL) {
        Modem_API_Unlock();
        return false;
    }
    snprintf(text, buffer_size, "%s\x1A\n", message_content);
    status = MODEM_API_SendAndWait(text, 5000);
    Memory_API_Free(text);
    Modem_API_Unlock();
    return (status == eModemStatus_Ok);
}
bool SMS_API_ReceiveMessage (sSmsMessage_t *smsMessage) {
    if (smsMessage == NULL) {
        return false;
    }
    return MODEM_API_ReceiveFromQueue(eQueueType_SMS, smsMessage, 1000);
}

bool SMS_API_DeleteMessage (uint32_t index) {
    if (Modem_API_Lock() == false) {
        return eModemStatus_Busy;
    }
    snprintf(g_cmd_buffer, sizeof(g_cmd_buffer), "AT+CMGD=%u\r\n", index);
    eModemStatusEnum_t status = MODEM_API_SendAndWaitRepeat(g_cmd_buffer, 5000, 3, 200);
    Modem_API_Unlock();
    return (status == eModemStatus_Ok);
}

bool SMS_API_ReadMessageByIndex (uint32_t index) {
    snprintf(g_cmd_buffer, sizeof(g_cmd_buffer), "AT+CMGR=%u\r\n", index);
    eModemStatusEnum_t modemStatus = MODEM_API_SendAndWait(g_cmd_buffer, 600);
    return (modemStatus == eModemStatus_Ok);
}

bool SMS_API_ListAllMessages (void) {
    if (Modem_API_Lock() == false) {
        return false;
    }
    char *at_set_text_mode = "AT+CMGF=1\r\n";
    eModemStatusEnum_t status = MODEM_API_SendAndWait(at_set_text_mode, 5000);
    if (status != eModemStatus_Ok) {
        Modem_API_Unlock();
        return false;
    }
    char *at_command_list_all = "AT+CMGL=\"ALL\"\r\n";
    status = MODEM_API_SendAndWait(at_command_list_all, 15000);
    Modem_API_Unlock();
    return (status == eModemStatus_Ok);
}
