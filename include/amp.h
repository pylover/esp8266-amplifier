#ifndef _AMP_H_
#define _AMP_H_

#define AMP_MUTE()  GPIO_OUTPUT_SET(GPIO_ID_PIN(MUTE_NUM), 1)
#define AMP_UNMUTE()  GPIO_OUTPUT_SET(GPIO_ID_PIN(MUTE_NUM), 0)
#define AMP_MUTESTATUS()    GPIO_INPUT_GET(GPIO_ID_PIN(MUTE_NUM))


void amp_volup();
void amp_voldown();


#endif
