#ifndef SOURCE_APP_SMS_APP_H_
#define SOURCE_APP_SMS_APP_H_
/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdbool.h>
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
bool SMS_APP_Init(void);
bool SMS_APP_AddIndex (uint32_t *index);
#endif /* SOURCE_APP_SMS_APP_H_ */