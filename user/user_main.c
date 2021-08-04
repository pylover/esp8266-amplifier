// Internal 
#include "user_config.h"
#include "partition.h"
#include "wifi.h"
#include "params.h" 
#include "debug.h"
#include "status.h"
#include "uns.h"
#include "amp.h"
#include "ir.h"
#include "bt.h"
#include "http.h"

// SDK
#include <ets_sys.h>
#include <osapi.h>
#include <mem.h>
#include <user_interface.h>
#include <driver/uart.h>
#include <upgrade.h>
#include <c_types.h>
#include <ip_addr.h> 
#include <espconn.h>

static bool configured;
static struct params params;

// FIXME: Update it during boot
static int powerstatus = 0;

#define HTTPSTATUS_POWERON  700
#define HTTPSTATUS_POWEROFF  701

void enableirr() {
    irr_enable();
    status_update(100, 100, 1, NULL);
}


void poweron_cb() {
    AMP_UNMUTE();
    enableirr();
}

void httpcb(int status, char * body, void *args) {
    DEBUG("HTTP callback: %d\n", status);
    if (status == HTTPSTATUS_POWERON) {
        DEBUG("Power is On\n");
        powerstatus = 1;
        status_update(100, 100, 15, poweron_cb);
    }
    else if (status == HTTPSTATUS_POWEROFF) {
        DEBUG("Power is Off\n");
        powerstatus = 0;
        BT_PLAYPAUSE();
    }
}


void ir_cmd(uint16_t code, bool toggle) {
    /* Ir is disabled by just before calling this callback, So it must enabled
     * again after processing message
     */
    status_update(100, 1, 1, NULL);
    switch (code) {
        case IRCMD_POWER:
            AMP_MUTE();
            status_update(500, 100, 0, NULL);
            http_nobody_uns(params.psu, powerstatus? "OFF": "ON", "/", httpcb, 
                    NULL);
            break;
        case IRCMD_VOLUP:
            amp_volup();
            irr_enable();
            break;
        case IRCMD_VOLDOWN:
            amp_voldown();
            irr_enable();
            break;
        case IRCMD_BTPLAYPAUSE:
            BT_PLAYPAUSE();
            break;
        case IRCMD_MUTE:
            amp_togglemute();
            status_update(100, 100, 5, enableirr);
            break;
        case IRCMD_NEXT:
            BT_NEXT();
            break;
        case IRCMD_PREVIOUS:
            BT_PREVIOUS();
            break;
        default:
            INFO("Unknown Command: %d, TGL: %d\r\n", code, toggle);
            irr_enable();
    }
}

static ICACHE_FLASH_ATTR 
void reboot_appmode() {
	system_upgrade_flag_set(UPGRADE_FLAG_FINISH);
	system_upgrade_reboot();
}


void wifi_connect_cb(uint8_t status) {
    if(status == STATION_GOT_IP) {
        char hostname[UNS_HOSTNAME_MAXLEN];
        os_sprintf(hostname, "%s.%s", params.zone, params.name);
        uns_init(hostname);
        INFO("WIFI Connected to: %s", params.station_ssid);
        wifi_ap_stop();
        MEMCHECK();
    } 
    else {
        uns_deinit();    
        INFO("WIFI Disonnected from: %s", params.station_ssid);
        wifi_ap_start();
    }
}


ICACHE_FLASH_ATTR
void boothello() {
    uint8_t image = system_upgrade_userbin_check();
    INFO("");
    INFO("%s.%s version: "__version__, params.zone, params.name);
    INFO("My full name is: %s.%s", params.zone, params.name);
    INFO("Boot image: user%d", image + 1);
    INFO("Free memory: %d KB", system_get_free_heap_size());
    if (!configured) {
        INFO(
            "Connect to WIFI Access point: %s, "
            "open http://192.168.43.1 to configure me.",
            params.name
        );
    }
    status_update(100, 1300, INFINITE, NULL);

    /* Web UI */
	webadmin_start(&params);

    struct rst_info *r = system_get_rst_info();
    INFO("Boot reason: %d\n", r->reason);
    if (r->reason == REASON_WDT_RST || 
            r->reason == REASON_EXCEPTION_RST ||
            r->reason == REASON_SOFT_WDT_RST) {
        
        if (r->reason == REASON_EXCEPTION_RST) {
            ERROR("Fatal exception (%d)", r->exccause);
        }
    
        DEBUG("epc1=0x%08x, epc2=0x%08x, epc3=0x%08x, excvaddr=0x%08x, "
                "depc=0x%08x\n", r->epc1, r->epc2, r->epc3, r->excvaddr, 
                r->depc); //The address of the last crash is printed, which is used to debug garbled output.
    }
}


void user_init(void) {
    //uart_init(BIT_RATE_115200, BIT_RATE_115200);
    uart_div_modify(UART0, UART_CLK_FREQ / BIT_RATE_115200);
    uart_rx_intr_disable(UART0);
    uart_rx_intr_disable(UART1);
    //system_show_malloc(); 
    
    /* Uncomment and edit the interrupt.c to configure interrupts */
    //interrupt_init();

	configured = params_load(&params);
	if (!configured) {
		ERROR("Cannot load params");
		if(!params_defaults(&params)) {
			ERROR("Cannot save params");
			return;
		}
	}
    
    INFO("");
    PARAMS_PRINT(params);

    // Bluetooth
    bt_init();

    // AMP GPIO setup
    amp_init();

    // Interrupt, FOTA Button and Infra Red
    irr_register_callback(ir_cmd);
    irr_init();

    // Disable wifi led before infrared
    wifi_status_led_uninstall();

    // Status LED
    status_init();

    /* Start WIFI */
    wifi_start(&params, wifi_connect_cb);

    /* Disable wifi led before infrared */
    wifi_status_led_uninstall();

    status_update(100, 400, 5, boothello);
}


ICACHE_FLASH_ATTR 
void user_pre_init(void) {
    MEMCHECK();
    if(!system_partition_table_regist(at_partition_table, 
				sizeof(at_partition_table)/sizeof(at_partition_table[0]),
				SPI_FLASH_SIZE_MAP)) {
		ERROR("system_partition_table_regist fail");
		while(1);
	}
    MEMCHECK();
}

