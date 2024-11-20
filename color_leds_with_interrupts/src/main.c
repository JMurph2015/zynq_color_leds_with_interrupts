#include <sleep.h>
#include <stdbool.h>
#include <stdio.h>
#include <xgpio.h>
#include <xil_io.h>
#include <xil_printf.h>
#include <xil_types.h>
#include <xiltimer.h>
#include <xinterrupt_wrap.h>
#include <xparameters.h>
#include <xparameters_ps.h>
#include <xstatus.h>

#include "global_timer.h"
#include "pwm_controller.h"

#define INPUTS_GPIO_BASEADDR XPAR_AXI_GPIO_0_BASEADDR
#define PWM_BASEADDR XPAR_MY_PWM_CONTROLLER_0_BASEADDR

#define BUTTON_CHANNEL 1
#define BUTTON_CHANNEL_MASK XGPIO_IR_CH1_MASK
#define LED_CHANNEL 2

u32 init_inputs_gpio(XGpio *inst_ptr);
u32 init_leds_gpio(XGpio *inst_ptr);
u32 init_interrupts(XGpio *input_dev);
void handle_button(void *inst_ptr);

static volatile u32 current_color_channel = 0;
static volatile u32 prev_button_state = 0b0000;
static volatile XTime last_press_time[4] = {0x0, 0x0, 0x0, 0x0};
static volatile u32 rgb_0_color[3] = {0x0, 0x0, 0x0};

static const u32 LOOKUP_TABLE[3] = {
    0b1000,
    0b0100,
    0b0010,
};

static PwmController *pwm_global = NULL;

int main() {
  u32 status;

  status = GlobalTimer_Init(XPAR_GLOBAL_TIMER_BASEADDR);
  if (status != XST_SUCCESS) {
    xil_printf("Failed to init global timer.\n\r");
    return status;
  }

  status = GlobalTimer_Start();
  if (status != XST_SUCCESS) {
    xil_printf("Failed to start global timer");
    return status;
  }

  XGpio inputs;
  status = init_inputs_gpio(&inputs);
  if (status != XST_SUCCESS) {
    xil_printf("Failed to init inputs gpio device.");
    return status;
  }

  PwmController pwm_device;
  status = PwmController_Init(&pwm_device, PWM_BASEADDR);
  if (status != XST_SUCCESS) {
    xil_printf("Failed to init PWM controller device.");
    return status;
  }
  pwm_global = &pwm_device;

//   status = init_interrupts(&inputs);
//   if (status != XST_SUCCESS) {
//     xil_printf("Failed to init interrupts for input buttons");
//     return status;
//   }

  XGpio_SetDataDirection(&inputs, LED_CHANNEL, 0x0);
  xil_printf("Dual channel mode: %d\n\r", inputs.IsDual);
  printf("Direction mask 1: %u\n\r", XGpio_GetDataDirection(&inputs, BUTTON_CHANNEL));
  printf("Direction mask 2: %u\n\r", XGpio_GetDataDirection(&inputs, LED_CHANNEL));
  XGpio_DiscreteWrite(&inputs, LED_CHANNEL, 0x0001);
  if ((XGpio_GetDataDirection(&inputs, LED_CHANNEL) & 0b1111) != 0) {
      xil_printf("Found wrong data direction on LED channel.\n\r");
  }

  XTime start_time = 0;
  GlobalTimer_GetCount(&start_time);
  for (u32 i = 0; i < 0xFFFF; i++) {
    XTime new_time = 0;
    GlobalTimer_GetCount(&new_time);
    double sec_since_start_approx = ((double)(new_time - start_time)) /
                                    ((double)GLOBAL_TMR_COUNTS_PER_SECOND);
    printf("Time since loop start: %lf\n\r", sec_since_start_approx);
    xil_printf("Current color channel: %ld\n\r", current_color_channel);
    printf("Current colors: {%d, %d, %d}\n\r", rgb_0_color[0], rgb_0_color[1], rgb_0_color[2]);
    sleep(1);
  }

  status = GlobalTimer_Stop();
  return status;
}

