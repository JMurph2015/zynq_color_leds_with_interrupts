// Copyright 2024 Joseph Murphy
// All Rights Reserved

#include <xil_types.h>

typedef struct {
    UINTPTR base_addr;
} PwmController;

u32 PwmController_Init(PwmController *inst_ptr, UINTPTR base_addr);
u32 PwmController_Set(PwmController *inst_ptr, u32 channel, u32 value);