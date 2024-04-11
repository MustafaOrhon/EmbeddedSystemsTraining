/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "cmsis_os.h"
#include "memory_api.h"
#include "modem_api.h"
#include "debug_api.h"
#include "str_utils.h"
#include "tcp_api.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define TCP_API_CMD_DELAY_BETWEEN_REPEAT 100
DEFINE_DEBUG_MODULE_TAG(TCP_API);
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
typedef struct {
    const char *cmd;
    uint32_t timeout;
    uint32_t repeat_count;
} sTcpCommandInfo_t;

typedef enum {
    eTcpCmd_First = 0,
    eTcpCmd_Creq = eTcpCmd_First,
    eTcpCmd_Cgatt,
    eTcpCmd_CgattCheck,
    eTcpCmd_CgdCont,
    eTcpCmd_Cgact,
    eTcpCmd_CgactCheck,
    eTcpCmd_Ctzr,
    eTcpCmd_CtzrCheck,
    eTcpCmd_CgpAddr,
    eTcpCmd_EgdCont,
    eTcpCmd_Etl,
    eTcpCmd_Eipsend,
    eTcpCmd_Last
} eTcpConnectCommandEnum_t;

static const sTcpCommandInfo_t g_tcp_api_cmd_table[eTcpCmd_Last] = {
    [eTcpCmd_Creq] = {"AT+CREG?\r\n", 500, 3},
    [eTcpCmd_Cgatt] = {"AT+CGATT=1\r\n", 5000, 3},
    [eTcpCmd_CgattCheck] = {"AT+CGATT?\r\n", 5000, 3},
    [eTcpCmd_CgdCont] = {"AT+CGDCONT=1,\"IP\",\"banga\"\r\n", 5000, 3},
    [eTcpCmd_Cgact] = {"AT+CGACT=1,1\r\n", 1000, 3},
    [eTcpCmd_CgactCheck] = {"AT+CGACT?\r\n", 500, 3},
    [eTcpCmd_Ctzr] = {"AT+CTZR=1\r\n", 500, 3},
    [eTcpCmd_CtzrCheck] = {"AT+CTZR?\r\n", 500, 3},
    [eTcpCmd_CgpAddr] = {"AT+CGPADDR=1\r\n", 500, 3},
    [eTcpCmd_EgdCont] = {"AT+EGDCONT=1,\"IP\",\"banga\"\r\n", 500, 3},
    [eTcpCmd_Etl] = {"AT+ETL=1,1,0,\"%s\",%d\r\n", 5000, 3},
    [eTcpCmd_Eipsend] = {"AT+EIPSEND=%d,\"%s\"\r\n", 500, 3},
};

typedef enum {
    eTcpDisconnectCmd_First = 0,
    eTcpDisconnectCmd_EtlClose = eTcpDisconnectCmd_First,
    eTcpDisconnectCmd_CtzrClose,
    eTcpDisconnectCmd_CgattClose,
    eTcpDisconnectCmd_Last
} eTcpDisconnectCommandEnum_t;

