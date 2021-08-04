#include "user_config.h"
#include "amp.h"

#include <osapi.h>
#include <gpio.h>


void amp_togglemute() {
    if (AMP_MUTESTATUS() == 1) {
        AMP_UNMUTE();
    }
    else { 
        AMP_MUTE();
    }
}


void amp_volup() {
    GPIO_OUTPUT_SET(GPIO_ID_PIN(VOLOSC_NUM), 1);
    os_delay_us(AMP_PULSE_US); 
    GPIO_OUTPUT_SET(GPIO_ID_PIN(VOLOSC_NUM), 0);
}


void amp_voldown() {
    GPIO_OUTPUT_SET(GPIO_ID_PIN(VOLUD_NUM), 1);
    GPIO_OUTPUT_SET(GPIO_ID_PIN(VOLOSC_NUM), 1);
    os_delay_us(AMP_PULSE_US); 
    GPIO_OUTPUT_SET(GPIO_ID_PIN(VOLOSC_NUM), 0);
    GPIO_OUTPUT_SET(GPIO_ID_PIN(VOLUD_NUM), 0);
}


void amp_init() {
    PIN_FUNC_SELECT(VOLOSC_MUX, VOLOSC_FUNC);
    PIN_PULLUP_DIS(VOLOSC_MUX);
    GPIO_OUTPUT_SET(GPIO_ID_PIN(VOLOSC_NUM), 0);

    PIN_FUNC_SELECT(VOLUD_MUX, VOLUD_FUNC);
    PIN_PULLUP_DIS(VOLUD_MUX);
    GPIO_OUTPUT_SET(GPIO_ID_PIN(VOLUD_NUM), 0);

    PIN_FUNC_SELECT(MUTE_MUX, MUTE_FUNC);
    PIN_PULLUP_EN(MUTE_MUX);
    AMP_MUTE();
}
