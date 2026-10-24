/* Host declarations for a PRIVATE task-scheduler compatibility probe.
 * No Nitro SDK implementation or header is included here.
 */
#ifndef HG_TASK_PROBE_GLOBAL_H
#define HG_TASK_PROBE_GLOBAL_H
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
typedef uint16_t u16;
typedef uint32_t u32;
typedef int32_t s32;
typedef int32_t FSOverlayID;
typedef int32_t BOOL;
#define TRUE 1
#define FALSE 0
#define GF_ASSERT(expression) assert(expression)
#endif
