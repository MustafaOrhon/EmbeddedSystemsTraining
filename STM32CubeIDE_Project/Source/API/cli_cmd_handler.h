#ifndef SOURCE_API_CLI_CMD_HANDLER_H_
#define SOURCE_API_CLI_CMD_HANDLER_H_
/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/
typedef bool (*Command_Handler)(const char *params, size_t length, char *response, size_t response_size);

typedef struct {
    const char *command;
    size_t command_size;
    Command_Handler handler;
} sCommandEntry_t;

typedef enum {
        eCliCmd_First = 0,
        eCliCmd_Set = eCliCmd_First,
        eCliCmd_Reset,
        eCliCmd_Toggle,
        eCliCmd_Blink,
        eCliCmd_Last,
} eCliCmdEnum_t;
/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool CLI_CMD_Handler_Set(const char *params, size_t length, char *response, size_t response_size);
bool CLI_CMD_Handler_Reset(const char *params, size_t length, char *response, size_t response_size);
bool CLI_CMD_Handler_Toggle(const char *params, size_t length, char *response, size_t response_size);
bool CLI_CMD_Handler_Blink(const char *params, size_t length, char *response, size_t response_size);
#endif /* SOURCE_API_CLI_CMD_HANDLER_H_ */
