#ifndef _KEY_MAP_H_
#define _KEY_MAP_H_

#include <linux/key_code.h>
#include <boot_mode.h>

//#define KEYPAD_DEBUG
static uint32_t board_key_map[]={
    0x01, KEY_FASTBOOT, //home
    0x0, KEY_RECOVERY, //back
    0x10, KEY_ENGTEST, //menu
	0x1001, KEY_UPDATE, //reserve
};

#define CONFIG_KEYPAD_ROW_CNT 4
#define CONFIG_KEYPAD_COL_CNT 3
#define CONFIG_KEYPAD_LONG_CNT 0xc
#define CONFIG_KEYPAD_DEBOUNCE_CNT 0x5
#endif //_KEY_MAP_H_
