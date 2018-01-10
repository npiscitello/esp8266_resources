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

// LED is always on for 100ms, period changes based on state
// steady on: idle
// flickering: error (check AP name / password)
// fast blink: connecting
// slow blink: connected
#define LEDPER_IDLE 100
#define LEDPER_ERR 120
#define LEDPER_NOCONN 500
#define LEDPER_CONN 5000
#define LEDON 100

// we need to remember what the wifi status is
uint8_t last_wifi_status;

static volatile os_timer_t on_timer;
static volatile os_timer_t off_timer;
static volatile os_timer_t poll_timer;

void ICACHE_FLASH_ATTR led_off(void *arg) {
  (void)arg;
  gpio_output_set(GPIO, 0, 0, 0);
  os_timer_disarm(&off_timer);
}

void ICACHE_FLASH_ATTR led_on(void *arg) {
  (void)arg;
  gpio_output_set(0, GPIO, 0, 0);
  os_timer_arm(&off_timer, LEDON, 1);
}

void ICACHE_FLASH_ATTR poll_connection(void *arg) {
  (void)arg;

  // we only want to do something if the status has changed.
  if( wifi_station_get_connect_status() != last_wifi_status ) {

    last_wifi_status = wifi_station_get_connect_status();
    os_timer_disarm(&on_timer);

    switch( last_wifi_status ) {
      case STATION_GOT_IP:
        os_timer_arm(&on_timer, LEDPER_CONN, 1);
        break;

      case STATION_IDLE:
        os_timer_arm(&on_timer, LEDPER_IDLE, 1);
        break;

      case STATION_CONNECTING:
        os_timer_arm(&on_timer, LEDPER_NOCONN, 1);
        break;

      default:
        os_timer_arm(&on_timer, LEDPER_ERR, 1);
        break;
    }
  }
}

void ICACHE_FLASH_ATTR user_init() {

  // init gpio subsytem
  gpio_init();

  // config GPIO5 to actually be a GPIO and enable output
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
  gpio_output_set(0, 0, GPIO, 0);
  gpio_output_set(GPIO, 0, 0, 0);
  wifi_station_connect();
  last_wifi_status = wifi_station_get_connect_status();

  // set up led timers and wifi status polling
  os_timer_setfn(&on_timer, (os_timer_func_t *)led_on, NULL);
  os_timer_setfn(&off_timer, (os_timer_func_t *)led_off, NULL);
  os_timer_setfn(&poll_timer, (os_timer_func_t *)poll_connection, NULL);
  os_timer_arm(&poll_timer, CONN_POLL, 1);

  // make sure we start with a blink (kind of a hello world)
  led_on(0);

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
