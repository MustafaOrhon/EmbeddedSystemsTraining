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
#define TRACE_Info(tag, format, ...)    DEBUG_API_Print(tag, eDebugMessage_Info, NULL, 0, format, ##__VA_ARGS__)
#define TRACE_Warning(tag, format, ...) DEBUG_API_Print(tag, eDebugMessage_Warning, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define TRACE_Error(tag, format, ...)   DEBUG_API_Print(tag, eDebugMessage_Error, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define DEFINE_DEBUG_MODULE_TAG(tag) static const char *module_tag = #tag
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
bool DEBUG_API_Print(const char *module_tag, eDebugMessageEnum_t type, const char *file, int line, const char *format, ...);
#endif /* SOURCE_API_DEBUG_API_H_ */
