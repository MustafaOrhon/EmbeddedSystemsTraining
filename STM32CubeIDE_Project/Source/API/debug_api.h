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
#define DEFINE_DEBUG_MODULE_TAG(tag) static const char *module_tag = #tag
#define TRACE_INFO(...)    DEBUG_API_Print(module_tag, eDebugMessage_Info, NULL, 0, __VA_ARGS__)
#define TRACE_WARNING(...) DEBUG_API_Print(module_tag, eDebugMessage_Warning, __FILE__, __LINE__, __VA_ARGS__)
#define TRACE_ERROR(...)   DEBUG_API_Print(module_tag, eDebugMessage_Error, __FILE__, __LINE__, __VA_ARGS__)
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
