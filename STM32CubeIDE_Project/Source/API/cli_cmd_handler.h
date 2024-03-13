#ifndef SOURCE_API_CLI_CMD_HANDLER_H_
#define SOURCE_API_CLI_CMD_HANDLER_H_
/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdint.h>
/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/
#define HANDLER_API_MAX_LED_NUMBER    1
#define HANDLER_API_MAX_TIME          60
#define HANDLER_API_MIN_BLINK_FREQ    1
#define HANDLER_API_MAX_BLINK_FREQ    100
/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/
typedef enum {
        eCliCmd_First,
        eCliCmd_Set = eCliCmd_First,
        eCliCmd_Reset,
        eCliCmd_Toggle,
        eCliCmd_Blink,
        eCliCmd_Last,
} eCliCmd_t;

typedef void (*Command_Handler) (const char *params,size_t length);

typedef struct {
    const char *command;
    size_t command_size;
    Command_Handler handler;
} sCommandEntry_t;
/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
void CLI_CMD_Handler_Set(const char *params,size_t length);
void CLI_CMD_Handler_Reset(const char *params,size_t length);
void CLI_CMD_Handler_Toggle(const char *params,size_t length);
void CLI_CMD_Handler_Blink(const char *params,size_t length);

#endif /* SOURCE_API_CLI_CMD_HANDLER_H_ */
