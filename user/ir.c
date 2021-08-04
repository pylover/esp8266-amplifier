#include "ir.h"
#include "debug.h"

#include <c_types.h>
#include <gpio.h>
#include <osapi.h>


static IRCallback callback;
static uint32_t lastbit_time;
static uint16_t sample;
static bool lastbit = 0;
static int8_t cursor = -1;

#define TGLMASK     0b00100000000000
#define ADDRMASK    0b00011111000000
#define DATAMASK    0b00000000111111

typedef enum {
    IR_DONE,
    IR_MORE
} irbitstate_t;

static irbitstate_t addbit(bool b) {
    uint16_t addr;
    if (cursor == -1) {
        sample = 0;
    }
    
    sample = (sample << 1) | b;
    cursor++;
    lastbit = b;
    if ((MAX_BITS - cursor) == 1) {
        addr = (sample & ADDRMASK) >> 6;
        if (addr == 0) {
            //INFO(
            //    "Addr: %d, Toggle: %d, Data: %d\n", 
            //    addr,
            //    (sample & TGLMASK) >> 11,
            //    (sample & DATAMASK)
            //);
            callback((sample & DATAMASK), (sample & TGLMASK) >> 11);
            return IR_DONE;
        }
    }
    return IR_MORE;
}


static void irr_intr_handler() {
    uint32_t timedelta;
    uint32_t now;
    bool bit; 
    irr_disable();
    
    bit = !GPIO_INPUT_GET(GPIO_ID_PIN(IR_NUM));
    now = system_get_time();
    timedelta = now - lastbit_time;
    lastbit_time = now;
    
    if (timedelta > LEADER_MIN) {
        cursor = -1;
    }

    if ((lastbit == bit) || (timedelta > STEP_MAX)) {
        if (addbit(bit) == IR_MORE) {
            irr_enable();
        }
    }
    else {
        irr_enable();
    }
}


void interrupt_dispatch()
{
    s32 gpio_status;
    gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
    if((gpio_status >> IR_NUM) & BIT0){
        irr_intr_handler();
        GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status & BIT(IR_NUM));
    }  
    else{
        INFO("gpio num mismached: %lu\r\n", gpio_status);
    }
}


void irr_register_callback(IRCallback c) {
    callback = c;
}


void irr_enable() {
    gpio_pin_intr_state_set(GPIO_ID_PIN(IR_NUM), GPIO_PIN_INTR_ANYEDGE);
}


void irr_disable() {
    gpio_pin_intr_state_set(GPIO_ID_PIN(IR_NUM), 
            GPIO_PIN_INTR_DISABLE);
}


void irr_init() {
    // LED
    PIN_FUNC_SELECT(LED_MUX, LED_FUNC);
    GPIO_OUTPUT_SET(GPIO_ID_PIN(LED_NUM), 1);

    /* Interrupt */
    ETS_GPIO_INTR_ATTACH(interrupt_dispatch, NULL);
    ETS_GPIO_INTR_DISABLE();

    // IR Diode
    GPIO_DIS_OUTPUT(GPIO_ID_PIN(IR_NUM));
    PIN_FUNC_SELECT(IR_MUX, IR_FUNC);
    PIN_PULLUP_DIS(IR_MUX);
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(IR_NUM));
    //gpio_pin_intr_state_set(GPIO_ID_PIN(IR_NUM), GPIO_PIN_INTR_NEGEDGE);
    gpio_pin_intr_state_set(GPIO_ID_PIN(IR_NUM), GPIO_PIN_INTR_ANYEDGE);

    ETS_GPIO_INTR_ENABLE();
}
