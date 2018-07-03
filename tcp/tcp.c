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
// uart printf
#include "driver/uart.h"

// pin the LED is on
#define GPIO BIT5

// START UART TEST BLOCK
static volatile os_timer_t uart_timer;

void ICACHE_FLASH_ATTR uart(void *arg) {
  os_printf("test\n");
  return;
}

void ICACHE_FLASH_ATTR user_init() {
  uart_init(BIT_RATE_115200, BIT_RATE_115200);
  os_timer_setfn(&uart_timer, (os_timer_func_t *)uart, NULL);
  os_timer_arm(&uart_timer, 1000, 1);
  return;
}

// END UART TEST BLOCK

/*
void ICACHE_FLASH_ATTR usr_udp_recv(void *arg, char *data, unsigned short length) {
  // get and store the station IP
  //struct ip_info ip_addr;
  //wifi_get_ip_info(STATION_IF, &ip_addr);
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

  // actually connect to wifi
  wifi_set_opmode( STATION_MODE );
  os_memcpy(&stationConf.ssid, ssid, 32);
  os_memcpy(&stationConf.password, password, 32);
  wifi_station_set_config(&stationConf);

  // initialize TCP connection
  tcp_init();
  return;
}
*/
