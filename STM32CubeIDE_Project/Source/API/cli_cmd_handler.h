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
typedef struct {
    const char *params;
    size_t length;
    char *response;
    size_t response_size;
} sCommandParams_t;

typedef bool (*sCmdHandler_t) (const sCommandParams_t *cmd_params);

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
bool CLI_CMD_SetHandler(const sCommandParams_t *cmd_params);
bool CLI_CMD_ResetHandler(const sCommandParams_t *cmd_params);
bool CLI_CMD_ToggleHandler(const sCommandParams_t *cmd_params);
bool CLI_CMD_BlinkHandler(const sCommandParams_t *cmd_params);
#endif /* SOURCE_API_CLI_CMD_HANDLER_H_ */
