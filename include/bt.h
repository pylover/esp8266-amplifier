#ifndef _BT_H_
#define _BT_H_

#include <c_types.h>

void simulatekeypress(uint32_t p);

#define BT_PLAYPAUSE() simulatekeypress(BTPLAYPAUSE_NUM)
#define BT_NEXT() simulatekeypress(BTNEXT_NUM)
#define BT_PREVIOUS() simulatekeypress(BTPREVIOUS_NUM)


#endif