u32 init_inputs_gpio(XGpio *inst_ptr) {
    XGpio_Config *cfg = XGpio_LookupConfig(INPUTS_GPIO_BASEADDR);
    u32 status = XGpio_CfgInitialize(inst_ptr, cfg, cfg->BaseAddress);
    if (status != XST_SUCCESS) {
        return status;
    }
    XGpio_SetDataDirection(inst_ptr, LED_CHANNEL, 0x00000000);
    XGpio_SetDataDirection(inst_ptr, BUTTON_CHANNEL, 0xFFFFFFFF);
    status = XGpio_SelfTest(inst_ptr);
    return status;
}

u32 init_interrupts(XGpio *input_dev) {
    XGpio_Config *cfg = XGpio_LookupConfig(input_dev->BaseAddress);
    u32 status = XSetupInterruptSystem(input_dev, &handle_button, cfg->IntrId, cfg->IntrParent, XINTERRUPT_DEFAULT_PRIORITY);
    if (status != XST_SUCCESS) {
        return status;
    }
    XGpio_InterruptGlobalEnable(input_dev);
    XGpio_InterruptEnable(input_dev, BUTTON_CHANNEL_MASK);
    if (XGpio_InterruptGetEnabled(input_dev) == 0) {
        status = XST_NOT_ENABLED;
        xil_printf("Failed to enable interrupt using XGpio_InterruptEnable().\n\r");
        return status;
    }
    return status;
}

void handle_button(void *inst_ptr) {
    XGpio *dev = (XGpio *)inst_ptr;

    // Read current button data.
    u32 button_data = XGpio_DiscreteRead(dev, BUTTON_CHANNEL);
    // XOR with last known state to quickly figure out what changed.
    u32 button_pressed = button_data ^ prev_button_state;
    // Save current data to the prev_button_state for next press.
    prev_button_state = button_data;

    // discard failling edge of button presses
    if ((button_data & button_pressed) == 0) {
        XGpio_InterruptClear(dev, BUTTON_CHANNEL_MASK);
        return;
    }

    #if __has_builtin(__builtin_ctz)
        u32 which_button = __builtin_ctz(button_pressed);
    #else
        u32 which_button;
        switch (button_pressed) {
            case 0b0001:
                which_button = 0;
                break;
            case 0b0010:
                which_button = 1;
                break;
            case 0b0100:
                which_button = 2;
                break;
            default:
                which_button = 3;
        }
    #endif

    XTime time_now;
    u32 status = GlobalTimer_GetCount(&time_now);
    if (status == XST_SUCCESS) {
        bool is_bounce = (time_now - last_press_time[which_button] < (GLOBAL_TMR_COUNTS_PER_SECOND / 20));
        last_press_time[which_button] = time_now;

        if (is_bounce) {
            XGpio_InterruptClear(dev, BUTTON_CHANNEL_MASK);
            return;
        }
    }

    if (which_button < 2) {
        if (which_button == 0) {
            current_color_channel = current_color_channel == 0 ? 2 : (current_color_channel - 1);
        } else {
            current_color_channel = current_color_channel == 2 ? 0 : (current_color_channel + 1);
        }
        XGpio_DiscreteWrite(dev, LED_CHANNEL, LOOKUP_TABLE[current_color_channel]);
    } else {
        if (which_button == 2) {
            rgb_0_color[current_color_channel] = rgb_0_color[current_color_channel] == 0 ? 0 : (rgb_0_color[current_color_channel] - 1);
        } else {
            rgb_0_color[current_color_channel] = rgb_0_color[current_color_channel] == 255 ? 255 : (rgb_0_color[current_color_channel] + 1);
        }
        if (pwm_global != NULL) {
            PwmController_Set(pwm_global, current_color_channel, rgb_0_color[current_color_channel]);
        }
    }

    XGpio_InterruptClear(dev, BUTTON_CHANNEL_MASK);
}