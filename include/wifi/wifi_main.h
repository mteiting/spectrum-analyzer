#ifndef __WIFI_MAIN_H__
#define __WIFI_MAIN_H__

#define WIFI_QUEUE_LEN 4
#define WIFI_STATION_MODE 0x01
#define WIFI_SOFT_AP_MODE 0x02
#define WIFI_STATION_SOFTAP_MODE 0x03

#define WIFI_SOFT_AP_SSID "SpectrumAnalyzer"
#define WIFI_SOFT_AP_PW "password"

void wifi_task(os_event_t *e);

#endif