static const sTcpCommandInfo_t g_tcp_api_disc_cmd_table[eTcpDisconnectCmd_Last] = {
    [eTcpDisconnectCmd_EtlClose] = {"AT+ETL=0,%d\r\n", 2000, 3},
    [eTcpDisconnectCmd_CtzrClose] = {"AT+CTZR=0\r\n", 2000, 3},
    [eTcpDisconnectCmd_CgattClose] = {"AT+CGATT=0\r\n", 2000, 3},
};
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static char g_command_buffer[256] = {0};
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
bool TCP_API_Connect (const char *ip, uint16_t port) {
    if (Modem_API_Lock() == false) {
        return false;
    }

    eModemStatusEnum_t status;
    status = MODEM_API_SendAndWaitRepeat(g_tcp_api_cmd_table[eTcpCmd_Creq].cmd, g_tcp_api_cmd_table[eTcpCmd_Creq].timeout, g_tcp_api_cmd_table[eTcpCmd_Creq].repeat_count, TCP_API_CMD_DELAY_BETWEEN_REPEAT);
    if (status != eModemStatus_Ok) {
        TRACE_WARNING("CREG FAIL");
        Modem_API_Unlock();
        return false;
    }
    status = MODEM_API_SendAndWaitRepeat(g_tcp_api_cmd_table[eTcpCmd_Cgatt].cmd, g_tcp_api_cmd_table[eTcpCmd_Cgatt].timeout, g_tcp_api_cmd_table[eTcpCmd_Cgatt].repeat_count, TCP_API_CMD_DELAY_BETWEEN_REPEAT);
    if (status != eModemStatus_Ok) {
        TRACE_WARNING("CGATT FAIL");
        Modem_API_Unlock();
        return false;
    }
    status = MODEM_API_SendAndWaitRepeat(g_tcp_api_cmd_table[eTcpCmd_CgattCheck].cmd, g_tcp_api_cmd_table[eTcpCmd_CgattCheck].timeout, g_tcp_api_cmd_table[eTcpCmd_CgattCheck].repeat_count, TCP_API_CMD_DELAY_BETWEEN_REPEAT);
    if (status != eModemStatus_Ok) {
        TRACE_WARNING("CGATT  CHECK FAIL");
        Modem_API_Unlock();
        return false;
    }
    status = MODEM_API_SendAndWaitRepeat(g_tcp_api_cmd_table[eTcpCmd_CgdCont].cmd, g_tcp_api_cmd_table[eTcpCmd_CgdCont].timeout, g_tcp_api_cmd_table[eTcpCmd_CgdCont].repeat_count, TCP_API_CMD_DELAY_BETWEEN_REPEAT);
    if (status != eModemStatus_Ok) {
        TRACE_WARNING("CGDCONTFAIL");
        Modem_API_Unlock();
        return false;
    }
    status = MODEM_API_SendAndWaitRepeat(g_tcp_api_cmd_table[eTcpCmd_Cgact].cmd, g_tcp_api_cmd_table[eTcpCmd_Cgact].timeout, g_tcp_api_cmd_table[eTcpCmd_Cgact].repeat_count, TCP_API_CMD_DELAY_BETWEEN_REPEAT);
    if (status != eModemStatus_Ok) {
        TRACE_WARNING("CGACT FAIL");
        Modem_API_Unlock();
        return false;
    }
    status = MODEM_API_SendAndWaitRepeat(g_tcp_api_cmd_table[eTcpCmd_CgactCheck].cmd, g_tcp_api_cmd_table[eTcpCmd_CgactCheck].timeout, g_tcp_api_cmd_table[eTcpCmd_CgactCheck].repeat_count, TCP_API_CMD_DELAY_BETWEEN_REPEAT);
    if (status != eModemStatus_Ok) {
        Modem_API_Unlock();
        return false;
    }
    status = MODEM_API_SendAndWaitRepeat(g_tcp_api_cmd_table[eTcpCmd_Ctzr].cmd, g_tcp_api_cmd_table[eTcpCmd_Ctzr].timeout, g_tcp_api_cmd_table[eTcpCmd_Ctzr].repeat_count, TCP_API_CMD_DELAY_BETWEEN_REPEAT);
    if (status != eModemStatus_Ok) {
        TRACE_WARNING("CTZRFAIL\r");
        Modem_API_Unlock();
        return false;
    }
    status = MODEM_API_SendAndWaitRepeat(g_tcp_api_cmd_table[eTcpCmd_CtzrCheck].cmd, g_tcp_api_cmd_table[eTcpCmd_CtzrCheck].timeout, g_tcp_api_cmd_table[eTcpCmd_CtzrCheck].repeat_count, TCP_API_CMD_DELAY_BETWEEN_REPEAT);
    if (status != eModemStatus_Ok) {
        Modem_API_Unlock();
        return false;
    }
    status = MODEM_API_SendAndWaitRepeat(g_tcp_api_cmd_table[eTcpCmd_CgpAddr].cmd, g_tcp_api_cmd_table[eTcpCmd_CgpAddr].timeout, g_tcp_api_cmd_table[eTcpCmd_CgpAddr].repeat_count, TCP_API_CMD_DELAY_BETWEEN_REPEAT);
    if (status != eModemStatus_Ok) {
        TRACE_WARNING("CGPADDR FAIL\r");
        Modem_API_Unlock();
        return false;
    }
    status = MODEM_API_SendAndWaitRepeat(g_tcp_api_cmd_table[eTcpCmd_EgdCont].cmd, g_tcp_api_cmd_table[eTcpCmd_EgdCont].timeout, g_tcp_api_cmd_table[eTcpCmd_EgdCont].repeat_count, TCP_API_CMD_DELAY_BETWEEN_REPEAT);
    if (status != eModemStatus_Ok) {
        TRACE_WARNING("EGD FAIL\r");
        Modem_API_Unlock();
        return false;
    }
    snprintf(g_command_buffer, sizeof(g_command_buffer), g_tcp_api_cmd_table[eTcpCmd_Etl].cmd, ip, port);
    status = MODEM_API_SendAndWaitRepeat(g_command_buffer, g_tcp_api_cmd_table[eTcpCmd_Etl].timeout, g_tcp_api_cmd_table[eTcpCmd_Etl].repeat_count, TCP_API_CMD_DELAY_BETWEEN_REPEAT * 10);
    if (status != eModemStatus_Ok) {
        TRACE_WARNING("ETLFAIL\r");
        Modem_API_Unlock();
        return false;
    }
    Modem_API_Unlock();
    return true;
}

