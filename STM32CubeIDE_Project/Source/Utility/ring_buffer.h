#ifndef SOURCE_UTILITY_RING_BUFFER_H_
#define SOURCE_UTILITY_RING_BUFFER_H_
/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/
typedef struct {
    uint8_t *buffer;
    size_t capacity;
    volatile size_t head;
    volatile size_t tail;
    size_t count;
} sRingBuffer_t;
/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
sRingBuffer_t *Ring_Buffer_Init(size_t capacity);
bool Ring_Buffer_Write(sRingBuffer_t *ring_buffer, uint8_t data);
bool Ring_Buffer_Read(sRingBuffer_t *ring_buffer, uint8_t *data);
#endif /* SOURCE_UTILITY_RING_BUFFER_H_ */
