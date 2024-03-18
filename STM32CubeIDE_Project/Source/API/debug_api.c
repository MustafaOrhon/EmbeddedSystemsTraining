/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <cmsis_os.h>
#include "uart_api.h"
#include "debug_api.h"
/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define DEBUG_API_BUFFER_SIZE     128
#define OVERFLOW_MESSAGE     "[Error] Buffer overflow detected."
#define OVERFLOW_MESSAGE_LEN (sizeof(OVERFLOW_MESSAGE) - 1)
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
 static const osMutexAttr_t g_debug_mutex_attr = {
    "Debug Mutex",
    osMutexPrioInherit,
    NULL,
    0U
};
static const char *g_overflow_message = OVERFLOW_MESSAGE;
static const size_t g_overflow_message_len = OVERFLOW_MESSAGE_LEN;
/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
 static char g_debug_buffer[DEBUG_API_BUFFER_SIZE];
 static osMutexId_t g_debug_mutex_id = NULL;
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
bool DEBUG_API_Init (void) {
    if (g_debug_mutex_id == NULL) {
        g_debug_mutex_id = osMutexNew(&g_debug_mutex_attr);
        if (g_debug_mutex_id == NULL) {
            return false;
        }
    }
    return true;
}

bool DEBUG_API_Print(const char *module_tag, eDebugMessageEnum_t type, const char *file, int line, const char *format, ...) {
    if ((module_tag == NULL) || (format == NULL) || (type < eDebugMessage_First) || (type >= eDebugMessage_Last)) {
        return false;
    }
    if (((type == eDebugMessage_Warning) || (type == eDebugMessage_Error)) && (file == NULL)) {
        return false;
    }
    if (osMutexAcquire(g_debug_mutex_id, osWaitForever) != osOK) {
        return false;
    }
    bool result = false;
    memset(g_debug_buffer, 0, DEBUG_API_BUFFER_SIZE);
    int message_length = 0;
    switch (type) {
        case eDebugMessage_Info:
            message_length = snprintf(g_debug_buffer, DEBUG_API_BUFFER_SIZE, "[%s] Info: ", module_tag);
            break;
        case eDebugMessage_Warning:
            message_length = snprintf(g_debug_buffer, DEBUG_API_BUFFER_SIZE, "[%s] Warning [%s:%d]: ", module_tag, file, line);
            break;
        case eDebugMessage_Error:
            message_length = snprintf(g_debug_buffer, DEBUG_API_BUFFER_SIZE, "[%s] Error [%s:%d]: ", module_tag, file, line);
            break;
        default:
            message_length = snprintf(g_debug_buffer, DEBUG_API_BUFFER_SIZE, "[%s] [Unknown] Unknown message type: %d", module_tag, type);
            break;
    }
    if ((message_length < 0) || (message_length >= DEBUG_API_BUFFER_SIZE)) {
        UART_API_SendString(eUartApiPort_Debug, g_overflow_message, g_overflow_message_len);
        osMutexRelease(g_debug_mutex_id);
        return false;
    }
    va_list args;
    va_start(args, format);
    message_length += vsnprintf(g_debug_buffer + message_length, DEBUG_API_BUFFER_SIZE - message_length, format, args);
    va_end(args);
    if (message_length >= DEBUG_API_BUFFER_SIZE) {
        UART_API_SendString(eUartApiPort_Debug, g_overflow_message, g_overflow_message_len);
        osMutexRelease(g_debug_mutex_id);
        return false;
    }
    result = UART_API_SendString(eUartApiPort_Debug, g_debug_buffer, (size_t)message_length);
    osMutexRelease(g_debug_mutex_id);
    return result;
}