bool TCP_API_Disconnect(uint8_t socket_id) {
    if (Modem_API_Lock() == false) {
        return false;
    }
    eModemStatusEnum_t status;
    snprintf(g_command_buffer, sizeof(g_command_buffer), g_tcp_api_disc_cmd_table[eTcpDisconnectCmd_EtlClose].cmd, socket_id);
    status = MODEM_API_SendAndWaitRepeat(g_command_buffer, g_tcp_api_disc_cmd_table[eTcpDisconnectCmd_EtlClose].timeout, g_tcp_api_disc_cmd_table[eTcpDisconnectCmd_EtlClose].repeat_count, TCP_API_CMD_DELAY_BETWEEN_REPEAT);
    if (status != eModemStatus_Ok) {
        TRACE_WARNING("ETL CLOSE FAIL\r");
        Modem_API_Unlock();
        return false;
    }
    status = MODEM_API_SendAndWaitRepeat(g_tcp_api_disc_cmd_table[eTcpDisconnectCmd_CtzrClose].cmd, g_tcp_api_disc_cmd_table[eTcpDisconnectCmd_CtzrClose].timeout, g_tcp_api_disc_cmd_table[eTcpDisconnectCmd_CtzrClose].repeat_count, TCP_API_CMD_DELAY_BETWEEN_REPEAT);
    if (status != eModemStatus_Ok) {
        TRACE_WARNING("CTZR CLOSE FAIL\r");
        Modem_API_Unlock();
        return false;
    }
    status = MODEM_API_SendAndWaitRepeat(g_tcp_api_disc_cmd_table[eTcpDisconnectCmd_CgattClose].cmd, g_tcp_api_disc_cmd_table[eTcpDisconnectCmd_CgattClose].timeout, g_tcp_api_disc_cmd_table[eTcpDisconnectCmd_CgattClose].repeat_count, TCP_API_CMD_DELAY_BETWEEN_REPEAT);
    if (status != eModemStatus_Ok) {
        TRACE_WARNING("CGATT CLOSE FAIL\r");
        Modem_API_Unlock();
        return false;
    }
    Modem_API_Unlock();
    return true;
}

bool TCP_API_Send (const uint8_t *data, size_t data_length,uint8_t socket_id) {
    if (Modem_API_Lock() == false) {
        return false;
    }
    char *hex_data = StringUtils_ConvertToHex(data, data_length);
    if (hex_data == NULL) {
        Modem_API_Unlock();
        return false;
    }
    snprintf(g_command_buffer, sizeof(g_command_buffer), g_tcp_api_cmd_table[eTcpCmd_Eipsend].cmd, socket_id, hex_data);
    eModemStatusEnum_t status = MODEM_API_SendAndWaitRepeat(g_command_buffer, g_tcp_api_cmd_table[eTcpCmd_Eipsend].timeout, g_tcp_api_cmd_table[eTcpCmd_Eipsend].repeat_count, TCP_API_CMD_DELAY_BETWEEN_REPEAT);
    Memory_API_Free(hex_data);
    Modem_API_Unlock();
    return (status == eModemStatus_Ok);
}
