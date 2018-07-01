#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"

/*
PIN_FUNC_SELECT(<COL_1>, FUNC_<COL_0>);
GPIO0:  PERIPHS_IO_MUX_GPIO0_U
GPIO1:  PERIPHS_IO_MUX_U0TXD_U
GPIO2:  PERIPHS_IO_MUX_GPIO2_U
GPIO3:  PERIPHS_IO_MUX_U0RXD_U
GPIO4:  PERIPHS_IO_MUX_GPIO4_U
GPIO5:  PERIPHS_IO_MUX_GPIO5_U
GPIO6:  PERIPHS_IO_MUX_SD_CLK_U
GPIO7:  PERIPHS_IO_MUX_SD_DATA0_U
GPIO8:  PERIPHS_IO_MUX_SD_DATA1_U
GPIO9:  PERIPHS_IO_MUX_SD_DATA2_U
GPIO10: PERIPHS_IO_MUX_SD_DATA3_U
GPIO11: PERIPHS_IO_MUX_SD_CMD_U
GPIO12: PERIPHS_IO_MUX_MTDI_U
GPIO13: PERIPHS_IO_MUX_MTCK_U
GPIO14: PERIPHS_IO_MUX_MTMS_U
GPIO15: PERIPHS_IO_MUX_MTDO_U
*/

// LED pin. Do not change, the pin is hardcoded in user_init()
#define LED BIT5

// Extra GPIO pin. Do not change, the pin is hardcoded in user_init()
#define GPIO BIT0

// the LED is on for 50ms every 250ms
#define LEDPER 250
#define LEDON 50

static volatile os_timer_t on_timer;
static volatile os_timer_t off_timer;

void ICACHE_FLASH_ATTR led_off(void *arg) {
  (void)arg;
  // LED is sunk, so it has to be opposite the actual GPIO
  gpio_output_set(LED, GPIO, 0, 0);
  os_timer_disarm(&off_timer);
}

void ICACHE_FLASH_ATTR led_on(void *arg) {
  (void)arg;
  // LED is sunk, so it has to be opposite the actual GPIO
  gpio_output_set(GPIO, LED, 0, 0);
  os_timer_arm(&off_timer, LEDON, 1);
}

void ICACHE_FLASH_ATTR user_init()
{
  // init gpio subsytem
  gpio_init();

  // config GPIO5 (internal LED) to actually be a GPIO and enable output
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
  // set, clear, enable, disable (32 bit args)
  gpio_output_set(0, 0, LED | GPIO, 0);

  // setup timers
  os_timer_setfn(&on_timer, (os_timer_func_t *)led_on, NULL);
  os_timer_setfn(&off_timer, (os_timer_func_t *)led_off, NULL);
  os_timer_arm(&on_timer, LEDPER, 1);
}
