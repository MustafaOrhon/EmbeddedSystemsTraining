    #ifndef SOURCE_API_CLI_CMD_HANDLER_H_
#define SOURCE_API_CLI_CMD_HANDLER_H_
/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "cmd_api.h"
/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool CLI_CMD_LedSetHandler(const sCommandParams_t *cmd_params);
bool CLI_CMD_LedResetHandler(const sCommandParams_t *cmd_params);
bool CLI_CMD_LedToggleHandler(const sCommandParams_t *cmd_params);
bool CLI_CMD_LedBlinkHandler(const sCommandParams_t *cmd_params);
bool CLI_CMD_SendATHandler(const sCommandParams_t *cmd_params);
bool CLI_CMD_StartTCPHandler(const sCommandParams_t *cmd_params);
bool CLI_CMD_StopTCPHandler(const sCommandParams_t *cmd_params);
#endif /* SOURCE_API_CLI_CMD_HANDLER_H_ */
