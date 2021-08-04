#ifndef _IR_TEST_H
#define _IR_TEST_H

#include <c_types.h>
#include <gpio.h>
#include <eagle_soc.h>


typedef void (*IRCallback)(uint16_t code, bool toggle);


void irr_init();
void irr_register_callback(IRCallback);
void irr_disable();
void irr_enable();


#define LEADER_MIN      60000
#define STEP_MAX        1200
#define MAX_BITS        14

#endif


