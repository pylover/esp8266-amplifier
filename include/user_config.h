#ifndef USER_CONFIG_H
#define USER_CONFIG_H

#define USE_OPTIMIZE_PRINTF

#define __name__ "esp8266-amplifier"
#define __version__ "0.1.0"

#define PARAMS_DEFAULT_ZONE "dev"
#define PARAMS_DEFAULT_NAME __name__

/* Amplifier volume control pulse width. */
#define AMP_PULSE_US    200


/* Bluetooth module control pulse width. */
#define BT_PULSE_MS     100


// Available IO     IO  INIT
#define GPIO0        0  //    (BT_PREVIOUS, FLASH, FOTABTN)
#define GPIO1        1  //    [UART TX]
#define GPIO2        2  //    (LED)
#define GPIO3        3  //    [UART RX]
#define GPIO4        4  //    (VOL_OSC)
#define GPIO5        5  //    (IR)
//                   6
//                   7
//                   8
//                   9
#define GPIO10      10  //    (MUTE)
//                  11
#define GPIO12      12  //    (BT_PLAYPAUSE)
#define GPIO13      13  //    (BT_NEXT)
#define GPIO14      14  //    (VOL_UPDOWN)
#define GPIO15      15  //    (
#define GPIO16      16  //    (

// Mute
#define MUTE_MUX            PERIPHS_IO_MUX_SD_DATA3_U
#define MUTE_NUM            GPIO10
#define MUTE_FUNC           FUNC_GPIO10

// BTNEXT
#define BTNEXT_MUX             PERIPHS_IO_MUX_MTCK_U
#define BTNEXT_NUM             GPIO13
#define BTNEXT_FUNC            FUNC_GPIO13

// BTPREVIOUS
#define BTPREVIOUS_MUX         PERIPHS_IO_MUX_GPIO0_U
#define BTPREVIOUS_NUM         GPIO0
#define BTPREVIOUS_FUNC        FUNC_GPIO0


// BTPLAYPAUSE
#define BTPLAYPAUSE_MUX         PERIPHS_IO_MUX_MTDI_U
#define BTPLAYPAUSE_NUM         GPIO12
#define BTPLAYPAUSE_FUNC        FUNC_GPIO12

// LED
#define LED_MUX                 PERIPHS_IO_MUX_GPIO2_U
#define LED_NUM                 GPIO2
#define LED_FUNC                FUNC_GPIO2

// IR
#define IR_MUX                PERIPHS_IO_MUX_GPIO5_U
#define IR_NUM                GPIO5
#define IR_FUNC                FUNC_GPIO5

// VOL_OSC
#define VOLOSC_MUX            PERIPHS_IO_MUX_GPIO4_U
#define VOLOSC_NUM            GPIO4
#define VOLOSC_FUNC            FUNC_GPIO4

// VOL_UPDOWN
#define VOLUD_MUX            PERIPHS_IO_MUX_MTMS_U
#define VOLUD_NUM            GPIO14
#define VOLUD_FUNC            FUNC_GPIO14


// IR Remote commands
#define IRCMD_POWER         12
#define IRCMD_MUTE          13
#define IRCMD_VOLUP         16
#define IRCMD_VOLDOWN       17
#define IRCMD_BTPLAYPAUSE   23
#define IRCMD_NEXT          32
#define IRCMD_PREVIOUS      33


#endif

