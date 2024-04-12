#ifndef SOURCE_API_MODEM_CMD_HANDLER_H_
#define SOURCE_API_MODEM_CMD_HANDLER_H_
/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdbool.h>
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
bool MODEM_CMD_OkHandler (const sCommandParams_t *cmd_params);
bool MODEM_CMD_ErrorHandler (const sCommandParams_t *cmd_params);
bool MODEM_CMD_CmglHandler (const sCommandParams_t *cmd_params);
bool MODEM_CMD_CmgrHandler (const sCommandParams_t *cmd_params);
bool MODEM_CMD_SendSMSHandler (const sCommandParams_t *cmd_params);
bool MODEM_CMD_CMTIHandler (const sCommandParams_t *cmd_params);
bool MODEM_CMD_ETLHandler (const sCommandParams_t *cmd_params);
bool MODEM_CMD_EindHandler (const sCommandParams_t *cmd_params);
#endif /* SOURCE_API_MODEM_CMD_HANDLER_H_ */
