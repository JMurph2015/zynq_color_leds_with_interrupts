#include "pwm_controller.h"
#include <xil_io.h>
#include <xil_types.h>
#include <xstatus.h>

u32 PwmController_Init(PwmController *inst_ptr, UINTPTR base_addr) {
    inst_ptr->base_addr = base_addr;
    return XST_SUCCESS;
}

u32 PwmController_Set(PwmController *inst_ptr, u32 channel, u32 value) {
    if (channel > 5) {
        return XST_FAILURE;
    }
    if (value > 255) {
        return XST_FAILURE;
    }
    UINTPTR addr = inst_ptr->base_addr + (4 * channel);
    Xil_Out32(addr, value);
    return XST_SUCCESS;
}