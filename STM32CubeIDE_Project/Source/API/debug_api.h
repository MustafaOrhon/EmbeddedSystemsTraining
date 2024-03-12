#ifndef SOURCE_API_DEBUG_API_H_
#define SOURCE_API_DEBUG_API_H_
/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/
#define DEBUG_API_Info(format, ...)    DEBUG_API_Print(eDebugMessage_Info, NULL, 0, format, ##__VA_ARGS__)
#define DEBUG_API_Warning(format, ...) DEBUG_API_Print(eDebugMessage_Warning, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define DEBUG_API_Error(format, ...)   DEBUG_API_Print(eDebugMessage_Error, __FILE__, __LINE__, format, ##__VA_ARGS__)
/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/
typedef enum {
    eDebugMessage_First = 0,
    eDebugMessage_Info = eDebugMessage_First,
    eDebugMessage_Warning,
    eDebugMessage_Error,
    eDebugMessage_Last
} eDebugMessageEnum_t;
/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool DEBUG_API_Init (void);
bool DEBUG_API_Print (eDebugMessageEnum_t type, const char *file, int line, const char *format, ...);
#endif /* SOURCE_API_DEBUG_API_H_ */
