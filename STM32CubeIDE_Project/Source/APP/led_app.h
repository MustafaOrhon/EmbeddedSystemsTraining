#ifndef SOURCE_APP_LED_APP_H_
#define SOURCE_APP_LED_APP_H_
/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdint.h>
/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/
typedef struct {
    unsigned int led_number;
} sLedBasicCommandParams_t;

typedef struct {
    unsigned int led_number;
    unsigned int time;
    unsigned int frequency;
} sBlinkCommandParams_t;

typedef enum {
    eLedAppCmd_First = 0,
    eLedAppCmd_Set = eLedAppCmd_First,
    eLedAppCmd_Reset,
    eLedAppCmd_Toggle,
    eLedAppCmd_Blink,
    eLedAppCmd_Last
} eLedAppCmdEnum_t;

typedef struct {
    eLedAppCmdEnum_t cmd;
    void *data;
} sLedAppCmd_t;
/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
bool LED_APP_Init(void);
bool LED_APP_Blink(uint32_t led, uint32_t total_time, uint32_t frequency);
bool LED_APP_SendCmd(const sLedAppCmd_t *params);
#endif /* SOURCE_APP_LED_APP_H_ */
