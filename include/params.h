#ifndef PARAMS_H
#define PARAMS_H

#include "debug.h"
#include "partition.h"
#include "user_config.h"

#include <c_types.h>
#include <user_interface.h>


#define PARAMS_SECTOR SYSTEM_PARTITION_PARAMS_ADDR / 4096 

#define PARAMS_MAGIC '@'


#define PARAMS_PRINT(p) \
    INFO(CR"%s.%s, ssid: %s psk: %s ap-psk: %s PSMCU: %s", \
		p.zone, \
		p.name, \
		p.station_ssid, \
		p.station_psk, \
		p.ap_psk, \
        p.psu \
	)

#ifndef PARAMS_ZONE_MAXLEN
#define PARAMS_ZONE_MAXLEN  32
#endif

#ifndef PARAMS_NAME_MAXLEN
#define PARAMS_NAME_MAXLEN  32
#endif


struct params {
	char zone[PARAMS_ZONE_MAXLEN];
	char name[PARAMS_NAME_MAXLEN];
	char ap_psk[32];
	char station_ssid[32];
	char station_psk[32];
	char magic;
    char psu[32];
};


bool params_save(struct params* params);
bool params_load(struct params* params);
bool params_defaults(struct params* params);

#endif

