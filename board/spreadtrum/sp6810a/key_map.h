#ifndef _KEY_MAP_H_
#define _KEY_MAP_H_

#include <linux/key_code.h>
#include <boot_mode.h>

//#define KEYPAD_DEBUG
static uint32_t board_key_map[]={
    0x23, KEY_RECOVERY, //back
    0x33, KEY_FASTBOOT, //home
    0x32, KEY_ENGTEST, //menu
};

#define CONFIG_KEYPAD_ROW_CNT 5
#define CONFIG_KEYPAD_COL_CNT 5
#define CONFIG_KEYPAD_LONG_CNT 0xc
#define CONFIG_KEYPAD_DEBOUNCE_CNT 0x5

#define CONFIG_HOME_GPIO 28
#define CONFIG_VOLUMEUP_GPIO 25
#endif //_KEY_MAP_H_
