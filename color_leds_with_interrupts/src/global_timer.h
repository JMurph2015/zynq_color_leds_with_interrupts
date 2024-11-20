#ifndef GLOBAL_TMR_H

#include <xtimer_config.h>
#include <xil_types.h>

#define GLOBAL_TMR_CNT_UPPER_OFFSET 0x04
#define GLOBAL_TMR_CNT_LOWER_OFFSET 0x00
#define GLOBAL_TMR_CONTROL_OFFSET 0x08
#define GLOBAL_TMR_CONTROL_ENABLE 0x01
#define GLOBAL_TMR_CONTROL_DISABLE 0x00
#define GLOBAL_TMR_COUNTS_PER_SECOND COUNTS_PER_SECOND

u32 GlobalTimer_Init();
u32 GlobalTimer_Start();
u32 GlobalTimer_Stop();
u32 GlobalTimer_GetCount(u64 *buf);

#endif