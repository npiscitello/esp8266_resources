#include "ets_sys.h"
// timers
#include "osapi.h"
// gpio...obvs
#include "gpio.h"
#include "os_type.h"
// wifi stuff
#include "user_interface.h"

// pin the LED is on
#define GPIO BIT5

// check connection status every second
#define CONN_POLL 1000

void ICACHE_FLASH_ATTR user_init() {

  // init gpio subsytem
  gpio_init();

  // config GPIO5 to actually be a GPIO and enable output
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
  gpio_output_set(0, 0, GPIO, 0);
  gpio_output_set(GPIO, 0, 0, 0);

  // wifi credentials
  const char ssid[32] = "ssid";
  const char password[32] = "password";
  struct station_config stationConf;

  // actually connect to wifi
  wifi_set_opmode( STATION_MODE );
  os_memcpy(&stationConf.ssid, ssid, 32);
  os_memcpy(&stationConf.password, password, 32);
  wifi_station_set_config(&stationConf);
}
