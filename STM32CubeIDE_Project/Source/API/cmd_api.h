#ifndef SOURCE_API_CMD_API_H_
#define SOURCE_API_CMD_API_H_
/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
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

typedef struct {
    const char *command;
    size_t command_size;
    sCmdHandler_t handler;
    const char *separator;
    size_t separator_length;
} sCommand_t;

typedef struct {
    const sCommand_t *command_table;
    size_t command_table_size;
    char *response;
    size_t response_size;
} sCmdParser_t;
/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool CMD_API_ProcessCommand(const char *data, size_t length, const sCmdParser_t *command_context);
#endif /* SOURCE_API_CMD_API_H_ */
