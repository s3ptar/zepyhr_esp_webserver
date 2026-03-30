/***********************************************************************
*! \file:                   main.c
*  \projekt:                USB_HID
*  \created on:             2026 03 29
*  \author:                 R. Gräber
*  \version:                0
   \history:                -
*  \brief:                  Blinky and HTML example for ESP32 using Zephyr RTOS
***********************************************************************/

/***********************************************************************
* Includes
***********************************************************************/

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_event.h>


/***********************************************************************
* Declarations
***********************************************************************/

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

#define SSID CONFIG_MY_WIFI_SSID //"smarthome24"
#define PSK "zdx1bmuJ7FQaI8owjhHkDABN3UyFTNP7tEaptdqTxAPkIQN98T6RsPPLqTN3gVF"//CONFIG_MY_WIFI_PASSWORD// "zdx1bmuJ7FQaI8owjhHkDABN3UyFTNP7tEaptdqTxAPkIQN98T6RsPPLqTN3gVF"

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
/***********************************************************************
* Constant
***********************************************************************/


/***********************************************************************
* Global Variable
***********************************************************************/


/***********************************************************************
* local Variable
***********************************************************************/

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct device *gpio_ct_dev = DEVICE_DT_GET(DT_NODELABEL(gpio0));

/***********************************************************************
* Local Funtions
***********************************************************************/


static struct net_mgmt_event_callback mgmt_cb;

/***********************************************************************
*! \fn          int main(){
*  \brief       start up function
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
/*
// Dieser Callback wird aufgerufen, wenn sich am Netzwerk-Status etwas ändert
void handler(struct net_mgmt_event_callback *cb, uint32_t mgmt_event, struct net_if *iface)
{
    if (mgmt_event == NET_EVENT_IPV4_ADDR_ADD) {
        char buf[NET_IPV4_ADDR_STR_LEN];
        
        // Suche die erste gültige IPv4 Adresse des Interfaces
        for (int i = 0; i < NET_IF_MAX_IPV4_ADDR; i++) {
            if (iface->config.ip.ipv4->unicast[i].addr_type == NET_ADDR_DHCP) {
                printk("Verbunden! IP-Adresse: %s\n",
                       net_addr_ntop(AF_INET,
                                     &iface->config.ip.ipv4->unicast[i].address.in_addr,
                                     buf, sizeof(buf)));
            }
        }
    }
}*/


/***********************************************************************
*! \fn          int main(){
*  \brief       start up function
*  \param       none
*  \exception   none
*  \return      none
***********************************************************************/
int main(void)
{
	int ret;
	printk("Hello World! %s\n", CONFIG_BOARD);

	uint32_t cpu_count = arch_num_cpus();

	bool led_state = true;

	if(!device_is_ready(gpio_ct_dev)){
        return 0;
    }

	printk("--- ESP32 MULTICORE DIAGNOSE ---\n");
    printk("Anzahl erkannter CPUs: %d\n", cpu_count);
    printk("Aktueller Kern: %d\n", arch_curr_cpu()->id);


	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params params = {
        .ssid = SSID,
        .ssid_length = strlen(SSID),
        .psk = PSK,
        .psk_length = strlen(PSK),
        .channel = WIFI_CHANNEL_ANY,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    printk("Verbinde mit %s...\n", SSID);

    if (net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params))) {
        printk("WiFi Verbindung fehlgeschlagen!\n");
    } else {
        printk("WiFi Verbindung wird aufgebaut...\n");
    }

	/*net_mgmt_init_event_callback(&mgmt_cb, handler, NET_EVENT_IPV4_ADDR_ADD);
    net_mgmt_add_event_callback(&mgmt_cb);

	printk("Verbinde mit %s...\n", SSID);

    if (net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params))) {
        printk("Fehler beim Verbindungsaufbau!\n");
    }*/

	while (1) {
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
			return 0;
		}

		led_state = !led_state;
		printf("LED state: %s\n", led_state ? "ON" : "OFF");
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;

}
 