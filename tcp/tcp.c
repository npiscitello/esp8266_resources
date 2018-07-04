// types
#include "ets_sys.h"
// gpio...obvs
#include "gpio.h"
#include "os_type.h"
// wifi stuff
#include "user_interface.h"
// tcp stuff
#include "espconn.h"
// zalloc
#include "mem.h"
// memcpy
#include "osapi.h"
// uart printf
#include "driver/uart.h"

// pin the LED is on
#define GPIO BIT5

// ip poll timer
#define IP_POLL_DELAY 100
os_timer_t ip_poll_timer;

// connection storage
struct espconn connection;
struct _esp_tcp tcp_info;
ip_addr_t tcp_server_ip;

// what to connect to
#define REMOTE_HOST "example.com"

void ICACHE_FLASH_ATTR connect_to_server() {
  os_printf("Connected to server...");
  return;
}

void ICACHE_FLASH_ATTR dns_found( const char* name, ip_addr_t* ipaddr, void* arg) {
  // make sure we got something before we start doing work...
  if( ipaddr == NULL ) {
    os_printf("DNS query returned no results!\n");
    return;
  }

  struct espconn* func_espconn = (struct espconn*)arg;

  os_printf(  "DNS query returned %d.%d.%d.%d\n",
              *((uint8 *)&ipaddr->addr), *((uint8 *)&ipaddr->addr + 1),
              *((uint8 *)&ipaddr->addr + 2), *((uint8 *)&ipaddr->addr + 3));

  // copy in the found ip address and set up other network parameters
  tcp_server_ip.addr = ipaddr->addr;
  os_memcpy(func_espconn->proto.tcp->remote_ip, &ipaddr->addr, 4);
  func_espconn->proto.tcp->remote_port = 80;
  func_espconn->proto.tcp->local_port = espconn_port();

  // callbacks!
  espconn_regist_connectcb(func_espconn, connect_to_server);
  espconn_connect(func_espconn);

  return;
}

void ICACHE_FLASH_ATTR tcp_init() {
  os_printf("initializing tcp connection...\n");
  connection.proto.tcp = &tcp_info;
  connection.type = ESPCONN_TCP;
  connection.state = ESPCONN_NONE;
  
  os_printf("kicking off DNS search...\n");
  tcp_server_ip.addr = 0;
  espconn_gethostbyname(&connection, REMOTE_HOST, &tcp_server_ip, dns_found);
  return;
}

void ICACHE_FLASH_ATTR ip_poll(void) {
  struct ip_info ip;
  os_timer_disarm(&ip_poll_timer);

  if( wifi_station_get_connect_status() == STATION_GOT_IP ) {
    os_printf("IP address assigned\n");
    tcp_init();
  } else {
    os_timer_arm(&ip_poll_timer, IP_POLL_DELAY, 0);
  }

  return;
}

void ICACHE_FLASH_ATTR user_init() {

  // init uart console
  uart_div_modify(0, UART_CLK_FREQ / 115200);
  os_printf("\n\nUART debug interface enabled at 115200 bps\n\n");

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

  // check when the board gets an IP address
  os_timer_setfn(&ip_poll_timer, (os_timer_func_t *)ip_poll, NULL);
  os_timer_arm(&ip_poll_timer, IP_POLL_DELAY, 0);

  return;
}
