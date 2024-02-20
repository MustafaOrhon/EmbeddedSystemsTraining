#ifndef SOURCE_UTILITY_RING_BUFFER_H_
#define SOURCE_UTILITY_RING_BUFFER_H_
/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdbool.h>
#include <stdint.h>
/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/
typedef struct {
    uint8_t* buffer;     // Pointer to the buffer holding the data
    size_t capacity;     // Maximum number of elements in the buffer
    volatile size_t head;    // Index of the head in the buffer
    volatile size_t tail;    // Index of the tail in the buffer
    size_t count;        // Number of elements currently in the buffer
} sRingBuffer_t;
/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
sRingBuffer_t* Ring_Buffer_Init (size_t capacity);
bool Ring_Buffer_Write (sRingBuffer_t *ringBuffer, uint8_t data);
bool Ring_Buffer_Read (sRingBuffer_t *ringBuffer, uint8_t *data);
#endif /* SOURCE_UTILITY_RING_BUFFER_H_ */
