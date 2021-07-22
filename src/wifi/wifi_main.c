#include "osapi.h"
#include "mem.h"
#include "wifi/wifi_main.h"
#include "user_interface.h"


void wifi_task(os_event_t *e)
{
    struct softap_config wifiConfig = {0};
    wifi_set_opmode(WIFI_SOFT_AP_MODE);
    wifi_softap_get_config(&wifiConfig); // Get config first
    wifiConfig.ssid_len = sizeof(WIFI_SOFT_AP_SSID);
    wifiConfig.channel = 1;
    wifiConfig.authmode = AUTH_WPA2_PSK;
    wifiConfig.beacon_interval = 100;
    wifiConfig.ssid_hidden = 0;
    os_memcpy(&wifiConfig.ssid, WIFI_SOFT_AP_SSID, sizeof(WIFI_SOFT_AP_SSID));
    os_memcpy(&wifiConfig.password, WIFI_SOFT_AP_PW, sizeof(WIFI_SOFT_AP_PW));
    wifi_softap_set_config(&wifiConfig);
    wifi_softap_dhcps_start();
    while (1)
    {
        os_delay_us(1000);  
        system_soft_wdt_restart();
        // switch(wifi_softap_dhcps_status())
        // {
        //     case DHCP_STOPPED: os_printf("[WIFI]: DHCP Stopped\n"); break;
        //     case DHCP_STARTED: os_printf("[WIFI]: DHCP Started\n"); break;
        //     default: break;
        // } 
    }
}