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
#define TRACE_Info(module,format, ...)    DEBUG_API_Print(module,eDebugMessage_Info, NULL, 0, format, ##__VA_ARGS__)
#define TRACE_Warning(module,format, ...) DEBUG_API_Print(module,eDebugMessage_Warning, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define TRACE_Error(module,format, ...)   DEBUG_API_Print(module,eDebugMessage_Error, __FILE__, __LINE__, format, ##__VA_ARGS__)
/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/
typedef enum {
    eDebugModule_First = 0,
    eDebugModule_Led = eDebugModule_First,
    eDebugModule_Memory,
    eDebugModule_Uart,
    eDebugModule_Cli,
    eDebugModule_Driver,
    eDebugModule_Utility,
    eDebugModule_Other,
    eDebugModuke_Last
} eDebugModuleEnum_t;
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
bool DEBUG_API_Print (eDebugModuleEnum_t module,eDebugMessageEnum_t type, const char *file, int line, const char *format, ...);
#endif /* SOURCE_API_DEBUG_API_H_ */
