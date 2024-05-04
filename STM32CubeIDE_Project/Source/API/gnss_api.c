/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdio.h>
#include "cmsis_os.h"
#include "cmd_api.h"
#include "uart_api.h"
#include "memory_api.h"
#include "debug_api.h"
#include "gnss_cmd_handler.h"
#include "math_utils.h"
#include "gpio_driver.h"
#include "gnss_api.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define GNSS_UART_PORT                eUartApiPort_Gnss
#define GNSS_BAUD_RATE                115200
#define GNSS_UART_DELIMITER           "\r\n"
#define GNSS_UART_DELIMITER_LENGTH    (sizeof(GNSS_UART_DELIMITER) - 1)
#define GNSS_RESPONSE_BUFFER_SIZE     512
DEFINE_DEBUG_MODULE_TAG(GNSS_API);
#define DEFINE_CMD(cmd, handler_func, sep) { \
    .command = #cmd, \
    .command_size = sizeof(#cmd) - 1, \
    .handler = &handler_func, \
    .separator = sep, \
    .separator_length = sizeof(sep) - 1 \
}
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
typedef enum {
    eGnssCmdEnum_Gngga,
    eGnssCmdEnum_Last,
} eGnssCmdEnum_t;
/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
static const osThreadAttr_t g_gnss_api_thread_attr = {
    .name = "GNSS_API_THREAD",
    .stack_size = 4 * 200,
    .priority = osPriorityBelowNormal,
};

static const sCommand_t g_gnss_cmd_table[eGnssCmdEnum_Last] = {
  DEFINE_CMD($GNGGA,GNSS_CMD_GNGGAHandler,",")
};
/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static osThreadId_t g_gnss_api_thread_id = NULL;
sMessage_t g_gnss_received_message = {0};
static char g_gnss_response_buffer[GNSS_RESPONSE_BUFFER_SIZE] = {0};
static sCmdParser_t g_command_parser = {
    .command_table = g_gnss_cmd_table,
    .command_table_size = DEFINE_ARRAY_LEN(g_gnss_cmd_table),
    .response = g_gnss_response_buffer,
    .response_size = GNSS_RESPONSE_BUFFER_SIZE
};

static sGNSSDataParams_t g_current_gnss_data = {0};
/**********************************************************************************************************************
 * Exported variables and references
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of private functions
 *********************************************************************************************************************/
static void GNSS_API_Thread (void *argument);
static void GNSS_API_GnssTurnOn (void);
static void GNSS_API_FormatTime (double numeric_time, char *buffer, size_t buffer_size);
/**********************************************************************************************************************
 * Definitions of private functions
 *********************************************************************************************************************/
static void GNSS_API_Thread (void *argument) {
    while (1) {
        if (UART_API_ReceiveMessage(GNSS_UART_PORT, &g_gnss_received_message, osWaitForever) == false) {
            continue;
        }
        if(CMD_API_ProcessCommand(g_gnss_received_message.data,g_gnss_received_message.length,&g_command_parser) == true){
            //TRACE_INFO(g_command_parser.response);
        }
        Memory_API_Free(g_gnss_received_message.data);
    }
}

static void GNSS_API_GnssTurnOn (void) {
    GPIO_Driver_WritePin(eGpioDriverPin_GnssOnPin, true);
}

static void GNSS_API_FormatTime (double numeric_time, char *buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size < 9) {
        return;
    }
    int hours = (int) numeric_time / 10000;
    int minutes = ((int) numeric_time % 10000) / 100;
    int seconds = (int) numeric_time % 100;
    snprintf(buffer, buffer_size, "%02d:%02d:%02d", hours, minutes, seconds);
}
/**********************************************************************************************************************
 * Definitions of exported functions
 *********************************************************************************************************************/
bool GNSS_API_Init(void){
    if(UART_API_Init(GNSS_UART_PORT,GNSS_BAUD_RATE,GNSS_UART_DELIMITER,GNSS_UART_DELIMITER_LENGTH) == false){
        return false;
    }
    if (g_gnss_api_thread_id == NULL) {
        g_gnss_api_thread_id = osThreadNew(GNSS_API_Thread, NULL, &g_gnss_api_thread_attr);
        if (g_gnss_api_thread_id == NULL) {
            return false;
        }
    }
    GNSS_API_GnssTurnOn();
    return true;
}

void GNSS_API_UpdateGNSSCoordinates(double timestamp, double latitude, double longitude) {
    g_current_gnss_data.timestamp = timestamp;
    g_current_gnss_data.latitude = latitude;
    g_current_gnss_data.longitude = longitude;
}

void GNSS_API_FormatGNSSData (char *buffer, size_t buffer_size) {
    if ((buffer == NULL) || (buffer_size == 0)) {
        return;
    }
    char formatted_time[9] = {0};
    GNSS_API_FormatTime(g_current_gnss_data.timestamp, formatted_time, sizeof(formatted_time));
    int lat_int_part = (int)g_current_gnss_data.latitude;
    int lat_dec_part = (int)((g_current_gnss_data.latitude - lat_int_part) * 1000000);
    int lon_int_part = (int)g_current_gnss_data.longitude;
    int lon_dec_part = (int)((g_current_gnss_data.longitude - lon_int_part) * 1000000);
    snprintf(buffer, buffer_size, "Timestamp: %s, Latitude: %d.%06d, Longitude: %d.%06d\r\n",formatted_time, lat_int_part, lat_dec_part, lon_int_part, lon_dec_part);
}
