#include "global_timer.h"
#include <stdbool.h>
#include <xil_io.h>
#include <xil_types.h>
#include <xstatus.h>

static UINTPTR GlobalTimer_BaseAddr = (UINTPTR) NULL;

u32 GlobalTimer_Init(UINTPTR addr) {
    GlobalTimer_BaseAddr = addr;
    return XST_SUCCESS;
}

u32 GlobalTimer_Start() {
    if (GlobalTimer_BaseAddr != (UINTPTR) NULL) {
        Xil_Out32(GlobalTimer_BaseAddr + GLOBAL_TMR_CONTROL_OFFSET, GLOBAL_TMR_CONTROL_ENABLE);
        return XST_SUCCESS;
    }
    return XST_DEVICE_NOT_FOUND;
}

u32 GlobalTimer_Stop() {
    if (GlobalTimer_BaseAddr != (UINTPTR) NULL) {
        Xil_Out32(GlobalTimer_BaseAddr + GLOBAL_TMR_CONTROL_OFFSET, GLOBAL_TMR_CONTROL_DISABLE);
        return XST_SUCCESS;
    }
    return XST_DEVICE_NOT_FOUND;
}

u32 GlobalTimer_GetCount(u64 *buf) {
    if (GlobalTimer_BaseAddr == (UINTPTR) NULL) {
        return XST_DEVICE_NOT_FOUND;
    }
    u64 upper_bits = Xil_In32(GlobalTimer_BaseAddr + GLOBAL_TMR_CNT_UPPER_OFFSET);
    u64 lower_bits = Xil_In32(GlobalTimer_BaseAddr + GLOBAL_TMR_CNT_LOWER_OFFSET);
    u64 upper_bits_check = Xil_In32(GlobalTimer_BaseAddr + GLOBAL_TMR_CNT_UPPER_OFFSET);
    if (upper_bits != upper_bits_check) {
        #pragma clang optimize off
        lower_bits = Xil_In32(GlobalTimer_BaseAddr + GLOBAL_TMR_CNT_LOWER_OFFSET);
        #pragma clang optimize on
        *buf = (upper_bits_check << 32) | lower_bits;
        return XST_SUCCESS;
    } else {
        #pragma clang optimize off
        u64 lower_bits_dummy_read = Xil_In32(GlobalTimer_BaseAddr + GLOBAL_TMR_CNT_LOWER_OFFSET);
        (void)lower_bits_dummy_read; // Used to make the function constant time
        #pragma clang optimize on
        *buf = (upper_bits << 32) | lower_bits;
        return XST_SUCCESS;
    }
}