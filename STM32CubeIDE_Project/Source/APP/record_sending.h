#ifndef SOURCE_APP_RECORD_SENDING_H_
#define SOURCE_APP_RECORD_SENDING_H_
/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "stdint.h"
#include "stdbool.h"
/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/
typedef struct {
    char ip[16];
    uint16_t port;
} sRecordSendingAPPCommandParams_t;

typedef enum {
    eRecordSendingAPPTask_First = 0,
    eRecordSendingAPPTask_Start = eRecordSendingAPPTask_First,
    eRecordSendingAPPTask_Stop,
    eRecordSendingAPPTask_Last,
} eRecordSendingAPPTaskEnum_t;

typedef struct {
    eRecordSendingAPPTaskEnum_t task;
    sRecordSendingAPPCommandParams_t *params;
} sRecordSendingAPPTask_t;
/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool RecordSending_APP_Init (void);
bool RecordSending_APP_AddTask (const sRecordSendingAPPTask_t *params);
#endif /* SOURCE_APP_RECORD_SENDING_H_ */
