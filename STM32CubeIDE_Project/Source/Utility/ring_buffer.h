#ifndef SOURCE_UTILITY_RING_BUFFER_H_
#define SOURCE_UTILITY_RING_BUFFER_H_
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
//use static in main.c
//care for coding styles
typedef struct {
    void* buffer;
    size_t element_size;//remove
    size_t capacity;
    volatile size_t head;
    volatile size_t tail;
    size_t count;
} sGenericRingBuffer_t;
/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
//change names ringbuffer_...
bool GenericRingBuffer_Init (sGenericRingBuffer_t *ringBuffer, size_t element_size, size_t capacity);
bool GenericRingBuffer_Write (sGenericRingBuffer_t *ringBuffer, const void *data);
bool GenericRingBuffer_Read (sGenericRingBuffer_t *ringBuffer, void *data);
#endif /* SOURCE_UTILITY_RING_BUFFER_H_ */
