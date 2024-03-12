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
#define DEBUG_API_MUTEX_TIMEOUT   100
/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
 static const osMutexAttr_t debug_mutex_attr = {
    "Debug Mutex",
    osMutexPrioInherit,
    NULL,
    0U
};
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
        g_debug_mutex_id = osMutexNew(&debug_mutex_attr);
        if (g_debug_mutex_id == NULL) {
            return false;
        }
    }
    return true;
}

bool DEBUG_API_Print (eDebugMessageEnum_t type, const char *file, int line, const char *format, ...) {
    if (osMutexAcquire(g_debug_mutex_id, DEBUG_API_MUTEX_TIMEOUT) != osOK) {
        return false;
    }
    int offset = 0;
    bool result = false;
    memset(g_debug_buffer, 0, DEBUG_API_BUFFER_SIZE);
    switch (type) {
        case eDebugMessage_Info:
            offset += snprintf(g_debug_buffer + offset, DEBUG_API_BUFFER_SIZE - offset, "Info: ");
            break;
        case eDebugMessage_Warning:
            offset += snprintf(g_debug_buffer + offset, DEBUG_API_BUFFER_SIZE - offset, "Warning [%s:%d]: ", file, line);
            break;
        case eDebugMessage_Error:
            offset += snprintf(g_debug_buffer + offset, DEBUG_API_BUFFER_SIZE - offset, "Error [%s:%d]: ", file, line);
            break;
        default:
            snprintf(g_debug_buffer, DEBUG_API_BUFFER_SIZE, "Unknown message type: %d", type);
            break;
    }
    va_list args;
    va_start(args, format);
    vsnprintf(g_debug_buffer + offset, DEBUG_API_BUFFER_SIZE - offset, format, args);
    va_end(args);
    result = UART_API_SendString(eUartApiPort_First, g_debug_buffer, strlen(g_debug_buffer));
    osMutexRelease(g_debug_mutex_id);
    return result;
}
