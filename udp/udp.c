// types
#include "ets_sys.h"
// gpio...obvs
#include "gpio.h"
#include "os_type.h"
// wifi stuff
#include "user_interface.h"
// UDP stuff
#include "espconn.h"
// zalloc
#include "mem.h"
// memcpy
#include "osapi.h"

// pin the LED is on
#define GPIO BIT5

// port on which to listen for UDP packets
#define UDP_PORT 1025

// struct for the UDP listener
struct espconn udp_listener;

// LED state
uint8_t led_state;
#define LED_OFF 0
#define LED_ON 1

void ICACHE_FLASH_ATTR set_led(uint8_t state) {
  switch( state ) {
    case LED_OFF:
      gpio_output_set(GPIO, 0, 0, 0);
      break;

    case LED_ON:
      gpio_output_set(0, GPIO, 0, 0);
      break;
  }
  led_state = state;
}

void ICACHE_FLASH_ATTR usr_udp_recv(void *arg, char *data, unsigned short length) {
  // get and store the station IP
  //struct ip_info ip_addr;
  //wifi_get_ip_info(STATION_IF, &ip_addr);
  if( led_state == LED_ON ) {
    set_led(LED_OFF);
  } else {
    set_led(LED_ON);
  }
  return;
}

void ICACHE_FLASH_ATTR udp_init() {
  // we want UDP
  udp_listener.type = ESPCONN_UDP;
  // allocate space as a UDP listener (.proto is a pointer to a UDP or TCP struct)
  udp_listener.proto.udp = (esp_udp *)os_zalloc(sizeof(esp_udp));
  // fill in the UDP listening data
  udp_listener.proto.udp->local_port = UDP_PORT;
  udp_listener.proto.udp->remote_port = UDP_PORT;
  // register the callback for recieved UDP packets
  espconn_regist_recvcb(&udp_listener, usr_udp_recv);
  // start listening
  espconn_create(&udp_listener);
  return;
}


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

  // start with the led off
  set_led(LED_OFF);

  // actually connect to wifi
  wifi_set_opmode( STATION_MODE );
  os_memcpy(&stationConf.ssid, ssid, 32);
  os_memcpy(&stationConf.password, password, 32);
  wifi_station_set_config(&stationConf);

  // start listening for UDP packets
  udp_init();
  return;
}
