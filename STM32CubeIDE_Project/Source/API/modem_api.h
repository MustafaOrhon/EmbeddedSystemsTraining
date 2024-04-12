#ifndef SOURCE_API_MODEM_API_H_
#define SOURCE_API_MODEM_API_H_
/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/
typedef enum {
    eModemStatus_Ok,
    eModemStatus_Error,
    eModemStatus_Timeout,
    eModemStatus_Busy,
    eModemStatus_ReadyToSendMessage,
    eModemStatus_Unknown
} eModemStatusEnum_t;

typedef enum {
    eQueueType_First = 0,
    eQueueType_General = eQueueType_First,
    eQueueType_SMS,
    eQueueType_Last
} eQueueTypeEnum_t;

typedef enum {
    eSmsStatus_Unknown = 0,
    eSmsStatus_Unread,
    eSmsStatus_Read,
} eSmsStatusEnum_t;

typedef struct {
    uint32_t index;
    eSmsStatusEnum_t status;
    char phone_number[16];
    char originator_name[30];
    uint64_t time_stamp;
    char *message_content;
    size_t message_content_len;
} sSmsMessage_t;
/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool MODEM_API_Init (void);
eModemStatusEnum_t MODEM_API_SendAndWait (const char *cmd, uint32_t wait_time);
eModemStatusEnum_t MODEM_API_SendAndWaitRepeat (const char *cmd, uint32_t wait_time, uint32_t repeat, uint32_t delay);
bool Modem_API_Lock (void);
bool Modem_API_Unlock (void);
void Modem_API_SetCommandResult (eModemStatusEnum_t status);
bool MODEM_API_ReceiveFromQueue (eQueueTypeEnum_t queue, void *message, uint32_t queue_wait_time);
bool MODEM_API_PutToQueue (eQueueTypeEnum_t queue, const void *message, uint32_t queue_wait_time);
#endif /* SOURCE_API_MODEM_API_H_ */
