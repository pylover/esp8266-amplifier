#include "user_config.h"
#include "bt.h"
#include "ir.h"
#include "debug.h"
#include "status.h"

#include <osapi.h>
#include <gpio.h>

static uint32_t pin;


static ICACHE_FLASH_ATTR
void keypress_done() {
    INFO("BT keypress done");
    GPIO_OUTPUT_SET(GPIO_ID_PIN(pin), 1);
    irr_enable();
}

void simulatekeypress(uint32_t p) {
    pin = p;
    INFO("BT keypress...");
    irr_disable();
    GPIO_OUTPUT_SET(GPIO_ID_PIN(pin), 0);
    status_update(BT_PULSE_MS - 2, 2, 1, keypress_done);
}


void bt_init() {
    PIN_FUNC_SELECT(BTPLAYPAUSE_MUX, BTPLAYPAUSE_FUNC);
    PIN_PULLUP_DIS(BTPLAYPAUSE_MUX);
    GPIO_OUTPUT_SET(GPIO_ID_PIN(BTPLAYPAUSE_NUM), 1);

    PIN_FUNC_SELECT(BTNEXT_MUX, BTNEXT_FUNC);
    PIN_PULLUP_DIS(BTNEXT_MUX);
    GPIO_OUTPUT_SET(GPIO_ID_PIN(BTNEXT_NUM), 1);
    
    PIN_FUNC_SELECT(BTPREVIOUS_MUX, BTPREVIOUS_FUNC);
    PIN_PULLUP_DIS(BTPREVIOUS_MUX);
    GPIO_OUTPUT_SET(GPIO_ID_PIN(BTPREVIOUS_NUM), 1);
}
